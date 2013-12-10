#include "GL.h"
#include "GlutWindow.h"
#include "GLContext.h"
#include "Camera.h"

#include "Logger.h"

std::array<Scene*, GlutWindow::MaxWindowsCount> GlutWindow::Windows;
std::array<void (*)(int,int), GlutWindow::MaxWindowsCount> GlutWindow::ReshapeFuncs;

struct GlutWindowData
{
	GLContext context;;

	GlutWindowData(int _windowId):context(_windowId){}
};

GlutWindow::GlutWindow(int windowId):
	data(new GlutWindowData(windowId)),
	counter(new int(1))
{
	if (static_cast<std::size_t>(windowId) >= Windows.size()) LOG_ERROR("Too many windows");

	Windows[windowId] = 0;

	glClearColor(0.5f, 0.5f, 0.5f, 0.0f);
	glutDisplayFunc(&GlutWindow::DisplayFunc);
	glutReshapeFunc(&GlutWindow::ReshapeFunc);
}

GlutWindow::GlutWindow(const GlutWindow& window):
	data(window.data),
	counter(window.counter)
{
	++(*counter);
}

GlutWindow::~GlutWindow(void)
{
	(*counter)--;
	if (*counter == 0)
	{
		delete data;
		delete counter;
	}
}

void GlutWindow::DisplayFunc()
{
	Scene* scene = Windows[glutGetWindow()];
	if (scene)
		scene->display();
}

GLContext GlutWindow::getContext() const
{
	return data->context;
}

void GlutWindow::setScene(Scene* scene)
{
	scene->setContext(getContext());
	Windows[glutGetWindow()] = scene;
}

void GlutWindow::setMainReshape(void (*reshapeFunc)(int,int))
{
	ReshapeFuncs[glutGetWindow()] = reshapeFunc;
}

void GlutWindow::ReshapeFunc(int width, int height)
{
	Scene* scene = Windows[glutGetWindow()];
	if (scene)
		scene->reshape(width, height);
	if (ReshapeFuncs[glutGetWindow()])
		(*ReshapeFuncs[glutGetWindow()])(width, height);
}
