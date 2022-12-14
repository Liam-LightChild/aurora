/*
 * This file is part of Aurora Game Engine.
 * https://github.com/liam-lightchild/aurora
 */
#include "controller_registry.h"
#include "controller.h"
#include "controllers/cameras/camera_2d_controller.h"
#include "controllers/cameras/camera_3d_controller.h"
#include "controllers/renderer_controller.h"
#include "controllers/mesh_asset_controller.h"
#include <unordered_map>

namespace aurora::level {
	struct RegisteredController {
		std::function<Controller *(Level*, Object*, const aether::Level::Controller &)> createFn;
		std::function<void(Controller *)> deleteFn;
	};

	std::unordered_map<std::string, RegisteredController> registeredControllers;

	void registerController(const std::string &pType,
	                        const std::function<Controller*(Level*, Object*, const aether::Level::Controller&)> &pConstruct,
	                        const std::function<void(Controller*)> &pDelete) {
		registeredControllers.insert({pType, {pConstruct, pDelete}});
	}

	Controller *createController(Level *pLevel, Object *pObject, const aether::Level::Controller &pAether) {
		if(registeredControllers.empty()) initializeRegistry();
		if(!registeredControllers.contains(pAether.type)) throw std::runtime_error("Controller registry does not contain: " + pAether.type);
		return registeredControllers.at(pAether.type).createFn(pLevel, pObject, pAether);
	}

	void deleteController(Controller *pController) {
		registeredControllers.at(pController->getType()).deleteFn(pController);
	}

	void initializeRegistry() {
		registeredControllers.clear();

		registerController<Camera2DController>();
		registerController<Camera3DController>();
		registerController<MeshAssetController>();
		registerController<RendererController>();
	}
}
