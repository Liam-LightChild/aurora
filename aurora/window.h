//
// Created by liam on 12/2/22.
//

#ifndef AURORA_WINDOW_H
#define AURORA_WINDOW_H

#include <string>
#include <GLFW/glfw3.h>

namespace aurora {

	class Window {
		GLFWwindow *m_Window;
		std::string m_Title;

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

	private:
		static void staticWindowSizeChanged([[maybe_unused]] GLFWwindow *pWindow, int pWidth, int pHeight);
	};

}// namespace aurora

#endif// AURORA_WINDOW_H
