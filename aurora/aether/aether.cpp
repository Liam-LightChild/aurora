/*
 * This file is part of Aurora Game Engine.
 * https://github.com/liam-lightchild/aurora
 */

#include "aether.h"
#include <fstream>

namespace aurora::aether {
	Resource::Resource(const nlohmann::json &pJson) {
		id = pJson["@id"];
	}

	nlohmann::json Resource::serialize() {
		nlohmann::json j;
		j["@id"] = id;
		return j;
	}

	nlohmann::json Resource::readFromFile(const std::filesystem::path &pPath) {
		std::ifstream input(pPath);
		return nlohmann::json::from_cbor(input);
	}
}
