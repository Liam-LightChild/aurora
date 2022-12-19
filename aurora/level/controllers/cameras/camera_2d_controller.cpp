/*
 * This file is part of Aurora Game Engine.
 * https://github.com/liam-lightchild/aurora
 */
#include "camera_2d_controller.h"
#include "aurora/global.h"

namespace aurora::level {
	const std::string Camera2DController::type = "aurora:camera/2d";

	Camera2DController::Camera2DController(Level *pLevel, Object *pObject, const aether::Level::Controller &pAether)
		: CameraController(pLevel, pObject, pAether) {
		if(pAether.properties.contains("Scale")) {
			m_Scale = std::stof(pAether.properties.at("Scale"));
		}

		if(pAether.properties.contains("ClearColor")) {
			auto i = std::stoi(pAether.properties.at("ClearColor"), nullptr, 16);
			m_ClearColor.r = (float) ((i & 0xFF0000) >> 16) / 256.f;
			m_ClearColor.g = (float) ((i & 0x00FF00) >> 8) / 256.f;
			m_ClearColor.b = (float) (i & 0x0000FF) / 256.f;
		}

		updateMatrices();
	}

	Camera2DController::~Camera2DController() = default;

	void Camera2DController::render() {}

	void Camera2DController::update() {
		auto size = global->getWindow()->getSize();
		if(m_LastPosition != object->getPosition() ||
		   m_LastRotation != object->getRotation() ||
		   size != m_Size) {
			updateMatrices();
			m_Size = size;
		}
	}

	std::string Camera2DController::getType() {
		return type;
	}

	void Camera2DController::updateMatrices() {
		if(!global->getWindow()->isReallyVisible()) return;

		auto wSize = global->getWindow()->getSize();
		auto t = m_Scale;
		auto r = ((float) wSize.x / (float) wSize.y) * m_Scale;
		setPerspectiveMatrix(glm::ortho(-r, r, -t, t, 0.001f, 100.f));
		setViewMatrix(glm::inverse(object->getObjectMatrix()));

		m_LastRotation = object->getRotation();
		m_LastPosition = object->getPosition();
	}

	void Camera2DController::preRender() {
		CameraController::preRender();
		global->getGraphics()->clear(m_ClearColor.r, m_ClearColor.g, m_ClearColor.b);
	}
} // aurora::level