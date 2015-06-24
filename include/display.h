#ifndef DISPLAY_H
#define DISPLAY_H
#include <SDL2/SDL.h>
#include <string>
#include "GL/glew.h"

class Display
{
    public:
        Display(int width, int height, const std::string& title);
        void Clear(float r, float g, float b, float a);
        void Update();
        void SetFrameBuffer(int n);
        void CopyFrameBuffer();
        bool IsClosed();
        virtual ~Display();

    private:
        SDL_Window* m_window;
        SDL_GLContext m_glContext;
        bool m_isClosed;
        enum {RB_COLOUR, RB_DEPTH, NUM_RENDER_BUFFERS};
        GLuint m_renderbuffers[NUM_RENDER_BUFFERS];
        GLuint m_renderbuffers2[NUM_RENDER_BUFFERS];
        GLuint m_framebuffer;
        GLuint m_framebuffer2;
        int m_width, m_height;

};

#endif // DISPLAY_H
