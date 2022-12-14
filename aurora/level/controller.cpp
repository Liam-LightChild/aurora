/*
 * This file is part of Aurora Game Engine.
 * https://github.com/liam-lightchild/aurora
 */

#include "controller.h"
#include "aurora/global.h"

aurora::level::CameraController::CameraController(aurora::level::Level *pLevel, aurora::level::Object *pObject,
                                                  const aurora::aether::Level::Controller &pAether) : Controller(pLevel,
                                                                                                                 pObject,
                                                                                                                 pAether) {
	auto size = global->getWindow()->getSize();
	m_Framebuffers[0] = new Framebuffer(size.x, size.y);
	m_Framebuffers[1] = new Framebuffer(size.x, size.y);
	if(!pAether.properties.contains("CameraId")) throw std::runtime_error("camera controller needs id");
	m_Id = std::stoi(pAether.properties.at("CameraId"));
	pLevel->registerCamera(m_Id, this);
}

aurora::level::CameraController::~CameraController() {
	level->unregisterCamera(m_Id);
	delete m_Framebuffers[0];
	delete m_Framebuffers[1];
}

int aurora::level::CameraController::getId() const {
	return m_Id;
}
