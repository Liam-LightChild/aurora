//
// Created by liam on 12/2/22.
//
#include "opengl_impl_node.h"
#include <GLFW/glfw3.h>
#include <stdexcept>

#pragma clang diagnostic push
#pragma ide diagnostic ignored "OCInconsistentNamingInspection"

bool aurora::checkOpenGLContext(int pMajor, int pMinor) {
	glfwDefaultWindowHints();
	glfwWindowHint(GLFW_VISIBLE, false);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, true);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, pMajor);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, pMinor);
	glfwCreateWindow(4, 4, "OpenGL Tester", nullptr, nullptr);

	const char *desc;
	auto err = glfwGetError(&desc);
	if(err == GLFW_API_UNAVAILABLE || err == GLFW_VERSION_UNAVAILABLE) {
		return false;
	} else if(err == GLFW_NO_ERROR) { return true; }
	else { throw std::runtime_error(desc); }
}

#pragma clang diagnostic pop
