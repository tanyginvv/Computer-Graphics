
#include "Cottage.h"

Cottage::Cottage()
	: m_section1({ -6, -2, 0 }, 4)
	, m_section2({ -2, -2, 0 }, 4)
	, m_section3({ 2, -1.5f, 0 }, 3)
	, m_window1({ -0.5f, -2.1f, 2 }, { 1, 0, 0 }, { 0, 0, -1 })
	, m_window2({ -4.5f, -2.1f, 2 }, { 1, 0, 0 }, { 0, 0, -1 })
	, m_window3({ -2.5f, -2.1f, 3.5f }, { 1, 0, 0 }, { 0, 0, -1 })
	, m_window4({ -0.5f, -5.1f, 2.0f }, { 1, 0, 0 }, { 0, 0, -1 })
	, m_doorTop({ -2.5f, -2.1f, 2 }, { 1, 0, 0 }, { 0, 0, -1 })
	, m_doorBottom({ -2.5f, -2.1f, 1 }, { 1, 0, 0 }, { 0, 0, -1 })
	, m_garageDoor({ 2.3f, -1.6f, 2.5f }, { 2.2f, 0, 0 }, { 0, 0, -2.5f })
	, m_roofFront({ -6, -2, 4 }, { 0, 2, 3 }, { 8, 0, 0 })
	, m_roofBack({ -6, 0, 7 }, { 0, 2, -3 }, { 8, 0, 0 })
	, m_roofLeft({ -6, 2, 4 }, { -6, -2, 4 }, { -6, 0, 7 })
	, m_roofRight({ 2, -2, 4 }, { 2, 2, 4 }, { 2, 0, 7 })
{
	m_roofFront.SetTextureScale(10);
	m_roofBack.SetTextureScale(10);
	m_roofLeft.SetTextureScale(10);
	m_roofRight.SetTextureScale(10);
}

void Cottage::Draw() const
{
	m_section1.Draw();
	m_section2.Draw();
	m_section3.Draw();

	m_window1.Draw();
	m_window2.Draw();
	m_window3.Draw();

	glRotatef(90.0f, 0.0f, 0.0f, 1.0f);
	m_window4.Draw();
	glRotatef(-90.0f, 0.0f, 0.0f, 1.0f);

	m_doorTop.Draw();
	m_doorBottom.Draw();

	m_garageDoor.Draw();

	m_roofFront.Draw();
	m_roofBack.Draw();
	m_roofLeft.Draw();
	m_roofRight.Draw();
}

void Cottage::SetWallTexture(TextureHandler const& texture)
{
	m_section1.SetTexture(texture);
}

void Cottage::SetWindowTexture(TextureHandler const& texture)
{
	m_window1.SetTexture(texture);
}

void Cottage::SetDoorTextures(TextureHandler const& doorTop, TextureHandler const& doorBottom)
{
	m_doorTop.SetTexture(doorTop);
	m_doorBottom.SetTexture(doorBottom);
}

void Cottage::SetGarageDoorTexture(TextureHandler const& texture)
{
	m_garageDoor.SetTexture(texture);
}

void Cottage::SetRoofTexture(TextureHandler const& texture)
{
	m_roofFront.SetTexture(texture);
	m_roofBack.SetTexture(texture);
	m_roofLeft.SetTexture(texture);
	m_roofRight.SetTexture(texture);
}