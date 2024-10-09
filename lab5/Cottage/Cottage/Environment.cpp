#include "Environment.h"

Environment::Environment()
	: m_ground({ -8, -8, 0 }, { 0, 16, 0 }, { 16, 0, 0 })
{
	m_ground.SetTextureScale(10);
}

void Environment::Draw() const
{
	m_ground.Draw();
}

void Environment::SetGroundTexture(TextureHandler const& texture)
{
	m_ground.SetTexture(texture);
}
