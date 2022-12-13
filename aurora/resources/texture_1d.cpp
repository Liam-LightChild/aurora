#include "texture_1d.h"
#include "../global.h"

namespace aurora {
	Texture1D::Texture1D(ObjRefBase *pReference) : m_Reference(pReference) {}

	Texture1D::Texture1D() {
		m_Reference = global->getImpl()->createTexture1D();
	}

	Texture1D::~Texture1D() {
		global->getImpl()->destroyTexture1D(m_Reference);
	}

	void Texture1D::setWrap(TextureWrapType pWrap) {
		global->getImpl()->setTexture2DWrapProperty(m_Reference, pWrap);
	}

	void Texture1D::setWrap(TextureWrapType pWrap, glm::vec3 pColor) {
		if(pWrap != TextureWrapType::BorderColor) setWrap(pWrap);
		else global->getImpl()->setTexture2DWrapPropertyBorder(m_Reference, pColor);
	}

	void Texture1D::setFilters(TextureMinFilter pMin, TextureMagFilter pMag) {
		global->getImpl()->setTexture1DFilter(m_Reference, pMin, pMag);
	}

	void Texture1D::update(int pWidth, const float *pDataRgba, bool pUpdateMipmaps) {
		auto i = global->getImpl();
		i->updateTexture1DData(m_Reference, pWidth, pDataRgba);
		if(pUpdateMipmaps) i->updateTexture1DMipmap(m_Reference);
	}

	void Texture1D::updateMipmaps() {
		global->getImpl()->updateTexture1DMipmap(m_Reference);
	}
}

