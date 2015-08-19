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
        void SetRenderBuffer(int n,int i);
        void CopyFrameBuffer();
        void ShowTexture(int n, int i);
        void SaveTexture(int n, int i, float* pixels);
        bool IsClosed();
        virtual ~Display();
        int render_width;
        int render_height;

    private:
        SDL_Window* m_window;
        SDL_GLContext m_glContext;
        bool m_isClosed;
        GLuint* m_framebuffers;
        GLuint* m_depthbuffers;
        GLuint** m_texturerenderbuffers;
        int m_width;
        int m_height;
        int NUM_FRAME_BUFFERS;
        int NUM_COLOUR_TEXTURE_BUFFERS;

        // Quad Stuff:
        GLuint m_quad_vertexarray;
        GLuint m_quad_vertexbuffer;
        GLuint m_quad_program;
        GLuint m_quad_fragshader;
        GLuint m_quad_vertshader;
        GLuint m_quad_texturesampler;


};

#endif // DISPLAY_H
