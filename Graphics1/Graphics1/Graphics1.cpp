#include "stdafx.h"
#include <cmath>
#include <fstream>
#include <memory>

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
#include "Plane.h"
#include "nvImage.h"
#include "Cube.h"
#include "Sphere.h"
#include "Texture.h"

MouseCamera mouseCamera;
GLint       LastTime;
GLfloat     MouseSpeed = 0.001f;
bool IsLeftButton  = false;
bool IsRightButton = false;

typedef enum { plane = 1, cube = 2, sphere = 3} MeshType;
MeshType meshTypeValue = plane;
TwEnumVal meshTypeEv[] = { {plane, "plane"}, {cube, "cube"}, {sphere, "sphere"} };

GLint magFilter = GL_NEAREST;
TwEnumVal magFilterEv[] = { {GL_NEAREST, "nearest"}, {GL_LINEAR, "linear"}};

GLint minFilter = GL_NEAREST;
TwEnumVal minFilterEv[] = { {GL_NEAREST, "nearest"}, {GL_LINEAR, "linear"}, {GL_NEAREST_MIPMAP_NEAREST, "nearest_mipmap_nearest"}, 
{GL_NEAREST_MIPMAP_LINEAR, "nearest_mipmap_linear"}, {GL_LINEAR_MIPMAP_NEAREST, "linear_mipmap_nearest"}, {GL_LINEAR_MIPMAP_LINEAR, "linear_mipmap_linea"}};

RenderObject* planePointer = nullptr;
RenderObject* cubePointer = nullptr;
RenderObject* spherePointer = nullptr;
RenderObject* carcasPointer = nullptr;

std::unique_ptr<Texture> earthTexture;
std::unique_ptr<Texture> lennaTexture;

UniformWrapper<GLfloat> uvScaleUniform;
GLfloat uvScale = 1.0f;

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

	TwType meshTypeType = TwDefineEnum("MeshType", meshTypeEv, 3);
	TwAddVarRW(myBar, "Mesh type", meshTypeType, &meshTypeValue, "");
	TwAddVarRW(myBar, "UV scale", TW_TYPE_FLOAT, &uvScale, "");

	TwType magFilterType = TwDefineEnum("MagFilter", magFilterEv, 2);
	TwAddVarRW(myBar, "Mag filter", magFilterType, &magFilter, "");

	TwType minFilterType = TwDefineEnum("MinFilter", minFilterEv, 6);
	TwAddVarRW(myBar, "Min filter", minFilterType, &minFilter, "");

	TwAddVarRW(myBar, "mouse speed", TW_TYPE_FLOAT, &MouseSpeed, "");

	mouseCamera.setDist(20.0f);

	Shader objShader(wnd.getContext());
	objShader.loadFromFile("obj", Shader::Vertex | Shader::Fragment);
	objShader.link();
	uvScaleUniform = objShader.getUniform<GLfloat>("uvScale");

	earthTexture.reset(new Texture());
	earthTexture->loadFromFile("earth_texture_grid.dds", GL_TEXTURE0);

	lennaTexture.reset(new Texture());
	lennaTexture->loadFromFile("lenna_head.dds", GL_TEXTURE0);

	UniformWrapper<GLuint> TextureID  = objShader.getUniform<GLuint>("textureSampler");
	TextureID.setValue(0);

	Plane plane;
	plane.setShader(&objShader);
	plane.setVisible(false);
	plane.init();

	planePointer = &plane;

	Cube cube;
	cube.setShader(&objShader);
	cube.setVisible(false);
	cube.init();

	cubePointer = &cube;

	Sphere sphere;
	sphere.setShader(&objShader);
	sphere.init(3);
	sphere.setPolygonOffset(1.0f, 1.0f);

	Shader sphereShader(wnd.getContext());
	sphereShader.loadFromFile("sphere", Shader::Vertex | Shader::Fragment | Shader::Geometry);
	sphereShader.link();

	Sphere carcasSphere;
	carcasSphere.setShader(&sphereShader);
	carcasSphere.init(3);

	spherePointer = &sphere;
	carcasPointer = &carcasSphere;
	
	Scene scene(wnd.getContext());
	scene.setCamera(&mouseCamera);
	scene.addObject(&sphere);
	scene.addObject(&carcasSphere);
	scene.addObject(&cube);
	scene.addObject(&plane);
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
	uvScaleUniform.setValue(uvScale);

	if (meshTypeValue == plane)
	{  
		planePointer->setVisible(true);  
		cubePointer->setVisible(false); 
		spherePointer->setVisible(false); 
		carcasPointer->setVisible(false);
		lennaTexture->activeAndBind();
		lennaTexture->setMagFilter(magFilter);		
		lennaTexture->setMinFilter(minFilter);		
	}
	if (meshTypeValue == cube)
	{  
		planePointer->setVisible(false); 
		cubePointer->setVisible(true);  
		spherePointer->setVisible(false); 
		carcasPointer->setVisible(false);
		lennaTexture->activeAndBind();
		lennaTexture->setMagFilter(magFilter);
		lennaTexture->setMinFilter(minFilter);		
	}
	if (meshTypeValue == sphere)
	{ 
		planePointer->setVisible(false); 
		cubePointer->setVisible(false); 
		spherePointer->setVisible(true); 
		carcasPointer->setVisible(true);
		earthTexture->activeAndBind();
		earthTexture->setMagFilter(magFilter);
		earthTexture->setMinFilter(minFilter);		
	}

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
}

void mouseFunc(int button, int state, int x, int y)
{
	if (button == GLUT_LEFT_BUTTON)
		IsLeftButton  = state == GLUT_DOWN;
	if (button == GLUT_RIGHT_BUTTON)
		IsRightButton = state == GLUT_DOWN;

	if (button == 3)
		mouseCamera.setDist(mouseCamera.getDist() + 0.2f);
	if (button == 4)
		mouseCamera.setDist(mouseCamera.getDist() - 0.2f);

	TwEventMouseButtonGLUT(button, state, x, y);
}
