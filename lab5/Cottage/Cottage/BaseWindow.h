#pragma once

#include "common_inc.h"

class BaseWindow
{
public:
	BaseWindow(int w, int h, char const* title);

	BaseWindow(BaseWindow const&) = delete;
	BaseWindow& operator=(BaseWindow const&) = delete;

	virtual ~BaseWindow();

	glm::ivec2 GetFramebufferSize() const;

	void Run();

	glm::dvec2 GetCursorPos() const;

private:
	static BaseWindow* GetBaseWindow(GLFWwindow* window);

	virtual void OnResize(int width, int height) {}
	virtual void OnMouseButton(int button, int action, int mods) {}
	virtual void OnMouseMove(double x, double y) {}
	virtual void OnKey(int key) {}

	virtual void OnRunStart() {}
	virtual void OnRunEnd() {}

	virtual void Draw(int width, int height) const = 0;

	static GLFWwindow* MakeWindow(int w, int h, char const* title);

	GLFWwindow* m_window;
};