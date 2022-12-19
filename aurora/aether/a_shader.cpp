/*
 * This file is part of Aurora Game Engine.
 * https://github.com/liam-lightchild/aurora
 */

#include "aether.h"

namespace aurora::aether {
	Shader::Shader(const nlohmann::json &pJson) : Resource(pJson) {
		for(const auto &item: pJson["parts"]) {
			Stage s;
			std::string stageStr = item["stage"];

			if(stageStr == "vertex") { s = Vertex; }
			else if(stageStr == "pixel") { s = Pixel; }
			else { throw std::runtime_error("invalid Shader Part stage"); }

			parts.emplace_back(s, item["src"]);
		}

		for(const auto &item: pJson["inputs"]) {
			inputs.emplace_back(item["name"], item["purpose"]);
		}
		for(const auto &item: pJson["uniforms"]) {
			uniforms.emplace_back(item["name"], item["purpose"]);
		}
		for(const auto &item: pJson["outputs"]) {
			outputs.emplace_back(item["name"], item["color"]);
		}
		for(const auto &item: pJson["vertexNodes"]) {
			vertexNodes.emplace_back(item["name"], item["type"], item["from"], item["size"]);
		}
	}

	nlohmann::json Shader::serialize() {
		nlohmann::json j = Resource::serialize();

		auto p = nlohmann::json::array();
		auto i = nlohmann::json::array();
		auto o = nlohmann::json::array();
		auto u = nlohmann::json::array();
		auto v = nlohmann::json::array();

		for(const auto &item: parts) {
			std::string stage;

			switch(item.stage) {
				case Vertex: stage = "vertex";
					break;
				case Pixel: stage = "pixel";
					break;
			}

			p.emplace_back(nlohmann::json::object({
				                                      {"stage", stage},
				                                      {"src",   item.source}
			                                      }));
		}

		for(const auto &item: inputs) {
			i.emplace_back(nlohmann::json::object({
				                                      {"name",    item.name},
				                                      {"purpose", item.purpose}
			                                      }));
		}
		for(const auto &item: outputs) {
			o.emplace_back(nlohmann::json::object({
				                                      {"name",  item.name},
				                                      {"color", item.color}
			                                      }));
		}
		for(const auto &item: uniforms) {
			u.emplace_back(nlohmann::json::object({
				                                      {"name",    item.name},
				                                      {"purpose", item.purpose}
			                                      }));
		}
		for(const auto &item: vertexNodes) {
			v.emplace_back(nlohmann::json::object({
				                                      {"name", item.name},
				                                      {"type", item.type},
				                                      {"from", item.from},
				                                      {"size", item.size}
			                                      }));
		}

		j["parts"] = p;
		j["inputs"] = i;
		j["outputs"] = o;
		j["uniforms"] = u;
		j["vertexNodes"] = v;

		return j;
	}
}
