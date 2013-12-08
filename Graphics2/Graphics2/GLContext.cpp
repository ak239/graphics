#include "stdafx.h"
#include "GLContext.h"

GLContextGetter::GLContextGetter(const GLContext& context):oldWindowId(glutGetWindow())
{
	if (context.windowId <= 0) 
		LOG_ERROR("Try to use context without context");
	glutSetWindow(context.windowId);
}

GLContextGetter::~GLContextGetter(void)
{
	glutSetWindow(oldWindowId);
}