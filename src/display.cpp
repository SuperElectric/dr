#include "display.h"
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
    NUM_COLOUR_RENDER_BUFFERS = numberOfColourRenderBuffers;
    if (NUM_FRAME_BUFFERS <= 0)
    {
        NUM_FRAME_BUFFERS = 0;
        return;
    }

    m_renderbuffers = new GLuint* [NUM_FRAME_BUFFERS];
    m_framebuffers = new GLuint [NUM_FRAME_BUFFERS];
    glGenFramebuffers(NUM_FRAME_BUFFERS, m_framebuffers);
    for (int i=0; i<NUM_FRAME_BUFFERS; i++)
    {
        m_renderbuffers[i] = new GLuint [NUM_COLOUR_RENDER_BUFFERS+1];
        glGenRenderbuffers(NUM_COLOUR_RENDER_BUFFERS+1, m_renderbuffers[i]);
        glBindFramebuffer(GL_DRAW_FRAMEBUFFER, m_framebuffers[i]);
        for (int j=0; j<NUM_COLOUR_RENDER_BUFFERS; j++)
        {
            glBindRenderbuffer(GL_RENDERBUFFER, m_renderbuffers[i][j]);
            glRenderbufferStorage(GL_RENDERBUFFER, GL_RGB32F, render_width, render_height);
            glFramebufferRenderbuffer(GL_DRAW_FRAMEBUFFER, GL_COLOR_ATTACHMENT0+j,
                                      GL_RENDERBUFFER, m_renderbuffers[i][j]);
        }
        glBindRenderbuffer(GL_RENDERBUFFER, m_renderbuffers[i][NUM_COLOUR_RENDER_BUFFERS]);
        glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT16, render_width, render_height);
        glFramebufferRenderbuffer(GL_DRAW_FRAMEBUFFER, GL_DEPTH_ATTACHMENT,
                                  GL_RENDERBUFFER, m_renderbuffers[i][NUM_COLOUR_RENDER_BUFFERS]);
    }

    glBindFramebuffer(GL_DRAW_FRAMEBUFFER, 0);
    glBindRenderbuffer(GL_RENDERBUFFER, 0);
    glEnable(GL_DEPTH_TEST);
}

Display::~Display()
{
    for (int i=0; i<NUM_FRAME_BUFFERS; i++)
    {
        glDeleteRenderbuffers(NUM_COLOUR_RENDER_BUFFERS+1, m_renderbuffers[i]);
        delete[] m_renderbuffers[i];
    }
    delete[] m_renderbuffers;

    glDeleteFramebuffers(NUM_FRAME_BUFFERS, m_framebuffers);
    delete[] m_framebuffers;

    SDL_GL_DeleteContext(m_glContext);
    SDL_DestroyWindow(m_window);
    SDL_Quit();
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
        GLenum* renderBufferList = new GLenum[NUM_COLOUR_RENDER_BUFFERS];
        for(int i=0; i<NUM_COLOUR_RENDER_BUFFERS; i++)
        {
            renderBufferList[i] = GL_COLOR_ATTACHMENT0 + i;
        }
        glDrawBuffers(NUM_COLOUR_RENDER_BUFFERS, renderBufferList);
        delete[] renderBufferList;
    }
    glViewport(0,0,render_width,render_height);
}

void Display::SetRenderBuffer(int n, int i)
{
    //glBindRenderbuffer(GL_RENDERBUFFER, m_renderbuffers[n][i]);
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


void Display::SaveRenderBuffer(float* pixels)
{
    glReadPixels(0,0,render_width,render_height,GL_RGB,GL_FLOAT,pixels);
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
