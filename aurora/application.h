/*
 * This file is part of Aurora Game Engine.
 * https://github.com/liam-lightchild/aurora
 */

#ifndef AURORA_APPLICATION_H
#define AURORA_APPLICATION_H

#include "instance.h"

namespace aurora {

	class Application {
	private:
		float m_DesiredFramerate = 60.0f;
		Instance *m_Instance;
		Window *m_Window;

	protected:
		[[nodiscard]] float getDesiredFramerate() const { return m_DesiredFramerate; }

		void setDesiredFramerate(float pFrameRate) { m_DesiredFramerate = pFrameRate; }

	public:
		Application();
		virtual ~Application();

		virtual void render();
		virtual void update();

		void run();

		[[nodiscard]] Instance *getInstance() const;
	};

}// namespace aurora

#endif// AURORA_APPLICATION_H
