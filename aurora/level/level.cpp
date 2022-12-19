/*
 * This file is part of Aurora Game Engine.
 * https://github.com/liam-lightchild/aurora
 */

#include "level.h"
#include "object.h"
#include "controller.h"
#include "aurora/global.h"
#include <fstream>

namespace aurora::level {
	Level::Level() {

	}

	Level::~Level() {

	}

	Level::Level(AssetLoader *, const std::filesystem::path &pPath, const std::string &)
		: Level(aether::Level(nlohmann::json::from_cbor(std::ifstream(pPath)))) {

	}

	Object *parseObject(Level *pLevel, const aether::Level::Object &pObj, Object *pParent) {
		auto obj = new Object(pLevel, pParent, pObj.position, pObj.rotation, pObj.name);

		for(const auto &item: pObj.objects) {
			obj->addChild(parseObject(pLevel, item.second, obj));
		}

		for(const auto &item: pObj.controllers) {
			obj->createController(item);
		}

		return obj;
	}

	Level::Level(const aether::Level &pAether) {
		for(const auto &item: pAether.objects) {
			m_Objects.emplace_back(parseObject(this, item.second, nullptr));
		}
	}

	void Level::render() {
		if(m_CurrentCamera >= 0) {
			auto defaultFb = Framebuffer::getDefault();
			renderCamera(m_CurrentCamera)->blit(defaultFb);
			delete defaultFb;
		}
	}

	void Level::update() {
		for(const auto &item: m_Objects) {
			item->update();
		}
	}

	Framebuffer *Level::renderCamera(int pCameraId) {
		auto cont = m_Cameras.at(pCameraId);
		auto i = global->getImpl();
		auto f = cont->getCurrentFramebuffer();
		i->activateFramebuffer(f->getReference());

		cont->preRender();

		for(const auto &item: m_Objects) {
			item->render();
		}

		cont->postRender();
		return cont->getCurrentFramebuffer(); // this might have changed!
	}

	int Level::getCurrentCamera() const {
		return m_CurrentCamera;
	}

	void Level::setCurrentCamera(int pCurrentCamera) {
		m_CurrentCamera = pCurrentCamera;
	}

	void Level::registerCamera(int pId, CameraController *pController) {
		m_Cameras.insert({
			                 pId,
			                 pController
		                 });
	}

	void Level::unregisterCamera(int pId) {
		m_Cameras.erase(pId);
	}

	const std::unordered_map<int, CameraController *> &Level::getCameras() const {
		return m_Cameras;
	}

	CameraController *Level::getCurrentCameraController() const {
		return m_Cameras.at(m_CurrentCamera);
	}
} // aurora::level
