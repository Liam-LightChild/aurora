//
// Created by liam on 12/2/22.
//

#pragma clang diagnostic push
#pragma ide diagnostic ignored "OCInconsistentNamingInspection"
#ifndef AURORA_OPENGL_IMPL_H
#define AURORA_OPENGL_IMPL_H

#include "../graphics/implementation.h"

namespace aurora {

	template<int major, int minor>
	class OpenGLImplementation : public Implementation {
	};

	template<>
	class OpenGLImplementation<3, 2> : public Implementation {
		class Reference : public ObjRefBase {
		public:
			uint32_t resource;

			explicit Reference(uint32_t resource) : resource(resource) {}
			~Reference() override = default;
		};

		class ShaderReference : public Reference {
		public:
			std::unordered_map<std::string, ShaderUniformType> uniforms;

			explicit ShaderReference(uint32_t pResource) : Reference(pResource) {}
			~ShaderReference() override = default;
		};

		class BufferReference : public Reference {
		public:
			BufferType type;

			BufferReference(uint32_t pResource, BufferType pType) : Reference(pResource), type(pType) {}
			~BufferReference() override = default;
		};

		class DrawObjectReference : public Reference {
		public:
			ShaderReference *shader;
			uint32_t vertexCount;
			IndexBufferItemType indexBufferItemType;

			DrawObjectReference(uint32_t pResource, const DrawObjectOptions &pOptions)
				: Reference(pResource),
				  shader(dynamic_cast<ShaderReference *>(pOptions.shader)),
				  vertexCount(pOptions.vertexCount),
				  indexBufferItemType(pOptions.indexBufferItemType) {}

			~DrawObjectReference() override = default;
		};

	public:
		~OpenGLImplementation() override = default;

		ObjRefBase *createShader(const aether::Shader &pShader) override;
		void destroyShader(ObjRefBase *pObject) override;
		void setupWindowHints() override;
		void setupWindowPostCreate() override;
		void updateViewportSize(int pWidth, int pHeight) override;
		void performFinishFrame(Window *pWindow) override;
		void setClearColor(float pRed, float pGreen, float pBlue, float pAlpha) override;
		void performClear(ClearOptions pOptions) override;
		ObjRefBase *createBuffer(BufferType pType) override;
		void destroyBuffer(ObjRefBase *pObject) override;
		void updateBufferData(ObjRefBase *pObject, void *pData, size_t pSize) override;
		void updateBufferData(ObjRefBase *pObject, void *pData, size_t pSize, size_t pOffset) override;
		void retrieveBufferData(ObjRefBase *pObject, void *pData, size_t pSize, size_t pOffset) override;
		ObjRefBase *createTexture2D() override;
		void destroyTexture2D(ObjRefBase *pObject) override;
		void setTexture2DWrapProperty(ObjRefBase *pObject, TextureWrapType pWrap) override;
		void setTexture2DWrapPropertyBorder(ObjRefBase *pObject, glm::vec3 pColor) override;
		void setTexture2DFilter(ObjRefBase *pObject, TextureMinFilter pMin, TextureMagFilter pMag) override;
		void updateTexture2DData(ObjRefBase *pObject, const sail::image &pImage) override;
		void updateTexture2DMipmap(ObjRefBase *pObject) override;
		ObjRefBase *createDrawObject(const DrawObjectOptions &pOptions) override;
		void destroyDrawObject(ObjRefBase *pObject) override;
		void performDraw(ObjRefBase *pDrawObject) override;
	};

	template<>
	class OpenGLImplementation<4, 5> : public Implementation {

	};

}// namespace aurora

#endif// AURORA_OPENGL_IMPL_H

#pragma clang diagnostic pop