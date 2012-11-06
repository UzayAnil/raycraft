// Raycraft internals
#include <rc/world.hpp>
#include <rc/renderer.hpp>

#include <GL/glfw.h>

#include <ctime>

// Configuration
const int WIDTH = 640;
const int HEIGHT = 480;

int main()
{
	// Initialize glfw
	glfwInit();

	// Open window
	glfwOpenWindowHint(GLFW_OPENGL_VERSION_MAJOR, 4);
	glfwOpenWindowHint(GLFW_OPENGL_VERSION_MINOR, 0);
	glfwOpenWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwOpenWindowHint(GLFW_WINDOW_NO_RESIZE, GL_TRUE);
	glfwOpenWindow(WIDTH, HEIGHT, 0, 0, 0, 0, 0, 0, GLFW_WINDOW);
	glfwSetWindowTitle("raycraft");

	// Center window on screen
	GLFWvidmode videoMode;
	glfwGetDesktopMode(&videoMode);
	glfwSetWindowPos(videoMode.Width / 2 - WIDTH / 2, videoMode.Height / 2 - HEIGHT / 2);

	// Create simple world
	rc::world world(3, 3, 3);
	world.createFlatWorld(1);
	world.set(1, 1, 1, rc::material::WOOD);
	world.set(2, 2, 0, rc::material::EMPTY);

	// Create renderer
	rc::renderer renderer;
	renderer.setWorld(world);

	// Main loop
	int frames = 0, curTime = time(nullptr), curMouse = 0;
	char titleBuf[128];

	while(glfwGetWindowParam(GLFW_OPENED))
	{
		// Update view
		renderer.setCameraTarget(glm::vec3(cos(glfwGetTime()) * 4.0f + 1.5f, sin(glfwGetTime()) * 4.0f + 1.5f, 3.0f), glm::vec3(1.5f, 1.5f, 0.5f), 70.0f, (float)WIDTH / (float)HEIGHT);

		// Draw frame
		renderer.drawFrame();
		
		// Do picking
		if (glfwGetMouseButton(GLFW_MOUSE_BUTTON_LEFT) != curMouse) {
			if (curMouse == GLFW_PRESS) {
				int x, y; glfwGetMousePos(&x, &y);
				glm::vec3 coord = renderer.pick(x, y);
				world.set(coord.x, coord.y, coord.z, rc::material::EMPTY);
			}

			curMouse = glfwGetMouseButton(GLFW_MOUSE_BUTTON_LEFT);
		}

		// Present
		glfwSwapBuffers();

		// Measure and display fps
		frames++;
		if (curTime != time(nullptr))
		{
			sprintf(titleBuf, "raycraft (%d fps)", frames);
			glfwSetWindowTitle(titleBuf);

			frames = 0;
			curTime = time(nullptr);
		}

		// Seems to work smoother than vsync for now
		glfwSleep(0.016);
	}

	// Clean up
	glfwTerminate();

	return 0;
}