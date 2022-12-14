/*
 * This file is part of Aurora Game Engine.
 * https://github.com/liam-lightchild/aurora
 */
#include "mesh_asset_controller.h"
#include "aurora/global.h"

namespace aurora::level {
	const std::string MeshAssetController::type = "aurora:mesh";

	MeshAssetController::MeshAssetController(Level *pLevel, Object *pObject,
	                                         const aether::Level::Controller &pAether) : Controller(pLevel, pObject,
	                                                                                                pAether) {
		if(!pAether.properties.contains("AssetId"))
			throw std::runtime_error("AssetId must be provided on aurora:mesh");
		m_Mesh = global->getAssetLoader()->load<aether::Mesh>(pAether.properties.at("AssetId"));
	}

	MeshAssetController::~MeshAssetController() {
		global->getAssetLoader()->unload<aether::Mesh>(m_Mesh);
	}

	void MeshAssetController::render() {}
	void MeshAssetController::update() {}

	std::string MeshAssetController::getType() {
		return type;
	}

	aether::Mesh MeshAssetController::getMesh() {
		return *m_Mesh;
	}
} // aurora::level