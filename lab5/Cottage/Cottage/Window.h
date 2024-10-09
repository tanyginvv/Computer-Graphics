#pragma once

#include "BaseWindow.h"
#include "Cottage.h"
#include "../Cottage/Environment.h"
#include "TextureLoader.h"

class Window : public BaseWindow
{
public:
	Window(int w, int h, char const* title);

	~Window() noexcept;

private:
	void OnMouseButton(int button, int action, [[maybe_unused]] int mods) override;

	void OnMouseMove(double x, double y) override;

	// Âðàùàåì êàìåðó âîêðóã íà÷àëà êîîðäèíàò
	void RotateCamera(double xAngleRadians, double yAngleRadians);

	void OnResize(int width, int height) override;

	void OnKey(int key) override;

	void OnRunStart() override;
	void SetupLight();
	void SetupTextures();

	void Draw(int width, int height) const override;

	void SetupCameraMatrix() const;

	void SetupFog() const;

	// Ðàññòîÿíèå îò êàìåðû äî òî÷êè âðàùåíèÿ
	static constexpr double DISTANCE_TO_ORIGIN = 12;

	static constexpr glm::vec4 NO_FOG = { 1, 1, 1, 1 };
	static constexpr glm::vec4 PLAIN_FOG = { 0.3f, 0.3f, 0.3f, 1 };
	static constexpr glm::vec4 NIGHT_FOG = { 0, 0, 0, 1 };

	TextureLoader m_textureLoader;

	bool m_leftButtonPressed = false;
	glm::dvec2 m_mousePos = {};
	glm::dmat4x4 m_cameraMatrix = glm::lookAt(
		glm::dvec3{ 0.0, -DISTANCE_TO_ORIGIN, 10.0 },
		glm::dvec3{ 0.0, 0.0, 0.0 },
		glm::dvec3{ 0.0, 0.0, 1.0 });

	bool m_fogEnabled = false;
	glm::vec4 m_fogColor = NO_FOG;

	Cottage m_cottage;
	Environment m_environment;

	TextureHandler m_groundTexture;

	TextureHandler m_wallTexture;
	TextureHandler m_windowTexture;
	TextureHandler m_doorTopTexture;
	TextureHandler m_doorBottomTexture;
	TextureHandler m_garageDoorTexture;
	TextureHandler m_roofTexture;
};