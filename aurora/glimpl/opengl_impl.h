/*
 * This file is part of Aurora Game Engine.
 * https://github.com/liam-lightchild/aurora
 */

#pragma clang diagnostic push
#pragma ide diagnostic ignored "OCInconsistentNamingInspection"
#ifndef AURORA_OPENGL_IMPL_H
#define AURORA_OPENGL_IMPL_H

#include "../graphics/implementation.h"

namespace aurora {

	// Base class is abstract and cannot be used.
	template<int major, int minor>
	class OpenGLImplementation : public Implementation {
	};

	// See base class for useful method descriptions.
	template<>
	class OpenGLImplementation<3, 2> : public Implementation {
		/*
		 * Contains an ambiguous reference to a resource created by
		 * OpenGL.
		 */
		class Reference : public ObjRefBase {
		public:
			uint32_t resource;

			explicit Reference(uint32_t resource) : resource(resource) {}

			~Reference() override = default;
		};

		/*
		 * Extends the Reference class to add information about the
		 * uniforms that the shader uses.
		 */
		class ShaderReference : public Reference {
		public:
			std::unordered_map<std::string, ShaderUniformType> uniforms;

			explicit ShaderReference(uint32_t pResource) : Reference(pResource) {}

			~ShaderReference() override = default;
		};

		/*
		 * Extends the Reference class to add information about the
		 * type of buffer that is being referenced, used in binding.
		 */
		class BufferReference : public Reference {
		public:
			BufferType type;

			BufferReference(uint32_t pResource, BufferType pType) : Reference(pResource), type(pType) {}

			~BufferReference() override = default;
		};

		/*
		 * Extends the Reference class to include information about the
		 * shader to bind, the number of vertices, and the type contained
		 * in the index buffer part of a draw object.
		 *
		 * The Reference refers to a VAO object, and none of the objects
		 * contained within are discarded when the object is destroyed.
		 */
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

		/*
		 * Extends the Reference class to include information about the
		 * objects contained within framebuffers.
		 *
		 * The colorTexture and depthStencilRendernode are both discarded
		 * when the object is destroyed.
		 */
		class FramebufferReference : public Reference {
		public:
			Reference *colorTexture;

			/*
			 * Refers to a renderbuffer.
			 */
			Reference *depthStencilRendernode;

			int width, height;

			FramebufferReference(uint32_t pResource, Reference *pColorTexture, Reference *pDepthStencilTexture,
			                     int pWidth,
			                     int pHeight)
				: Reference(pResource), colorTexture(pColorTexture), depthStencilRendernode(pDepthStencilTexture),
				  width(pWidth), height(pHeight) {}

			~FramebufferReference() override = default;
		};

		/*
		 * Distinct from FramebufferReference, this class extends the Reference
		 * class to refer to the default framebuffer built into OpenGL. The
		 * value is almost certainly always zero.
		 */
		class DefaultFramebufferReference : public Reference {
		public:
			explicit DefaultFramebufferReference(uint32_t pResource) : Reference(pResource) {}

			~DefaultFramebufferReference() override = default;
		};

		int max1DDim, max2DDim, max3DDim;

		DefaultFramebufferReference m_DefaultFramebufferRef{0};

	public:
		~OpenGLImplementation() override = default;

		// See base class for documentation.

		ObjRefBase *createShader(const aether::Shader &pShader) override;
		void destroyShader(ObjRefBase *pObject) noexcept override;
		void setupWindowHints() override;
		void setupWindowPostCreate() override;
		void updateViewportSize(int pWidth, int pHeight) override;
		void performFinishFrame(Window *pWindow) override;
		void setClearColor(float pRed, float pGreen, float pBlue, float pAlpha) override;
		void performClear(ClearOptions pOptions) override;
		ObjRefBase *createBuffer(BufferType pType) override;
		void destroyBuffer(ObjRefBase *pObject) noexcept override;
		void updateBufferData(ObjRefBase *pObject, void *pData, size_t pSize) override;
		void updateBufferData(ObjRefBase *pObject, void *pData, size_t pSize, size_t pOffset) override;
		void retrieveBufferData(ObjRefBase *pObject, void *pData, size_t pSize, size_t pOffset) override;
		ObjRefBase *createTexture2D() override;
		void destroyTexture2D(ObjRefBase *pObject) noexcept override;
		void setTexture2DWrapProperty(ObjRefBase *pObject, TextureWrapType pWrap) override;
		void setTexture2DWrapPropertyBorder(ObjRefBase *pObject, glm::vec3 pColor) override;
		void setTexture2DFilter(ObjRefBase *pObject, TextureMinFilter pMin, TextureMagFilter pMag) override;
		void updateTexture2DData(ObjRefBase *pObject, const sail::image &pImage) override;
		void updateTexture2DMipmap(ObjRefBase *pObject) override;
		ObjRefBase *createDrawObject(const DrawObjectOptions &pOptions) override;
		void destroyDrawObject(ObjRefBase *pObject) noexcept override;
		void performDraw(ObjRefBase *pDrawObject, const MatrixSet &pMatrices) override;
		ObjRefBase *createTexture1D() override;
		void destroyTexture1D(ObjRefBase *pObject) noexcept override;
		void setTexture1DWrapProperty(ObjRefBase *pObject, TextureWrapType pWrap) override;
		void setTexture1DWrapPropertyBorder(ObjRefBase *pObject, glm::vec3 pColor) override;
		void setTexture1DFilter(ObjRefBase *pObject, TextureMinFilter pMin, TextureMagFilter pMag) override;
		void updateTexture1DData(ObjRefBase *pObject, int pWidth, const uint8_t *pDataRgba) override;
		void updateTexture1DMipmap(ObjRefBase *pObject) override;
		ObjRefBase *createTexture3D() override;
		void destroyTexture3D(ObjRefBase *pObject) noexcept override;
		void setTexture3DWrapProperty(ObjRefBase *pObject, TextureWrapType pWrap) override;
		void setTexture3DWrapPropertyBorder(ObjRefBase *pObject, glm::vec3 pColor) override;
		void setTexture3DFilter(ObjRefBase *pObject, TextureMinFilter pMin, TextureMagFilter pMag) override;
		void updateTexture3DData(ObjRefBase *pObject, int pWidth, int pHeight, int pDepth,
		                         const uint8_t *pDataRgba) override;
		void updateTexture3DMipmap(ObjRefBase *pObject) override;
		void updateTexture2DData(ObjRefBase *pObject, int pWidth, int pHeight, const uint8_t *pDataRgba) override;
		ObjRefBase *createFramebuffer(int pWidth, int pHeight) override;
		void reinitializeFramebuffer(ObjRefBase *pObject, int pWidth, int pHeight) override;
		void destroyFramebuffer(ObjRefBase *pObject) noexcept override;
		ObjRefBase *getFramebufferColorTexture2D(ObjRefBase *pObject) override;
		ObjRefBase *getFramebufferDepthStencilTexture2D(ObjRefBase *pObject) override;
		void performBlitFramebuffer(ObjRefBase *pSource, ObjRefBase *pTarget) override;
		void performBlitFramebuffer(ObjRefBase *pSource, ObjRefBase *pTarget, int pStartX, int pStartY, int pWidth,
		                            int pHeight) override;
		ObjRefBase *getDefaultFramebuffer() override;
		void activateFramebuffer(ObjRefBase *pObject) override;
	};

	template<>
	class OpenGLImplementation<4, 5> : public Implementation {

	};

}// namespace aurora

#endif// AURORA_OPENGL_IMPL_H

#pragma clang diagnostic pop