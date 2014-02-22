#include "Window.h"

#include <GL/glut.h>

vector<Window*> Window::Windows(4, nullptr);

Window::Window(int argc, char* argv[], const char* _name) : id_(glutCreateWindow(_name))
{
	initContext();

	if (id_ >= Windows.size())
		Windows.resize(id_ + 1);
	Windows[id_] = this;
}


Window::~Window(void)
{
	deinitContext();
}


void Window::initContext()
{
	glutInit(&argc, argv);
	glutCreateWindow(WINDOW_NAME);
	GLenum err = glewInit();
	if (GLEW_OK != err)
	{
		fprintf(stderr, ERROR_FORMAT, glewGetErrorString(err));
		return false;
	}
	fprintf(stdout, GLEW_STATUS_FORMAT, glewGetString(GLEW_VERSION));
	TwInit(TW_OPENGL_CORE, nullptr);
	return true;
}


void Window::deinitContext()
{
}
