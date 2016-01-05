// third_lab.cpp : Defines the entry point for the application.
#include <stdafx.h>
#include <windows.h>

#include <GL/gl.h>
#include <GL/glu.h>
#include "glaux.h"

static	HGLRC hRC;		
static	HDC hDC;		
GLvoid Initial(GLsizei Width, GLsizei Height);
LRESULT CALLBACK WndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);
GLvoid Resize(GLsizei Width, GLsizei Height);
GLvoid Draw(GLvoid);	
static float x,y, scale, angle;
int lp=0;

GLvoid Initial(GLsizei Width, GLsizei Height)
{
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
	glClearDepth(1.0);
	glDepthFunc(GL_LESS);
	glEnable(GL_DEPTH_TEST);

    glEnable(GL_LIGHTING);
    glLightModelf(GL_LIGHT_MODEL_TWO_SIDE, GL_TRUE);
    glEnable(GL_NORMALIZE);

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	
	gluPerspective(45.0f, (GLfloat)Width / (GLfloat)Height, 0.1f, 100.0f);
	glMatrixMode(GL_MODELVIEW);

	x=0.0;y=0.0;
	scale=0.6; angle=90;
}

GLvoid Resize(GLsizei Width, GLsizei Height)
{
	if(Height==0)
		Height = 1;
	glViewport(0, 0, Width, Height);

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();

	gluPerspective(45.0f, (GLfloat)Width / (GLfloat)Height, 0.1f, 100.0f);

	glMatrixMode(GL_MODELVIEW);
}

GLvoid Draw(GLvoid)
{

    angle += 0.1f;

	GLUquadricObj *quadObj;
	quadObj = gluNewQuadric(); 
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);	

    glEnable(GL_LIGHT0);


    float light_ambient[] = { 0.0,0.0,0.0,1.0 };
    float light_diffuse[] = { 0.5,0.5,0.5,1.0 };
    float light_specular[] = { 1.0,1.0,1.0,1.0 };
    float light_position[] = { 1.0, 0.0, 1.0 ,0.0 };
    glRotatef(-angle, 0.0f, 1.0f, 0.0f);
    glLightfv(GL_LIGHT0, GL_AMBIENT, light_ambient);
    glLightfv(GL_LIGHT0, GL_DIFFUSE, light_diffuse);
    glLightfv(GL_LIGHT0, GL_SPECULAR, light_specular);
    glLightfv(GL_LIGHT0, GL_POSITION, light_position);

	glLoadIdentity();			
	gluQuadricDrawStyle(quadObj, GLU_FILL); 
	glClearColor(1.0,1.0,1.0,0.0);
	
    glTranslatef(x, -3.0f, -15.0f);
	
    glScalef(scale, scale, scale);
	glRotatef(angle, 0.0f, 1.0f, 0.0f);


	glColor3f(0.0f, 0.0f, 0.5f);
	glTranslatef(0.0, 0.0, 5.0);
	gluSphere(quadObj, 2.5, 30, 30);
	glColor3f(0.0f, 0.0f, 0.1f);
	glTranslatef(0.0, 0.0, -5.0);
	glTranslatef(0.0, 0.0, -5.0);
	gluCylinder (quadObj, 1, 1, 8, 20, 20);
	glTranslatef(0.0f, 0.0f, 0.0f);
	glTranslatef(0.0, 0.0, 5.0);
	glTranslatef(0.0, 0.0, -7.0);
	glColor3f(0.0f, 0.0f, 0.5f);
	gluSphere(quadObj, 2.5, 30, 30);



    glDisable(GL_LIGHT0);
}




LRESULT CALLBACK WndProc( HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	RECT Screen;
	GLuint PixelFormat;

	static PIXELFORMATDESCRIPTOR pfd =
	{
		sizeof(PIXELFORMATDESCRIPTOR),		
		1,									
		PFD_DRAW_TO_WINDOW|					// format must support Window
		PFD_SUPPORT_OPENGL|					// format must support OpenGL
		PFD_DOUBLEBUFFER,					// must support double buffer
		PFD_TYPE_RGBA,						// требуется RGBA формат
		16,									// 16Bit color depth
		0, 0, 0, 0, 0, 0,					// Color bits ignored ?
		0,									// No Alpha buffer
		0,									// shift bit ignored
		0,									// no accumulation buffer
		0, 0, 0, 0,							// accumulation buffer bits ignored
		16,									// 16bit z-buffer (depth buffer)
		0,									// no stencil buffer
		0,									// no auxiliary buffer
		PFD_MAIN_PLANE,						// main drawing layer
		0,									// reserved
		0, 0, 0								// layer mask ignored
	};

	switch(msg)
	{
	case WM_CREATE:
		hDC = GetDC(hWnd);
		PixelFormat = ChoosePixelFormat(hDC, &pfd);
		SetPixelFormat(hDC, PixelFormat, &pfd);
		hRC = wglCreateContext(hDC);
		wglMakeCurrent(hDC, hRC);
		GetClientRect(hWnd, &Screen);
		Initial(Screen.right, Screen.bottom);
		break;
	case WM_DESTROY:
		ChangeDisplaySettings(NULL, 0);

		wglMakeCurrent(hDC, NULL);
		wglDeleteContext(hRC);
		ReleaseDC(hWnd, hDC);

		PostQuitMessage(0);
		break;
		case WM_KEYDOWN:{
		switch( wParam )
		{
			case VK_ESCAPE:PostQuitMessage(0);break;
			case VK_DOWN:{angle -= 1.0f; break; }
			case VK_RIGHT:{ x += 0.1f; break; }
	     	case VK_UP:{ angle += 1.0f; break; }
			case VK_LEFT:{x -= 0.1f; break; }
			case VK_PRIOR: //Page Up
                scale += 0.1f;
				break; 
			case VK_NEXT: //PageDown
                scale -= 0.1f;
				break;
			case 0x31: {lp=1;break;}
			case 0x32: {lp=0; break;}
		}
		}
		break;
		case WM_SIZE:
		Resize(LOWORD(lParam), HIWORD(lParam));
		break;
	default:
		return( DefWindowProc(hWnd, msg, wParam, lParam));
	}
	return(0);
}

int WINAPI WinMain( HINSTANCE hInst, HINSTANCE hPrevInst, LPSTR lpCmd, int nShowCmd)
{
	MSG			msg;
	WNDCLASS	wc;
	HWND		hWnd;

	wc.style		= CS_HREDRAW | CS_VREDRAW | CS_OWNDC;
	wc.lpfnWndProc	= (WNDPROC)WndProc;
	wc.cbClsExtra	= 0;
	wc.cbWndExtra	= 0;
	wc.hInstance	= hInst;
	wc.hIcon		= NULL;
	wc.hCursor		= LoadCursor(NULL, IDC_ARROW);
	wc.hbrBackground	= NULL;
	wc.lpszMenuName		= NULL;
	wc.lpszClassName	= "third_lab";

	RegisterClass(&wc); 
	hWnd = CreateWindow("third_lab","third_lab",WS_POPUP |WS_OVERLAPPEDWINDOW |WS_CLIPSIBLINGS, 100, 80, 
						800, 600, NULL, NULL, hInst, NULL);
	ShowWindow(hWnd, SW_SHOW);
	UpdateWindow(hWnd);
	SetFocus(hWnd);

	while(1)
	{
		while(PeekMessage(&msg, NULL, 0, 0, PM_NOREMOVE))
		{
			if(GetMessage(&msg, NULL, 0, 0))
			{
				TranslateMessage(&msg);
				DispatchMessage(&msg);
			}
			else
			{
				return TRUE;
			}
		}
		Draw();
		SwapBuffers(hDC);
	}
}

