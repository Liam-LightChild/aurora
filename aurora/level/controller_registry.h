/*
 * This file is part of Aurora Game Engine.
 * https://github.com/liam-lightchild/aurora
 */
#ifndef AURORA_CONTROLLER_REGISTRY_H
#define AURORA_CONTROLLER_REGISTRY_H

#include <string>
#include <functional>
#include "../aether/aether.h"

namespace aurora::level {
	class Controller;
	class Level;
	class Object;

	void registerController(const std::string &pType,
							const std::function<Controller*(Level*, Object*, const aether::Level::Controller&)> &pConstruct,
							const std::function<void(Controller*)> &pDelete);
	Controller *createController(Level *pLevel, Object *pObject, const aether::Level::Controller &pAether);
	void deleteController(Controller *pController);

	template<typename T>
	void registerController() {
		registerController(T::type, [](Level *pLevel, Object *pObject, const aether::Level::Controller &pAether) {
			return new T(pLevel, pObject, pAether);
		}, [](Controller *pPointer) {
			delete dynamic_cast<T*>(pPointer);
		});
	}

	void initializeRegistry();
}

#endif //AURORA_CONTROLLER_REGISTRY_H
