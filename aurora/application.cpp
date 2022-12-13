/*
 * This file is part of Aurora Game Engine.
 * https://github.com/liam-lightchild/aurora
 */

#include "application.h"
#include "glimpl/opengl_impl_node.h"
#include "global.h"
#include <thread>

namespace aurora {
	Application::Application() {
		init();
		auto implFinder = new aurora::ImplementationFinder();
		implFinder->registerImpl(new aurora::OpenGLImplementationNode<3, 2>);

		m_Instance = new aurora::Instance(implFinder, ".");
		aurora::global = m_Instance;

		m_Window = new aurora::Window(800, 600, "Title", false);
		aurora::injectBuiltinAssets(m_Instance->getAssetLoader());
	}

	Application::~Application() {
		delete m_Window;
		delete m_Instance;
		aurora::terminate();
	}

	void Application::render() {
		m_Instance->getGraphics()->clear();
	}

	void Application::update() {

	}

	Instance *Application::getInstance() const {
		return m_Instance;
	}

	void Application::run() {
		while(m_Window->shouldWindowBeOpen()) {
			auto start = std::chrono::steady_clock::now();
			render();
			update();
			m_Window->finishFrame();
			auto end = std::chrono::steady_clock::now();
			auto
				frameDuration = std::chrono::nanoseconds(static_cast<long>(1000000000.0 / (double) m_DesiredFramerate));
			auto waitTime = frameDuration - (end - start);
			if(waitTime.count() > 0) { std::this_thread::sleep_for(waitTime); }
		}
	}
}// namespace aurora