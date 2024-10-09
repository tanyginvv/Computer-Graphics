#pragma once

#include "common_inc.h"

class DirectLight
{
public:
	explicit DirectLight(glm::vec3 direction);

	void SetDiffuseColor(glm::vec4 color);
	void SetAmbientColor(glm::vec4 color);
	void SetSpecularColor(glm::vec4 color);

	void Apply(GLenum light) const;

private:
	glm::vec3 m_direction;
	glm::vec4 m_diffuseColor{ 0.8f, 0.8f, 0.8f, 1.0f };
	glm::vec4 m_ambientColor{ 0.2f, 0.2f, 0.2f, 1.0f };
	glm::vec4 m_specularColor{ 0.5f, 0.5f, 0.5f, 1.0f };
};