#include "TextureHandler.h"

TextureHandler::TextureHandler(GLuint texture)
	: m_texture(texture)
{
}

void TextureHandler::Delete() noexcept
{
	if (m_texture)
	{
		glDeleteTextures(1, &m_texture);
	}
}

TextureHandler::operator GLuint() const
{
	return m_texture;
}

void TextureHandler::Bind() const
{
	if (m_texture)
	{
		glBindTexture(GL_TEXTURE_2D, m_texture);
	}
}