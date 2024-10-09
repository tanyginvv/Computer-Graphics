
#pragma once

#include "common_inc.h"

class GLFWInitializer final
{
public:
	GLFWInitializer();

	GLFWInitializer(const GLFWInitializer&) = delete;
	GLFWInitializer& operator=(const GLFWInitializer&) = delete;

	~GLFWInitializer();
};
