#ifndef DISPLAY_H
#define DISPLAY_H
#include <SDL2/SDL.h>
#include <string>
#include "GL/glew.h"

class Display
{
    public:
        Display(int width, int height, const std::string& title,
                 const int numberOfFrameBuffers,
                 const int numberOfColourRenderBuffers);
        void Clear(float r, float g, float b, float a);
        void Update();
        void SetFrameBuffer(int n=-1);
        void SetRenderBuffer(int n, int i);
        void CopyFrameBuffer();
        void SaveRenderBuffer(float* pixels);
        bool IsClosed();
        virtual ~Display();
        int render_width;
        int render_height;

    private:
        SDL_Window* m_window;
        SDL_GLContext m_glContext;
        bool m_isClosed;
        GLuint** m_renderbuffers;
        GLuint* m_framebuffers;
        int m_width;
        int m_height;
        int NUM_FRAME_BUFFERS;
        int NUM_COLOUR_RENDER_BUFFERS;

};

#endif // DISPLAY_H
