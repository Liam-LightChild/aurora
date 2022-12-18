/*
 * This file is part of Aurora Game Engine.
 * https://github.com/liam-lightchild/aurora
 */

#ifndef AURORA_WINDOW_H
#define AURORA_WINDOW_H

#include <string>
#include <GLFW/glfw3.h>
#include <glm/vec2.hpp>
#include <vector>
#include "resources/framebuffer.h"

namespace aurora {

	class Window {
		GLFWwindow *m_Window;
		std::string m_Title;
		std::vector<Framebuffer *> m_Framebuffers;

	public:
		Window(int pWidth, int pHeight, const std::string &pTitle, bool pIsFullscreen);
		virtual ~Window();

		[[nodiscard]] GLFWwindow *getGlfw() const {
			return m_Window;
		}

		[[nodiscard]] const std::string &getTitle() const {
			return m_Title;
		}

		bool shouldWindowBeOpen();
		void setWindowShouldBeOpen(bool pValue);
		void finishFrame();
		glm::ivec2 getSize();
		void addFramebuffer(Framebuffer *pFramebuffer);
		void removeFramebuffer(Framebuffer *pFramebuffer);

	private:
		static void staticWindowSizeChanged([[maybe_unused]] GLFWwindow *pWindow, int pWidth, int pHeight);
	};

}// namespace aurora

#endif// AURORA_WINDOW_H
