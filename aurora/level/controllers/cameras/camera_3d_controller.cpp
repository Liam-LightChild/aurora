/*
 * This file is part of Aurora Game Engine.
 * https://github.com/liam-lightchild/aurora
 */
#include "camera_3d_controller.h"
#include "aurora/global.h"

namespace aurora::level {
	const std::string Camera3DController::type = "aurora:camera/3d";

	Camera3DController::Camera3DController(Level *pLevel, Object *pObject, const aether::Level::Controller &pAether)
		: CameraController(pLevel, pObject, pAether) {
		if(pAether.properties.contains("Scale")) {
			m_Scale = std::stof(pAether.properties.at("Scale"));
		}

		if(pAether.properties.contains("FieldOfView")) {
			m_FieldOfView = glm::radians(std::stof(pAether.properties.at("FieldOfView")));
		}

		if(pAether.properties.contains("ClearColor")) {
			auto i = std::stoi(pAether.properties.at("ClearColor"), nullptr, 16);
			m_ClearColor.r = (float)((i & 0xFF0000) >> 16) / 256.f;
			m_ClearColor.g = (float)((i & 0x00FF00) >> 8) / 256.f;
			m_ClearColor.b = (float)(i & 0x0000FF) / 256.f;
		}

		updateMatrices();
	}

	Camera3DController::~Camera3DController() = default;

	void Camera3DController::render() {}

	void Camera3DController::update() {
		if(m_LastPosition != object->getPosition() ||
		   m_LastRotation != object->getRotation() ||
		   global->getWindow()->getSize() != m_Size) {
			updateMatrices();
		}
	}

	std::string Camera3DController::getType() {
		return type;
	}

	void Camera3DController::updateMatrices() {
		auto wSize = global->getWindow()->getSize();
		auto height = m_Scale;
		auto width = ((float)wSize.x / (float)wSize.y) * m_Scale;
		setPerspectiveMatrix(glm::perspectiveFov(m_FieldOfView, width, height, 0.001f, 100.f));
		setViewMatrix(glm::inverse(object->getObjectMatrix()));

		m_LastRotation = object->getRotation();
		m_LastPosition = object->getPosition();
		m_Size = wSize;
	}

	void Camera3DController::preRender() {
		CameraController::preRender();
		global->getGraphics()->clear(m_ClearColor.r, m_ClearColor.g, m_ClearColor.b);
	}
} // aurora::level