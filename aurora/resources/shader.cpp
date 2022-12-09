//
// Created by liam on 12/2/22.
//

#include "shader.h"
#include "../global.h"

namespace aurora {
	Shader::Shader(const aether::Shader &pShader) {
		m_Reference = global->getImpl()->createShader(pShader);
	}

	Shader::~Shader() {
		global->getImpl()->destroyShader(m_Reference);
	}

	Shader::Shader(AssetLoader *pLoader, const std::filesystem::path &pPath, const std::string &pAssetId)
		: Shader(*pLoader->load<aether::Shader>(pAssetId)) {}
}// namespace aurora