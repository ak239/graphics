#include <tchar.h>
#include "GL.h"

#include "GlutApp.h"
#include "Logger.h"
#include "UniformWrapper.h"
#include "MouseCamera.h"
#include "Scene.h"
#include "Shader.h"
#include "UniformWrapper.h"
#include "Particles.h"
#include "Emitter.h"
#include "Texture.h"
#include "Bar.h"
#include "CubemapTexture.h"

#include "Sphere.h"
#include "Cube.h"



MouseCamera mouseCamera;
GLint       LastTime;
bool IsLeftButton  = false;
bool IsRightButton = false;

Bar* bar = nullptr;
GLfloat* MouseSpeed = nullptr;

UniformWrapper<vec3> cameraPosUniform;
UniformWrapper<GLfloat> sizeUniform;
UniformWrapper<GLfloat> timeUniform;
UniformWrapper<GLfloat> cycleTime;

void keyboardFunc(unsigned char key, int x, int y);
void motionFunc(int x, int y);
void timerFunc(int value);
void closeFunc();
void mouseFunc(int button, int state, int x, int y);

Particles* pariclesPointer = nullptr;
GLint startTime = 0;
void __stdcall rerun(void* data);

const string BasePoint     = "Base Point";
const string FromColorStr  = "From color";
const string ToColorStr    = "To color";
const string BaseVelocity  = "Base Velocity";
const string ParticleCount = "Particle Count";
const string OneCycleTime  = "One Cycle Time";
const string BaseRadius    = "Base radius";
const string ScatterOfVelocity = "Scatter of velocity";

int _tmain(int argc, _TCHAR* argv[])
{
	Logger::instance().setStream(std::cout);

	GlutApp app(argc, argv, "Graphics3 App");

	GlutWindow wnd = app.getMainWindow();

	TwInit(TW_OPENGL_CORE, NULL);

	bar = new Bar("Settings");
	MouseSpeed = bar->addVar("Mouse speed", 0.001f);
	bar->addVar("Size", 0.5f);
	bar->addVar<vec3, Color>(FromColorStr, vec3(1.0, 1.0, 1.0));
	bar->addVar<vec3, Color>(ToColorStr, vec3(1.0, 1.0, 1.0));

	bar->addVar<vec3, Position>(BasePoint, vec3(0.0, 0.0, 0.0));
	bar->addVar<vec3, Position>(BasePoint, vec3(0.0, 0.0, 0.0));
	bar->addVar<vec3, Dir>(BaseVelocity, vec3(0.0, 0.3, 0.0));
	bar->addVar(ParticleCount, 300);
	bar->addVar(OneCycleTime, 7000.0f);
	bar->addVar(BaseRadius,    0.1f);
	bar->addVar(ScatterOfVelocity, 0.05f);
	bar->addButton("Rerun", rerun);

	mouseCamera.setDist(3.0f);

	Shader objShader(wnd.getContext());
	objShader.loadFromFile("particle", Shader::Vertex | Shader::Fragment | Shader::Geometry);
	objShader.link();

	objShader.use();
	cameraPosUniform = objShader.getUniform<vec3>("cameraPos");
	sizeUniform      = objShader.getUniform<GLfloat>("size");
	timeUniform      = objShader.getUniform<GLfloat>("time");
	cycleTime        = objShader.getUniform<GLfloat>("cycleTime");
	
	Texture billboard;
	billboard.loadFromFile("Smoke.dds", GL_TEXTURE0);
	UniformWrapper<GLuint> TextureID  = objShader.getUniform<GLuint>("textureSampler");
	TextureID.setValue(0);

	Texture1D colorTex;
	if (!colorTex.loadFromFile("images.dds"))
		std::cout << "error!" << std::endl;
	colorTex.active(GL_TEXTURE1);
	colorTex.bind(GL_TEXTURE1);
	UniformWrapper<GLuint> TextureColorID  = objShader.getUniform<GLuint>("colorSampler");
	TextureColorID.setValue(1);

	Particles p(wnd.getContext());
	pariclesPointer = &p;
	p.setShader(&objShader);
	p.setTexture(&billboard);

	Shader cubeShader(wnd.getContext());
	cubeShader.loadFromFile("obj", Shader::Vertex | Shader::Fragment);
	cubeShader.link();

	cubeShader.use();
	Cube cube(wnd.getContext());
	cube.init();
	cube.setShader(&cubeShader);

	CubemapTexture cubemap;
	cubemap.loadFromFile("");
	//cubemap.activeAndBind(GL_TEXTURE1);
	cube.setTexture(&cubemap);

	cubeShader.use();
	cubeShader.getUniform<GLuint>("textureSampler").setValue(1);

	// Create a pixmap font from a TrueType file.

	//Shader carcas(wnd.getContext());
	//carcas.loadFromFile("carcas", Shader::Vertex | Shader::Fragment | Shader::Geometry);
	//carcas.link();


	//Sphere sp(wnd.getContext());
	//sp.init(2);
	//sp.setShader(&carcas);

	Scene scene(wnd.getContext());
	scene.setCamera(&mouseCamera);
	scene.addObject(&cube);
	scene.addObject(&p);
	
	wnd.setScene(&scene);

	glutKeyboardFunc(&keyboardFunc);
	glutMotionFunc(&motionFunc);
	glutTimerFunc(33, &timerFunc, 0);
	glutCloseFunc(&closeFunc);
	glutMouseFunc(&mouseFunc);

	glutPassiveMotionFunc((GLUTmousemotionfun)TwEventMouseMotionGLUT); 
	glutSpecialFunc((GLUTspecialfun)TwEventSpecialGLUT);
	TwGLUTModifiersFunc(glutGetModifiers);

	rerun(0);

	return app.exec(), 0;
}

void keyboardFunc(unsigned char key, int x, int y)
{
	switch(key){
	case 'w':
		mouseCamera.setDist(mouseCamera.getDist() - 0.05f);
		break;
	case 's':
		mouseCamera.setDist(mouseCamera.getDist() + 0.05f);
		break;
	case 'p':
		glutExit();
		break;
	}
	TwEventKeyboardGLUT(key, x, y);
}

void motionFunc(int x, int y)
{
	if (TwEventMouseMotionGLUT(x, y)) return;
	int halfXWindow = glutGet(GLUT_WINDOW_WIDTH) / 2;
	int halfYWindow = glutGet(GLUT_WINDOW_HEIGHT) / 2;

	GLint time = glutGet(GLUT_ELAPSED_TIME);
	GLint deltaTime = time - LastTime;
	if (deltaTime > 128)
		LastTime = time;

	if (deltaTime > 0){
		LastTime = time;

		GLfloat deltaHor = 0.0f;
		if (IsLeftButton) deltaHor = (*MouseSpeed) * deltaTime * float(halfXWindow - x );
		GLfloat deltaVer = 0.0f;
		if (IsRightButton) deltaVer = (*MouseSpeed) * deltaTime * float(halfYWindow - y );

		if (std::abs(deltaHor) > 0.03f) deltaHor = 0.03f * (deltaHor > 0.0 ? 1 : -1);
		if (std::abs(deltaVer) > 0.03f) deltaVer = 0.03f * (deltaVer > 0.0 ? 1 : -1);

		mouseCamera.rotate(deltaHor, deltaVer);

		glutWarpPointer(halfXWindow, halfYWindow);
	}
}

void timerFunc(int value)
{
	int timeSinceStart = glutGet(GLUT_ELAPSED_TIME) - startTime; 

	cameraPosUniform.getShader()->use();
	cameraPosUniform.setValue(mouseCamera.getPos());
	sizeUniform.setValue(*bar->getVar<GLfloat>("Size"));
	timeUniform.setValue(timeSinceStart);

	glutPostRedisplay();
	glutTimerFunc(33, &timerFunc, 0);
}

void closeFunc()
{
	TwTerminate();
}

void mouseFunc(int button, int state, int x, int y)
{
	if (button == GLUT_LEFT_BUTTON)
		IsLeftButton  = state == GLUT_DOWN;
	if (button == GLUT_RIGHT_BUTTON)
		IsRightButton = state == GLUT_DOWN;

	if (button == 3)
		mouseCamera.setDist(mouseCamera.getDist() + 0.05f);
	if (button == 4)
		mouseCamera.setDist(mouseCamera.getDist() - 0.05f);

	TwEventMouseButtonGLUT(button, state, x, y);
}

void __stdcall rerun(void* data)
{
	cycleTime.getShader()->use();
	cycleTime.setValue(*bar->getVar<GLfloat>(OneCycleTime));

	startTime = glutGet(GLUT_ELAPSED_TIME);

	Emitter emitter(*bar->getVar<vec3>(BasePoint), *bar->getVar<vec3>(BaseVelocity),
		*bar->getVar<GLint>(ParticleCount), *bar->getVar<GLfloat>(OneCycleTime),
		*bar->getVar<GLfloat>(BaseRadius), *bar->getVar<GLfloat>(ScatterOfVelocity),
		*bar->getVar<vec3>(FromColorStr), *bar->getVar<vec3>(ToColorStr));

	pariclesPointer->setData(&emitter);
}
