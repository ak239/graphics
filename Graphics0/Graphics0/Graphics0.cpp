#include "stdafx.h"
#include <cmath>
#include <fstream>

#include "GlutApp.h"
#include "Logger.h"
#include "UniformWrapper.h"
#include "Camera.h"
#include "MouseCamera.h"
#include "Scene.h"
#include "Mover.h"
#include "Shader.h"
#include "StreamObject.h"
#include "UniformWrapper.h"

MouseCamera mouseCamera;
GLint       LastTime;
GLfloat     MouseSpeed = 0.001f;
bool IsLeftButton  = false;
bool IsRightButton = false;

UniformWrapper<glm::vec2> farNear;
glm::vec2                 farNearValue = glm::vec2(16.0f, 22.0f);
StreamObject*             objPointer = nullptr;

typedef enum { fromVS = 1, fromZFS = 2} ZCalcType;
ZCalcType zCalcTypeValue = fromVS;
TwEnumVal zCalcTypeEv[] = { {fromVS, "From vertex shader"}, {fromZFS, "From z in fragment shader"} };
UniformWrapper<GLuint> zCalcType;

UniformWrapper<glm::vec2> windowSize;

void keyboardFunc(unsigned char key, int x, int y);
void motionFunc(int x, int y);
void idleFunc();
void timerFunc(int value);
void closeFunc();
void reshapeFunc(int width, int height);
void mouseFunc(int button, int state, int x, int y);

int _tmain(int argc, _TCHAR* argv[])
{
	Logger::instance().setStream(std::cout);

	GlutApp app(argc, argv, "Graphics0 App");

	GlutWindow wnd = app.getMainWindow();
	wnd.setMainReshape(reshapeFunc);

	TwInit(TW_OPENGL_CORE, NULL);
	TwBar *myBar;
	myBar = TwNewBar("Settings");
	//TwDefine(" GLOBAL help='This example shows how to integrate AntTweakBar with SDL and OpenGL.\nPress [Space] to toggle fullscreen.' ");

	TwAddVarRW(myBar, "far",  TW_TYPE_FLOAT, &farNearValue[0], "");
	TwAddVarRW(myBar, "near", TW_TYPE_FLOAT, &farNearValue[1], "");
	TwType zCalcTypeType = TwDefineEnum("ZCalcTypeType", zCalcTypeEv, 2);
	TwAddVarRW(myBar, "Z from", zCalcTypeType, &zCalcTypeValue, "");
	TwAddVarRW(myBar, "mouse speed", TW_TYPE_FLOAT, &MouseSpeed, "");

	StreamObject obj(wnd.getContext());
	std::fstream file;
	file.open("model.obj", std::ios::in);
	if (file.is_open())
		obj.loadObject(file);
	objPointer = &obj;
	obj.setPolygonMode(GL_FILL);
	obj.setPolygonOffset(1.0f, 1.0f);

	StreamObject carcas(obj);
	carcas.setPolygonMode(GL_LINE);
	
	mouseCamera.setDist(20.0f);

	Shader objShader(wnd.getContext());
	objShader.loadFromFile("obj", Shader::Vertex | Shader::Fragment);
	objShader.link();
	farNear    = objShader.getUniform<glm::vec2>("farNear");
	zCalcType  = objShader.getUniform<GLuint>(   "zCalcType");
	windowSize = objShader.getUniform<glm::vec2>("windowSize");
	obj.setShader(&objShader);
	carcas.setShader(&objShader);
	
	Scene scene(wnd.getContext());
	scene.setCamera(&mouseCamera);
	scene.addObject(&obj);
	scene.addObject(&carcas);
	wnd.setScene(&scene);

	glutKeyboardFunc(&keyboardFunc);
	glutMotionFunc(&motionFunc);
	glutIdleFunc(&idleFunc);
	glutTimerFunc(33, &timerFunc, 0);
	glutCloseFunc(&closeFunc);
	glutMouseFunc(&mouseFunc);

	glutPassiveMotionFunc((GLUTmousemotionfun)TwEventMouseMotionGLUT); 
	glutSpecialFunc((GLUTspecialfun)TwEventSpecialGLUT);
	TwGLUTModifiersFunc(glutGetModifiers);
	return app.exec(), 0;
}

void keyboardFunc(unsigned char key, int x, int y)
{
	switch(key){
	case 'w':
		mouseCamera.setDist(mouseCamera.getDist() - 0.2f);
		break;
	case 's':
		mouseCamera.setDist(mouseCamera.getDist() + 0.2f);
		break;

	case 'p':
		glutExit();
		break;

	}
	TwEventKeyboardGLUT(key, x, y);
}

void motionFunc(int x, int y)
{
	int halfXWindow = glutGet(GLUT_WINDOW_WIDTH) / 2;
	int halfYWindow = glutGet(GLUT_WINDOW_HEIGHT) / 2;

	GLint time = glutGet(GLUT_ELAPSED_TIME);
	GLint deltaTime = time - LastTime;
	if (deltaTime > 128)
		LastTime = time;

	if (deltaTime > 0){
		LastTime = time;

		GLfloat deltaHor = 0.0f;
		if (IsLeftButton) deltaHor = MouseSpeed * deltaTime * float(halfXWindow - x );
		GLfloat deltaVer = 0.0f;
		if (IsLeftButton) deltaVer = MouseSpeed * deltaTime * float(halfYWindow - y );

		if (std::abs(deltaHor) > 0.03f) deltaHor = 0.03f * (deltaHor > 0.0 ? 1 : -1);
		if (std::abs(deltaVer) > 0.03f) deltaVer = 0.03f * (deltaVer > 0.0 ? 1 : -1);

		mouseCamera.rotate(deltaHor, deltaVer);

		glutWarpPointer(halfXWindow, halfYWindow);
	}

	TwEventMouseMotionGLUT(x, y);
}

void timerFunc(int value)
{
	farNear.setValue(farNearValue);
	zCalcType.setValue(zCalcTypeValue);
//	if (objPointer) objPointer->setPolygonMode(renderType);

	glutPostRedisplay();
	glutTimerFunc(33, &timerFunc, 0);
}

void idleFunc()
{
}

void closeFunc()
{
	TwTerminate();
}

void reshapeFunc(int width, int height)
{
	windowSize.setValue(glm::vec2(width, height));
}

void mouseFunc(int button, int state, int x, int y)
{
	if (button == GLUT_LEFT_BUTTON)
		IsLeftButton  = state == GLUT_DOWN;
	if (button == GLUT_RIGHT_BUTTON)
		IsRightButton = state == GLUT_DOWN;

	TwEventMouseButtonGLUT(button, state, x, y);
}
