/*
 * This file is part of Aurora Game Engine.
 * https://github.com/liam-lightchild/aurora
 */

#include "asset_loader.h"
#include <fstream>
#include <nlohmann/json.hpp>
#include <aurora/resources.h>
#include <aurora/shaders/shaders.h>

namespace aurora {
	AssetLoader::AssetLoader(const std::filesystem::path &pPath) {
		if(!std::filesystem::exists(pPath / "assets.idx.aet")) {
			throw std::runtime_error("no assets index present in " + std::filesystem::absolute(pPath).string());
		}
		std::ifstream indexIn(pPath / "assets.idx.aet");
		auto indexJson = nlohmann::json::from_cbor(indexIn);

		for(const auto &item: indexJson.items()) {
			m_Index[item.key()] = (std::string) item.value();
		}
	}

	void injectBuiltinAssets(AssetLoader *pLoader) {
		pLoader->inject("aurora:test.shader", new Shader(shaders::test));

		auto tex = new Texture2D;
		uint8_t missingTexture2D[4 * 4]{
			255,
			0,
			255,
			255,
			0,
			0,
			0,
			255,
			0,
			0,
			0,
			255,
			255,
			0,
			255,
			255,
		};
		tex->setFilters(TextureMinFilter::NearestMipmap, TextureMagFilter::Nearest);
		tex->setWrap(TextureWrapType::Repeat);
		tex->update(2, 2, missingTexture2D, true);
		pLoader->inject(Texture2D::missingAssetName, tex);
	}
}// namespace aurora
