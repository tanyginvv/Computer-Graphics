#pragma once

#include "TextureHandler.h"

class TexturedTriangle
{
public:
	TexturedTriangle(glm::vec3 vertex1, glm::vec3 vertex2, glm::vec3 vertex3);

	void Draw() const;

	void SetTexture(TextureHandler const& texture);
	void SetTextureScale(float scale);

private:
	glm::vec3 m_vertex1;
	glm::vec3 m_vertex2;
	glm::vec3 m_vertex3;

	float m_textureScale = 1;

	TextureHandler m_texture;
};