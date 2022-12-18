/*
 * This file is part of Aurora Game Engine.
 * https://github.com/liam-lightchild/aurora
 */
#ifndef AURORA_CAMERA_2D_CONTROLLER_H
#define AURORA_CAMERA_2D_CONTROLLER_H

#include "../../controller.h"

namespace aurora::level {

	class Camera2DController : public CameraController {
	private:
		glm::dvec3 m_LastPosition{};
		glm::dquat m_LastRotation{};
		float m_Scale = 1.0f;
		glm::ivec2 m_Size;
		glm::vec3 m_ClearColor{
			0,
			0,
			0
		};

	public:
		static const std::string type;

		Camera2DController(Level *pLevel, Object *pObject, const aether::Level::Controller &pAether);
		~Camera2DController() override;

		void updateMatrices();
		void render() override;
		void update() override;
		std::string getType() override;
		void preRender() override;
	};

} // aurora::level

#endif //AURORA_CAMERA_2D_CONTROLLER_H
