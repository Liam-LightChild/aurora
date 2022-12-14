/*
 * This file is part of Aurora Game Engine.
 * https://github.com/liam-lightchild/aurora
 */

#ifndef AURORA_SHADER_H
#define AURORA_SHADER_H

#include "../asset_loader.h"
#include "../aether/aether.h"
#include "../graphics/obj_ref_base.h"
#include "../graphics/implementation.h"
#include <filesystem>
#include <string>

namespace aurora {

	class Shader {
	private:
		aether::Shader m_Aether;
		ObjRefBase *m_Reference;
		VertexArrangement m_Arrangement;

	public:
		Shader(AssetLoader *pLoader, const std::filesystem::path &pPath, const std::string &pAssetId);
		explicit Shader(const aether::Shader &pShader);
		virtual ~Shader();

		[[nodiscard]] ObjRefBase *getReference() const {
			return m_Reference;
		}

		[[nodiscard]] const VertexArrangement &getArrangement() const {
			return m_Arrangement;
		}

		[[nodiscard]] const aether::Shader &getAether() const {
			return m_Aether;
		}
	};

}// namespace aurora

#endif// AURORA_SHADER_H
