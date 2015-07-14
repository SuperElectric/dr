#include "display.h"
#include <GL/glew.h>
#include <iostream>

Display::Display(int width, int height, const std::string& title,
                 const int numberOfFrameBuffers)
{
    //
    // Set up SDL window
    //

    m_width = width;
    m_height = height;
    // for now, render resolution and window size are set to be the same.
    m_render_width = 1.0*width;
    m_render_height = 1.0*height;
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
        m_renderbuffers[i] = new GLuint [NUM_RENDER_BUFFERS];
        glGenRenderbuffers(NUM_RENDER_BUFFERS, m_renderbuffers[i]);
        glBindRenderbuffer(GL_RENDERBUFFER, m_renderbuffers[i][RB_COLOUR]);
        glRenderbufferStorage(GL_RENDERBUFFER, GL_RGBA32F, m_render_width, m_render_height);
        glBindRenderbuffer(GL_RENDERBUFFER, m_renderbuffers[i][RB_DEPTH]);
        glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT16, m_render_width, m_render_height);
        glBindFramebuffer(GL_DRAW_FRAMEBUFFER, m_framebuffers[i]);
        glFramebufferRenderbuffer(GL_DRAW_FRAMEBUFFER, GL_COLOR_ATTACHMENT0,
                                  GL_RENDERBUFFER, m_renderbuffers[i][RB_COLOUR]);
        glFramebufferRenderbuffer(GL_DRAW_FRAMEBUFFER, GL_DEPTH_ATTACHMENT,
                                  GL_RENDERBUFFER, m_renderbuffers[i][RB_DEPTH]);
    }

    glBindFramebuffer(GL_DRAW_FRAMEBUFFER, 0);
    glBindRenderbuffer(GL_RENDERBUFFER, 0);
    glEnable(GL_DEPTH_TEST);
}

Display::~Display()
{
    for (int i=0; i<NUM_FRAME_BUFFERS; i++)
    {
        glDeleteRenderbuffers(NUM_RENDER_BUFFERS, m_renderbuffers[i]);
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
    }
    glViewport(0,0,m_render_width,m_render_height);
}

void Display::CopyFrameBuffer()
{
    glBindFramebuffer(GL_DRAW_FRAMEBUFFER, 0);
    glViewport(0,0,m_width,m_height);
    //glClearColor(0.0,0.0,1.0,1.0);
    //glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glBlitFramebuffer(0,0,m_render_width-1,m_render_height-1,
                      0,0,m_width-1,m_height-1,
                      GL_COLOR_BUFFER_BIT, GL_NEAREST);
};

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
