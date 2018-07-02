#include "stdafx.h"

#pragma comment (lib, "opengl32.lib")

// Do not show console window
#pragma comment( linker, "/subsystem:\"windows\" /entry:\"mainCRTStartup\"" )

#include <glew.h>
#include <glut.h>
#include <freeglut.h>

#include "glm/vec3.hpp"  // glm::vec3
#include "glm/gtc/matrix_transform.hpp"  // glm::translate, glm::rotate, glm::scale, glm::perspective

#include "resource.h"

#include "Global.h"

Global* G = nullptr;

void glut_idle_func(void)  // Update logic
{
	G->game_root.Update();
	glutPostRedisplay();
}

void GL_init()
{
	// glEnable(GL_DEPTH_TEST);  // Z-buffer is not used. Z-order depends only on draw order.
	// glDepthFunc(GL_LESS);

	glClearColor(0.2f, 0.2f, 0.2f, 1.0f);

	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
}

void glut_render()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glUseProgram(G->RC.simple_texture_program);

	G->gl_render.Update_view_matrices();
	G->game_root.Draw(G->gl_render);  // Draw

	glutSwapBuffers();
}

// Only mouse buttons up/down events
void glut_mouse_func(int button, int state, int x, int y)
{
	glm::vec2 pos_f(static_cast<float>(x), static_cast<float>(y));

#ifdef _DEBUG
	utils::Log("%s", "------------------------------------------------\n");
	utils::Log("Window_X = %f  ", pos_f.x);
	utils::Log("Window_Y = %f\n", pos_f.y);
#endif

	G->gl_render.Update_view_matrices();
	glm::vec2 mouse_game_coordinates = G->gl_render.To_game_coordinates(pos_f);

#ifdef _DEBUG
	utils::Log("Game_X = %f  ", mouse_game_coordinates.x);
	utils::Log("Game_Y = %f\n", mouse_game_coordinates.y);
#endif

	switch (button)
	{
	case GLUT_LEFT_BUTTON:
		if (state == GLUT_DOWN)
		{
			G->game_root.LMB_down(mouse_game_coordinates);
		}

		if (state == GLUT_UP)
		{
			G->game_root.LMB_up(mouse_game_coordinates);
		}
	break;
	case GLUT_RIGHT_BUTTON:
		if (state == GLUT_DOWN)
		{
			G->game_root.RMB_down(mouse_game_coordinates);
		}

		if (state == GLUT_UP)
		{
			G->game_root.RMB_up(mouse_game_coordinates);
		}
	break;
	}
}

// To track mouse position inside of main window (aim the gun) when some mouse button pressed
void glut_motion_func(int x, int y)
{
	G->game_root.LMB_move(glm::vec2(static_cast<float>(x), static_cast<float>(y)));
}

// To track mouse position inside of main window (aim the gun) when NO mouse button pressed
void glut_passive_motion_func(int x, int y)
{
	G->game_root.LMB_move(glm::vec2(static_cast<float>(x), static_cast<float>(y)));
}

void glut_mouse_wheel_func(int button, int dir, int x, int y)
{}

void glut_keyboard_func(unsigned char key, int x, int y)
{
	switch (key)
	{
	case '+':  break;
	case '-':  break;
	case ' ':
#ifdef _DEBUG
		if (G->gl_render.gl_primitive_id_to_draw == GL_TRIANGLES)
		{
			G->gl_render.gl_primitive_id_to_draw = GL_LINE_STRIP;  // Debug drawing (wireframe mode)
		}
		else
		{
			G->gl_render.gl_primitive_id_to_draw = GL_TRIANGLES;  // Normal drawing mode
		}
#endif
	break;
	}
}

void glut_reshape_func(int new_w, int new_h)  // On window resize
{
	glViewport(0, 0, new_w, new_h);  // Update OpenGL rendering view port size
}

void Set_main_window_icon()
{
	HWND main_window_handle = GetActiveWindow();  // Get main window handle
	HINSTANCE hInstance = GetModuleHandle(NULL);  // Get current application instance handle
	HICON hIcon = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_ICON1));  // Load icon by app instance handle and icon ID

	_ASSERTE(hIcon != 0);

	// Set main window icon
	SendMessage(main_window_handle, WM_SETICON, ICON_BIG, (LPARAM)hIcon);
	SendMessage(main_window_handle, WM_SETICON, ICON_SMALL, (LPARAM)hIcon);
}

int main(int argc, char** argv)
{
	utils::Log("%s", "App start ---------------------------------------------------------------\n");

#ifdef _DEBUG  // To find memory leaks
	// _CrtSetReportMode( _CRT_WARN, _CRTDBG_MODE_FILE );  // Enable file output
	// _CrtSetReportFile( _CRT_WARN, _CRTDBG_FILE_STDOUT );  // Set file to stdout
	// _CrtMemState _ms; 
	// _CrtMemCheckpoint(&_ms);  // Now forget about all pieces of memory allocated before
#endif

	G = new Global();

	glutInit(&argc, argv);

	glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE);
	glutInitWindowPosition(G->game_root.g_s.initial_window_x, G->game_root.g_s.initial_window_y);
	glutInitWindowSize(G->game_root.g_s.initial_window_w, G->game_root.g_s.initial_window_h);

	glutCreateWindow("Demo Serhiy Paperniuk V1.0");
	glutIdleFunc(&glut_idle_func);  // Update logic
	glutDisplayFunc(&glut_render);
	glutReshapeFunc(&glut_reshape_func);

	glutMouseFunc(glut_mouse_func);
	glutPassiveMotionFunc(glut_passive_motion_func);
	glutMotionFunc(glut_motion_func);
	glutKeyboardFunc(glut_keyboard_func);
	glutMouseWheelFunc(glut_mouse_wheel_func);

	glewInit();
	if (!GLEW_VERSION_2_0)
	{
		utils::Log("%s", "ERROR: OpenGL 2.0 not available\n");
		return 1;
	}

	G->RC.Load_resources();
	GL_init();

	Set_main_window_icon();

	glutSetOption(GLUT_ACTION_ON_WINDOW_CLOSE, GLUT_ACTION_GLUTMAINLOOP_RETURNS);  // Exit freeglut main loop when we close main window
	glutMainLoop();  // Run main loop

	G->RC.Free_resources();
	delete G;

#ifdef _DEBUG  // To find memory leaks
	int* test_ptr = new int;  // Test memory leak (To check if it writes something to output)
	*test_ptr = -1;  // Init test block of memory with some value (FF FF FF FF)

	// unsigned char* test_bytes_array_pointer = new unsigned char[ 100 ];  // Test memory leak (To check if it writes something to output)
	// _CrtMemDumpAllObjectsSince(&_ms);  // Dump memory leaks

	_CrtDumpMemoryLeaks();  // Write all memory leaks information (from C Run-time Library (CRT)) to Visual Studio output

	delete test_ptr;
#endif

	utils::Log("%s", "App stop ----------------------------------------------------------------\n");

	return 0;
}
