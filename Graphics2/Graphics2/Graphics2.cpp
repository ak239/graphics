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
#include "ObjectFromObj.h"
#include "ObjWithFragNormals.h"

MouseCamera mouseCamera;
GLint       LastTime;
GLfloat     MouseSpeed = 0.001f;
bool IsLeftButton  = false;
bool IsRightButton = false;

// settings
typedef enum { phong = 1, blinnPhong = 2 } ModelType;
ModelType modelType = phong;
UniformWrapper<GLuint> modelTypeUniform;
TwEnumVal modelTypeTwEnum[] = {{ phong, "Phong"}, { blinnPhong, "Blinn-Phong"}}; 

typedef enum { flat = 1, perVertex = 2, perFragment = 3 } FillType;
FillType fillType = flat;
UniformWrapper<GLuint> fillTypeUniform;
TwEnumVal fillTypeTwEnum[] = {{ flat, "flat"}, { perVertex, "per-vertex"}, { perFragment, "per-fragment" }};

glm::vec3 pointPosition(0.0f, 2.0f, 0.0f);
UniformWrapper<vec3> pointPositionUniform;
GLfloat   pointAttenuation = 0.5f;
UniformWrapper<GLfloat> pointAttenuationUniform;

glm::vec3 ambientColor;
UniformWrapper<vec3> ambientColorUniform;

glm::vec3 diffuseColor(1.0f, 1.0f, 1.0f);
UniformWrapper<vec3> diffuseColorUniform;

glm::vec3 specularColor(0.0f, 1.0f, 1.0f);
UniformWrapper<vec3> specularColorUniform;

GLfloat   specularPower = 20.0f;
UniformWrapper<GLfloat> specularPowerUniform;

UniformWrapper<vec3> cameraPosUniform;

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

	GlutApp app(argc, argv, "Graphics2 App");

	GlutWindow wnd = app.getMainWindow();
	wnd.setMainReshape(reshapeFunc);

	TwInit(TW_OPENGL_CORE, NULL);
	TwBar *myBar;
	myBar = TwNewBar("Settings");

	TwType modelTypeTwType = TwDefineEnum("ModelType", modelTypeTwEnum, 2);
	TwAddVarRW(myBar, "Model type", modelTypeTwType, &modelType, "");

	TwType fillTypeTwType = TwDefineEnum("FillType", fillTypeTwEnum, 3);
	TwAddVarRW(myBar, "Fill type", fillTypeTwType, &fillType, "");

	TwAddSeparator(myBar, "Point", "");
	//TwAddVarRW(myBar, "Color",       TW_TYPE_COLOR3F, &pointColor.x,       "");
	TwAddVarRW(myBar, "Position.X",  TW_TYPE_FLOAT,   &pointPosition.x,  "");
	TwAddVarRW(myBar, "Position.Y",  TW_TYPE_FLOAT,   &pointPosition.y,  "");
	TwAddVarRW(myBar, "Position.Z",  TW_TYPE_FLOAT,   &pointPosition.z,  "");
	TwAddVarRW(myBar, "Position.Z",  TW_TYPE_FLOAT,   &pointPosition.z,  "");
	TwAddVarRW(myBar, "Attenuation", TW_TYPE_FLOAT,   &pointAttenuation, "");

	TwAddSeparator(myBar, "Scene", "");
	TwAddVarRW(myBar, "Ambient Color",  TW_TYPE_COLOR3F, &ambientColor.x,  "");
	TwAddVarRW(myBar, "Diffuse Color",  TW_TYPE_COLOR3F, &diffuseColor.x,  "");
	TwAddVarRW(myBar, "Specular Color", TW_TYPE_COLOR3F, &specularColor.x, "");
	TwAddVarRW(myBar, "Specular Power", TW_TYPE_FLOAT,   &specularPower,   "");

	TwAddSeparator(myBar, "Global", "");
	TwAddVarRW(myBar, "mouse speed", TW_TYPE_FLOAT, &MouseSpeed, "");

	mouseCamera.setDist(20.0f);

	Shader objShader(wnd.getContext());
//	objShader.loadFromFile("obj", Shader::Vertex | Shader::Fragment);
	objShader.loadFromFile("flat", Shader::Vertex | Shader::Fragment);
	objShader.link();

	fillTypeUniform         = objShader.getUniform<GLuint>("fillType");

	modelTypeUniform        = objShader.getUniform<GLuint>("modelType");

	//pointColorUniform       = objShader.getUniform<vec3>("pointColor");
	pointPositionUniform    = objShader.getUniform<vec3>("pointPosition");
	pointAttenuationUniform = objShader.getUniform<GLfloat>("pointAttenuation");

	ambientColorUniform = objShader.getUniform<vec3>("ambientColor");
	diffuseColorUniform = objShader.getUniform<vec3>("diffuseColor");

	specularColorUniform = objShader.getUniform<vec3>("specularColor");
	specularPowerUniform = objShader.getUniform<GLfloat>("specularPower");

	cameraPosUniform = objShader.getUniform<vec3>("cameraPos");


	ObjectFromObj obj(wnd.getContext());
	ObjWithFragNormals data;
    std::fstream file;
    file.open("bunny_n.obj", std::ios::in);
    if (file.is_open())
        file >> data;
	obj.setData(&data);
	obj.setShader(&objShader);
    //obj.setPolygonMode();

	Sphere sphere;
	sphere.init(1);
	sphere.setShader(&objShader);

	Scene scene(wnd.getContext());
	scene.setCamera(&mouseCamera);
	scene.addObject(&obj);
	//scene.addObject(&sphere);

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
	// here processing changes
	fillTypeUniform.setValue(fillType);
	modelTypeUniform.setValue(modelType);

	//pointColorUniform.setValue(pointColor);
	pointPositionUniform.setValue(pointPosition);
	pointAttenuationUniform.setValue(pointAttenuation);

	ambientColorUniform.setValue(ambientColor);
	diffuseColorUniform.setValue(diffuseColor);

	specularColorUniform.setValue(specularColor);
	specularPowerUniform.setValue(specularPower);

	cameraPosUniform.setValue(mouseCamera.getPos());

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
