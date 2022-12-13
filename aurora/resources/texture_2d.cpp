/*
 * This file is part of Aurora Game Engine.
 * https://github.com/liam-lightchild/aurora
 */

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
		if(pWrap != TextureWrapType::BorderColor) setWrap(pWrap);
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

	Texture2D::Texture2D(AssetLoader*, const std::filesystem::path &pPath, const std::string &pAssetId) : Texture2D() {
		if(!exists(pPath)) throw std::runtime_error("Asset " + pAssetId + ": cannot find " + pPath.string());

		sail::image img;
		auto absPath = absolute(pPath);
		auto meta = aether::TextureMeta(nlohmann::json::from_cbor(std::ifstream(absPath)));
		auto texPath = absPath.parent_path() / meta.path;
		img.load(absolute(texPath).string());

		if(meta.wrap == TextureWrapType::BorderColor) setWrap(meta.wrap, meta.borderColor);
		else setWrap(meta.wrap);
		setFilters(meta.minFilter, meta.magFilter);
		update(img, meta.useMipmap);
	}

	void Texture2D::update(int pWidth, int pHeight, const uint8_t *pDataRgba, bool pUpdateMipmaps) {
		auto i = global->getImpl();
		i->updateTexture2DData(m_Reference, pWidth, pHeight, pDataRgba);
		if(pUpdateMipmaps) i->updateTexture2DMipmap(m_Reference);
	}

	const std::string Texture2D::missingAssetName = "aurora:_internal/missing.texture";
} // aurora