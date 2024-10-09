#pragma once

#include "common_inc.h"

class TextureHandler
{
public:
	TextureHandler() = default;
	TextureHandler(GLuint texture);

	void Delete() noexcept;
	void Bind() const;

	operator GLuint() const;

private:
	GLuint m_texture = 0;
};