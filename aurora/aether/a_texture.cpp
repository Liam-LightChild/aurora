/*
 * This file is part of Aurora Game Engine.
 * https://github.com/liam-lightchild/aurora
 */

#include "aether.h"

namespace aurora::aether {
	TextureMinFilter TextureMeta::parseMinFilter(const std::string &pString) const {
		if(pString == "linear") { return useMipmap ? TextureMinFilter::Linear : TextureMinFilter::LinearMipmap; }
		else if(pString == "nearest") {
			return useMipmap
			       ? TextureMinFilter::Nearest
			       : TextureMinFilter::NearestMipmap;
		} else { throw std::runtime_error("invalid min filter " + pString); }
	}

	TextureMagFilter TextureMeta::parseMagFilter(const std::string &pString) {
		if(pString == "linear") { return TextureMagFilter::Linear; }
		else if(pString == "nearest") { return TextureMagFilter::Nearest; }
		else { throw std::runtime_error("invalid mag filter " + pString); }
	}

	TextureWrapType TextureMeta::parseWrapType(const std::string &pString) {
		if(pString == "repeat") { return TextureWrapType::Repeat; }
		else if(pString == "border") { return TextureWrapType::BorderColor; }
		else if(pString == "clamp_to_edge") { return TextureWrapType::ClampToEdge; }
		else { throw std::runtime_error("invalid wrap type " + pString); }
	}

	TextureMeta::TextureMeta(const nlohmann::json &pJson) : Resource(pJson) {
		path = pJson["path"];

		if(pJson.contains("mipmap")) { useMipmap = pJson["mipmap"]; }

		if(pJson.contains("filter")) {
			auto f = pJson["filter"];
			if(f.contains("min")) { minFilter = parseMinFilter(f["min"]); }
			if(f.contains("mag")) { magFilter = parseMagFilter(f["mag"]); }
		}

		if(pJson.contains("wrap")) { wrap = parseWrapType(pJson["wrap"]); }
		if(pJson.contains("border_color")) {
			borderColor = {
				pJson["r"],
				pJson["g"],
				pJson["b"]
			};
		}
	}

	nlohmann::json TextureMeta::serialize() {
		auto j = Resource::serialize();
		j["path"] = path;
		if((minFilter != TextureMinFilter::Linear && minFilter != TextureMinFilter::LinearMipmap) ||
		   magFilter != TextureMagFilter::Linear) {
			auto f = nlohmann::json::object();
			if(minFilter != TextureMinFilter::Linear && minFilter != TextureMinFilter::LinearMipmap) {
				f["min"] = "nearest";
			}
			if(magFilter != TextureMagFilter::Linear) {
				f["mag"] = "nearest";
			}
			j["filter"] = f;
		}

		if(wrap != TextureWrapType::Repeat) {
			switch(wrap) {
				case TextureWrapType::ClampToEdge: j["wrap"] = "clamp_to_edge";
					break;
				case TextureWrapType::BorderColor: j["wrap"] = "border";
					j["border_color"] = {
						{"r", borderColor.r},
						{"g", borderColor.g},
						{"b", borderColor.b}
					};
					break;
				default: break;
			}
		}

		if(useMipmap) { j["mipmap"] = useMipmap; }

		return j;
	}
}