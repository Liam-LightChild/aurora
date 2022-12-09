//
// Created by liam on 12/2/22.
//

#include "shader.h"
#include "aurora/global.h"

namespace aurora {
	Shader::Shader(const aether::Shader &pShader) {
		m_Reference = global->getImpl()->createShader(pShader);
	}

	Shader::~Shader() {
		global->getImpl()->destroyShader(m_Reference);
	}

	void Shader::use() {

	}
}// namespace aurora