#pragma once

// Фикс GDI+ для версий Windows 10 SDK ниже 2104
#define NOMINMAX

#include <windows.h>
#include <assert.h>
#include <GL/glew.h>
#include <GL/glut.h>
#include <stdexcept>
#include <glm/ext/matrix_clip_space.hpp>
#include <glm/ext/matrix_transform.hpp>
#define GLM_ENABLE_EXPERIMENTAL
#include <glm/ext.hpp>


// Фикс GDI+ для версий Windows 10 SDK ниже 2104
using std::max;
using std::min;

#include <gdiplus.h>
