/*
 * This file is part of Aurora Game Engine.
 * https://github.com/liam-lightchild/aurora
 */

#ifndef AURORA_TEXTURE_3D_H
#define AURORA_TEXTURE_3D_H

#include <glm/vec3.hpp>
#include "../graphics/obj_ref_base.h"
#include "../graphics/enums.h"

namespace aurora {

	class Texture3D {
	private:
		ObjRefBase *m_Reference;

	public:
		explicit Texture3D(ObjRefBase *pReference);
		Texture3D();
		virtual ~Texture3D();

		void setWrap(TextureWrapType pWrap);
		void setWrap(TextureWrapType pWrap, glm::vec3 pColor);
		void setFilters(TextureMinFilter pMin, TextureMagFilter pMag);
		void update(int pWidth, int pHeight, int pDepth, const uint8_t *pDataRgba, bool pUpdateMipmaps = false);
		void updateMipmaps();

		[[nodiscard]] ObjRefBase *getReference() const {
			return m_Reference;
		}
	};

} // aurora

#endif //AURORA_TEXTURE_3D_H
