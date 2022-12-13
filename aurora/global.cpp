/*
 * This file is part of Aurora Game Engine.
 * https://github.com/liam-lightchild/aurora
 */

#include "global.h"
#include <GLFW/glfw3.h>
#include <stdexcept>

aurora::Instance *aurora::global;

void aurora::init() {
	if(!glfwInit()) {
		const char *desc;
		glfwGetError(&desc);
		throw std::runtime_error(desc);
	}
}

void aurora::terminate() {
	glfwTerminate();
}
