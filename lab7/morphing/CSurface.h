#pragma once

#include "../libgl/pch.h"

struct Vertex
{
	float x, y, z;
};

class CSurface
{
public:
	CSurface(
		int columns, int rows, float xMin, float xMax, float yMin, float yMax);
	void Draw() const;
	virtual ~CSurface(void);

private:
	mutable GLuint m_displayList;

	int m_columns;
	int m_rows;

	float m_xMin;
	float m_xMax;
	float m_yMin;
	float m_yMax;
};