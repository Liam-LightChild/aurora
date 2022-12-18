/*
 * This file is part of Aurora Game Engine.
 * https://github.com/liam-lightchild/aurora
 */
#include "renderer_controller.h"
#include "aurora/global.h"

namespace aurora::level {
	const std::string RendererController::type = "aurora:renderer";

	RendererController::RendererController(Level *pLevel, Object *pObject, const aether::Level::Controller &pAether)
		: Controller(pLevel, pObject, pAether) {
		auto mesh = object->findControllerByType<MeshProvider>()->getMesh();
		if(!pAether.properties.contains("ShaderAssetId")) {
			throw std::runtime_error("RendererController does not have ShaderAssetId property");
		}
		m_Shader = global->getAssetLoader()->load<Shader>(pAether.properties.at("ShaderAssetId"));
		aether::OptimisedMesh opt(mesh, m_Shader->getAether());

		m_VertexBuffer = new Buffer(VertexBuffer);
		m_IndexBuffer = new Buffer(IndexBuffer);
		m_VertexBuffer->update(opt.vertexData.data(), opt.vertexData.size() * sizeof(float));
		m_IndexBuffer->update(opt.indexData.data(), opt.indexData.size() * sizeof(uint32_t));

		m_DrawObject = new DrawObject(DrawObjectOptions{
			.shader = m_Shader->getReference(),
			.vertexBuffer = m_VertexBuffer->getReference(),
			.indexBuffer = m_IndexBuffer->getReference(),
			.vertexCount = static_cast<uint32_t>(opt.indexData.size()),
			.indexBufferItemType = IndexBufferItemType::UnsignedInt,
			.arrangement = m_Shader->getArrangement(),
		});
	}

	void RendererController::render() {
		auto camera = level->getCurrentCameraController();
		m_DrawObject->draw(MatrixSet{
			object->getObjectMatrix(),
			camera->getViewMatrix(),
			camera->getPerspectiveMatrix()
		});
	}

	void RendererController::update() {

	}

	std::string RendererController::getType() {
		return std::string();
	}

	RendererController::~RendererController() {
		delete m_DrawObject;
		delete m_VertexBuffer;
		delete m_IndexBuffer;
		global->getAssetLoader()->unload<Shader>(m_Shader);
	}

} // aurora::level
