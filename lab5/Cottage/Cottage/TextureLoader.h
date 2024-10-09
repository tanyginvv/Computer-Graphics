#pragma once

#include "TextureHandler.h"

class TextureLoader
{
public:
	TextureHandler LoadTexture(std::string const& filename, GLuint textureName = 0, GLuint level = 0) const;

private:
	GLenum m_minFilter = GL_NEAREST;
	GLenum m_magFilter = GL_NEAREST;
	GLenum m_wrapS = GL_REPEAT;
	GLenum m_wrapT = GL_REPEAT;
};