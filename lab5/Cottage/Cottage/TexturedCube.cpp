#include "TexturedCube.h"

TexturedCube::TexturedCube(glm::vec3 position, float size)
	: m_position(position)
	, m_size(size)
{
	for (std::size_t i = 0; i < 8; ++i)
	{
		for (std::size_t j = 0; j < 3; ++j)
		{
			m_vertices[i][j] = (BASE_VERTICES[i][j] + 1) * m_size * 0.5f + m_position[j];
		}
	}
}

void TexturedCube::Draw() const
{
	glEnable(GL_COLOR_MATERIAL);
	glColorMaterial(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE);
	glm::vec4 specularColor = { 0.3f, 0.3f, 0.3f, 1 };
	glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, glm::value_ptr(specularColor));
	glMaterialf(GL_FRONT_AND_BACK, GL_SHININESS, 0.5);

	for (unsigned faceIndex = 0; faceIndex < FACE_COUNT; ++faceIndex)
	{
		CubeFace const& face = FACES[faceIndex];

		m_texture.Bind();

		glBegin(GL_QUADS);
		{
			auto const p0 = glm::make_vec3(m_vertices[face.vertex0].data());
			auto const p1 = glm::make_vec3(m_vertices[face.vertex1].data());
			auto const p2 = glm::make_vec3(m_vertices[face.vertex2].data());
			auto const p3 = glm::make_vec3(m_vertices[face.vertex3].data());

			auto const v01 = p1 - p0;
			auto const v02 = p2 - p0;
			auto const normal = glm::normalize(glm::cross(v01, v02));

			glNormal3fv(glm::value_ptr(normal));

			glTexCoord2f(face.tex0s, face.tex0t);
			glVertex3fv(glm::value_ptr(p0));

			glTexCoord2f(face.tex1s, face.tex1t);
			glVertex3fv(glm::value_ptr(p1));

			glTexCoord2f(face.tex2s, face.tex2t);
			glVertex3fv(glm::value_ptr(p2));

			glTexCoord2f(face.tex3s, face.tex3t);
			glVertex3fv(glm::value_ptr(p3));
		}
		glEnd();
	}
}

void TexturedCube::SetTexture(TextureHandler const& texture)
{
	m_texture = texture;
}