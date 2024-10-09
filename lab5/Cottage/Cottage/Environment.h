#pragma once

#include "TexturedRectangle.h"

class Environment
{
public:
	Environment();

	void Draw() const;

	void SetGroundTexture(TextureHandler const& texture);

private:
	TexturedRectangle m_ground;
};