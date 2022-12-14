/*
 * This file is part of Aurora Game Engine.
 * https://github.com/liam-lightchild/aurora
 */

#ifndef AURORA_CONTROLLER_H
#define AURORA_CONTROLLER_H

#include "../aether/aether.h"
#include "level.h"
#include "object.h"
#include "aurora/resources/framebuffer.h"

namespace aurora::level {

	class Controller {
	protected:
		Level *level;
		Object *object;

	public:
		Controller(Level *pLevel, Object *pObject, const aether::Level::Controller &pAether)
			: level(pLevel), object(pObject) {}
		virtual ~Controller() = default;

		virtual void render() = 0;
		virtual void update() = 0;

		virtual std::string getType() = 0;
	};

	class CameraController : public Controller {
	private:
		int m_Id;
		glm::mat4 m_ViewMatrix = glm::identity<glm::mat4>();
		glm::mat4 m_PerspectiveMatrix = glm::identity<glm::mat4>();
		Framebuffer *m_Framebuffers[2];
		bool m_CurrentlyFlipped = false;

	public:
		CameraController(Level *pLevel, Object *pObject, const aether::Level::Controller &pAether);
		~CameraController() override;

		virtual void preRender() {}
		virtual void postRender() {}

		[[nodiscard]] const glm::mat4 &getViewMatrix() const {
			return m_ViewMatrix;
		}

		[[nodiscard]] const glm::mat4 &getPerspectiveMatrix() const {
			return m_PerspectiveMatrix;
		}

		[[nodiscard]] Framebuffer *getCurrentFramebuffer() const {
			return m_Framebuffers[m_CurrentlyFlipped ? 1 : 0];
		}

		[[nodiscard]] Framebuffer *getOppositeFramebuffer() const {
			return m_Framebuffers[m_CurrentlyFlipped ? 0 : 1];
		}

		void swapFramebuffers() {
			m_CurrentlyFlipped = !m_CurrentlyFlipped;
		}

		int getId() const;

	protected:
		void setViewMatrix(const glm::mat4 &pViewMatrix) {
			m_ViewMatrix = pViewMatrix;
		}

		void setPerspectiveMatrix(const glm::mat4 &pPerspectiveMatrix) {
			m_PerspectiveMatrix = pPerspectiveMatrix;
		}
	};

} // aurora::level

#endif //AURORA_CONTROLLER_H
