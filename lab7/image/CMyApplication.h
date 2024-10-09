#pragma once

#include "../libgl/GLApplication.h"
#include "../libgl/Shaders.h"
#include "../libgl/pch.h"

class CMyApplication : public CGLApplication
{
public:
	CMyApplication(char const* title);

protected:
	void OnInit() override;
	void OnDisplay() override;
	void OnReshape(int width, int height) override;

private:
	void InitShaders();

	CProgram m_program;
};