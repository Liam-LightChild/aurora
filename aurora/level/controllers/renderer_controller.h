/*
 * This file is part of Aurora Game Engine.
 * https://github.com/liam-lightchild/aurora
 */
#ifndef AURORA_RENDERER_CONTROLLER_H
#define AURORA_RENDERER_CONTROLLER_H

#include "../controller.h"

#include "../../aether/aether.h"
#include "../../resources/shader.h"
#include "../../resources/buffer.h"
#include "../../resources/draw_object.h"

namespace aurora::level {

	class MeshProvider {
	public:
		virtual aether::Mesh getMesh() = 0;
	};

	class RendererController : public Controller {
	private:
		aether::Mesh m_Mesh;
		Shader *m_Shader;
		Buffer *m_VertexBuffer, *m_IndexBuffer;
		DrawObject *m_DrawObject;

	public:
		static const std::string type;

		RendererController(Level *pLevel, Object *pObject, const aether::Level::Controller &pAether);
		~RendererController() override;
		void render() override;
		void update() override;
		std::string getType() override;
	};

} // aurora::level

#endif //AURORA_RENDERER_CONTROLLER_H
