
#include "TexturedRectangle.h"

TexturedRectangle::TexturedRectangle(glm::vec3 position, glm::vec3 direction1, glm::vec3 direction2)
	: m_position(position)
	, m_direction1(direction1)
	, m_direction2(direction2)
{
}

void TexturedRectangle::Draw() const
{
	glEnable(GL_COLOR_MATERIAL);
	glColorMaterial(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE);
	glm::vec4 specularColor = { 0.3f, 0.3f, 0.3f, 1 };
	glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, glm::value_ptr(specularColor));
	glMaterialf(GL_FRONT_AND_BACK, GL_SHININESS, 0.5);

	m_texture.Bind();

	glBegin(GL_QUADS);
	{
		auto const normal = glm::normalize(glm::cross(m_direction2, m_direction1));
		glNormal3fv(glm::value_ptr(normal));

		glTexCoord2f(0, 0);
		glVertex3fv(glm::value_ptr(m_position));

		glTexCoord2f(0, m_textureScale);
		glVertex3fv(glm::value_ptr(m_position + m_direction2));

		glTexCoord2f(m_textureScale, m_textureScale);
		glVertex3fv(glm::value_ptr(m_position + m_direction1 + m_direction2));

		glTexCoord2f(m_textureScale, 0);
		glVertex3fv(glm::value_ptr(m_position + m_direction1));
	}
	glEnd();
}

void TexturedRectangle::SetTexture(TextureHandler texture)
{
	m_texture = texture;
}

void TexturedRectangle::SetTextureScale(float scale)
{
	m_textureScale = scale;
}
