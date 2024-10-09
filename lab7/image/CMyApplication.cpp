#include "CMyApplication.h"
#include "../libgl/ShaderLoader.h"

CMyApplication::CMyApplication(char const* title)
	: CGLApplication(title, 800, 600)
{
}

void CMyApplication::OnInit()
{
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_MULTISAMPLE);
	glClearColor(1, 1, 1, 1);

	glLoadIdentity();
	gluLookAt(
		0, 0, 3,
		0, 0, 0,
		0, 1, 0);

	InitShaders();
}

// доработать программу чтобы рядом была зеленая звезда
void CMyApplication::OnDisplay()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glUseProgram(m_program);

	glColor3f(0, 0, 0);
	glBegin(GL_QUADS);
	{
		glTexCoord2f(0, 0);
		glVertex2f(-0.8f, -0.8f);

		glTexCoord2f(4, 0);
		glVertex2f(0.8f, -0.8f);

		glTexCoord2f(4, 4);
		glVertex2f(0.8f, 0.8f);

		glTexCoord2f(0, 4);
		glVertex2f(-0.8f, 0.8f);
	}
	glEnd();

	glUseProgram(0);
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
	CShader vertexShader = loader.LoadShader(GL_VERTEX_SHADER, "image.vsh");
	CShader fragmentShader = loader.LoadShader(GL_FRAGMENT_SHADER, "image.fsh");

	vertexShader.Compile();
	fragmentShader.Compile();

	m_program.Create();
	m_program.AttachShader(vertexShader);
	m_program.AttachShader(fragmentShader);

	m_program.Link();
}