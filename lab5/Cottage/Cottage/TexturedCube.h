#pragma once

#include "TextureHandler.h"

class TexturedCube
{
public:
	TexturedCube(glm::vec3 position, float size);

	void Draw() const;

	void SetTexture(TextureHandler const& texture);

private:
	struct CubeFace
	{
		unsigned char vertex0;
		unsigned char vertex1;
		unsigned char vertex2;
		unsigned char vertex3;
		GLfloat tex0s, tex0t;
		GLfloat tex1s, tex1t;
		GLfloat tex2s, tex2t;
		GLfloat tex3s, tex3t;
	};

	static float constexpr BASE_VERTICES[8][3] = {
		{ -1, -1, -1 }, // 0
		{ +1, -1, -1 }, // 1
		{ +1, +1, -1 }, // 2
		{ -1, +1, -1 }, // 3
		{ -1, -1, +1 }, // 4
		{ +1, -1, +1 }, // 5
		{ +1, +1, +1 }, // 6
		{ -1, +1, +1 }, // 7
	};

	static constexpr CubeFace FACES[] = {
		{ 4, 7, 3, 0, 0, 0, 4, 0, 4, 4, 0, 4 }, // ãðàíü x<0
		{ 5, 1, 2, 6, 0, 4, 0, 0, 4, 0, 4, 4 }, // ãðàíü x>0
		{ 4, 0, 1, 5, 0, 4, 0, 0, 4, 0, 4, 4 }, // ãðàíü y<0
		{ 7, 6, 2, 3, 0, 0, 4, 0, 4, 4, 0, 4 }, // ãðàíü y>0
		{ 0, 3, 2, 1, 0, 4, 0, 0, 4, 0, 4, 4 }, // ãðàíü z<0
		{ 4, 5, 6, 7, 0, 0, 4, 0, 4, 4, 0, 4 }, // ãðàíü z>0
	};
	static unsigned constexpr FACE_COUNT = sizeof(FACES) / sizeof(*FACES);

	glm::vec3 m_position = { 0, 0, 0 };
	float m_size = 1;
	std::array<std::array<float, 3>, 8> m_vertices;

	TextureHandler m_texture;
};