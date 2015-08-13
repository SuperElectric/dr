#include "display.h"
#include "shader.h"
#include <GL/glew.h>
#include <iostream>

Display::Display(int width, int height, const std::string& title,
                 const int numberOfFrameBuffers,
                 const int numberOfColourRenderBuffers)
{
    //
    // Set up SDL window
    //

    m_width = width;
    m_height = height;
    // for now, render resolution and window size are set to be the same.
    render_width = 1.0*width;
    render_height = 1.0*height;
    SDL_Init(SDL_INIT_EVERYTHING);
    SDL_GL_SetAttribute(SDL_GL_RED_SIZE, 8);
    SDL_GL_SetAttribute(SDL_GL_GREEN_SIZE, 8);
    SDL_GL_SetAttribute(SDL_GL_BLUE_SIZE, 8);
    SDL_GL_SetAttribute(SDL_GL_ALPHA_SIZE, 8);
    SDL_GL_SetAttribute(SDL_GL_BUFFER_SIZE, 32);
    SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 16);
    SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
    //SDL_GL_SetAttribute(SDL_GL_FRAMEBUFFER_SRGB_CAPABLE, 1);
    //SDL_GL_SetAttribute(SDL_GL_MULTISAMPLESAMPLES, 0);
    m_window = SDL_CreateWindow(title.c_str(), SDL_WINDOWPOS_CENTERED,
                                SDL_WINDOWPOS_CENTERED, width, height,
                                SDL_WINDOW_OPENGL);
    m_glContext = SDL_GL_CreateContext(m_window);
    GLenum status = glewInit();
    if(status != GLEW_OK)
    {
        std::cerr << "Glew failed to initialize!" << std::endl;
    }
    m_isClosed = false;

    //
    // Set up offscreen framebuffers
    //

    NUM_FRAME_BUFFERS = numberOfFrameBuffers;
    NUM_COLOUR_TEXTURE_BUFFERS = numberOfColourRenderBuffers;
    if (NUM_FRAME_BUFFERS <= 0)
    {
        NUM_FRAME_BUFFERS = 0;
        return;
    }

    m_framebuffers = new GLuint [NUM_FRAME_BUFFERS];
    m_depthbuffers = new GLuint [NUM_FRAME_BUFFERS];
    m_texturerenderbuffers = new GLuint* [NUM_FRAME_BUFFERS];

    glGenFramebuffers(NUM_FRAME_BUFFERS, m_framebuffers);
    glGenRenderbuffers(NUM_FRAME_BUFFERS, m_depthbuffers);
    for (int i=0; i<NUM_FRAME_BUFFERS; i++)
    {
        // set up multiple colour texture renderbuffers
        m_texturerenderbuffers[i] = new GLuint [NUM_COLOUR_TEXTURE_BUFFERS];
        glGenTextures (NUM_COLOUR_TEXTURE_BUFFERS, m_texturerenderbuffers[i]);
        glBindFramebuffer(GL_DRAW_FRAMEBUFFER, m_framebuffers[i]);
        for (int j=0; j<NUM_COLOUR_TEXTURE_BUFFERS; j++)
        {
            glBindTexture(GL_TEXTURE_2D, m_texturerenderbuffers[i][j]);
            glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB32F, render_width, render_height, 0, GL_RGB, GL_FLOAT, NULL);
            glFramebufferTexture2D(GL_DRAW_FRAMEBUFFER, GL_COLOR_ATTACHMENT0+j,
                                      GL_TEXTURE_2D, m_texturerenderbuffers[i][j], 0);
        }

        // set up a depth renderbuffer
        glBindRenderbuffer(GL_RENDERBUFFER, m_depthbuffers[i]);
        glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT16, render_width, render_height);
        glFramebufferRenderbuffer(GL_DRAW_FRAMEBUFFER, GL_DEPTH_ATTACHMENT,
                                  GL_RENDERBUFFER, m_depthbuffers[i]);
    }

    glBindFramebuffer(GL_DRAW_FRAMEBUFFER, 0);
    glBindRenderbuffer(GL_RENDERBUFFER, 0);
    glEnable(GL_DEPTH_TEST);

    // Set up a single textured point for rendering.
    glGenVertexArrays(1, &m_quad_vertexarray);
    /* perhaps not needed, since we may not need to pass in any attributes */
    //
    //
    GLint currentBufferBinding;
    glGetIntegerv(GL_ARRAY_BUFFER_BINDING, &currentBufferBinding);
    glBindVertexArray(m_quad_vertexarray);
    glGenBuffers(1, &m_quad_vertexbuffer);
    float dummyAttribute = 0.5;
    glBindBuffer(GL_ARRAY_BUFFER, m_quad_vertexbuffer);
    glBufferData(GL_ARRAY_BUFFER, sizeof(dummyAttribute), &dummyAttribute, GL_STATIC_DRAW);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0,1,GL_FLOAT,GL_FALSE,0,0);
    glBindVertexArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, currentBufferBinding);
    //
    //
    m_quad_program = glCreateProgram();
    char* label ("DR_DIRECTORY");
    std::string DR_DIRECTORY (getenv(label));
    m_quad_vertshader = CreateShader(LoadShader(DR_DIRECTORY + "/assets/drawTexture.vs"), GL_VERTEX_SHADER);
    m_quad_fragshader = CreateShader(LoadShader(DR_DIRECTORY + "/assets/drawTexture.fs"), GL_FRAGMENT_SHADER);
    glAttachShader(m_quad_program, m_quad_vertshader);
    glAttachShader(m_quad_program, m_quad_fragshader);
    //
    glBindAttribLocation(m_quad_program, 0, "dummyAttribute");
    //
    glLinkProgram(m_quad_program);
    CheckShaderError(m_quad_program, GL_LINK_STATUS, true, "Error: drawTexture program linking failed: ");
    glValidateProgram(m_quad_program);
    CheckShaderError(m_quad_program, GL_VALIDATE_STATUS, true, "Error: Program is invalid: ");

}

Display::~Display()
{
    for (int i=0; i<NUM_FRAME_BUFFERS; i++)
    {
        glDeleteTextures(NUM_COLOUR_TEXTURE_BUFFERS, m_texturerenderbuffers[i]);
        delete[] m_texturerenderbuffers[i];
    }
    delete[] m_texturerenderbuffers;

    glDeleteRenderbuffers(NUM_FRAME_BUFFERS, m_depthbuffers);
    delete[] m_depthbuffers;

    glDeleteFramebuffers(NUM_FRAME_BUFFERS, m_framebuffers);
    delete[] m_framebuffers;

    SDL_GL_DeleteContext(m_glContext);
    SDL_DestroyWindow(m_window);
    SDL_Quit();

    // destroy single Gl point and shaders
    //
    glDeleteBuffers(1, &m_quad_vertexbuffer);
    //
    glDeleteVertexArrays(1, &m_quad_vertexarray);
    glDetachShader(m_quad_program, m_quad_vertshader);
    glDetachShader(m_quad_program, m_quad_fragshader);
    glDeleteProgram(m_quad_program);
    //glDeleteSamplers(1, &m_quad_texturesampler); // sampler is not needed
}

void Display::Clear(float r, float g, float b, float a)
{
    glClearColor(r,g,b,a);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

void Display::SetFrameBuffer(int n)
{
    if (n<0 or n>NUM_FRAME_BUFFERS-1)
    {
        glBindFramebuffer(GL_FRAMEBUFFER, 0);
    }
    else
    {
        glBindFramebuffer(GL_FRAMEBUFFER, m_framebuffers[n]);
        GLenum* renderBufferList = new GLenum[NUM_COLOUR_TEXTURE_BUFFERS];
        for(int i=0; i<NUM_COLOUR_TEXTURE_BUFFERS; i++)
        {
            renderBufferList[i] = GL_COLOR_ATTACHMENT0 + i;
        }
        glDrawBuffers(NUM_COLOUR_TEXTURE_BUFFERS, renderBufferList);
        delete[] renderBufferList;
    }
    glViewport(0,0,render_width,render_height);
}


void Display::SetRenderBuffer(int n,int i){
    glBindFramebuffer(GL_FRAMEBUFFER, m_framebuffers[n]);
    glReadBuffer(GL_COLOR_ATTACHMENT0+i);
}


void Display::CopyFrameBuffer()
{
    glBindFramebuffer(GL_DRAW_FRAMEBUFFER, 0);
    glViewport(0,0,m_width,m_height);
    //glClearColor(0.0,0.0,1.0,1.0);
    //glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glBlitFramebuffer(0,0,render_width-1,render_height-1,
                      0,0,m_width-1,m_height-1,
                      GL_COLOR_BUFFER_BIT, GL_NEAREST);
};


void Display::ShowTexture(int n, int i){
    glUseProgram(m_quad_program);

    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, m_texturerenderbuffers[n][i]);
    glUniform1i(glGetUniformLocation(m_quad_program, "sampler0"), 0);

    glBindFramebuffer(GL_FRAMEBUFFER, 0);

    glBindVertexArray(m_quad_vertexarray);
    glPointSize(1000.0);
    glDrawArrays(GL_POINTS, 0,1);
    glBindVertexArray(0);
    glBindTexture(GL_TEXTURE_2D, 0);
}


// this might work. See if it works before changing to use glGetTexImage
void Display::SaveTexture(int n, int i, float* pixels)
{
    glBindFramebuffer(GL_FRAMEBUFFER, m_framebuffers[n]);
    glReadBuffer(GL_COLOR_ATTACHMENT0+i);
    glReadPixels(0,0,render_width,render_height,GL_RGB,GL_FLOAT,pixels);
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
}


bool Display::IsClosed()
{
    return m_isClosed;
}

void Display::Update()
{
    SDL_GL_SwapWindow(m_window);

    SDL_Event e;
    while(SDL_PollEvent(&e))
    {
        if(e.type == SDL_QUIT)
        {
            m_isClosed = true;
        }
    }
}
