//
// Created by liam on 12/2/22.
//

#include "asset_loader.h"
#include <fstream>
#include <nlohmann/json.hpp>

namespace aurora {
	AssetLoader::AssetLoader(const std::filesystem::path &pPath) {
		if(!std::filesystem::exists(pPath / "assets.idx.aet"))
			throw std::runtime_error("no assets index present in " + std::filesystem::absolute(pPath).string());
		std::ifstream indexIn(pPath / "assets.idx.aet");
		auto indexJson = nlohmann::json::from_cbor(indexIn);

		for(const auto &item: indexJson.items()) {
			m_Index[item.key()] = (std::string) item.value();
		}
	}
}// namespace aurora
