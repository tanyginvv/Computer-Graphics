#include "CMyApplication.h"
#include "../libgl/ShaderLoader.h"
#include "../libgl/Vector3.h"

DWORD g_lastTick = GetTickCount();

CMyApplication::CMyApplication(char const* title, int width, int height)
	: CGLApplication(title, width, height)
	, m_surface(20, 10, -1, 1, -0.5, 0.5)
	, m_leftButtonPressed(false) 
	, m_mouseX(0)
	, m_mouseY(0)
	, m_windowHeight(height)
	, m_windowWidth(width)
{
}

void CMyApplication::OnInit()
{
	glEnable(GL_DEPTH_TEST);
	glDepthMask(GL_TRUE);
	glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	glEnable(GL_MULTISAMPLE);
	glClearColor(1, 1, 1, 1);

	glLoadIdentity();
	gluLookAt(
		2.5, 2.5, -1,
		0, 0, 0,
		1, 1, 0);

	InitShaders();
}

void CMyApplication::OnDisplay()
{
	DWORD currentTick = (DWORD)GetTickCount64();
	float time = (currentTick - g_lastTick) / 1000.0f;
	g_lastTick = currentTick;

	static float workTime = 0;
	workTime += time;

	if (workTime >= 3)
	{
		workTime -= 4;
	}

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glPushMatrix();

	glUseProgram(m_program);

	GLint timeUniform = glGetUniformLocation(m_program, "u_time");
	glUniform1f(timeUniform, workTime);

	m_surface.Draw();

	glUseProgram(0);
	glPopMatrix();

	PostRedisplay();
}

void CMyApplication::OnReshape(int width, int height)
{
	glViewport(0, 0, width, height);

	double aspect = double(width) / double(height);

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glOrtho(-aspect, +aspect, -1, 1, 1, 50);
	glMatrixMode(GL_MODELVIEW);
}

void CMyApplication::InitShaders()
{
	CShaderLoader loader;
	CShader vertexShader = loader.LoadShader(GL_VERTEX_SHADER, "shader.vsh");
	CShader fragmentShader = loader.LoadShader(GL_FRAGMENT_SHADER, "shader.fsh");

	vertexShader.Compile();
	fragmentShader.Compile();

	m_program.Create();
	m_program.AttachShader(vertexShader);
	m_program.AttachShader(fragmentShader);

	m_program.Link();
}

void CMyApplication::OnMouse(int button, int state, int x, int y)
{
	// —обытие от левой кнопки мыши
	if (button == GLUT_LEFT_BUTTON)
	{
		// —охран€ем состо€ние левой кнопки мыши
		m_leftButtonPressed = (state == GLUT_DOWN);
		// —охран€ем координаты мыши
		m_mouseX = x;
		m_mouseY = y;
	}
}

void CMyApplication::OnMotion(int x, int y)
{
	// ≈сли нажата лева€ кнопка мыши
	if (m_leftButtonPressed)
	{
		// ¬ычисл€ем смещение курсора мыши
		int dx = x - m_mouseX;
		int dy = y - m_mouseY;

		// ¬ычисл€ем угол поворота вокруг осей Y и X как линейно завис€щие
		// от смещени€ мыши по ос€м X и Y
		GLfloat rotateX = GLfloat(dy) * 180 / m_windowHeight;
		GLfloat rotateY = GLfloat(dx) * 180 / m_windowWidth;
		RotateCamera(rotateX, rotateY);

		// —охран€ем текущие координаты мыши
		m_mouseX = x;
		m_mouseY = y;

		// »нициируем перерисовку окна
		PostRedisplay();
	}
}

// ¬ращаем камеру вокруг начала кординат на заданный угол
void CMyApplication::RotateCamera(GLfloat rotateX, GLfloat rotateY)
{
	// »звлекаем текущее значение матрицы моделировани€-вида
	GLfloat modelView[16];
	glGetFloatv(GL_MODELVIEW_MATRIX, &modelView[0]);

	// »звлекаем направлени€ координатных осей камеры в 3д пространстве
	// как коэффициенты строк матрицы моделировани€-вида
	CVector3f xAxis(modelView[0], modelView[4], modelView[8]);
	CVector3f yAxis(modelView[1], modelView[5], modelView[9]);

	// ѕоворачиваем вокруг осей x и y камеры
	glRotatef(rotateX, xAxis.x, xAxis.y, xAxis.z);
	glRotatef(rotateY, yAxis.x, yAxis.y, yAxis.z);

	// ¬ ходе умножени€ матриц могут возникать погрешности, которые,
	// накаплива€сь могут сильно искажать картинку
	// ƒл€ их компенсации после каждой модификации матрицы моделировани€-вида
	// проводим ее ортонормирование
	NormalizeModelViewMatrix();
}

// ѕроизводим ортонормирование матрицы моделировани€-вида
void CMyApplication::NormalizeModelViewMatrix(void)
{
	GLfloat modelView[16];
	glGetFloatv(GL_MODELVIEW_MATRIX, modelView);
	/*
	ќртонормирование - приведение координатных осей к единичной длине (нормирование)
	и взаимной перпендикул€рности (ортогонализаци€)
	ƒостичь этого можно при помощи нормализации координатных осей
	и векторного произведени€
	*/
	CVector3f xAxis(modelView[0], modelView[4], modelView[8]);
	xAxis.Normalize();
	CVector3f yAxis(modelView[1], modelView[5], modelView[9]);
	yAxis.Normalize();

	// ќсь Z вычисл€ем через векторное произведение X и Y
	// Z будет перпендикул€рна плоскости векторов X и Y
	CVector3f zAxis = Cross(xAxis, yAxis);
	// » иметь единичную длину
	zAxis.Normalize();
	// “о же самое проделываем с ос€ми x и y
	xAxis = Cross(yAxis, zAxis);
	xAxis.Normalize();
	yAxis = Cross(zAxis, xAxis);
	yAxis.Normalize();

	// —охран€ем вектора координатных осей обратно в массив
	modelView[0] = xAxis.x;
	modelView[4] = xAxis.y, modelView[8] = xAxis.z;
	modelView[1] = yAxis.x;
	modelView[5] = yAxis.y, modelView[9] = yAxis.z;
	modelView[2] = zAxis.x;
	modelView[6] = zAxis.y, modelView[10] = zAxis.z;

	// » загружаем матрицу моделирвани€-вида
	glLoadMatrixf(modelView);
}