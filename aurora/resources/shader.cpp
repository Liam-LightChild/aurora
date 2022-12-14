/*
 * This file is part of Aurora Game Engine.
 * https://github.com/liam-lightchild/aurora
 */

#include "shader.h"
#include "../global.h"

namespace aurora {
	Shader::Shader(const aether::Shader &pShader) : m_Aether(pShader) {
		m_Reference = global->getImpl()->createShader(pShader);
		std::vector<VertexArrangement::Node> nodes;
		for(const auto &item: pShader.vertexNodes) {
			VertexInputType type;

			if(item.type == "Float") type = VertexInputType::Float;
			else if(item.type == "Int") type = VertexInputType::Int;
			else if(item.type == "Boolean") type = VertexInputType::Boolean;
			else {
				BOOST_LOG_TRIVIAL(error) << "Invalid shader input type " << item.type << "; skipping";
				continue;
			}

			nodes.emplace_back(item.name, type, item.size);
		}

		m_Arrangement = VertexArrangement(nodes);
	}

	Shader::~Shader() {
		global->getImpl()->destroyShader(m_Reference);
	}

	Shader::Shader(AssetLoader *pLoader, const std::filesystem::path &pPath, const std::string &pAssetId)
		: Shader(*pLoader->load<aether::Shader>(pAssetId)) {}
}// namespace aurora