/*
 * This file is part of Aurora Game Engine.
 * https://github.com/liam-lightchild/aurora
 */


#include "icon.h"
#include <aurora/aether/aether.h>
#include <fstream>
#include <sail-c++/sail-c++.h>

namespace aurora {
	Icon::Icon(AssetLoader*, const std::filesystem::path &pPath, const std::string&) {
		auto j = nlohmann::json::from_cbor(std::ifstream(pPath));
		aether::TextureMeta tex(j);
		m_SailImage = new sail::image((absolute(pPath).parent_path() / tex.path).string());
		m_SailImage->convert(SAIL_PIXEL_FORMAT_BPP32_RGBA);
		m_Image = new GLFWimage();
		m_Image->width = (int)m_SailImage->width();
		m_Image->height = (int)m_SailImage->height();
		m_Image->pixels = static_cast<unsigned char*>(m_SailImage->pixels());
	}

	Icon::~Icon() {
		delete m_Image;
		delete m_SailImage;
	}
} // aurora