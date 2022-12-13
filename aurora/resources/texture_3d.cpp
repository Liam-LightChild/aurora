//
// Created by liam on 12/12/22.
//

#include "texture_3d.h"
#include "aurora/global.h"

namespace aurora {
	Texture3D::Texture3D(ObjRefBase *pReference) : m_Reference(pReference) {}

	Texture3D::Texture3D() {
		m_Reference = global->getImpl()->createTexture3D();
	}

	Texture3D::~Texture3D() {
		global->getImpl()->destroyTexture3D(m_Reference);
	}

	void Texture3D::setWrap(TextureWrapType pWrap) {
		global->getImpl()->setTexture3DWrapProperty(m_Reference, pWrap);
	}

	void Texture3D::setWrap(TextureWrapType pWrap, glm::vec3 pColor) {
		if(pWrap != TextureWrapType::BorderColor) setWrap(pWrap);
		else global->getImpl()->setTexture3DWrapPropertyBorder(m_Reference, pColor);
	}

	void Texture3D::setFilters(TextureMinFilter pMin, TextureMagFilter pMag) {
		global->getImpl()->setTexture3DFilter(m_Reference, pMin, pMag);
	}

	void Texture3D::update(int pWidth, int pHeight, int pDepth, const float *pDataRgba, bool pUpdateMipmaps) {
		auto i = global->getImpl();
		i->updateTexture3DData(m_Reference, pWidth, pHeight, pDepth, pDataRgba);
		if(pUpdateMipmaps) i->updateTexture3DMipmap(m_Reference);
	}
} // aurora