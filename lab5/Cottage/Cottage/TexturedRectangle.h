#pragma once

#include "TextureHandler.h"

class TexturedRectangle
{
public:
	TexturedRectangle(glm::vec3 position, glm::vec3 direction1, glm::vec3 direction2);

	void Draw() const;

	void SetTexture(TextureHandler texture);
	void SetTextureScale(float scale);

private:
	glm::vec3 m_position = { 0, 0, 0 };
	glm::vec3 m_direction1 = { 1, 0, 0 };
	glm::vec3 m_direction2 = { 0, 1, 0 };

	float m_textureScale = 1;

	TextureHandler m_texture;
};