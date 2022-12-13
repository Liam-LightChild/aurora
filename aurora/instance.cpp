/*
 * This file is part of Aurora Game Engine.
 * https://github.com/liam-lightchild/aurora
 */

#include "instance.h"

namespace aurora {
	Instance::Instance(ImplementationFinder *pFinder, const std::filesystem::path &pAssetPath) {
		m_AssetLoader = new AssetLoader(pAssetPath);
		m_Implementation = pFinder->construct();
		m_Graphics = new Graphics(m_Implementation);
	}

}// namespace aurora
