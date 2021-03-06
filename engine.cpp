﻿#include "stdafx.h"


using namespace std;
engine eng;



void mouse_button_callback(GLFWwindow* window, int button, int action, int mods)
{
	if(eng.sc != nullptr)
		if (eng.sc->cam != nullptr)
			eng.sc->cam->mouse_button_callback(window, button, action, mods);
}
void cursor_pos_callback(GLFWwindow* window, double xpos, double ypos)
{
	if (eng.sc != nullptr)
		if (eng.sc->cam != nullptr)
			eng.sc->cam->cursor_pos_callback(window, xpos, ypos);
}
void mouse_wheel_callback(GLFWwindow* window, double xoffset, double yoffset)
{
	if (eng.sc != nullptr)
		if (eng.sc->cam != nullptr)
			eng.sc->cam->mouse_wheel_callback(window, xoffset, yoffset);
}

engine::engine()
{
}
engine::~engine()
{
}

bool engine::init(int width, int height)
{
	if (!glfwInit())
		return false;
	
	window = glfwCreateWindow(width, height, "digital globe", NULL, NULL);
	if (window == NULL) 
		return false;

	glfwMakeContextCurrent(window);

	glfwSetMouseButtonCallback(window, mouse_button_callback);
	glfwSetCursorPosCallback(window, cursor_pos_callback);
	glfwSetScrollCallback(window, mouse_wheel_callback);
	
	if (glewInit() != GLEW_OK)
		return false;
	
	glfwSetInputMode(window, GLFW_STICKY_KEYS, GL_TRUE);
	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
	 
	glfwPollEvents();
	glfwSetCursorPos(window, 1024 / 2, 768 / 2);

	glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LESS);
	
	glEnable(GL_CULL_FACE);
	
	sc.reset(new scene());
	glGenVertexArrays(1, &sc->vao_mesh_id);
	glGenVertexArrays(1, &sc->vao_lights_id);

	maxfps = 25;

	return true;
}

void engine::run()
{
	sc->generate_shaders();

	double timer = 0;
	double timer2 = glfwGetTime();
	int fps = 0;

	while (glfwGetKey(window, GLFW_KEY_ESCAPE) != GLFW_PRESS && glfwWindowShouldClose(window) == 0)
	{
		double diff = glfwGetTime() - timer;

		if (diff > (1.0f / maxfps) )
		{
			timer = glfwGetTime();
			fps++;
			if ((glfwGetTime() - timer2) > 1.0)
			{
				std::stringstream ss;
				ss << "3d_engine - fps: " << fps;
				ss << " / zoom: " << sc->cam->getzoomlevel();
				
				glm::vec3 cameraPos = sc->cam->getpos();
				std::array<double, 3> lla = ecef_to_geo({ cameraPos.x, cameraPos.y, cameraPos.z });

				ss << " / lat:" << lla[0] << " lon:" << lla[1];
				ss << " / x:" << to_string(cameraPos.x) << " y:" << to_string(cameraPos.y) << " z:" << to_string(cameraPos.z);

				glfwSetWindowTitle(window, ss.str().c_str());
				timer2 = glfwGetTime();
				fps = 0;
			}
		}
		else
			continue;

		sc->draw();
	}
	glfwTerminate();
}


