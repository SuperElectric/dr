#ifndef TEXTURE_H
#define TEXTURE_H

#include <string>
#include <GL/glew.h>

class Texture
{
    public:
        Texture(const char* fileName);
        void Bind(int i, char* samplerName);
        virtual ~Texture();
    //private:
        Texture(const Texture& texture) {}
        void operator=(const Texture& texture) {}
        GLuint m_texture;
};

#endif
