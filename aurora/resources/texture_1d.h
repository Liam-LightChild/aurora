/*
 * This file is part of Aurora Game Engine.
 * https://github.com/liam-lightchild/aurora
 */

#ifndef AURORA_TEXTURE_2D_H
#define AURORA_TEXTURE_2D_H

#include "../graphics/obj_ref_base.h"
#include "../graphics/implementation.h"
#include <glm/vec3.hpp>

namespace aurora {

	class Texture1D {
	private:
		ObjRefBase *m_Reference;

	public:
		explicit Texture1D(ObjRefBase *pReference);
		Texture1D();
		virtual ~Texture1D();

		void setWrap(TextureWrapType pWrap);
		void setWrap(TextureWrapType pWrap, glm::vec3 pColor);
		void setFilters(TextureMinFilter pMin, TextureMagFilter pMag);
		void update(int pWidth, const uint8_t *pDataRgba, bool pUpdateMipmaps = false);
		void updateMipmaps();

		[[nodiscard]] ObjRefBase *getReference() const {
			return m_Reference;
		}
	};

} // aurora

#endif //AURORA_TEXTURE_2D_H
