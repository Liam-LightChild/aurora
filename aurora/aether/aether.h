/*
 * This file is part of Aurora Game Engine.
 * https://github.com/liam-lightchild/aurora
 */

#ifndef AURORA_AETHER_H
#define AURORA_AETHER_H

#include <filesystem>
#include <nlohmann/json.hpp>
#include <string>
#include <glm/glm.hpp>
#include <glm/gtx/quaternion.hpp>
#include "../graphics/enums.h"

namespace aurora {
	class AssetLoader;
}

namespace aurora::aether {
	struct Resource {
		static constexpr const char *schemaUri = "https://www.liamcoalstudio.com/aurora/aether.xsd";

		std::string id;

		Resource() = default;
		virtual ~Resource() = default;

		explicit Resource(const nlohmann::json &pJson);

		virtual nlohmann::json serialize();

		static nlohmann::json readFromFile(const std::filesystem::path &pPath);
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

		explicit Shader(const nlohmann::json &pJson);

		static inline Shader load(const std::filesystem::path &pPath) { return Shader(readFromFile(pPath)); }

		explicit Shader(class AssetLoader *, const std::filesystem::path &pPath, const std::string &)
			: Shader(readFromFile(pPath)) {}

		nlohmann::json serialize() override;
	};

	struct TextureMeta : public Resource {
		std::string path;
		bool useMipmap = false;
		TextureMinFilter minFilter = TextureMinFilter::Linear;
		TextureMagFilter magFilter = TextureMagFilter::Linear;
		TextureWrapType wrap = TextureWrapType::Repeat;
		glm::vec3 borderColor = {
			0,
			0,
			0
		};

	private:
		[[nodiscard]] TextureMinFilter parseMinFilter(const std::string &pString) const;
		[[nodiscard]] static TextureMagFilter parseMagFilter(const std::string &pString);
		[[nodiscard]] static TextureWrapType parseWrapType(const std::string &pString);

	public:
		TextureMeta() = default;
		explicit TextureMeta(const nlohmann::json &pJson);
		~TextureMeta() override = default;

		nlohmann::json serialize() override;
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

		explicit Mesh(const nlohmann::json &pJson);

		Mesh(AssetLoader *, const std::filesystem::path &pPath, const std::string &);

		~Mesh() override = default;

		nlohmann::json serialize() override;
	};

	struct OptimisedMesh {
		std::vector<float> vertexData;
		std::vector<uint32_t> indexData;

		OptimisedMesh(const std::vector<float> &pVertexData, const std::vector<uint32_t> &pIndexData) : vertexData(
			pVertexData), indexData(pIndexData) {}

		OptimisedMesh(const Mesh &pMesh, const Shader &pShader, const glm::vec4 &pColor = {
			1,
			1,
			1,
			1
		});
	};

	struct Level : public Resource {
		static constexpr const char *schemaUri = "https://www.liamcoalstudio.com/aurora/alvl.xsd";

		struct Controller {
			std::string type;
			std::unordered_map<std::string, std::string> properties;

			Controller() = default;
			explicit Controller(const nlohmann::json &pJson);

			[[nodiscard]] nlohmann::json serialize() const;
		};

		struct Object {
			std::string name;
			glm::dvec3 position{};
			glm::dquat rotation{};
			std::unordered_map<std::string, Object> objects;
			std::vector<Controller> controllers;

			Object() = default;
			explicit Object(const nlohmann::json &pJson);

			[[nodiscard]] nlohmann::json serialize() const;
		};

		std::unordered_map<std::string, Object> objects;

		Level() = default;
		explicit Level(const nlohmann::json &pJson);

		nlohmann::json serialize() override;
	};
}


#endif// AURORA_AETHER_H
