#ifndef DISPLAY_H
#define DISPLAY_H
#include <SDL2/SDL.h>
#include <string>
#include "GL/glew.h"

class Display
{
    public:
        Display(int width, int height, const std::string& title,
                const int numberOfFrameBuffers=0);
        void Clear(float r, float g, float b, float a);
        void Update();
        void SetFrameBuffer(int n=-1);
        void CopyFrameBuffer();
        bool IsClosed();
        virtual ~Display();

    private:
        SDL_Window* m_window;
        SDL_GLContext m_glContext;
        bool m_isClosed;
        enum {RB_COLOUR, RB_DEPTH, NUM_RENDER_BUFFERS};
        GLuint** m_renderbuffers;
        GLuint* m_framebuffers;
        int m_width;
        int m_height;
        int m_render_width;
        int m_render_height;
        int NUM_FRAME_BUFFERS;

};

#endif // DISPLAY_H
