/*
 * This file is part of Aurora Game Engine.
 * https://github.com/liam-lightchild/aurora
 */
#ifndef AURORA_MESH_ASSET_CONTROLLER_H
#define AURORA_MESH_ASSET_CONTROLLER_H

#include "../controller.h"
#include "renderer_controller.h"

namespace aurora::level {

	class MeshAssetController : public Controller, public MeshProvider {
	private:
		aether::Mesh *m_Mesh;

	public:
		static const std::string type;

		MeshAssetController(Level *pLevel, Object *pObject, const aether::Level::Controller &pAether);
		~MeshAssetController() override;

		void render() override;
		void update() override;
		std::string getType() override;
		aether::Mesh getMesh() override;
	};

} // aurora::level

#endif //AURORA_MESH_ASSET_CONTROLLER_H
