//
// Created by liam on 12/7/22.
//

#include "texture_2d.h"
#include "../global.h"

namespace aurora {
	Texture2D::Texture2D(ObjRefBase *pReference) : m_Reference(pReference) {}

	Texture2D::Texture2D() {
		m_Reference = global->getImpl()->createTexture2D();
	}

	Texture2D::~Texture2D() {
		global->getImpl()->destroyTexture2D(m_Reference);
	}

	void Texture2D::setWrap(TextureWrapType pWrap) {
		global->getImpl()->setTexture2DWrapProperty(m_Reference, pWrap);
	}

	void Texture2D::setWrap(TextureWrapType pWrap, glm::vec3 pColor) {
		if(pWrap != BorderColor) setWrap(pWrap);
		else global->getImpl()->setTexture2DWrapPropertyBorder(m_Reference, pColor);
	}

	void Texture2D::setFilters(TextureMinFilter pMin, TextureMagFilter pMag) {
		global->getImpl()->setTexture2DFilter(m_Reference, pMin, pMag);
	}

	void Texture2D::update(const sail::image &pImage, bool pUpdateMipmaps) {
		auto i = global->getImpl();
		i->updateTexture2DData(m_Reference, pImage);
		if(pUpdateMipmaps) i->updateTexture2DMipmap(m_Reference);
	}

	void Texture2D::updateMipmaps() {
		global->getImpl()->updateTexture2DMipmap(m_Reference);
	}
} // aurora