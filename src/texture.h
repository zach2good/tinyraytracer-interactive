#pragma once

#include <glad/glad.h>

class Texture
{
public:
    Texture()
    {
        glGenTextures(1, &id);
        glBindTexture(GL_TEXTURE_2D, id);

        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

        glBindTexture(GL_TEXTURE_2D, 0);
    }

    ~Texture()
    {
        unbind();
        glDeleteTextures(1, &id);
    }

    GLuint get_id()
    {
        return id;
    }

    void upload(int width, int height, void* data)
    {
        bind();
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_FLOAT, data);
        unbind();
    }

private:

    void bind()
    {
        glBindTexture(GL_TEXTURE_2D, id);
    }

    void unbind()
    {
        glBindTexture(GL_TEXTURE_2D, 0);
    }

    GLuint id;
};