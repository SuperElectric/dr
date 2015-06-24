#include "display.h"
#include <GL/glew.h>
#include <iostream>

Display::Display(int width, int height, const std::string& title)
{
    //
    // Set up SDL window
    //
    m_width = width;
    m_height = height;
    SDL_Init(SDL_INIT_EVERYTHING);
    SDL_GL_SetAttribute(SDL_GL_RED_SIZE, 8);
    SDL_GL_SetAttribute(SDL_GL_GREEN_SIZE, 8);
    SDL_GL_SetAttribute(SDL_GL_BLUE_SIZE, 8);
    SDL_GL_SetAttribute(SDL_GL_ALPHA_SIZE, 8);
    SDL_GL_SetAttribute(SDL_GL_BUFFER_SIZE, 32);
    SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 16);
    SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
    //SDL_GL_SetAttribute(SDL_GL_MULTISAMPLESAMPLES, 1);
    m_window = SDL_CreateWindow(title.c_str(), SDL_WINDOWPOS_CENTERED,
                                SDL_WINDOWPOS_CENTERED, width, height,
                                SDL_WINDOW_OPENGL);
    m_glContext = SDL_GL_CreateContext(m_window);
    GLenum status = glewInit();
    if(status != GLEW_OK){
        std::cerr << "Glew failed to initialize!" << std::endl;
    }
    m_isClosed = false;

    //
    // Set up offscreen framebuffer
    //

    glGenRenderbuffers(NUM_RENDER_BUFFERS, m_renderbuffers);
    glBindRenderbuffer(GL_RENDERBUFFER, m_renderbuffers[RB_COLOUR]);
    glRenderbufferStorage(GL_RENDERBUFFER, GL_RGBA, 256, 256);
    glBindRenderbuffer(GL_RENDERBUFFER, m_renderbuffers[RB_DEPTH]);
    glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT16, 256, 256);
    glGenFramebuffers(1, &m_framebuffer);
    glBindFramebuffer(GL_DRAW_FRAMEBUFFER, m_framebuffer);
    glFramebufferRenderbuffer(GL_DRAW_FRAMEBUFFER, GL_COLOR_ATTACHMENT0,
                              GL_RENDERBUFFER, m_renderbuffers[RB_COLOUR]);
    glFramebufferRenderbuffer(GL_DRAW_FRAMEBUFFER, GL_DEPTH_ATTACHMENT,
                              GL_RENDERBUFFER, m_renderbuffers[RB_DEPTH]);


    glGenRenderbuffers(NUM_RENDER_BUFFERS, m_renderbuffers2);
    glBindRenderbuffer(GL_RENDERBUFFER, m_renderbuffers2[RB_COLOUR]);
    glRenderbufferStorage(GL_RENDERBUFFER, GL_RGBA, 256, 256);
    glBindRenderbuffer(GL_RENDERBUFFER, m_renderbuffers2[RB_DEPTH]);
    glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT16, 256, 256);
    glGenFramebuffers(1, &m_framebuffer2);
    glBindFramebuffer(GL_DRAW_FRAMEBUFFER, m_framebuffer2);
    glFramebufferRenderbuffer(GL_DRAW_FRAMEBUFFER, GL_COLOR_ATTACHMENT0,
                              GL_RENDERBUFFER, m_renderbuffers2[RB_COLOUR]);
    glFramebufferRenderbuffer(GL_DRAW_FRAMEBUFFER, GL_DEPTH_ATTACHMENT,
                              GL_RENDERBUFFER, m_renderbuffers2[RB_DEPTH]);

    glBindFramebuffer(GL_DRAW_FRAMEBUFFER, 0);
    glBindRenderbuffer(GL_RENDERBUFFER, 0);
    glEnable(GL_DEPTH_TEST);
}

Display::~Display()
{
    SDL_GL_DeleteContext(m_glContext);
    SDL_DestroyWindow(m_window);
    SDL_Quit();
}

void Display::Clear(float r, float g, float b, float a){
    glClearColor(r,g,b,a);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

void Display::SetFrameBuffer(int n){
    if (n==0)
        glBindFramebuffer(GL_FRAMEBUFFER, 0);
    if (n==1)
        glBindFramebuffer(GL_FRAMEBUFFER, m_framebuffer);
    if (n==2)
        glBindFramebuffer(GL_FRAMEBUFFER, m_framebuffer2);
    glViewport(0,0,256,256);
}

void Display::CopyFrameBuffer(){
    glBindFramebuffer(GL_DRAW_FRAMEBUFFER, 0);
    glViewport(0,0,m_width,m_height);
    //glClearColor(0.0,0.0,1.0,1.0);
    //glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glBlitFramebuffer(0,0,255,255,0,0,m_width-1,m_height-1,
                      GL_COLOR_BUFFER_BIT, GL_NEAREST);
};

bool Display::IsClosed(){
    return m_isClosed;
}

void Display::Update(){
    SDL_GL_SwapWindow(m_window);
//    SDL_Event e;
//    while(SDL_PollEvent(&e)){
//        if(e.type == SDL_QUIT){
//            m_isClosed = true;
//        }
//    }
}
