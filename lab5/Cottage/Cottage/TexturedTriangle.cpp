#include "TexturedTriangle.h"

TexturedTriangle::TexturedTriangle(glm::vec3 vertex1, glm::vec3 vertex2, glm::vec3 vertex3)
	: m_vertex1(vertex1)
	, m_vertex2(vertex2)
	, m_vertex3(vertex3)
{
}

void TexturedTriangle::Draw() const
{
	glEnable(GL_COLOR_MATERIAL);
	glColorMaterial(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE);
	glm::vec4 specularColor = { 0.3f, 0.3f, 0.3f, 1 };
	glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, glm::value_ptr(specularColor));
	glMaterialf(GL_FRONT_AND_BACK, GL_SHININESS, 0.5);

	m_texture.Bind();

	glBegin(GL_TRIANGLES);
	{
		auto const v01 = m_vertex2 - m_vertex1;
		auto const v02 = m_vertex3 - m_vertex1;
		auto const normal = glm::normalize(glm::cross(v01, v02));

		glNormal3fv(glm::value_ptr(normal));

		glTexCoord2f(0, 0);
		glVertex3fv(glm::value_ptr(m_vertex1));

		glTexCoord2f(m_textureScale, 0);
		glVertex3fv(glm::value_ptr(m_vertex2));

		glTexCoord2f(0.5f * m_textureScale, m_textureScale);
		glVertex3fv(glm::value_ptr(m_vertex3));
	}
	glEnd();
}

void TexturedTriangle::SetTexture(TextureHandler const& texture)
{
	m_texture = texture;
}

void TexturedTriangle::SetTextureScale(float scale)
{
	m_textureScale = scale;
}