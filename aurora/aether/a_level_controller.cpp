/*
 * This file is part of Aurora Game Engine.
 * https://github.com/liam-lightchild/aurora
 */

#include "aether.h"

namespace aurora::aether {
	Level::Controller::Controller(const nlohmann::json &pJson) {
		type = pJson["type"];

		for(const auto &[key, value]: pJson["properties"].items()) {
			properties.insert({
				                  key,
				                  value
			                  });
		}
	}

	nlohmann::json Level::Controller::serialize() const {
		nlohmann::json j;
		j["type"] = type;
		nlohmann::json prop;

		for(const auto &[key, value]: properties) {
			prop[key] = value;
		}

		j["properties"] = prop;
		return j;
	}
}
