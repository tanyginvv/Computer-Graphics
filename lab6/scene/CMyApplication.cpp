#define _USE_MATH_DEFINES
#include <cmath>
#include "CMyApplication.h"
#include <GL/glut.h>
#include "CModelLoader.h"
#include "CDirectLight.h"
#include <iostream>

constexpr double FIELD_OF_VIEW = 50;

CMyApplication::CMyApplication(const char* title, int width, int height)
	: CGLApplication(title, width, height)
	, m_leftButtonPressed(false) 
	, m_mouseX(0)
	, m_mouseY(0)
	, m_windowHeight(height)
	, m_windowWidth(width)
{
}

// Вращаем камеру вокруг начала кординат на заданный угол
void CMyApplication::RotateCamera(GLfloat rotateX, GLfloat rotateY)
{
	// Извлекаем текущее значение матрицы моделирования-вида
	GLfloat modelView[16];
	glGetFloatv(GL_MODELVIEW_MATRIX, &modelView[0]);

	// Извлекаем направления координатных осей камеры в 3д пространстве
	// как коэффициенты строк матрицы моделирования-вида
	CVector3f xAxis(modelView[0], modelView[4], modelView[8]);
	CVector3f yAxis(modelView[1], modelView[5], modelView[9]);

	// Поворачиваем вокруг осей x и y камеры
	glRotatef(rotateX, xAxis.x, xAxis.y, xAxis.z);
	glRotatef(rotateY, yAxis.x, yAxis.y, yAxis.z);

	// В ходе умножения матриц могут возникать погрешности, которые,
	// накапливаясь могут сильно искажать картинку
	// Для их компенсации после каждой модификации матрицы моделирования-вида
	// проводим ее ортонормирование
	NormalizeModelViewMatrix();
}

void CMyApplication::OnMotion(int x, int y)
{
	// Если нажата левая кнопка мыши
	if (m_leftButtonPressed)
	{
		// Вычисляем смещение курсора мыши
		int dx = x - m_mouseX;
		int dy = y - m_mouseY;

		// Вычисляем угол поворота вокруг осей Y и X как линейно зависящие
		// от смещения мыши по осям X и Y
		GLfloat rotateX = GLfloat(dy) * 180 / m_windowHeight;
		GLfloat rotateY = GLfloat(dx) * 180 / m_windowWidth;
		RotateCamera(rotateX, rotateY);

		// Сохраняем текущие координаты мыши
		m_mouseX = x;
		m_mouseY = y;

		// Инициируем перерисовку окна
		PostRedisplay();
	}
}

void CMyApplication::OnKeyboard(unsigned char key, int x, int y)
{
	if (key == ' ')
	{
		ShowAnimation();
	}
}

void CMyApplication::OnMouse(int button, int state, int x, int y)
{
	// Событие от левой кнопки мыши
	if (button == GLUT_LEFT_BUTTON)
	{
		// Сохраняем состояние левой кнопки мыши
		m_leftButtonPressed = (state == GLUT_DOWN);
		// Сохраняем координаты мыши
		m_mouseX = x;
		m_mouseY = y;
	}
}

void CMyApplication::OnInit()
{
	glEnable(GL_DEPTH_TEST);
	glClearColor(1, 1, 1, 1);

	// Включаем свет и задаем его параметры
	glEnable(GL_LIGHTING);
	glEnable(GL_LIGHT0);
	CDirectLight light;
	light.SetSpecularIntensity(0.3f, 0.3f, 0.3f);
	light.SetLight(GL_LIGHT0);

	// Задаем параметры материала
	CMaterial material;
	material.SetSpecular(0.3f, 0.3f, 0.3f);
	material.SetShininess(20);
	material.SetDiffuse(0.8f, 0.8f, 0.2f);
	material.Activate();

	// Загружаем трехмерную модель
	CModelLoader loader;
	loader.Load3dsFile("models/Board.3ds", m_board);
	loader.Load3dsFile("models/Queen.3ds", m_queen);
	loader.Load3dsFile("models/King.3ds", m_king);
	loader.Load3dsFile("models/Knight.3ds", m_knight);
	loader.Load3dsFile("models/Bishop.3ds", m_bishop);
	loader.Load3dsFile("models/Rook.3ds", m_rook);
	loader.Load3dsFile("models/Pawn.3ds", m_pawn);

	// Определяем ограничивающий блой данной модели
	CBoundingBox const& boardBoundingBox = m_board.GetBoundingBox();

	if (boardBoundingBox.IsEmpty())
	{
		throw std::runtime_error("Model is empty. Nothing to render");
	}

	// Центр bounding box-а модели
	CVector3f boardCenter = boardBoundingBox.GetCenter();
	// Длина диагонали bounding box-а
	float boardDiagonal = boardBoundingBox.GetSize().GetLength();
	// Отодвигаем камеру от центра на расстояние, равное диагонали
	// bounding box-а
	CVector3f cameraPosition = boardCenter + CVector3f(boardDiagonal, 0, 0);

	// Задаем параметры камеры
	glLoadIdentity();
	gluLookAt(
		cameraPosition.x, cameraPosition.y, cameraPosition.z,
		boardCenter.x, boardCenter.y, boardCenter.z,
		0, 1, 0);
}

void CMyApplication::OnDisplay()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glClearColor(0.2f, 0.2f, 0.2f, 1);

	glColor3ub(0, 0, 0);
	glEnable(GL_CULL_FACE);

	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

	// Задаем параметры материала, используемого по умолчанию для
	// граней, для которых материал не был указан
	CMaterial material;
	material.SetSpecular(0.3f, 0.3f, 0.3f);
	material.SetShininess(20);
	material.SetDiffuse(0.3f, 0.3f, 0.7);
	material.Activate();
	
	m_renderer.RenderModel(m_board);

	DrawWhiteTeam();
	DrawBlackTeam();
}

void CMyApplication::OnReshape(int width, int height)
{
	glViewport(0, 0, width, height);

	// Вычисляем соотношение сторон клиентской области окна
	double aspect = double(width) / double(height);

	float zFar = m_board.GetBoundingBox().GetSize().GetLength() * 2;
	float zNear = zFar * 0.01f;
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(FIELD_OF_VIEW, aspect, zNear, zFar);
	glMatrixMode(GL_MODELVIEW);
}

// Производим ортонормирование матрицы моделирования-вида
void CMyApplication::NormalizeModelViewMatrix(void)
{
	GLfloat modelView[16];
	glGetFloatv(GL_MODELVIEW_MATRIX, modelView);
	/*
	Ортонормирование - приведение координатных осей к единичной длине (нормирование)
	и взаимной перпендикулярности (ортогонализация)
	Достичь этого можно при помощи нормализации координатных осей
	и векторного произведения
	*/
	CVector3f xAxis(modelView[0], modelView[4], modelView[8]);
	xAxis.Normalize();
	CVector3f yAxis(modelView[1], modelView[5], modelView[9]);
	yAxis.Normalize();

	// Ось Z вычисляем через векторное произведение X и Y
	// Z будет перпендикулярна плоскости векторов X и Y
	CVector3f zAxis = Cross(xAxis, yAxis);
	// И иметь единичную длину
	zAxis.Normalize();
	// То же самое проделываем с осями x и y
	xAxis = Cross(yAxis, zAxis);
	xAxis.Normalize();
	yAxis = Cross(zAxis, xAxis);
	yAxis.Normalize();

	// Сохраняем вектора координатных осей обратно в массив
	modelView[0] = xAxis.x;
	modelView[4] = xAxis.y, modelView[8] = xAxis.z;
	modelView[1] = yAxis.x;
	modelView[5] = yAxis.y, modelView[9] = yAxis.z;
	modelView[2] = zAxis.x;
	modelView[6] = zAxis.y, modelView[10] = zAxis.z;

	// И загружаем матрицу моделирвания-вида
	glLoadMatrixf(modelView);
}

void CMyApplication::DrawWhiteTeam() const
{
	// pawn
	glPushMatrix();
	glScalef(0.025f, 0.025f, 0.025f);
	glTranslatef(40.0f, 8.0f, 0.0f);
	m_renderer.RenderModel(m_pawn);
	glPopMatrix();
	// pawn
	glPushMatrix();
	glScalef(0.025f, 0.025f, 0.025f);
	glTranslatef(40.0f, 24.0f, 0.0f);
	m_renderer.RenderModel(m_pawn);
	glPopMatrix();
	// pawn
	glPushMatrix();
	glScalef(0.025f, 0.025f, 0.025f);
	glTranslatef(40.0f, 40.0f, 0.0f);
	m_renderer.RenderModel(m_pawn);
	glPopMatrix();
	// pawn
	glPushMatrix();
	glScalef(0.025f, 0.025f, 0.025f);
	glTranslatef(40.0f, 56.0f, 0.0f);
	m_renderer.RenderModel(m_pawn);
	glPopMatrix();
	// pawn
	glPushMatrix();
	glScalef(0.025f, 0.025f, 0.025f);
	glTranslatef(40.0f, -8.0f, 0.0f);
	m_renderer.RenderModel(m_pawn);
	glPopMatrix();
	// pawn
	glPushMatrix();
	glScalef(0.025f, 0.025f, 0.025f);
	glTranslatef(40.0f, -24.0f, 0.0f);
	m_renderer.RenderModel(m_pawn);
	glPopMatrix();
	// pawn
	glPushMatrix();
	glScalef(0.025f, 0.025f, 0.025f);
	glTranslatef(40.0f, -40.0f, 0.0f);
	m_renderer.RenderModel(m_pawn);
	glPopMatrix();
	// pawn
	glPushMatrix();
	glScalef(0.025f, 0.025f, 0.025f);
	glTranslatef(40.0f, -56.0f, 0.0f);
	m_renderer.RenderModel(m_pawn);
	glPopMatrix();

	// rook
	glPushMatrix();
	glScalef(0.025f, 0.025f, 0.025f);
	glTranslatef(56.0f, -56.0f, 0.0f);
	m_renderer.RenderModel(m_rook);
	glPopMatrix();
	// rook
	glPushMatrix();
	glScalef(0.025f, 0.025f, 0.025f);
	glTranslatef(56.0f, 56.0f, 0.0f);
	m_renderer.RenderModel(m_rook);
	glPopMatrix();

	// knight
	glPushMatrix();
	glScalef(0.025f, 0.025f, 0.025f);
	glTranslatef(56.0f, -40.0f, 0.0f);
	m_renderer.RenderModel(m_knight);
	glPopMatrix();
	// knight
	glPushMatrix();
	glScalef(0.025f, 0.025f, 0.025f);
	glTranslatef(56.0f, 40.0f, 0.0f);
	m_renderer.RenderModel(m_knight);
	glPopMatrix();

	// bishop
	glPushMatrix();
	glScalef(0.025f, 0.025f, 0.025f);
	glTranslatef(56.0f, -24.0f, 0.0f);
	m_renderer.RenderModel(m_bishop);
	glPopMatrix();
	// bishop
	glPushMatrix();
	glScalef(0.025f, 0.025f, 0.025f);
	glTranslatef(56.0f, 24.0f, 0.0f);
	m_renderer.RenderModel(m_bishop);
	glPopMatrix();

	// queen
	glPushMatrix();
	glScalef(0.025f, 0.025f, 0.025f);
	glTranslatef(56.0f, -8.0f, 0.0f);
	glColor3f(1.0f, 1.0f, 1.0f);
	m_renderer.RenderModel(m_queen);
	glPopMatrix();
	// king
	glPushMatrix();
	glScalef(0.025f, 0.025f, 0.025f);
	glTranslatef(56.0f, 8.0f, 0.0f);
	m_renderer.RenderModel(m_king);
	glPopMatrix();
}

void CMyApplication::DrawBlackTeam() const
{
	// pawn
	glPushMatrix();
	glScalef(0.025f, 0.025f, 0.025f);
	glTranslatef(-40.0f, 8.0f, 0.0f);
	m_renderer.RenderModel(m_pawn);
	glPopMatrix();
	// pawn
	glPushMatrix();
	glScalef(0.025f, 0.025f, 0.025f);
	glTranslatef(-40.0f, 24.0f, 0.0f);
	m_renderer.RenderModel(m_pawn);
	glPopMatrix();
	// pawn
	glPushMatrix();
	glScalef(0.025f, 0.025f, 0.025f);
	glTranslatef(-40.0f, 40.0f, 0.0f);
	m_renderer.RenderModel(m_pawn);
	glPopMatrix();
	// pawn
	glPushMatrix();
	glScalef(0.025f, 0.025f, 0.025f);
	glTranslatef(-40.0f, 56.0f, 0.0f);
	m_renderer.RenderModel(m_pawn);
	glPopMatrix();
	// pawn
	glPushMatrix();
	glScalef(0.025f, 0.025f, 0.025f);
	glTranslatef(-40.0f, -8.0f, 0.0f);
	m_renderer.RenderModel(m_pawn);
	glPopMatrix();
	// pawn
	glPushMatrix();
	glScalef(0.025f, 0.025f, 0.025f);
	glTranslatef(-40.0f, -24.0f, 0.0f);
	m_renderer.RenderModel(m_pawn);
	glPopMatrix();
	// pawn
	glPushMatrix();
	glScalef(0.025f, 0.025f, 0.025f);
	glTranslatef(-40.0f, -40.0f, 0.0f);
	m_renderer.RenderModel(m_pawn);
	glPopMatrix();
	// pawn
	glPushMatrix();
	glScalef(0.025f, 0.025f, 0.025f);
	glTranslatef(-40.0f, -56.0f, 0.0f);
	m_renderer.RenderModel(m_pawn);
	glPopMatrix();

	// rook
	glPushMatrix();
	glScalef(0.025f, 0.025f, 0.025f);
	glTranslatef(-56.0f, -56.0f, 0.0f);
	m_renderer.RenderModel(m_rook);
	glPopMatrix();
	// rook
	glPushMatrix();
	glScalef(0.025f, 0.025f, 0.025f);
	glTranslatef(-56.0f, 56.0f, 0.0f);
	m_renderer.RenderModel(m_rook);
	glPopMatrix();

	// knight
	glPushMatrix();
	glScalef(0.025f, 0.025f, 0.025f);
	glTranslatef(-56.0f, -40.0f, 0.0f);
	m_renderer.RenderModel(m_knight);
	glPopMatrix();
	// knight
	glPushMatrix();
	glScalef(0.025f, 0.025f, 0.025f);
	glTranslatef(-56.0f, 40.0f, 0.0f);
	m_renderer.RenderModel(m_knight);
	glPopMatrix();

	// bishop
	glPushMatrix();
	glScalef(0.025f, 0.025f, 0.025f);
	glTranslatef(-56.0f, -24.0f, 0.0f);
	m_renderer.RenderModel(m_bishop);
	glPopMatrix();
	// bishop
	glPushMatrix();
	glScalef(0.025f, 0.025f, 0.025f);
	glTranslatef(-56.0f, 24.0f, 0.0f);
	m_renderer.RenderModel(m_bishop);
	glPopMatrix();

	// queen
	glPushMatrix();
	glScalef(0.025f, 0.025f, 0.025f);
	glTranslatef(-56.0f, -8.0f, 0.0f);
	glColor3f(1.0f, 1.0f, 1.0f);
	m_renderer.RenderModel(m_queen);
	glPopMatrix();
	// king
	glPushMatrix();
	glScalef(0.025f, 0.025f, 0.025f);
	glTranslatef(-56.0f, 8.0f, 0.0f);
	m_renderer.RenderModel(m_king);
	glPopMatrix();
}

void CMyApplication::ShowFirstMove() const
{
}

void CMyApplication::ShowSecondMove() const
{
}

void CMyApplication::ShowThirdMove() const
{
}

void CMyApplication::ShowFourthMove() const
{
}

void CMyApplication::ShowAnimation() const
{
	std::cout << "1";
	ShowFirstMove();
	ShowSecondMove();
	ShowThirdMove();
	ShowFourthMove();
}
