#pragma once

#define GLM_ENABLE_EXPERIMENTAL
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtx/rotate_vector.hpp>

#include "../libgl/GLApplication.h"
#include "../libgl/Shaders.h"
#include "CSurface.h"
#include "../libgl/pch.h"

class CMyApplication : public CGLApplication
{
public:
	CMyApplication(char const* title, int width, int height);

protected:
	void OnInit() override;
	void OnDisplay() override;
	void OnReshape(int width, int height) override;

	virtual void OnMouse(int button, int state, int x, int y);
	virtual void OnMotion(int x, int y);

private:
	void InitShaders();
	
	// ������� ������ ������ ������ �������� �� �������� ����
	static void RotateCamera(GLfloat rotateX, GLfloat rotateY);

	// ���������� ���������������� ������� �������������-����
	static void NormalizeModelViewMatrix(void);

	// ����, ����������������� � ��������� ����� ������ ����
	bool m_leftButtonPressed;

	// ������ ���������� ������� ����
	int m_mouseX;
	int m_mouseY;

	int m_windowWidth;
	int m_windowHeight;

	CSurface m_surface;
	CProgram m_program;
};