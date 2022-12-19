/*
 * This file is part of Aurora Game Engine.
 * https://github.com/liam-lightchild/aurora
 */

#ifndef AURORA_ICON_H
#define AURORA_ICON_H

#include <GLFW/glfw3.h>
#include "../asset_loader.h"
#include <sail-c++/sail-c++.h>

namespace aurora {

	class Icon {
		friend class Window;

	private:
		sail::image *m_SailImage;
		GLFWimage *m_Image;

	public:
		Icon(sail::image *pSailImage, GLFWimage *pImage) : m_SailImage(pSailImage), m_Image(pImage) {}
		Icon(AssetLoader*, const std::filesystem::path &pPath, const std::string&);
		virtual ~Icon();
	};

} // aurora

#endif //AURORA_ICON_H
