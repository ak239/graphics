#include <GL/glew.h>
#include <GL/freeglut.h>
#include <FTGL/ftgl.h>
#include <AntTweakBar.h>
#include <glm.hpp>
using glm::mat4;
using glm::vec3;
#include <ext.hpp>

#include <cstdio>

#include "Cube.h"
#include "Shader.h"

const char *WINDOW_NAME  = "VolumeCasting";
const char *ERROR_FORMAT = "Error: %s\n";
const char *GLEW_STATUS_FORMAT = "Status: Using GLEW %s\n";

const char *ERROR_FTGL_FORMAT = "Font Error: %d\n";
const char *FONT_FILE = "fontana.ttf";

const char *VERTEX_NAME   = "vol.vs";
const char *FRAGMENT_NAME = "vol3.fs";

const char *SETTINGS_BAR = "settings";
const char *TURBULENCE_COUNT = "turbulence sum count";
const char *STEP_SIZE = "step size";
const char *INIT_FREQ = "init freq";
const char *CAM_POS = "camera position";

#define DEF_UNIFORM_FLOAT_VAR(name, d) \
	const char* name##_VAR = #name; \
	GLfloat name##_val = d; \
	GLuint name##_loc = 0; 

#define ADD_UNIFORM_FLOAT_TO_BAR(name, def) \
	TwAddVarRW(bar, name##_VAR, TW_TYPE_FLOAT, &name##_val, "");

#define GET_UNIFORM_LOC(name, def) \
	name##_loc = glGetUniformLocation(Program, name##_VAR);

#define UPDATE_FLOAT_UNIFORM(name, def) \
	glUniform1f(name##_loc, name##_val);

#define FOR_FLOAT_VARS(DO) \
	DO(FX0, 1.0) \
	DO(FY0, 1.0) \
	DO(C0,  0.3) \
	DO(T0,  0.4) \
	DO(k,   0.2) \
	DO(PX0, 1.0) \
	DO(PY0, 1.0) \
	DO(n,   4.0) \
	DO(T1,  1.0) \
	DO(T2,  1.0) \
	DO(D,   1.0) \
	DO(Fmax, 1.0) \
	DO(a1, 0.07) \
	DO(a2, 1.0)

FOR_FLOAT_VARS(DEF_UNIFORM_FLOAT_VAR);

const unsigned char KEY_ESCAPE = 27;
const int TIMER_POST_REDISPLAY = 1;
const int FRAMES_PER_SECOND = 60;

bool initAll(int argc, char* argv[]);
void keyboardFunc(unsigned char key, int x, int y);
void displayFunc();
void timerFunc(int val);
void motionFunc(int x, int y);
void mouseFunc(int button, int state, int x, int y);

void deinitAll();
void displayFrameCount();

bool IsTerminate = false;
FTGLPixmapFont *FrameCounterFont = nullptr;
int PrevTime = 0;
GLuint CubeVertexBuffer = 0;
GLuint Program = 0;
GLuint MVPloc = 0;
mat4 MVP;
GLuint camLoc = 0;

GLfloat initFreq = 0.5f;
GLuint  initFreqLoc = 0;

bool IsLeftButton  = false;
bool IsRightButton = false;
GLint LastTime = 0;

GLfloat Dist    = 10.0f;
GLfloat MouseSpeed = 0.001f;

vec3 camPos(0.0f, 0.0f, Dist);
glm::quat curCamRot = glm::normalize(glm::quat());
mat4 View();

GLint  turbulenceCount = 2;
GLuint turbulenceLoc = 0;
GLfloat stepSize = 2.0 / 50.0;
GLuint stepSizeLoc = 0;

vec3 curCamPos;

int main(int argc, char* argv[])
{
	// init glut, glew and AntTweakBar
	if (!initAll(argc, argv))
		return 1;

	// setup callbacks
	glutDisplayFunc(&displayFunc);
	glutKeyboardFunc(&keyboardFunc);
	glutMouseFunc(&mouseFunc);
	glutMotionFunc(&motionFunc);
	atexit(&deinitAll);

	// init glut/GL vars
	//glClearColor(135.0f/255.0f, 206.0f/255.0f, 235.0f/255.0f, 1.0f);
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
	glutReshapeWindow(1024, 768);
	glViewport(0,0,1024,768);
	glEnable(GL_DEPTH_TEST);
	glDepthMask(GL_TRUE);
	//glEnable(GL_TEXTURE_2D);
	glDepthFunc(GL_LESS);

	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	// load program
	Program = loadShaders(VERTEX_NAME, FRAGMENT_NAME);

	MVPloc = glGetUniformLocation(Program, "MVP");
	camLoc = glGetUniformLocation(Program, "camPos");
	turbulenceLoc = glGetUniformLocation(Program, "turbulenceCount");
	stepSizeLoc = glGetUniformLocation(Program, "stepSize");
	initFreqLoc = glGetUniformLocation(Program, "initFreq");
	
	FOR_FLOAT_VARS(GET_UNIFORM_LOC);	

	// create frame counter font
	FTGLPixmapFont font(FONT_FILE);
	if (font.Error())
	{
		fprintf(stdout, ERROR_FTGL_FORMAT, font.Error());
		return 2;
	}
	font.FaceSize(72);
	FrameCounterFont = &font;

	// init one vertex array object
	GLuint vertexArrayID = 0;
	glGenVertexArrays(1, &vertexArrayID);
	glBindVertexArray(vertexArrayID);

	CubeVertexBuffer = cubeVertexBuffer();

	TwBar* bar = TwNewBar(SETTINGS_BAR);
	TwAddVarRW(bar, TURBULENCE_COUNT, TW_TYPE_INT32, &turbulenceCount, "");
	TwAddVarRW(bar, STEP_SIZE, TW_TYPE_FLOAT, &stepSize, "");
	TwAddVarRW(bar, INIT_FREQ, TW_TYPE_FLOAT, &initFreq, "");
	TwAddVarRO(bar, CAM_POS,   TW_TYPE_DIR3F, &curCamPos.x, "");

	FOR_FLOAT_VARS(ADD_UNIFORM_FLOAT_TO_BAR);

	timerFunc(TIMER_POST_REDISPLAY);
	while (!IsTerminate)
	{
		glutMainLoopEvent();
		GLint err = glGetError();
		if (GL_NO_ERROR != err)
		{
			fprintf(stderr, "GL error: %d\n", err);
			break;
		}
	}
	return 0;
}

bool initAll(int argc, char* argv[])
{
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_DEPTH);

	glutCreateWindow(WINDOW_NAME);
	GLenum err = glewInit();
	if (GLEW_OK != err)
	{
		fprintf(stderr, ERROR_FORMAT, glewGetErrorString(err));
		return false;
	}
	fprintf(stdout, GLEW_STATUS_FORMAT, glewGetString(GLEW_VERSION));
	TwInit(TW_OPENGL_CORE, nullptr);
	TwWindowSize(1024, 768);
	return true;
}

void keyboardFunc(unsigned char key, int x, int y)
{
	if (key == KEY_ESCAPE)
		IsTerminate = true;

	TwEventKeyboardGLUT(key, x, y);
}

void displayFunc()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glUseProgram(Program);

	glm::mat4 Projection = glm::perspective(45.0f, 4.0f / 3.0f, 0.1f, 500.0f);
    glm::mat4 Model      = glm::translate(vec3(0.0f, 0.0f, 0.0f)) * glm::mat4(1.0f);
    MVP = Projection * View() * Model;

	glUniformMatrix4fv(MVPloc, 1, GL_FALSE, &MVP[0][0]);
	glm::vec3 camReal = camPos * curCamRot;
	glUniform3f(camLoc, camReal.x, camReal.y, camReal.z);
	glUniform1i(turbulenceLoc, turbulenceCount);
	glUniform1f(stepSizeLoc, stepSize);
	glUniform1f(initFreqLoc, initFreq);

	FOR_FLOAT_VARS(UPDATE_FLOAT_UNIFORM);

	drawCube(CubeVertexBuffer);

	displayFrameCount();

	TwDraw();

	glutSwapBuffers();
}

void timerFunc(int val)
{
	if (val == TIMER_POST_REDISPLAY)
	{
		glutPostRedisplay();
		glutTimerFunc(1000 / FRAMES_PER_SECOND, &timerFunc, TIMER_POST_REDISPLAY);
	}
}

void deinitAll()
{
	TwTerminate();
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
		if (IsLeftButton) deltaHor = (MouseSpeed) * deltaTime * float(halfXWindow - x );
		GLfloat deltaVer = 0.0f;
		if (IsRightButton) deltaVer = (MouseSpeed) * deltaTime * float(halfYWindow - y );

		if (std::abs(deltaHor) > 0.03f) deltaHor = 0.03f * (deltaHor > 0.0 ? 1 : -1);
		if (std::abs(deltaVer) > 0.03f) deltaVer = 0.03f * (deltaVer > 0.0 ? 1 : -1);

		vec3 up = glm::normalize(vec3(0.0f, 1.0f, 0.0f));
		curCamRot = glm::normalize(glm::angleAxis(deltaHor * 50.0f, vec3(0.0f, 1.0f, 0.0f))) * curCamRot;

		vec3 right = glm::normalize(glm::cross(up, camPos));
		curCamRot = glm::normalize(glm::angleAxis(deltaVer * 50.0f, right)) * curCamRot;

		glutWarpPointer(halfXWindow, halfYWindow);
	}
}

void mouseFunc(int button, int state, int x, int y)
{
	if (button == GLUT_LEFT_BUTTON)
		IsLeftButton  = state == GLUT_DOWN;
	if (button == GLUT_RIGHT_BUTTON)
		IsRightButton = state == GLUT_DOWN;

	if (button == 3)
	{
		Dist += 0.5f;
		camPos = vec3(0.0f, 0.0f, Dist);
	}
	if (button == 4)
	{
		Dist -= 0.5f;
		camPos = vec3(0.0f, 0.0f, Dist);
	}

	TwEventMouseButtonGLUT(button, state, x, y);
}

void displayFrameCount()
{
	glUseProgram(0);
	char frameCountBuf[6];
	int curTime = glutGet(GLUT_ELAPSED_TIME);
	if (curTime != PrevTime && _itoa_s(1000 / (curTime - PrevTime), frameCountBuf, 10) == 0)
		FrameCounterFont->Render(frameCountBuf);
	else
		fprintf(stderr, ERROR_FORMAT, "invalid frame count");
	PrevTime = curTime;
}

mat4 View()
{
	curCamPos = camPos * curCamRot;
	return glm::lookAt(curCamPos, vec3(0.0f, 0.0f, 0.0f), vec3(0.0f, 1.0f, 0.0f));
}
