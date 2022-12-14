/*
 * This file is part of Aurora Game Engine.
 * https://github.com/liam-lightchild/aurora
 */

#ifndef AURORA_AETHER_H
#define AURORA_AETHER_H

#include <filesystem>
#include <fstream>
#include <nlohmann/json.hpp>
#include <string>
#include <utility>
#include <glm/glm.hpp>
#include <glm/gtx/quaternion.hpp>
#include "aurora/graphics/enums.h"

namespace aurora {
	class AssetLoader;
}

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

		struct VertexNode {
			std::string name, type, from;
			int size;

			VertexNode(std::string pName, std::string pType, std::string pFrom, int pSize)
				: name(std::move(pName)), type(std::move(pType)),
				  size(pSize), from(std::move(pFrom)) {}
		};

		std::vector<Part> parts;
		std::vector<Input> inputs;
		std::vector<Output> outputs;
		std::vector<Uniform> uniforms;
		std::vector<VertexNode> vertexNodes;

		Shader() = default;
		~Shader() override = default;

		explicit Shader(const nlohmann::json &pJson) : Resource(pJson) {
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

		static inline Shader load(const std::filesystem::path &pPath) { return Shader(readFromFile(pPath)); }

		explicit Shader(class AssetLoader*, const std::filesystem::path &pPath, const std::string&)
			: Shader(readFromFile(pPath)) {}

		nlohmann::json serialize() override {
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
			for(const auto &item: vertexNodes) {
				v.emplace_back(nlohmann::json::object({
					{"name",    item.name},
					{"type",    item.type},
					{"from",    item.from},
					{"size",    item.size}
				}));
			}

			j["parts"] = p;
			j["inputs"] = i;
			j["outputs"] = o;
			j["uniforms"] = u;
			j["vertexNodes"] = v;

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

		Mesh(AssetLoader*, const std::filesystem::path &pPath, const std::string&)
			: Mesh(nlohmann::json::from_cbor(std::ifstream(pPath))) {}

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

			j[".p"] = p;
			j[".t"] = t;
			j[".n"] = n;
			j[".tris"] = ta;

			return j;
		}
	};

	struct OptimisedMesh {
		std::vector<float> vertexData;
		std::vector<uint32_t> indexData;

		OptimisedMesh(const std::vector<float> &pVertexData, const std::vector<uint32_t> &pIndexData) : vertexData(
			pVertexData), indexData(pIndexData) {}

		OptimisedMesh(const Mesh &pMesh, const Shader &pShader, const glm::vec4 &pColor = {1, 1, 1, 1}) {
			struct Vertex {
				glm::vec3 position;
				glm::vec2 texCoord;
				glm::vec3 normal;

				Vertex(const glm::vec3 &pPosition, const glm::vec2 &pTexCoord, const glm::vec3 &pNormal)
					: position(pPosition), texCoord(pTexCoord), normal(pNormal) {}

				bool operator==(const Vertex &pRhs) const {
					return position == pRhs.position &&
					       texCoord == pRhs.texCoord &&
					       normal == pRhs.normal;
				}

				bool operator!=(const Vertex &pRhs) const {
					return !(pRhs == *this);
				}
			};

			std::vector<Vertex> vertices;
			for(const auto &item: pMesh.tris) {
				for(int i = 0; i < 3; ++i) {
					auto vtx = Vertex(pMesh.positions[item.vertices[i]],
									  pMesh.texCoords[item.texVertices[i]],
									  pMesh.normals[item.normalVertices[i]]);
					auto iter = vertices.begin(), end = vertices.end();

					bool done = false;

					for(int j = 0; iter != end; iter++, j++) {
						if(*iter == vtx) {
							indexData.emplace_back(j);
							done = true;
							break;
						}
					}

					if(!done) {
						vertices.emplace_back(vtx);
						indexData.emplace_back(vertices.size() - 1);
					}
				}
			}

			for(const auto &item: vertices) {
				for(const auto &node: pShader.vertexNodes) {
					if(node.type != "Float") {
						throw std::runtime_error("Non-float input values are not supported by this class");
					}

					if(node.from == "position2") {
						vertexData.emplace_back(item.position.x);
						vertexData.emplace_back(item.position.y);
					} else if(node.from == "position3") {
						vertexData.emplace_back(item.position.x);
						vertexData.emplace_back(item.position.y);
						vertexData.emplace_back(item.position.z);
					} else if(node.from == "color3_rgb") {
						vertexData.emplace_back(pColor.r);
						vertexData.emplace_back(pColor.g);
						vertexData.emplace_back(pColor.b);
					} else if(node.from == "color4_rgba") {
						vertexData.emplace_back(pColor.r);
						vertexData.emplace_back(pColor.g);
						vertexData.emplace_back(pColor.b);
						vertexData.emplace_back(pColor.a);
					} else if(node.from == "tex1") {
						vertexData.emplace_back(item.texCoord.s);
					} else if(node.from == "tex2") {
						vertexData.emplace_back(item.texCoord.s);
						vertexData.emplace_back(item.texCoord.t);
					} else if(node.from == "tex3") {
						vertexData.emplace_back(item.texCoord.s);
						vertexData.emplace_back(item.texCoord.t);
						vertexData.emplace_back(item.texCoord.r);
					} else throw std::runtime_error("Unsupported mesh input value " + node.from);
				}
			}
		}
	};

	struct Level : public Resource {
		static constexpr const char *schemaUri = "https://www.liamcoalstudio.com/aurora/alvl.xsd";

		struct Controller {
			std::string type;
			std::unordered_map<std::string, std::string> properties;

			Controller() = default;

			explicit Controller(const nlohmann::json &pJson) {
				type = pJson["type"];

				for(const auto &[key, value]: pJson["properties"].items()) {
					properties.insert({key, value});
				}
			}

			nlohmann::json serialize() const {
				nlohmann::json j;
				j["type"] = type;
				nlohmann::json prop;

				for(const auto &[key, value]: properties) {
					prop[key] = value;
				}

				j["properties"] = prop;
				return j;
			}
		};

		struct Object {
			std::string name;
			glm::dvec3 position{};
			glm::dquat rotation{};
			std::unordered_map<std::string, Object> objects;
			std::vector<Controller> controllers;

			Object() = default;

			explicit Object(const nlohmann::json &pJson) {
				name = pJson["name"];
				auto pos = pJson["pos"], rot = pJson["rot"];
				position = {pos[0], pos[1], pos[2]};
				rotation = {rot[0], rot[1], rot[2], rot[3]};

				if(pJson.contains("children")) {
					for(const auto &item: pJson["children"]) {
						Object obj(item);
						objects.insert({obj.name, obj});
					}
				}

				if(pJson.contains("controllers")) {
					for(const auto &item: pJson["controllers"]) {
						controllers.emplace_back(item);
					}
				}
			}

			nlohmann::json serialize() const {
				nlohmann::json j;
				j["name"] = name;
				j["pos"] = {position.x, position.y, position.z};
				j["rot"] = {rotation.w, rotation.x, rotation.y, rotation.z};

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
		};

		std::unordered_map<std::string, Object> objects;

		Level() = default;

		explicit Level(const nlohmann::json &pJson) : Resource(pJson) {
			if(pJson.contains("root")) {
				for(const auto &item: pJson["root"]) {
					Object obj(item);
					objects.insert({obj.name, obj});
				}
			}
		}

		nlohmann::json serialize() override {
			auto j = Resource::serialize();
			auto root = nlohmann::json::array();

			for(const auto &item: objects) {
				root.emplace_back(item.second.serialize());
			}

			j["root"] = root;
			return j;
		}
	};
}


#endif// AURORA_AETHER_H
