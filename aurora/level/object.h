/*
 * This file is part of Aurora Game Engine.
 * https://github.com/liam-lightchild/aurora
 */

#ifndef AURORA_OBJECT_H
#define AURORA_OBJECT_H

#include <glm/vec3.hpp>
#include <glm/gtx/quaternion.hpp>
#include <vector>
#include <unordered_map>
#include <string>
#include "../aether/aether.h"
#include "controller_registry.h"

namespace aurora::level {

	class Level;
	class Controller;

	class Object {
	private:
		std::string m_Name;
		Level *m_Level;
		Object *m_Parent;
		glm::mat4 m_ObjectMatrix{};
		glm::dvec3 m_Position;
		glm::dquat m_Rotation;
		std::unordered_map<std::string, Object*> m_Children;
		std::vector<Controller*> m_Controllers;

	public:
		Object(Level *pLevel, Object *pParent, const glm::dvec3 &pPosition, const glm::dquat &pRotation,
		       std::string pName);
		virtual ~Object();

	private:
		void updateMatrix();

	public:
		void setLocalPosition(const glm::dvec3 &pPosition);
		void setLocalRotation(const glm::dquat &pRotation);

		const std::string &getName() const;
		const glm::mat4 &getObjectMatrix() const;
		[[nodiscard]] Level *getLevel() const;
		[[nodiscard]] Object *getParent() const;
		[[nodiscard]] const std::unordered_map<std::string, Object*> &getChildren() const;
		[[nodiscard]] const std::vector<Controller *> &getControllers() const;
		[[nodiscard]] glm::dvec3 getPosition() const;
		[[nodiscard]] glm::dquat getRotation() const;

		virtual void addChild(Object *pObject);
		virtual void removeChild(Object *pObject);
		virtual void removeChild(const std::string &pName);
		[[nodiscard]] virtual Object *child(const std::string &pName);
		[[nodiscard]] virtual const Object *child(const std::string &pName) const;

		virtual void addController(Controller *pController);
		virtual void removeController(Controller *pController);
		virtual void createController(const aether::Level::Controller &pAether);
		virtual void deleteController(Controller *pController);
		virtual Controller *getController(const std::string &pType) const;
		virtual Controller *findController(const std::function<bool(Controller*)> &pPredicate) const;

		template<typename T>
		T *getController() const {
			return dynamic_cast<T*>(getController(T::type));
		}

		template<typename T>
		T *findControllerByType() const {
			return dynamic_cast<T*>(findController([](Controller *pController) {
				return dynamic_cast<T*>(pController) != nullptr;
			}));
		}

		virtual void render();
		virtual void update();
	};

} // aurora::level

#endif //AURORA_OBJECT_H
