/*
 * This file is part of Aurora Game Engine.
 * https://github.com/liam-lightchild/aurora
 */

#include "aether.h"

namespace aurora::aether {
	Level::Object::Object(const nlohmann::json &pJson) {
		name = pJson["name"];
		auto pos = pJson["pos"], rot = pJson["rot"];
		position = {
			pos[0],
			pos[1],
			pos[2]
		};
		rotation = {
			rot[0],
			rot[1],
			rot[2],
			rot[3]
		};

		if(pJson.contains("children")) {
			for(const auto &item: pJson["children"]) {
				Object obj(item);
				objects.insert({
					               obj.name,
					               obj
				               });
			}
		}

		if(pJson.contains("controllers")) {
			for(const auto &item: pJson["controllers"]) {
				controllers.emplace_back(item);
			}
		}
	}

	nlohmann::json Level::Object::serialize() const {
		nlohmann::json j;
		j["name"] = name;
		j["pos"] = {
			position.x,
			position.y,
			position.z
		};
		j["rot"] = {
			rotation.w,
			rotation.x,
			rotation.y,
			rotation.z
		};

		auto children = nlohmann::json::array();
		auto controllerList = nlohmann::json::array();

		for(const auto &item: objects) {
			children.emplace_back(item.second.serialize());
		}

		for(const auto &item: controllers) {
			controllerList.emplace_back(item.serialize());
		}

		j["children"] = children;
		j["controllers"] = controllerList;
		return j;
	}
}
