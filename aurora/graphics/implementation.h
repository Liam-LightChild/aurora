//
// Created by liam on 12/2/22.
//

#ifndef AURORA_IMPLEMENTATION_H
#define AURORA_IMPLEMENTATION_H

#include "../aether/aether.h"
#include "../window.h"
#include "obj_ref_base.h"
#include "enums.h"
#include <memory>
#include <glm/glm.hpp>
#include <utility>
#include <sail-c++/sail-c++.h>

#define A_VERTEX(name, type, count) ::aurora::VertexArrangement::Node(#name, ::aurora::VertexInputType::type, count)

namespace aurora {

	struct ClearOptions {
		bool color: 1 = true;
		bool depth: 1 = true;
		bool stencil: 1 = false;
	};

	struct VertexArrangement {
		struct Node {
			std::string name;
			VertexInputType type;
			int count;

			Node(std::string pName, VertexInputType pType, int pCount = 1) : name(std::move(pName)), type(pType),
			                                                                 count(pCount) {}
		};

	private:
		std::vector<Node> m_Nodes;

	public:
		VertexArrangement() = default;
		explicit VertexArrangement(const std::vector<Node> &pNodes) : m_Nodes(pNodes) {}
		VertexArrangement(std::initializer_list<Node> pNodes) : m_Nodes(pNodes) {}

		[[nodiscard]] auto begin() const { return m_Nodes.begin(); }
		[[nodiscard]] auto end() const { return m_Nodes.end(); }
	};

	struct DrawObjectOptions {
		ObjRefBase *shader = nullptr;
		ObjRefBase *vertexBuffer = nullptr, *indexBuffer = nullptr;

		/**
		 * This is the count of the values in indexBuffer, not vertexBuffer.
		 */
		uint32_t vertexCount = 0;

		IndexBufferItemType indexBufferItemType = IndexBufferItemType::UnsignedInt;
		VertexArrangement arrangement;

		ObjRefBase *textures[16] {
			nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr,
			nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr,
		};

		ObjRefBase *textures1D[8] {
			nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr,
		};

		ObjRefBase *textures3D[8] {
			nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr,
		};
	};

	class Implementation {
	public:
		virtual ~Implementation() = default;

		class Node {
		public:
			virtual ~Node() = default;
			virtual int getPriority() = 0;
			virtual bool isValid() = 0;
			virtual Implementation *create() = 0;
		};

		virtual void setClearColor(float pRed, float pGreen, float pBlue, float pAlpha) = 0;
		virtual void performClear(ClearOptions pOptions = ClearOptions()) = 0; // NOLINT(google-default-arguments)

		virtual ObjRefBase *createShader(const aether::Shader &pAsset) = 0;
		virtual void destroyShader(ObjRefBase *pObject) = 0;

		virtual ObjRefBase *createBuffer(BufferType pType) = 0;
		virtual void destroyBuffer(ObjRefBase *pObject) = 0;
		virtual void updateBufferData(ObjRefBase *pObject, void *pData, size_t pSize) = 0;
		virtual void updateBufferData(ObjRefBase *pObject, void *pData, size_t pSize, size_t pOffset) = 0;
		virtual void retrieveBufferData(ObjRefBase *pObject, void *pData, size_t pSize, size_t pOffset) = 0;

		virtual ObjRefBase *createTexture1D() = 0;
		virtual void destroyTexture1D(ObjRefBase *pObject) = 0;
		virtual void setTexture1DWrapProperty(ObjRefBase *pObject, TextureWrapType pWrap) = 0;
		virtual void setTexture1DWrapPropertyBorder(ObjRefBase *pObject, glm::vec3 pColor) = 0;
		virtual void setTexture1DFilter(ObjRefBase *pObject, TextureMinFilter pMin, TextureMagFilter pMag) = 0;
		virtual void updateTexture1DData(ObjRefBase *pObject, int pWidth, const float *pDataRgba) = 0;
		virtual void updateTexture1DMipmap(ObjRefBase *pObject) = 0;

		virtual ObjRefBase *createTexture2D() = 0;
		virtual void destroyTexture2D(ObjRefBase *pObject) = 0;
		virtual void setTexture2DWrapProperty(ObjRefBase *pObject, TextureWrapType pWrap) = 0;
		virtual void setTexture2DWrapPropertyBorder(ObjRefBase *pObject, glm::vec3 pColor) = 0;
		virtual void setTexture2DFilter(ObjRefBase *pObject, TextureMinFilter pMin, TextureMagFilter pMag) = 0;
		virtual void updateTexture2DData(ObjRefBase *pObject, const sail::image &pImage) = 0;
		virtual void updateTexture2DMipmap(ObjRefBase *pObject) = 0;

		virtual ObjRefBase *createTexture3D() = 0;
		virtual void destroyTexture3D(ObjRefBase *pObject) = 0;
		virtual void setTexture3DWrapProperty(ObjRefBase *pObject, TextureWrapType pWrap) = 0;
		virtual void setTexture3DWrapPropertyBorder(ObjRefBase *pObject, glm::vec3 pColor) = 0;
		virtual void setTexture3DFilter(ObjRefBase *pObject, TextureMinFilter pMin, TextureMagFilter pMag) = 0;
		virtual void updateTexture3DData(ObjRefBase *pObject, int pWidth, int pHeight, int pDepth, const float *pDataRgba) = 0;
		virtual void updateTexture3DMipmap(ObjRefBase *pObject) = 0;

		virtual ObjRefBase *createDrawObject(const DrawObjectOptions &pOptions) = 0;
		virtual void destroyDrawObject(ObjRefBase *pObject) = 0;
		virtual void performDraw(ObjRefBase *pDrawObject) = 0;

		virtual void setupWindowHints() = 0;
		virtual void setupWindowPostCreate() = 0;
		virtual void updateViewportSize(int pWidth, int pHeight) = 0;
		virtual void performFinishFrame(Window *pWindow) = 0;
	};

}// namespace aurora

#endif// AURORA_IMPLEMENTATION_H
