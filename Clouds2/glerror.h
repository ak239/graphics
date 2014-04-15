#pragma once

#include <GL/glew.h>
#include <GL/freeglut.h>

#include <cstdio>

#define GL_ERRORABLE(func) \
	func; \
	{ \
	GLenum err = glGetError(); \
	if (err != GL_NO_ERROR){ \
		fprintf(stderr, "gl error with code: %d\nin %s : %d : %s\n", err, __FILE__, __LINE__, #func); \
		glutLeaveMainLoop(); \
	} \
	}

#define GE GL_ERRORABLE

#define DEBUG_MATRIX(m) \
	fprintf(stdout, "%f %f %f %f\n%f %f %f %f\n%f %f %f %f\n%f %f %f %f\n", \
	m[0][0], m[0][1], m[0][2], m[0][3], \
	m[1][0], m[1][1], m[1][2], m[1][3], \
	m[2][0], m[2][1], m[2][2], m[2][3], \
	m[3][0], m[3][1], m[3][2], m[3][3])
