//
// Created by liam on 12/2/22.
//

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
