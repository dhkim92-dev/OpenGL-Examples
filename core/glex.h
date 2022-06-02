#ifndef __GLEX_GLEX_H__
#define __GLEX_GLEX_H__
#ifdef __APPLE__
#include <OpenGL/gl3.h>
#elif __linux__
#define GL_GLEXT_PROTOTYPES
#include <GL/gl.h>
#include <GL/glext.h>
#endif
#include "shader.h"
#include "camera.h"
#include "utility.h"
#include "mesh.h"
#include "base_window.h"
#endif

