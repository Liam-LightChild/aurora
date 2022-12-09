//
// Created by liam on 12/2/22.
//

#ifndef AURORA_SHADER_H
#define AURORA_SHADER_H

#include "aurora/aether/aether.h"
#include "aurora/graphics/obj_ref_base.h"
#include <filesystem>
#include <string>

namespace aurora {

	class Shader {
	private:
		ObjRefBase *m_Reference;
	public:
		explicit Shader(const aether::Shader &pShader);
		virtual ~Shader();

		void use();

		ObjRefBase *getReference() const {
			return m_Reference;
		}
	};

}// namespace aurora

#endif// AURORA_SHADER_H
