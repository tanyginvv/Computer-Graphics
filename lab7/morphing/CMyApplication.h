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
	
	// ¬ращаем камеру вокруг начала кординат на заданный угол
	static void RotateCamera(GLfloat rotateX, GLfloat rotateY);

	// ѕроизводим ортонормирование матрицы моделировани€-вида
	static void NormalizeModelViewMatrix(void);

	// ‘лаг, свидетельствующий о состо€нии левой кнопки мыши
	bool m_leftButtonPressed;

	// —тарые координаты курсора мыши
	int m_mouseX;
	int m_mouseY;

	int m_windowWidth;
	int m_windowHeight;

	CSurface m_surface;
	CProgram m_program;
};