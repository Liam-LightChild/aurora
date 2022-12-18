/*
 * This file is part of Aurora Game Engine.
 * https://github.com/liam-lightchild/aurora
 */

#ifndef AURORA_INSTANCE_H
#define AURORA_INSTANCE_H

#include "asset_loader.h"
#include "graphics/graphics.h"
#include "graphics/implementation.h"
#include "graphics/implementation_finder.h"

namespace aurora {

	class Instance {
	private:
		Implementation *m_Implementation;
		AssetLoader *m_AssetLoader;
		Graphics *m_Graphics;
		Window *m_Window;

	public:
		Instance(ImplementationFinder *pFinder, const std::filesystem::path &pAssetPath);

		[[nodiscard]] inline Implementation *getImpl() const { return m_Implementation; }

		[[nodiscard]] inline AssetLoader *getAssetLoader() const { return m_AssetLoader; }

		[[nodiscard]] inline Graphics *getGraphics() const { return m_Graphics; }

		[[nodiscard]] inline Window *getWindow() const { return m_Window; }

		void setWindow(Window *pWindow) {
			m_Window = pWindow;
		}
	};

}// namespace aurora

#endif// AURORA_INSTANCE_H
