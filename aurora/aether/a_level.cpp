/*
 * This file is part of Aurora Game Engine.
 * https://github.com/liam-lightchild/aurora
 */

#include "aether.h"

namespace aurora::aether {
	Level::Level(const nlohmann::json &pJson) : Resource(pJson) {
		if(pJson.contains("root")) {
			for(const auto &item: pJson["root"]) {
				Object obj(item);
				objects.insert({
					               obj.name,
					               obj
				               });
			}
		}
	}

	nlohmann::json Level::serialize() {
		auto j = Resource::serialize();
		auto root = nlohmann::json::array();

		for(const auto &item: objects) {
			root.emplace_back(item.second.serialize());
		}

		j["root"] = root;
		return j;
	}
}
