#include "CSurface.h"

CSurface::CSurface(
	int columns, int rows, float xMin, float xMax, float yMin, float yMax)
	: m_rows(rows)
	, m_columns(columns)
	, m_xMin(xMin)
	, m_xMax(xMax)
	, m_yMin(yMin)
	, m_yMax(yMax)
	, m_displayList(0)
{
}

void CSurface::Draw() const
{
	// ѕри первом обращении к данной функции запишем команды
	// рисовани€ поверхности в дисплейный список
	if (m_displayList == 0)
	{
		m_displayList = glGenLists(1);
		glNewList(m_displayList, GL_COMPILE);

		// вычисл€ем шаг узлов сетки
		const float dy = (m_yMax - m_yMin) / (m_rows - 1);
		const float dx = (m_xMax - m_xMin) / (m_columns - 1);

		const float d = (dx < dy) ? dx : dy;

		float y = m_yMin;
		// пробегаем по строкам сетки
		for (int row = 0; row < m_rows - 1; ++row, y += d)
		{
			// каждой строке будет соответствовать сво€ лента из треугольников
			glBegin(GL_TRIANGLE_STRIP);
			float x = m_xMin;

			// пробегаем по столбцам текущей строки
			for (int column = 0; column <= m_columns; ++column, x += d)
			{
				// вычисл€ем параметры вершины в узлах пары соседних вершин
				// ленты из треугольников
				Vertex v0 = { x, y + d, 0 };
				Vertex v1 = { x, y, 0 };

				// задаем координаты вершины на четной позиции
				glVertex3f(v0.x, v0.y, v0.z);

				// задаем координаты вершины на нечетной позиции
				glVertex3f(v1.x, v1.y, v1.z);
			}
			glEnd();
		}
		glEndList();
	}

	// ¬ызовем ранее записанный дисплейный список команд рисовани€ сетки
	glCallList(m_displayList);
}

// Ќе забыли удалить дисплейный список при разрушении экземпл€ра класса
CSurface::~CSurface(void)
{
	if (m_displayList != 0)
	{
		glDeleteLists(m_displayList, 1);
	}
}