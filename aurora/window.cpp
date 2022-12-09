//
// Created by liam on 12/2/22.
//

#include "window.h"
#include "global.h"
#include <GLFW/glfw3.h>

namespace aurora {
	Window::Window(int pWidth, int pHeight, const std::string &pTitle, bool pIsFullscreen) : m_Title(pTitle) {
		glfwDefaultWindowHints();
		glfwWindowHint(GLFW_VISIBLE, true);
		global->getImpl()->setupWindowHints();
		m_Window = glfwCreateWindow(pWidth, pHeight, pTitle.c_str(), pIsFullscreen ? glfwGetPrimaryMonitor() : nullptr,
		                            nullptr);

		if(m_Window == nullptr) {
			const char *desc;
			glfwGetError(&desc);
			throw std::runtime_error(desc);
		}

		glfwSetWindowSizeCallback(m_Window, staticWindowSizeChanged);

		glfwMakeContextCurrent(m_Window);
		global->getImpl()->setupWindowPostCreate();
		glfwSwapInterval(0);
	}

	Window::~Window() {
		glfwDestroyWindow(m_Window);
	}

	void Window::staticWindowSizeChanged([[maybe_unused]] GLFWwindow *pWindow, int pWidth, int pHeight) {
		global->getImpl()->updateViewportSize(pWidth, pHeight);
	}

	bool Window::shouldWindowBeOpen() {
		return !glfwWindowShouldClose(m_Window);
	}

	void Window::setWindowShouldBeOpen(bool pValue) {
		glfwSetWindowShouldClose(m_Window, pValue);
	}

	void Window::finishFrame() {
		glfwPollEvents();
		global->getImpl()->performFinishFrame(this);
	}
}// namespace aurora