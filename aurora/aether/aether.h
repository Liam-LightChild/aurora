#ifndef AURORA_AETHER_H
#define AURORA_AETHER_H

#include <filesystem>
#include <fstream>
#include <nlohmann/json.hpp>
#include <string>
#include <utility>
#include <glm/glm.hpp>
#include "aurora/graphics/enums.h"

namespace aurora::aether {
	struct Resource {
		static constexpr const char *schemaUri = "https://www.liamcoalstudio.com/aurora/aether.xsd";

		std::string id;

		Resource() = default;
		virtual ~Resource() = default;

		explicit Resource(const nlohmann::json &pJson) {
			id = pJson["@id"];
		}

		virtual nlohmann::json serialize() {
			nlohmann::json j;
			j["@id"] = id;
			return j;
		}

		static inline nlohmann::json readFromFile(const std::filesystem::path &pPath) {
			std::ifstream input(pPath);
			input.seekg(0, std::ios::end);
			std::vector<uint8_t> bytes(input.tellg());
			input.seekg(0);
			input.read(reinterpret_cast<char *>(bytes.data()), static_cast<std::streamsize>(bytes.size()));
			return nlohmann::json::from_cbor(bytes);
		}
	};

	struct Shader : public Resource {
		static constexpr const char *schemaUri = "https://www.liamcoalstudio.com/aurora/ashdr.xsd";

		enum Stage {
			Vertex,
			Pixel
		};

		struct Part {
			Stage stage;
			std::string source;

			Part(Stage pStage, std::string pSource) : stage(pStage), source(std::move(pSource)) {}
		};

		struct Input {
			std::string name, purpose;

			Input(std::string pName, std::string pPurpose) : name(std::move(pName)), purpose(std::move(pPurpose)) {}
		};

		struct Uniform {
			std::string name, purpose;

			Uniform(std::string pName, std::string pPurpose) : name(std::move(pName)), purpose(std::move(pPurpose)) {}
		};

		struct Output {
			std::string name;
			int color;

			Output(std::string pName, int pColor) : name(std::move(pName)), color(pColor) {}
		};

		std::vector<Part> parts;
		std::vector<Input> inputs;
		std::vector<Output> outputs;
		std::vector<Uniform> uniforms;

		Shader() = default;
		~Shader() override = default;

		explicit Shader(const nlohmann::json &pJson) : Resource(pJson) {
			for(const auto &item: pJson["parts"]) {
				Stage s;
				std::string stageStr = item["stage"];

				if(stageStr == "vertex") { s = Vertex; }
				else if(stageStr == "vixel") { s = Pixel; }
				else { throw std::runtime_error("invalid Shader Part Stage"); }

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
		}

		static inline Shader load(const std::filesystem::path &pPath) { return Shader(readFromFile(pPath)); }

		explicit Shader(class AssetLoader*, const std::filesystem::path &pPath, const std::string&)
			: Shader(readFromFile(pPath)) {}

		nlohmann::json serialize() override {
			nlohmann::json j = Resource::serialize();

			auto p = nlohmann::json::array();
			auto i = nlohmann::json::array();
			auto o = nlohmann::json::array();
			auto u = nlohmann::json::array();

			for(const auto &item: parts) {
				std::string stage;

				switch(item.stage) {
					case Vertex: stage = "vertex";
						break;
					case Pixel: stage = "pixel";
						break;
				}

				p.emplace_back(nlohmann::json::object({
					{"stage",  stage},
					{"src", item.source}
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

			j["parts"] = p;
			j["inputs"] = i;
			j["outputs"] = o;
			j["uniforms"] = u;

			return j;
		}
	};

	struct TextureMeta : public Resource {
		std::string path;
		bool useMipmap = false;
		TextureMinFilter minFilter = TextureMinFilter::Linear;
		TextureMagFilter magFilter = TextureMagFilter::Linear;
		TextureWrapType wrap = TextureWrapType::Repeat;
		glm::vec3 borderColor = {0, 0, 0};

	private:
		[[nodiscard]] inline TextureMinFilter parseMinFilter(const std::string &pString) const {
			if(pString == "linear") return useMipmap ? TextureMinFilter::Linear : TextureMinFilter::LinearMipmap;
			else if(pString == "nearest") return useMipmap ? TextureMinFilter::Nearest : TextureMinFilter::NearestMipmap;
			else throw std::runtime_error("invalid min filter " + pString);
		}

		[[nodiscard]] static inline TextureMagFilter parseMagFilter(const std::string &pString) {
			if(pString == "linear") return TextureMagFilter::Linear;
			else if(pString == "nearest") return TextureMagFilter::Nearest;
			else throw std::runtime_error("invalid mag filter " + pString);
		}

		[[nodiscard]] static inline TextureWrapType parseWrapType(const std::string &pString) {
			if(pString == "repeat") return TextureWrapType::Repeat;
			else if(pString == "border") return TextureWrapType::BorderColor;
			else if(pString == "clamp_to_edge") return TextureWrapType::ClampToEdge;
			else throw std::runtime_error("invalid wrap type " + pString);
		}

	public:

		TextureMeta() = default;

		explicit TextureMeta(const nlohmann::json &pJson) : Resource(pJson) {
			path = pJson["path"];

			if(pJson.contains("mipmap")) useMipmap = pJson["mipmap"];

			if(pJson.contains("filter")) {
				auto f = pJson["filter"];
				if(f.contains("min")) minFilter = parseMinFilter(f["min"]);
				if(f.contains("mag")) magFilter = parseMagFilter(f["mag"]);
			}

			if(pJson.contains("wrap")) wrap = parseWrapType(pJson["wrap"]);
			if(pJson.contains("border_color")) borderColor = {pJson["r"], pJson["g"], pJson["b"]};
		}

		~TextureMeta() override = default;

		nlohmann::json serialize() override {
			auto j = Resource::serialize();
			j["path"] = path;
			if ((minFilter != TextureMinFilter::Linear && minFilter != TextureMinFilter::LinearMipmap) || magFilter != TextureMagFilter::Linear) {
				auto f = nlohmann::json::object();
				if(minFilter != TextureMinFilter::Linear && minFilter != TextureMinFilter::LinearMipmap)
					f["min"] = "nearest";
				if(magFilter != TextureMagFilter::Linear)
					f["mag"] = "nearest";
				j["filter"] = f;
			}

			if(wrap != TextureWrapType::Repeat) {
				switch(wrap) {
					case TextureWrapType::ClampToEdge:
						j["wrap"] = "clamp_to_edge";
						break;
					case TextureWrapType::BorderColor:
						j["wrap"] = "border";
						j["border_color"] = {
							{"r", borderColor.r},
							{"g", borderColor.g},
							{"b", borderColor.b}
						};
						break;
					default: break;
				}
			}

			if(useMipmap) j["mipmap"] = useMipmap;

			return j;
		}
	};

	struct MeshTri {
		int vertices[3], texVertices[3], normalVertices[3];
	};

	struct Mesh : public Resource {
		std::vector<glm::vec3> positions;
		std::vector<glm::vec2> texCoords;
		std::vector<glm::vec3> normals;
		std::vector<MeshTri> tris;

		Mesh() = default;

		explicit Mesh(const nlohmann::json &pJson) : Resource(pJson) {
			for(const auto &item: pJson[".p"]) {
				positions.emplace_back(item[0], item[1], item[2]);
			}

			for(const auto &item: pJson[".t"]) {
				texCoords.emplace_back(item[0], item[1]);
			}

			for(const auto &item: pJson[".n"]) {
				normals.emplace_back(item[0], item[1], item[2]);
			}

			for(const auto &item: pJson[".tris"]) {
				MeshTri tri{};
				tri.vertices[0] = item[0];
				tri.texVertices[0] = item[1];
				tri.normalVertices[0] = item[2];
				tri.vertices[1] = item[3];
				tri.texVertices[1] = item[4];
				tri.normalVertices[1] = item[5];
				tri.vertices[2] = item[6];
				tri.texVertices[2] = item[7];
				tri.normalVertices[2] = item[8];
				tris.emplace_back(tri);
			}
		}

		~Mesh() override = default;

		nlohmann::json serialize() override {
			auto j = Resource::serialize();
			auto p = nlohmann::json::array();
			auto t = nlohmann::json::array();
			auto n = nlohmann::json::array();
			auto ta = nlohmann::json::array();

			for(const auto &item: positions) p.emplace_back(nlohmann::json::array({item.x, item.y, item.z}));
			for(const auto &item: texCoords) t.emplace_back(nlohmann::json::array({item.x, item.y}));
			for(const auto &item: normals) n.emplace_back(nlohmann::json::array({item.x, item.y, item.z}));

			for(const auto &item: tris) {
				ta.emplace_back(nlohmann::json::array({
					item.vertices[0], item.texVertices[0], item.normalVertices[0],
					item.vertices[1], item.texVertices[1], item.normalVertices[1],
					item.vertices[2], item.texVertices[2], item.normalVertices[2],
				}));
			}

			return j;
		}
	};
}


#endif// AURORA_AETHER_H
