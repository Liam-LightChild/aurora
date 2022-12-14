/*
 * This file is part of Aurora Game Engine.
 * https://github.com/liam-lightchild/aurora
 */

#include "object.h"
#include <algorithm>
#include <utility>
#include "controller.h"

namespace aurora::level {
	Object::Object(Level *pLevel, Object *pParent, const glm::dvec3 &pPosition, const glm::dquat &pRotation,
	               std::string pName)
		: m_Level(pLevel), m_Parent(pParent), m_Position(pPosition), m_Rotation(glm::normalize(pRotation)), m_Name(std::move(pName)) {
		updateMatrix();
	}

	Object::~Object() {

	}

	void Object::setLocalPosition(const glm::dvec3 &pPosition) {
		m_Position = pPosition;
		updateMatrix();
	}

	void Object::setLocalRotation(const glm::dquat &pRotation) {
		m_Rotation = glm::normalize(pRotation);
		updateMatrix();
	}

	void Object::updateMatrix() {
		m_ObjectMatrix = glm::translate(glm::identity<glm::dmat4>(), getPosition()) * glm::toMat4(getRotation());
	}

	Level *Object::getLevel() const {
		return m_Level;
	}

	Object *Object::getParent() const {
		return m_Parent;
	}

	const std::unordered_map<std::string, Object*> &Object::getChildren() const {
		return m_Children;
	}

	const std::vector<Controller *> &Object::getControllers() const {
		return m_Controllers;
	}

	const glm::mat4 &Object::getObjectMatrix() const {
		return m_ObjectMatrix;
	}

	void Object::addChild(Object *pObject) {
		m_Children.insert({pObject->m_Name, pObject});
	}

	const std::string &Object::getName() const {
		return m_Name;
	}

	Object *Object::child(const std::string &pName) {
		return m_Children.at(pName);
	}

	const Object *Object::child(const std::string &pName) const {
		return m_Children.at(pName);
	}

	void Object::removeChild(Object *pObject) {
		m_Children.erase(pObject->m_Name);
	}

	void Object::removeChild(const std::string &pName) {
		m_Children.erase(pName);
	}

	void Object::addController(Controller *pController) {
		m_Controllers.emplace_back(pController);
	}

	void Object::removeController(Controller *pController) {
		m_Controllers.erase(std::remove(m_Controllers.begin(), m_Controllers.end(), pController), m_Controllers.end());
	}

	void Object::createController(const aether::Level::Controller &pAether) {
		addController(level::createController(m_Level, this, pAether));
	}

	void Object::deleteController(Controller *pController) {
		removeController(pController);
		level::deleteController(pController);
	}

	void Object::render() {
		for(const auto &item: m_Children) {
			item.second->render();
		}

		for(const auto &item: m_Controllers) {
			item->render();
		}
	}

	void Object::update() {
		for(const auto &item: m_Children) {
			item.second->update();
		}

		for(const auto &item: m_Controllers) {
			item->update();
		}
	}

	glm::dvec3 Object::getPosition() const {
		return m_Parent != nullptr ? m_Position + m_Parent->getPosition() : m_Position;
	}

	glm::dquat Object::getRotation() const {
		return m_Parent != nullptr ? m_Parent->getRotation() * m_Rotation : m_Rotation;
	}

	Controller *Object::getController(const std::string &pType) const {
		return *std::find_if(m_Controllers.begin(), m_Controllers.end(), [pType](Controller *pController) {
			return pController->getType() == pType;
		});
	}

	Controller *Object::findController(const std::function<bool(Controller *)> &pPredicate) const {
		return *std::find_if(m_Controllers.begin(), m_Controllers.end(), pPredicate);
	}
} // aurora::level