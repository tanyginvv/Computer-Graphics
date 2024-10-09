#pragma once

#include "TexturedCube.h"
#include "TexturedRectangle.h"
#include "TexturedTriangle.h"

class Cottage
{
public:
	Cottage();

	void Draw() const;

	void SetWallTexture(TextureHandler const& texture);
	void SetWindowTexture(TextureHandler const& texture);
	void SetDoorTextures(TextureHandler const& doorTop, TextureHandler const& doorBottom);
	void SetGarageDoorTexture(TextureHandler const& texture);
	void SetRoofTexture(TextureHandler const& texture);

private:
	TexturedCube m_section1;
	TexturedCube m_section2;
	TexturedCube m_section3;

	TexturedRectangle m_window1;
	TexturedRectangle m_window2;
	TexturedRectangle m_window3;
	TexturedRectangle m_window4;

	TexturedRectangle m_doorTop;
	TexturedRectangle m_doorBottom;

	TexturedRectangle m_garageDoor;

	TexturedRectangle m_roofFront;
	TexturedRectangle m_roofBack;

	TexturedTriangle m_roofLeft;
	TexturedTriangle m_roofRight;
};