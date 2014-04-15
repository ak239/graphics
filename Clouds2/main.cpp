// GL libs
#include <GL/glew.h>
#include <GL/freeglut.h>
#include <AntTweakBar.h>
#include <glm/glm.hpp>
#include <glm/ext.hpp>
// stdlibs
#include <cstdio>
// my headers
#include "geometric.h"
#include "glerror.h"
#include "shader.h"
#include "texture.h"

/*
freeflut
glutFullScreenToggle - change fullscreent to normal and back
glutPostRedisplay - repaint current window
glutSwapBuffers - swap back and front buffers
*/

void reshape(int w, int h);
void display();
void mouse(int button, int state, int x, int y);
void motion(int x,int y);
void passiveMotion(int x, int y);
void keyboard(unsigned char key, int x, int y);
void special(int key, int x, int y);
void close();

const char* WINDOW_TITLE = "Clouds Demo";
const int INIT_WINDOW_WIDTH = 640;
const int INIT_WINDOW_HEIGHT = 480;

GLuint cubemapTexture; 
GLuint cubeVertexArray;
GLuint skyboxShader;
GLuint uSkyboxWVP;
GLuint uSkyboxTexture;

GLuint cloudsVertexArray;
GLuint cloudsShader;
GLuint uCloudsWVP;

/// camera
bool IsLeftButton  = false;
bool IsRightButton = false;
GLint LastTime = 0;
GLfloat Dist    = 10.0f;
GLfloat MouseSpeed = 0.001f;
glm::quat cameraRotation = glm::normalize(glm::quat());

// all uniforms
glm::vec3 g_CameraPosition(0.0, 0.0, 10.0);
GLuint    uv3_CameraPosition;
glm::vec3 g_RealCamPosition;

GLfloat g_StepSize = 0.04f;
GLuint  uf_StepSize;

typedef enum { PERLIN_NOISE_2D = 0, SIMPLEX_NOISE_2D = 1 } Noises2D;
Noises2D g_Noise2D = PERLIN_NOISE_2D;
TwEnumVal noises2DEv[] = { {PERLIN_NOISE_2D, "Perlin 2D Noise"}, {SIMPLEX_NOISE_2D, "Simplex 2D Noise"} };
TwType noises2DType;
GLuint ui_Noise2DType;

GLint g_Noise2DTurbulence = 1;
GLuint ui_Noise2DTurbulence;

glm::vec2 g_Noise2DPhase;
GLuint    uv2_Noise2DPhase;

glm::vec2 g_Noise2DFreq = glm::vec2(0.1f);
GLuint uv2_Noise2DFreq;

GLfloat g_T;
GLuint uf_T;

GLfloat g_Ptop = 1.0f;
GLuint  uf_Ptop;

GLfloat g_Pbot = 0.2f;
GLuint  uf_Pbot;

GLfloat g_Ktop = 3.0f;
GLuint  uf_Ktop;

GLfloat g_Kbot = 1.0f;
GLuint  uf_Kbot;

typedef enum { CALC_H_CONST = 0, CALC_H_TWO_VALS = 1} CalcsH;
CalcsH g_CalcH = CALC_H_TWO_VALS;
TwEnumVal calcsHEv[] = {{ CALC_H_CONST, "Const H"}, {CALC_H_TWO_VALS, "Two H values"}};
TwType calcsHType;
GLuint ui_CalcHType;

GLfloat g_H = 0.8;
GLuint  uf_H;

typedef enum { PERLIN_NOISE_3D = 0, SIMPLEX_NOISE_3D = 1 } Noises3D;
Noises3D g_Noise3D = PERLIN_NOISE_3D;
TwEnumVal noises3DEv[] = { {PERLIN_NOISE_3D, "Perlin 3D Noise"}, {SIMPLEX_NOISE_3D, "Simplex 3D Noise"} };
TwType noises3DType;
GLuint ui_Noise3DType;

glm::vec3 g_Noise3DFreq = glm::vec3(1.0f);
GLuint uv3_Noise3DFreq;

GLint g_Noise3DTurbulence = 1;
GLuint ui_Noise3DTurbulence;

typedef enum { SUM = 0, MUL = 1 } BorderCalcs;
BorderCalcs g_BorderCalc = SUM;
TwEnumVal borderCalcsEv[] = {{ SUM, "sum"}, { MUL, "mul"}};
TwType borderCalcsType;
GLuint ui_BorderCalc;

typedef enum { LINEAR = 0, THREE_XX = 1 } Sigmoids;
Sigmoids g_Sigmoid = LINEAR;
TwEnumVal sigmoidsEv[] = {{ LINEAR, "linear"}, {THREE_XX, "3x^2"}};
TwType sigmoidType;
GLuint ui_Sigmoid;

GLfloat g_xWidthDiv2 = 10.0f;
GLuint uf_xWidthDiv2;
GLfloat g_zWidthDiv2 = 10.0f;
GLuint uf_zWidthDiv2;

GLfloat g_rho0 = 0.05;
GLuint uf_rho0;

GLuint um4_scaleMatrix;

GLfloat g_fogLowDist = 100.0;
GLuint uf_fogLowDist;

GLfloat g_fogHighDist = 12.0;
GLuint uf_fogHighDist;

int main(int argc, char* argv[])
{
	// --- START INIALIZE

	glutInit(&argc, argv);
	// not exit after glutLeaveMainLoop() or close main window
	glutSetOption(GLUT_ACTION_ON_WINDOW_CLOSE, GLUT_ACTION_CONTINUE_EXECUTION);
	// init double buffering
	glutInitDisplayMode(GLUT_DOUBLE|GLUT_RGBA|GLUT_DEPTH);
	// create window
	glutInitWindowPosition(0, 0);
	glutInitWindowSize(INIT_WINDOW_WIDTH, INIT_WINDOW_HEIGHT);
	glutCreateWindow(WINDOW_TITLE);
	// init glew
	GLenum err = glewInit();
	if (GLEW_OK != err)
	{
		fprintf(stderr, "Error: %s\n", glewGetErrorString(err));
		return 0;
	}
	fprintf(stdout, "Status: Using GLEW %s\n", glewGetString(GLEW_VERSION));
	// init AntTweakBar
	TwInit(TW_OPENGL_CORE, nullptr);
	TwWindowSize(INIT_WINDOW_WIDTH, INIT_WINDOW_HEIGHT);
	// init all callbacks
	glutReshapeFunc(reshape);
	glutDisplayFunc(display);
	glutMouseFunc(mouse);
	glutMotionFunc(motion);
	glutPassiveMotionFunc(passiveMotion);
	glutKeyboardFunc(keyboard);
	glutSpecialFunc(special);
	glutCloseFunc(close);
	TwGLUTModifiersFunc(glutGetModifiers);
	// gl enables
	glEnable(GL_DEPTH_TEST);
	glDepthMask(GL_TRUE);
	glDepthFunc(GL_LESS);

	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	glClearColor(135.0f/255.0f, 206.0f/255.0f, 235.0f/255.0f, 1.0f);

	// --- END INITIALIZE

	TwBar* bar = TwNewBar("bar");
	TwAddVarRO(bar, "camera position", TW_TYPE_DIR3F, &g_RealCamPosition[0], "");
	TwAddVarRW(bar, "step size", TW_TYPE_FLOAT, &g_StepSize, "");

	noises2DType = TwDefineEnum("Noises2DType", noises2DEv, 2);
	TwAddVarRW(bar, "Noise2D", noises2DType, &g_Noise2D, NULL);
	
	TwAddVarRW(bar, "Noise2D Phase.x", TW_TYPE_FLOAT, &g_Noise2DPhase.x, "");
	TwAddVarRW(bar, "Noise2D Phase.z", TW_TYPE_FLOAT, &g_Noise2DPhase.y, "");

	TwAddVarRW(bar, "Noise2D Freq.x", TW_TYPE_FLOAT, &g_Noise2DFreq.x, "");
	TwAddVarRW(bar, "Noise2D Freq.z", TW_TYPE_FLOAT, &g_Noise2DFreq.y, "");

	TwAddVarRW(bar, "Noise2D Turbulence", TW_TYPE_INT32, &g_Noise2DTurbulence, "");

	TwAddVarRW(bar, "T", TW_TYPE_FLOAT, &g_T, "");
	TwAddVarRW(bar, "Ptop", TW_TYPE_FLOAT, &g_Ptop, "");
	TwAddVarRW(bar, "Pbot", TW_TYPE_FLOAT, &g_Pbot, "");
	TwAddVarRW(bar, "kTop", TW_TYPE_FLOAT, &g_Ktop, "");
	TwAddVarRW(bar, "kBot", TW_TYPE_FLOAT, &g_Kbot, "");

	calcsHType = TwDefineEnum("CalcsHType", calcsHEv, 2);
	TwAddVarRW(bar, "Calc H", calcsHType, &g_CalcH, NULL);

	TwAddVarRW(bar, "H", TW_TYPE_FLOAT, &g_H, nullptr);

	noises3DType = TwDefineEnum("Noises3DType", noises3DEv, 2);
	TwAddVarRW(bar, "Noise3D", noises3DType, &g_Noise3D, NULL);

	TwAddVarRW(bar, "Noise3D Freq.x", TW_TYPE_FLOAT, &g_Noise3DFreq.x, "");
	TwAddVarRW(bar, "Noise3D Freq.y", TW_TYPE_FLOAT, &g_Noise3DFreq.y, "");
	TwAddVarRW(bar, "Noise3D Freq.z", TW_TYPE_FLOAT, &g_Noise3DFreq.z, "");

	TwAddVarRW(bar, "Noise3D Turbulence", TW_TYPE_INT32, &g_Noise3DTurbulence, nullptr);

	borderCalcsType = TwDefineEnum("BorderCalcsType", borderCalcsEv, 2);
	TwAddVarRW(bar, "Border Calc", borderCalcsType, &g_BorderCalc, nullptr);

	sigmoidType = TwDefineEnum("SigmoidType", sigmoidsEv, 2);
	TwAddVarRW(bar, "Sigmoid Type", sigmoidType, &g_Sigmoid, nullptr);

	TwAddVarRW(bar, "X Width / 2", TW_TYPE_FLOAT, &g_xWidthDiv2, nullptr);
	TwAddVarRW(bar, "Z Width / 2", TW_TYPE_FLOAT, &g_zWidthDiv2, nullptr);

	TwAddVarRW(bar, "rho0", TW_TYPE_FLOAT, &g_rho0, nullptr);

	TwAddVarRW(bar, "fog low dist", TW_TYPE_FLOAT, &g_fogLowDist, nullptr);
	TwAddVarRW(bar, "fog high dist", TW_TYPE_FLOAT, &g_fogHighDist, nullptr);

	glActiveTexture(GL_TEXTURE0);
	cubemapTexture  = loadCubemap("./Teide");
	cubeVertexArray = genCubeVertexBuffer(glm::vec3(40.0f));
	skyboxShader = loadShaders("skybox.vs", "skybox.fs");

	glUseProgram(skyboxShader);
	uSkyboxWVP     = glGetUniformLocation(skyboxShader, "uWVP");
	uSkyboxTexture = glGetUniformLocation(skyboxShader, "uSkyboxTexture");

	cloudsShader = loadShaders("vol.vs", "clouds.fs");
	glUseProgram(cloudsShader);
	uCloudsWVP = glGetUniformLocation(cloudsShader, "uWVP");

	uv3_CameraPosition = glGetUniformLocation(cloudsShader, "cameraPosition");
	uf_StepSize = glGetUniformLocation(cloudsShader, "stepSize");
	ui_Noise2DType = glGetUniformLocation(cloudsShader, "noise2dType");
	uv2_Noise2DPhase = glGetUniformLocation(cloudsShader, "noise2dPhase");
	uv2_Noise2DFreq = glGetUniformLocation(cloudsShader, "noise2dFreq");
	ui_Noise2DTurbulence = glGetUniformLocation(cloudsShader, "noise2dTurbulence");
	uf_T = glGetUniformLocation(cloudsShader, "T");
	uf_Ptop = glGetUniformLocation(cloudsShader, "Ptop");
	uf_Pbot = glGetUniformLocation(cloudsShader, "Pbot");
	uf_Ktop = glGetUniformLocation(cloudsShader, "ktop");
	uf_Kbot = glGetUniformLocation(cloudsShader, "kbot");
	ui_CalcHType = glGetUniformLocation(cloudsShader, "calcHtype");
	uf_H = glGetUniformLocation(cloudsShader, "H");
	ui_Noise3DType = glGetUniformLocation(cloudsShader, "noise3dType");
	ui_BorderCalc = glGetUniformLocation(cloudsShader, "borderCalcType");
	ui_Sigmoid = glGetUniformLocation(cloudsShader, "sigmoidType");
	uf_xWidthDiv2 = glGetUniformLocation(cloudsShader, "xWidthDiv2");
	uf_zWidthDiv2 = glGetUniformLocation(cloudsShader, "zWidthDiv2");
	um4_scaleMatrix = glGetUniformLocation(cloudsShader, "scaleMatrix");
	uf_rho0 = glGetUniformLocation(cloudsShader, "rho0");
	uv3_Noise3DFreq = glGetUniformLocation(cloudsShader, "noise3dFreq");
	ui_Noise3DTurbulence = glGetUniformLocation(cloudsShader, "noise3dTurbulence");
	uf_fogHighDist = glGetUniformLocation(cloudsShader, "fogHighDist");
	uf_fogLowDist = glGetUniformLocation(cloudsShader, "fogLowDist");

	cloudsVertexArray = genCubeVertexBuffer(glm::vec3(1.0f, 1.0f, 1.0f));

	glutMainLoop();

	return 0;
}

void reshape(int w, int h)
{
	glViewport(0, 0, w, h);
	TwWindowSize(w, h);
}

void display()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	//// prepare WVP
	glm::mat4 proj  = glm::perspective(60.0f, 4.0f / 3.0f, 0.1f, 500.0f);
	glm::mat4 view = glm::lookAt(g_CameraPosition * cameraRotation, glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f));
    glm::mat4 skyboxModel = glm::translate(g_CameraPosition * cameraRotation - glm::vec3(0.0f, 15.0f, 0.0f)) * glm::mat4(1.0f);
    glm::mat4 skyboxWVP = proj * view * skyboxModel;

	//DEBUG_MATRIX(WVP);

	// set skybox uniforms
	GE(glUseProgram(skyboxShader));
	GE(glUniformMatrix4fv(uSkyboxWVP, 1, GL_FALSE, &skyboxWVP[0][0]));
	GE(glUniform1i(uSkyboxTexture, 0));
	// activate texture and bind
	GE(glActiveTexture(GL_TEXTURE0));
	GE(glBindTexture(GL_TEXTURE_CUBE_MAP, cubemapTexture));
	// prepare to render skybox
	// draw vertexes
	GE(glEnableVertexAttribArray(0));
	GE(glBindBuffer(GL_ARRAY_BUFFER, cubeVertexArray));
	GE(glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, (void*)0));
	GE(glDrawArrays(GL_TRIANGLES, 0, 36));
	GE(glDisableVertexAttribArray(0));

	// clouds
	//glEnable(GL_DEPTH_TEST);
	//glDepthMask(GL_TRUE);
	//glDepthFunc(GL_LESS);

	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    glm::mat4 cloudsModel = glm::mat4(1.0f);
	glm::mat4 scaleMatrix = glm::scale(glm::vec3(g_xWidthDiv2, std::max(g_Ktop, g_Kbot), g_zWidthDiv2));
	glm::mat4 cloudsWVP = proj * view * cloudsModel * scaleMatrix;

	GE(glUseProgram(cloudsShader));
	GE(glUniformMatrix4fv(uCloudsWVP, 1, GL_FALSE, &cloudsWVP[0][0]));
	GE(glUniformMatrix4fv(um4_scaleMatrix, 1, GL_FALSE, &scaleMatrix[0][0]));
	g_RealCamPosition = g_CameraPosition * cameraRotation;
	
	glUniform3f(uv3_CameraPosition, g_RealCamPosition.x, g_RealCamPosition.y, g_RealCamPosition.z);
	glUniform1f(uf_StepSize, g_StepSize);
	glUniform1i(ui_Noise2DType, g_Noise2D);
	glUniform2f(uv2_Noise2DPhase, g_Noise2DPhase.x, g_Noise2DPhase.y);
	glUniform2f(uv2_Noise2DFreq, g_Noise2DFreq.x, g_Noise2DFreq.y);
	glUniform1i(ui_Noise2DTurbulence, g_Noise2DTurbulence);
	glUniform1f(uf_T, g_T);
	glUniform1f(uf_Ptop, g_Ptop);
	glUniform1f(uf_Pbot, g_Pbot);
	glUniform1f(uf_Ktop, g_Ktop);
	glUniform1f(uf_Kbot, g_Kbot);
	glUniform1i(ui_CalcHType, g_CalcH);
	glUniform1f(uf_H, g_H);
	glUniform1i(ui_Noise3DType, g_Noise3D);
	glUniform1i(ui_BorderCalc, g_BorderCalc);
	glUniform1i(ui_Sigmoid, g_Sigmoid);
	glUniform1f(uf_xWidthDiv2, g_xWidthDiv2);
	glUniform1f(uf_zWidthDiv2, g_zWidthDiv2);
	glUniform1f(uf_rho0, g_rho0);
	glUniform3f(uv3_Noise3DFreq, g_Noise3DFreq.x, g_Noise3DFreq.y, g_Noise3DFreq.z);
	glUniform1i(ui_Noise3DTurbulence, g_Noise3DTurbulence);
	glUniform1f(uf_fogHighDist, g_fogHighDist);
	glUniform1f(uf_fogLowDist,  g_fogLowDist);
	
	GE(glEnableVertexAttribArray(0));
	GE(glBindBuffer(GL_ARRAY_BUFFER, cloudsVertexArray));
	GE(glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, (void*)0));
	GE(glDrawArrays(GL_TRIANGLES, 0, 36));
	GE(glDisableVertexAttribArray(0));

	TwDraw();
	glutSwapBuffers();
	glutPostRedisplay();
}

void mouse(int button, int state, int x, int y)
{
	if (!TwEventMouseButtonGLUT(button, state, x, y))
	{
		if (button == GLUT_LEFT_BUTTON)
			IsLeftButton  = state == GLUT_DOWN;
		if (button == GLUT_RIGHT_BUTTON)
			IsRightButton = state == GLUT_DOWN;

		if (button == 3)
		{
			Dist += 0.5f;
			g_CameraPosition = glm::vec3(0.0f, 0.0f, Dist);
		}
		if (button == 4)
		{
			Dist -= 0.5f;
			g_CameraPosition = glm::vec3(0.0f, 0.0f, Dist);
		}
	}
}

void motion(int x,int y)
{
	if (!TwEventMouseMotionGLUT(x, y))
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
			if (IsLeftButton) deltaHor = (MouseSpeed) * deltaTime * float(halfXWindow - x );
			GLfloat deltaVer = 0.0f;
			if (IsRightButton) deltaVer = (MouseSpeed) * deltaTime * float(halfYWindow - y );

			if (std::abs(deltaHor) > 0.03f) deltaHor = 0.03f * (deltaHor > 0.0 ? 1 : -1);
			if (std::abs(deltaVer) > 0.03f) deltaVer = 0.03f * (deltaVer > 0.0 ? 1 : -1);

			glm::vec3 up = glm::normalize(glm::vec3(0.0f, 1.0f, 0.0f));
			cameraRotation = glm::normalize(glm::angleAxis(deltaHor * 50.0f, glm::vec3(0.0f, 1.0f, 0.0f))) * cameraRotation;

			glm::vec3 right = glm::normalize(glm::cross(up, g_CameraPosition));
			cameraRotation = glm::normalize(glm::angleAxis(deltaVer * 50.0f, right)) * cameraRotation;

			glutWarpPointer(halfXWindow, halfYWindow);
		}
	}
}

void passiveMotion(int x, int y)
{
	if (!TwEventMouseMotionGLUT(x, y))
	{
		// ...
	}
}

void keyboard(unsigned char key, int x, int y)
{
	if (!TwEventKeyboardGLUT(key, x, y))
	{
		// ...
	}
}

void special(int key, int x, int y)
{
	if (!TwEventSpecialGLUT(key, x, y))
	{
		// ...
	}
}

void close()
{
	TwTerminate();
}
