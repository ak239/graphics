#pragma once

#include <GL/glew.h>

#include <vector>
using std::vector;

GLuint load3DTexture(const vector<unsigned char>& data, GLint width, GLint height, GLint depth);
vector<unsigned char> genSphere(GLint width);
