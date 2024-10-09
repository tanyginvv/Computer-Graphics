#include "Window.h"
#include "DirectLight.h"
constexpr auto MATH_PI = 3.1415926535;;

namespace
{
	constexpr double FIELD_OF_VIEW = 60 * MATH_PI / 180.0;

	constexpr double Z_NEAR = 0.1;
	constexpr double Z_FAR = 20;

	glm::dmat4x4 Orthonormalize(glm::dmat4x4 const& m)
	{
		auto const normalizedMatrix = glm::orthonormalize(glm::dmat3x3{ m });
		return {
			glm::dvec4{ normalizedMatrix[0], 0.0 },
			glm::dvec4{ normalizedMatrix[1], 0.0 },
			glm::dvec4{ normalizedMatrix[2], 0.0 },
			m[3]
		};
	}
} 

Window::Window(int w, int h, char const* title)
	: BaseWindow(w, h, title)
{
}

Window::~Window() noexcept
{
	m_groundTexture.Delete();

	m_wallTexture.Delete();
	m_windowTexture.Delete();
	m_doorTopTexture.Delete();
	m_doorBottomTexture.Delete();
	m_garageDoorTexture.Delete();
	m_roofTexture.Delete();
}

void Window::OnMouseButton(int button, int action, int mods)
{
	if (button == GLFW_MOUSE_BUTTON_1)
	{
		m_leftButtonPressed = (action & GLFW_PRESS) != 0;
	}
}

void Window::OnMouseMove(double x, double y)
{
	const glm::dvec2 mousePos{ x, y };
	if (m_leftButtonPressed)
	{
		auto const windowSize = GetFramebufferSize();

		auto const mouseDelta = mousePos - m_mousePos;
		double const xAngle = mouseDelta.y * MATH_PI / windowSize.y;
		double const yAngle = mouseDelta.x * MATH_PI / windowSize.x;
		RotateCamera(xAngle, yAngle);
	}
	m_mousePos = mousePos;
}

void Window::RotateCamera(double xAngleRadians, double yAngleRadians)
{
	const glm::dvec3 xAxis{
		m_cameraMatrix[0][0], m_cameraMatrix[1][0], m_cameraMatrix[2][0]
	};
	const glm::dvec3 yAxis{
		m_cameraMatrix[0][1], m_cameraMatrix[1][1], m_cameraMatrix[2][1]
	};
	m_cameraMatrix = glm::rotate(m_cameraMatrix, xAngleRadians, xAxis);
	m_cameraMatrix = glm::rotate(m_cameraMatrix, yAngleRadians, yAxis);

	m_cameraMatrix = Orthonormalize(m_cameraMatrix);
}

void Window::OnResize(int width, int height)
{
	glViewport(0, 0, width, height);

	double aspect = double(width) / double(height);

	glMatrixMode(GL_PROJECTION);
	auto const proj = glm::perspective(FIELD_OF_VIEW, aspect, Z_NEAR, Z_FAR);
	glLoadMatrixd(&proj[0][0]);
	glMatrixMode(GL_MODELVIEW);
}

void Window::OnKey(int key)
{
	switch (key)
	{
	case GLFW_KEY_0:
		m_fogEnabled = false;
		m_fogColor = NO_FOG;
		break;
	case GLFW_KEY_1:
		m_fogEnabled = true;
		m_fogColor = PLAIN_FOG;
		break;
	case GLFW_KEY_2:
		m_fogEnabled = true;
		m_fogColor = NIGHT_FOG;
		break;
	default:
		break;
	}
}

void Window::OnRunStart()
{
	glClearColor(m_fogColor[0], m_fogColor[1], m_fogColor[2], m_fogColor[3]);
	glEnable(GL_DEPTH_TEST);

	glEnable(GL_CULL_FACE);
	glCullFace(GL_BACK);
	glFrontFace(GL_CCW);

	SetupLight();
	SetupTextures();
}

void Window::SetupLight()
{
	glEnable(GL_LIGHTING);
	glEnable(GL_LIGHT0);
	DirectLight light({ 0.0f, 0.0f, 1.0f });
	light.Apply(GL_LIGHT0);
}

void Window::SetupTextures()
{
	glEnable(GL_TEXTURE_2D);

	m_groundTexture = m_textureLoader.LoadTexture("Assets/grass_block_top.png");
	m_environment.SetGroundTexture(m_groundTexture);

	m_wallTexture = m_textureLoader.LoadTexture("Assets/bricks.png");
	m_cottage.SetWallTexture(m_wallTexture);

	m_windowTexture = m_textureLoader.LoadTexture("Assets/glass.png");
	m_cottage.SetWindowTexture(m_windowTexture);

	m_doorTopTexture = m_textureLoader.LoadTexture("Assets/birch_door_top.png");
	m_doorBottomTexture = m_textureLoader.LoadTexture("Assets/birch_door_bottom.png");
	m_cottage.SetDoorTextures(m_doorTopTexture, m_doorBottomTexture);

	m_garageDoorTexture = m_textureLoader.LoadTexture("Assets/iron_door_top.png");
	m_cottage.SetGarageDoorTexture(m_garageDoorTexture);

	m_roofTexture = m_textureLoader.LoadTexture("Assets/nether_bricks.png");
	m_cottage.SetRoofTexture(m_roofTexture);
}

void Window::Draw(int width, int height) const
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	SetupCameraMatrix();
	SetupFog();

	m_environment.Draw();
	m_cottage.Draw();
}

void Window::SetupCameraMatrix() const
{
	glMatrixMode(GL_MODELVIEW);
	glLoadMatrixd(&m_cameraMatrix[0][0]);
}

void Window::SetupFog() const
{
	glClearColor(m_fogColor[0], m_fogColor[1], m_fogColor[2], m_fogColor[3]);
	glFogfv(GL_FOG_COLOR, glm::value_ptr(m_fogColor));
	glFogi(GL_FOG_MODE, GL_EXP);
	glFogf(GL_FOG_DENSITY, 0.15f);

	if (m_fogEnabled)
	{
		glEnable(GL_FOG);
	}
	else
	{
		glDisable(GL_FOG);
	}
}