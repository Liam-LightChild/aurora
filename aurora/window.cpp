/*
 * This file is part of Aurora Game Engine.
 * https://github.com/liam-lightchild/aurora
 */

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

		glfwSetWindowUserPointer(m_Window, this);
		glfwSetWindowSizeCallback(m_Window, staticWindowSizeChanged);

		glfwMakeContextCurrent(m_Window);
		global->getImpl()->setupWindowPostCreate();
		glfwSwapInterval(0);
	}

	Window::~Window() {
		glfwDestroyWindow(m_Window);
	}

	void Window::staticWindowSizeChanged([[maybe_unused]] GLFWwindow *pWindow, int pWidth, int pHeight) {
		auto self = reinterpret_cast<Window *>(glfwGetWindowUserPointer(pWindow));
		global->getImpl()->updateViewportSize(pWidth, pHeight);
		for(const auto &item: self->m_Framebuffers) {
			item->reinitialize(pWidth, pHeight);
		}
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

	glm::ivec2 Window::getSize() {
		int w, h;
		glfwGetWindowSize(m_Window, &w, &h);
		return {
			w,
			h
		};
	}

	void Window::addFramebuffer(Framebuffer *pFramebuffer) {
		m_Framebuffers.emplace_back(pFramebuffer);
	}

	void Window::removeFramebuffer(Framebuffer *pFramebuffer) {
		m_Framebuffers.erase(std::remove(m_Framebuffers.begin(), m_Framebuffers.end(), pFramebuffer),
		                     m_Framebuffers.end());
	}
}// namespace aurora