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

// ������� ������ ������ ������ �������� �� �������� ����
void CMyApplication::RotateCamera(GLfloat rotateX, GLfloat rotateY)
{
	// ��������� ������� �������� ������� �������������-����
	GLfloat modelView[16];
	glGetFloatv(GL_MODELVIEW_MATRIX, &modelView[0]);

	// ��������� ����������� ������������ ���� ������ � 3� ������������
	// ��� ������������ ����� ������� �������������-����
	CVector3f xAxis(modelView[0], modelView[4], modelView[8]);
	CVector3f yAxis(modelView[1], modelView[5], modelView[9]);

	// ������������ ������ ���� x � y ������
	glRotatef(rotateX, xAxis.x, xAxis.y, xAxis.z);
	glRotatef(rotateY, yAxis.x, yAxis.y, yAxis.z);

	// � ���� ��������� ������ ����� ��������� �����������, �������,
	// ������������ ����� ������ �������� ��������
	// ��� �� ����������� ����� ������ ����������� ������� �������������-����
	// �������� �� ����������������
	NormalizeModelViewMatrix();
}

void CMyApplication::OnMotion(int x, int y)
{
	// ���� ������ ����� ������ ����
	if (m_leftButtonPressed)
	{
		// ��������� �������� ������� ����
		int dx = x - m_mouseX;
		int dy = y - m_mouseY;

		// ��������� ���� �������� ������ ���� Y � X ��� ������� ���������
		// �� �������� ���� �� ���� X � Y
		GLfloat rotateX = GLfloat(dy) * 180 / m_windowHeight;
		GLfloat rotateY = GLfloat(dx) * 180 / m_windowWidth;
		RotateCamera(rotateX, rotateY);

		// ��������� ������� ���������� ����
		m_mouseX = x;
		m_mouseY = y;

		// ���������� ����������� ����
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
	// ������� �� ����� ������ ����
	if (button == GLUT_LEFT_BUTTON)
	{
		// ��������� ��������� ����� ������ ����
		m_leftButtonPressed = (state == GLUT_DOWN);
		// ��������� ���������� ����
		m_mouseX = x;
		m_mouseY = y;
	}
}

void CMyApplication::OnInit()
{
	glEnable(GL_DEPTH_TEST);
	glClearColor(1, 1, 1, 1);

	// �������� ���� � ������ ��� ���������
	glEnable(GL_LIGHTING);
	glEnable(GL_LIGHT0);
	CDirectLight light;
	light.SetSpecularIntensity(0.3f, 0.3f, 0.3f);
	light.SetLight(GL_LIGHT0);

	// ������ ��������� ���������
	CMaterial material;
	material.SetSpecular(0.3f, 0.3f, 0.3f);
	material.SetShininess(20);
	material.SetDiffuse(0.8f, 0.8f, 0.2f);
	material.Activate();

	// ��������� ���������� ������
	CModelLoader loader;
	loader.Load3dsFile("models/Board.3ds", m_board);
	loader.Load3dsFile("models/Queen.3ds", m_queen);
	loader.Load3dsFile("models/King.3ds", m_king);
	loader.Load3dsFile("models/Knight.3ds", m_knight);
	loader.Load3dsFile("models/Bishop.3ds", m_bishop);
	loader.Load3dsFile("models/Rook.3ds", m_rook);
	loader.Load3dsFile("models/Pawn.3ds", m_pawn);

	// ���������� �������������� ���� ������ ������
	CBoundingBox const& boardBoundingBox = m_board.GetBoundingBox();

	if (boardBoundingBox.IsEmpty())
	{
		throw std::runtime_error("Model is empty. Nothing to render");
	}

	// ����� bounding box-� ������
	CVector3f boardCenter = boardBoundingBox.GetCenter();
	// ����� ��������� bounding box-�
	float boardDiagonal = boardBoundingBox.GetSize().GetLength();
	// ���������� ������ �� ������ �� ����������, ������ ���������
	// bounding box-�
	CVector3f cameraPosition = boardCenter + CVector3f(boardDiagonal, 0, 0);

	// ������ ��������� ������
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

	// ������ ��������� ���������, ������������� �� ��������� ���
	// ������, ��� ������� �������� �� ��� ������
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

	// ��������� ����������� ������ ���������� ������� ����
	double aspect = double(width) / double(height);

	float zFar = m_board.GetBoundingBox().GetSize().GetLength() * 2;
	float zNear = zFar * 0.01f;
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(FIELD_OF_VIEW, aspect, zNear, zFar);
	glMatrixMode(GL_MODELVIEW);
}

// ���������� ���������������� ������� �������������-����
void CMyApplication::NormalizeModelViewMatrix(void)
{
	GLfloat modelView[16];
	glGetFloatv(GL_MODELVIEW_MATRIX, modelView);
	/*
	���������������� - ���������� ������������ ���� � ��������� ����� (������������)
	� �������� ������������������ (���������������)
	������� ����� ����� ��� ������ ������������ ������������ ����
	� ���������� ������������
	*/
	CVector3f xAxis(modelView[0], modelView[4], modelView[8]);
	xAxis.Normalize();
	CVector3f yAxis(modelView[1], modelView[5], modelView[9]);
	yAxis.Normalize();

	// ��� Z ��������� ����� ��������� ������������ X � Y
	// Z ����� ��������������� ��������� �������� X � Y
	CVector3f zAxis = Cross(xAxis, yAxis);
	// � ����� ��������� �����
	zAxis.Normalize();
	// �� �� ����� ����������� � ����� x � y
	xAxis = Cross(yAxis, zAxis);
	xAxis.Normalize();
	yAxis = Cross(zAxis, xAxis);
	yAxis.Normalize();

	// ��������� ������� ������������ ���� ������� � ������
	modelView[0] = xAxis.x;
	modelView[4] = xAxis.y, modelView[8] = xAxis.z;
	modelView[1] = yAxis.x;
	modelView[5] = yAxis.y, modelView[9] = yAxis.z;
	modelView[2] = zAxis.x;
	modelView[6] = zAxis.y, modelView[10] = zAxis.z;

	// � ��������� ������� ������������-����
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
