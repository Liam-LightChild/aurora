//
// Created by liam on 12/7/22.
//

#ifndef AURORA_TEXTURE_2D_H
#define AURORA_TEXTURE_2D_H

#include "../graphics/obj_ref_base.h"
#include "../graphics/implementation.h"

namespace aurora {

	class Texture2D {
	private:
		ObjRefBase *m_Reference;

	public:
		explicit Texture2D(ObjRefBase *pReference);
		Texture2D();
		virtual ~Texture2D();

		void setWrap(TextureWrapType pWrap);
		void setWrap(TextureWrapType pWrap, glm::vec3 pColor);
		void setFilters(TextureMinFilter pMin, TextureMagFilter pMag);
		void update(const sail::image &pImage, bool pUpdateMipmaps = false);
		void updateMipmaps();
	};

} // aurora

#endif //AURORA_TEXTURE_2D_H
