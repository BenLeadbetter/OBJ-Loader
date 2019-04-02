// Primitieves.cpp
// OpenGL SuperBible, Chapter 2
// Demonstrates the 7 Geometric Primitives
// Program by Richard S. Wright Jr.

#include <GL/glew.h>
#include <GLTools.h>	// OpenGL toolkit
#include <GLMatrixStack.h>
#include <GLFrame.h>
#include <GLFrustum.h>
#include <GLBatch.h>
#include <GLGeometryTransform.h>

#include <math.h>
#ifdef __APPLE__
#include <glut/glut.h>
#else
#define FREEGLUT_STATIC
#include <GL/glut.h>
#endif

#include <iostream>
#include "OBJ_Loader.h"


/////////////////////////////////////////////////////////////////////////////////
// An assortment of needed classes
GLShaderManager		shaderManager;
GLMatrixStack		modelViewMatrix;
GLMatrixStack		projectionMatrix;
GLFrame				cameraFrame;
GLFrame             objectFrame;
GLFrustum			viewFrustum;

GLBatch				triangleBatch;

GLGeometryTransform	transformPipeline;
M3DMatrix44f		shadowMatrix;


GLfloat vGreen[] = { 0.0f, 1.0f, 0.0f, 1.0f };
GLfloat vBlack[] = { 0.0f, 0.0f, 0.0f, 1.0f };


// Keep track of effects step
//int nStep = 0;


///////////////////////////////////////////////////////////////////////////////
// This function does any needed initialization on the rendering context. 
// This is the first opportunity to do any OpenGL related tasks.
void SetupRC()
	{
    // Black background
    glClearColor(0.7f, 0.7f, 0.7f, 1.0f );

	shaderManager.InitializeStockShaders();

	glEnable(GL_DEPTH_TEST);

	transformPipeline.SetMatrixStacks(modelViewMatrix, projectionMatrix);

	cameraFrame.MoveForward(-15.0f);
       
        
    // For custom shape
    size_t iCounter = 0;
    GLfloat vPoints[10000][3];  // Scratch array, more than we need

    // Load up the data
    Loader loader;
    loader.loadFile("OBJ_Files/shape1.obj");
    iCounter = loader.FillTriangleBatch(vPoints);
    
    // Load the triangle strip
    triangleBatch.Begin(GL_TRIANGLES, iCounter);
    triangleBatch.CopyVertexData3f(vPoints);
    triangleBatch.End();    

    }


/////////////////////////////////////////////////////////////////////////
void DrawWireFramedBatch(GLBatch* pBatch)
    {
    // Draw the batch solid green
    shaderManager.UseStockShader(GLT_SHADER_FLAT, transformPipeline.GetModelViewProjectionMatrix(), vGreen);
    pBatch->Draw();
    
    // Draw black outline
    glPolygonOffset(-1.0f, -1.0f);      // Shift depth values
    glEnable(GL_POLYGON_OFFSET_LINE);

    // Draw lines antialiased
    glEnable(GL_LINE_SMOOTH);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    
    // Draw black wireframe version of geometry
    glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
    glLineWidth(2.5f);
    shaderManager.UseStockShader(GLT_SHADER_FLAT, transformPipeline.GetModelViewProjectionMatrix(), vBlack);
    pBatch->Draw();
    
    // Put everything back the way we found it
    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
    glDisable(GL_POLYGON_OFFSET_LINE);
    glLineWidth(1.0f);
    glDisable(GL_BLEND);
    glDisable(GL_LINE_SMOOTH);
    }


///////////////////////////////////////////////////////////////////////////////
// Called to draw scene
void RenderScene(void)
	{    
	// Clear the window with current clearing color
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);

	modelViewMatrix.PushMatrix();
		M3DMatrix44f mCamera;
		cameraFrame.GetCameraMatrix(mCamera);
		modelViewMatrix.MultMatrix(mCamera);

        M3DMatrix44f mObjectFrame;
        objectFrame.GetMatrix(mObjectFrame);
        modelViewMatrix.MultMatrix(mObjectFrame);

        shaderManager.UseStockShader(GLT_SHADER_FLAT, transformPipeline.GetModelViewProjectionMatrix(), vGreen);

        // Draw our revolution volume
        DrawWireFramedBatch(&triangleBatch);


		
	modelViewMatrix.PopMatrix();

	// Flush drawing commands
	glutSwapBuffers();
    }


// Respond to arrow keys by moving the camera frame of reference
void SpecialKeys(int key, int x, int y)
    {
	if(key == GLUT_KEY_UP)
		objectFrame.RotateWorld(m3dDegToRad(-5.0f), 1.0f, 0.0f, 0.0f);
    
	if(key == GLUT_KEY_DOWN)
		objectFrame.RotateWorld(m3dDegToRad(5.0f), 1.0f, 0.0f, 0.0f);
	
	if(key == GLUT_KEY_LEFT)
		objectFrame.RotateWorld(m3dDegToRad(-5.0f), 0.0f, 1.0f, 0.0f);
    
	if(key == GLUT_KEY_RIGHT)
		objectFrame.RotateWorld(m3dDegToRad(5.0f), 0.0f, 1.0f, 0.0f);
    
	glutPostRedisplay();
    }




///////////////////////////////////////////////////////////////////////////////
// A normal ASCII key has been pressed.
// In this case, advance the scene when the space bar is pressed
void KeyPressFunc(unsigned char key, int x, int y)
	{
	
                
    glutPostRedisplay();
	}

///////////////////////////////////////////////////////////////////////////////
// Window has changed size, or has just been created. In either case, we need
// to use the window dimensions to set the viewport and the projection matrix.
void ChangeSize(int w, int h)
	{
	glViewport(0, 0, w, h);
	viewFrustum.SetPerspective(35.0f, float(w) / float(h), 1.0f, 500.0f);
	projectionMatrix.LoadMatrix(viewFrustum.GetProjectionMatrix());
	modelViewMatrix.LoadIdentity();
	}

///////////////////////////////////////////////////////////////////////////////
// Main entry point for GLUT based programs
int main(int argc, char* argv[])
	{
	gltSetWorkingDirectory(argv[0]);
	
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA | GLUT_DEPTH | GLUT_STENCIL);
	glutInitWindowSize(800, 600);
	glutCreateWindow("GL_POINTS");
    glutReshapeFunc(ChangeSize);
    glutKeyboardFunc(KeyPressFunc);
    glutSpecialFunc(SpecialKeys);
    glutDisplayFunc(RenderScene);
        
	GLenum err = glewInit();
	if (GLEW_OK != err) {
		fprintf(stderr, "GLEW Error: %s\n", glewGetErrorString(err));
		return 1;
		}
	

	SetupRC();

	glutMainLoop();

	return 0;
	}
