//
// Created by liam on 12/2/22.
//

#ifndef AURORA_SHADER_H
#define AURORA_SHADER_H

#include "../asset_loader.h"
#include "../aether/aether.h"
#include "../graphics/obj_ref_base.h"
#include <filesystem>
#include <string>

namespace aurora {

	class Shader {
	private:
		ObjRefBase *m_Reference;
	public:
		Shader(AssetLoader *pLoader, const std::filesystem::path &pPath, const std::string &pAssetId);
		explicit Shader(const aether::Shader &pShader);
		virtual ~Shader();

		[[nodiscard]] ObjRefBase *getReference() const {
			return m_Reference;
		}
	};

}// namespace aurora

#endif// AURORA_SHADER_H
