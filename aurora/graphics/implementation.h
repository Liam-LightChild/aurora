/*
 * This file is part of Aurora Game Engine.
 * https://github.com/liam-lightchild/aurora
 */

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

#define A_VNODE(name, type, count) ::aurora::VertexArrangement::Node(#name, ::aurora::VertexInputType::type, count)

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

		ObjRefBase *textures[16]{
			nullptr,
			nullptr,
			nullptr,
			nullptr,
			nullptr,
			nullptr,
			nullptr,
			nullptr,
			nullptr,
			nullptr,
			nullptr,
			nullptr,
			nullptr,
			nullptr,
			nullptr,
			nullptr,
		};

		ObjRefBase *textures1D[8]{
			nullptr,
			nullptr,
			nullptr,
			nullptr,
			nullptr,
			nullptr,
			nullptr,
			nullptr,
		};

		ObjRefBase *textures3D[8]{
			nullptr,
			nullptr,
			nullptr,
			nullptr,
			nullptr,
			nullptr,
			nullptr,
			nullptr,
		};
	};

	struct MatrixSet {
		glm::mat4 object, view, perspective;

		MatrixSet(const glm::mat4 &pObject, const glm::mat4 &pView, const glm::mat4 &pPerspective)
			: object(pObject), view(pView), perspective(pPerspective) {}

		MatrixSet() : object(glm::identity<glm::mat4>()),
		              view(glm::identity<glm::mat4>()),
		              perspective(glm::identity<glm::mat4>()) {}
	};

	class Exception : public std::exception {
	public:
		Exception();
		explicit Exception(const char *pMessage);
		explicit Exception(const std::string &pMessage);
		Exception(const char *pMessage, int pI);
	};

	class EShaderCompile : public Exception {
	public:
		EShaderCompile();
		explicit EShaderCompile(const char *pMessage);
		explicit EShaderCompile(const std::string &pMessage);
		EShaderCompile(const char *pMessage, int pI);
	};

	class EBufferUnderflow : public Exception {
	public:
		EBufferUnderflow();
		explicit EBufferUnderflow(const char *pMessage);
		explicit EBufferUnderflow(const std::string &pMessage);
		EBufferUnderflow(const char *pMessage, int pI);
	};

	class EBufferOverflow : public Exception {
	public:
		EBufferOverflow();
		explicit EBufferOverflow(const char *pMessage);
		explicit EBufferOverflow(const std::string &pMessage);
		EBufferOverflow(const char *pMessage, int pI);
	};

	class EInvalidRef : public Exception {
	public:
		EInvalidRef();
		explicit EInvalidRef(const char *pMessage);
		explicit EInvalidRef(const std::string &pMessage);
		EInvalidRef(const char *pMessage, int pI);
	};

	class ETextureSize : public Exception {
	public:
		ETextureSize();
		explicit ETextureSize(const char *pMessage);
		explicit ETextureSize(const std::string &pMessage);
		ETextureSize(const char *pMessage, int pI);
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

		/*
		 * Almost all calls should throw an exception when passed destroyed
		 * resources, except destroy() methods, which should never throw an
		 * exception ever.
		 *
		 * Naming Conventions
		 * ------------------
		 *
		 * Methods:
		 * set: Sets a value, performs no "real" action on the backend.
		 * get: Gets a value, performs no modifying action on the backend.
		 * update: Sets some values or a larger buffer of data, performs
		 *         no "real" action on the backend.
		 * retrieve: Gets some values, a larger buffer of data, or maybe
		 *           just a get() with side effects.
		 * create: Creates a resource.
		 * destroy: Destroys a resource.
		 * reinitialize: Effectively destroys and creates a preexisting resource.
		 * activate: Activates a resource into the current context, which might be
		 *           affected by calls that do not *directly* reference the resource.
		 * setup: Usually only called once, sets up some aspect of something.
		 */

		// CLEAR

		/**
		 * Sets the color that performClear() clears the Color buffer to. This
		 * call affects only the color value and performs no action the buffer.
		 *
		 * @param pRed Red component, from 0 to 1.
		 * @param pGreen Green component, from 0 to 1.
		 * @param pBlue Blue component, from 0 to 1.
		 * @param pAlpha Alpha component, from 0 to 1. 1 is useful for many
		 *               necessities.
		 * @see performClear(options)
		 */
		virtual void setClearColor(float pRed, float pGreen, float pBlue, float pAlpha) = 0;

		/**
		 * Clears the specified buffers. The expected behavior is as follows:
		 *
		 * <ul>
		 * <li><b>Color:</b> All pixels are set to the current clear color.</li>
		 * <li><b>Depth:</b> All pixels are reset to the default value as if nothing
		 * was drawn.</li>
		 * <li><b>Stencil:</b> All pixels are reset to zero.</li>
		 * </ul>
		 *
		 * The Color and Depth buffers are cleared by default.
		 *
		 * @param pOptions Specifies the buffers that should be cleared. The stencil
		 * 				   buffer is not cleared by default.
		 * @see setClearColor(r, g, b, a)
		 * @throws std::runtime_error Possible implementation-dependent errors.
		 */
		virtual void performClear(ClearOptions pOptions = ClearOptions()) = 0; // NOLINT(google-default-arguments)
		// stfu google

		// SHADERS

		/**
		 * Creates a new shader object from the specified loaded asset.
		 *
		 * @param pAsset Asset to load from.
		 * @return Reference to the created resource.
		 * @throws EShaderCompile The shader failed to compile. (in the event that
		 * the shader needed compilation)
		 * @throws std::runtime_error Possible implementation-dependent errors.
		 */
		virtual ObjRefBase *createShader(const aether::Shader &pAsset) = 0;

		/**
		 * Destroys the provided shader resource. The object will be unusable
		 * afterwards.
		 *
		 * This method will never throw an exception.
		 *
		 * @param pObject The reference to the resource to destroy.
		 */
		virtual void destroyShader(ObjRefBase *pObject) noexcept = 0;

		// BUFFERS

		/**
		 * Creates a new buffer resource with the specified type.
		 *
		 * @param pType The type of the buffer to create. Invalid types (types
		 * that do not conform to the <code>BufferType</code> enums predefined
		 * values or possible special implementation-specific values) should
		 * throw an exception.
		 *
		 * @return A reference to the new buffer resource.
		 * @throws std::runtime_error Possible implementation-dependent errors.
		 */
		virtual ObjRefBase *createBuffer(BufferType pType) = 0;

		/**
		 * Destroys a buffer resource by reference.
		 *
		 * This method will never throw an exception.
		 *
		 * @param pObject The buffer to destroy.
		 */
		virtual void destroyBuffer(ObjRefBase *pObject) noexcept = 0;

		/**
		 * Updates the data contained within the specified buffer. The buffers
		 * pre-existing data is completely discarded, to be replaced with the
		 * data contained in the pointer. The implementation may optimise
		 * automatically.
		 *
		 * @param pObject Reference to an existing buffer.
		 * @param pData Pointer to the data to copy.
		 * @param pSize Size (in bytes) of the data to copy.
		 * @throws EInvalidRef The reference does not refer to a buffer.
		 * @throws std::runtime_error Possible implementation-dependent errors.
		 */
		virtual void updateBufferData(ObjRefBase *pObject, void *pData, size_t pSize) = 0;

		/**
		 * Updates part of the data contained within the specified buffer.
		 * The implementation may optimise automatically.
		 *
		 * <b>The buffer data must be allocated by a call to updateBufferData() without
		 * the pOffset parameter before this method can write there!</b>
		 *
		 * @param pObject Reference to an existing buffer.
		 * @param pData Pointer to the data to copy.
		 * @param pSize Size (in bytes) of the data to copy.
		 * @param pOffset Offset (in bytes) into the buffer to copy the data.
		 * @throws EInvalidRef The reference does not refer to a buffer.
		 * @throws EBufferOverflow The data pointer has too much data to fit in
		 * the buffer.
		 * @throws std::runtime_error Possible implementation-dependent errors.
		 */
		virtual void updateBufferData(ObjRefBase *pObject, void *pData, size_t pSize, size_t pOffset) = 0;

		/**
		 * Retrieves data from the buffer and stores it at the memory pointed to
		 * by the provided pointer.
		 *
		 * <b>The buffer data must be allocated by a call to updateBufferData() without
		 * the pOffset parameter before this method can read from there!</b>
		 *
		 * @param pObject Reference to an existing buffer.
		 * @param pData Pointer to the data to copy into.
		 * @param pSize Size (in bytes) of the data chunk to copy.
		 * @param pOffset Offset (in bytes) into the buffer to copy from.
		 * @throws EInvalidRef The reference does not refer to a buffer.
		 * @throws EBufferUnderflow The buffer does not have enough data left at
		 * pOffset to fill by pSize.
		 * @throws std::runtime_error Possible implementation-dependent errors.
		 */
		virtual void retrieveBufferData(ObjRefBase *pObject, void *pData, size_t pSize, size_t pOffset) = 0;

		// TEXTURE 1D

		/**
		 * Creates a new 1-dimensional texture.
		 *
		 * @return A reference to the new texture.
		 * @throws std::runtime_error Possible implementation-dependent errors.
		 */
		virtual ObjRefBase *createTexture1D() = 0;

		/**
		 * Destroys a texture-1d by reference. This method will never throw an
		 * exception.
		 *
		 * @param pObject Reference to the texture to destroy.
		 */
		virtual void destroyTexture1D(ObjRefBase *pObject) noexcept = 0;

		/**
		 * Tells the implementation how this texture should act if texture coordinates
		 * that are outside the bounds of the texture are passed.
		 *
		 * @param pObject Reference to the texture to update.
		 * @param pWrap The wrapping mode.
		 * @throws EInvalidRef The texture is not 1-dimensional, or is not a texture
		 * at all.
		 * @throws std::runtime_error The passed wrap type is BorderColor. (see WrapPropertyBorder)
		 * @throws std::runtime_error Other implementation-specific errors.
		 *
		 * @see setTexture1DWrapPropertyBorder()
		 */
		virtual void setTexture1DWrapProperty(ObjRefBase *pObject, TextureWrapType pWrap) = 0;

		/**
		 * Sets the texture wrap property to BorderColor and sets the color of the
		 * border.
		 *
		 * @param pObject Reference to the texture to update.
		 * @param pColor Color of the border.
		 * @throws EInvalidRef The texture is not 1-dimensional, or is not a texture
		 * at all.
		 * @throws std::runtime_error Other implementation-specific errors.
		 *
		 * @see setTexture1DWrapPropertyBorder()
		 */
		virtual void setTexture1DWrapPropertyBorder(ObjRefBase *pObject, glm::vec3 pColor) = 0;

		/**
		 * Sets the filter modes for the texture, which will be used when the texture
		 * is rendered smaller or larger than it is stored as. If you desire to use
		 * mipmaps, set pMin to a mipmap-specific value.
		 *
		 * @param pObject Reference to the texture to update.
		 * @param pMin Filter to use when the texture is rendered smaller than normal.
		 * @param pMag Filter to use when the texture is rendered larger than normal.
		 * @throws EInvalidRef The texture is not 1-dimensional, or is not a texture
		 * at all.
		 * @throws std::runtime_error Other implementation-specific errors.
		 */
		virtual void setTexture1DFilter(ObjRefBase *pObject, TextureMinFilter pMin, TextureMagFilter pMag) = 0;

		/**
		 * Updates a texture-1d's pixels. The data is formatted as numbers from 0-255 in
		 * the order: R, G, B, A. The implementation may decide how the pixels are actually
		 * stored in the graphics processor.
		 *
		 * @param pObject Reference to the texture to update.
		 * @param pWidth Width of the data, in pixels. / Size of the data, in bytes.
		 * @param pDataRgba Pointer to a data buffer, pWidth in size.
		 * @throws EInvalidRef The texture is not 1-dimensional, or is not a texture
		 * at all.
		 * @throws ETextureSize The texture is larger than the implementation-dependent
		 * maximum texture size.
		 * @throws std::runtime_error Other implementation-specific errors.
		 * @warning This method may cause a segmentation fault if not enough data is provided in
		 * the buffer.
		 */
		virtual void updateTexture1DData(ObjRefBase *pObject, int pWidth, const uint8_t *pDataRgba) = 0;

		/**
		 * (Re)generates the mipmap of a texture-1d. The data should be copied to the buffer
		 * before this call.
		 *
		 * @param pObject Reference to the texture to update.
		 * @throws EInvalidRef The texture is not 1-dimensional, or is not a texture
		 * at all.
		 * @throws std::runtime_error Other implementation-specific errors.
		 */
		virtual void updateTexture1DMipmap(ObjRefBase *pObject) = 0;

		// TEXTURE 2D

		/**
		 * Creates a new 2-dimensional texture.
		 *
		 * @return A reference to the new texture.
		 * @throws std::runtime_error Possible implementation-dependent errors.
		 */
		virtual ObjRefBase *createTexture2D() = 0;

		/**
		 * Destroys a texture-2d by reference. This method will never throw an
		 * exception.
		 *
		 * @param pObject Reference to the texture to destroy.
		 */
		virtual void destroyTexture2D(ObjRefBase *pObject) noexcept = 0;

		/**
		 * Tells the implementation how this texture should act if texture coordinates
		 * that are outside the bounds of the texture are passed.
		 *
		 * @param pObject Reference to the texture to update.
		 * @param pWrap The wrapping mode.
		 * @throws EInvalidRef The texture is not 2-dimensional, or is not a texture
		 * at all.
		 * @throws std::runtime_error The passed wrap type is BorderColor. (see WrapPropertyBorder)
		 * @throws std::runtime_error Other implementation-specific errors.
		 *
		 * @see setTexture2DWrapPropertyBorder()
		 */
		virtual void setTexture2DWrapProperty(ObjRefBase *pObject, TextureWrapType pWrap) = 0;

		/**
		 * Sets the texture wrap property to BorderColor and sets the color of the
		 * border.
		 *
		 * @param pObject Reference to the texture to update.
		 * @param pColor Color of the border.
		 * @throws EInvalidRef The texture is not 2-dimensional, or is not a texture
		 * at all.
		 * @throws std::runtime_error Other implementation-specific errors.
		 *
		 * @see setTexture2DWrapPropertyBorder()
		 */
		virtual void setTexture2DWrapPropertyBorder(ObjRefBase *pObject, glm::vec3 pColor) = 0;

		/**
		 * Sets the filter modes for the texture, which will be used when the texture
		 * is rendered smaller or larger than it is stored as. If you desire to use
		 * mipmaps, set pMin to a mipmap-specific value.
		 *
		 * @param pObject Reference to the texture to update.
		 * @param pMin Filter to use when the texture is rendered smaller than normal.
		 * @param pMag Filter to use when the texture is rendered larger than normal.
		 * @throws EInvalidRef The texture is not 2-dimensional, or is not a texture
		 * at all.
		 * @throws std::runtime_error Other implementation-specific errors.
		 */
		virtual void setTexture2DFilter(ObjRefBase *pObject, TextureMinFilter pMin, TextureMagFilter pMag) = 0;

		/**
		 * Updates a texture-2d's pixels. The data is converted from the passed image into
		 * a format that the implementation can use.
		 *
		 * @param pObject Reference to the texture to update.
		 * @param pImage Loaded image.
		 * @throws EInvalidRef The texture is not 2-dimensional, or is not a texture
		 * at all.
		 * @throws ETextureSize The texture is larger than the implementation-dependent
		 * maximum texture size.
		 * @throws std::runtime_error Other implementation-specific errors.
		 * @warning This method may cause a segmentation fault if not enough data is provided in
		 * the buffer.
		 */
		virtual void updateTexture2DData(ObjRefBase *pObject, const sail::image &pImage) = 0;

		/**
		 * Updates a texture-2d's pixels. The data is formatted as numbers from 0-255 in
		 * the order: R, G, B, A. The implementation may decide how the pixels are actually
		 * stored in the graphics processor.
		 *
		 * @param pObject Reference to the texture to update.
		 * @param pWidth Width of the texture, in pixels.
		 * @param pHeight Height of the texture, in pixels.
		 * @param pDataRgba Pointer to a data buffer, pWidth * pHeight in size.
		 * @throws EInvalidRef The texture is not 2-dimensional, or is not a texture
		 * at all.
		 * @throws ETextureSize The texture is larger than the implementation-dependent
		 * maximum texture size.
		 * @throws std::runtime_error Other implementation-specific errors.
		 * @warning This method may cause a segmentation fault if not enough data is provided in
		 * the buffer.
		 */
		virtual void updateTexture2DData(ObjRefBase *pObject, int pWidth, int pHeight, const uint8_t *pDataRgba) = 0;

		/**
		 * (Re)generates the mipmap of a texture-2d. The data should be copied to the buffer
		 * before this call.
		 *
		 * @param pObject Reference to the texture to update.
		 * @throws EInvalidRef The texture is not 2-dimensional, or is not a texture
		 * at all.
		 * @throws std::runtime_error Other implementation-specific errors.
		 */
		virtual void updateTexture2DMipmap(ObjRefBase *pObject) = 0;

		// TEXTURE 3D

		/**
		 * Creates a new 3-dimensional texture.
		 *
		 * @return A reference to the new texture.
		 * @throws std::runtime_error Possible implementation-dependent errors.
		 */
		virtual ObjRefBase *createTexture3D() = 0;

		/**
		 * Destroys a texture-3d by reference. This method will never throw an
		 * exception.
		 *
		 * @param pObject Reference to the texture to destroy.
		 */
		virtual void destroyTexture3D(ObjRefBase *pObject) noexcept = 0;

		/**
		 * Tells the implementation how this texture should act if texture coordinates
		 * that are outside the bounds of the texture are passed.
		 *
		 * @param pObject Reference to the texture to update.
		 * @param pWrap The wrapping mode.
		 * @throws EInvalidRef The texture is not 3-dimensional, or is not a texture
		 * at all.
		 * @throws std::runtime_error The passed wrap type is BorderColor. (see WrapPropertyBorder)
		 * @throws std::runtime_error Other implementation-specific errors.
		 *
		 * @see setTexture3DWrapPropertyBorder()
		 */
		virtual void setTexture3DWrapProperty(ObjRefBase *pObject, TextureWrapType pWrap) = 0;

		/**
		 * Sets the texture wrap property to BorderColor and sets the color of the
		 * border.
		 *
		 * @param pObject Reference to the texture to update.
		 * @param pColor Color of the border.
		 * @throws EInvalidRef The texture is not 3-dimensional, or is not a texture
		 * at all.
		 * @throws std::runtime_error Other implementation-specific errors.
		 *
		 * @see setTexture3DWrapPropertyBorder()
		 */
		virtual void setTexture3DWrapPropertyBorder(ObjRefBase *pObject, glm::vec3 pColor) = 0;

		/**
		 * Sets the filter modes for the texture, which will be used when the texture
		 * is rendered smaller or larger than it is stored as. If you desire to use
		 * mipmaps, set pMin to a mipmap-specific value.
		 *
		 * @param pObject Reference to the texture to update.
		 * @param pMin Filter to use when the texture is rendered smaller than normal.
		 * @param pMag Filter to use when the texture is rendered larger than normal.
		 * @throws EInvalidRef The texture is not 3-dimensional, or is not a texture
		 * at all.
		 * @throws std::runtime_error Other implementation-specific errors.
		 */
		virtual void setTexture3DFilter(ObjRefBase *pObject, TextureMinFilter pMin, TextureMagFilter pMag) = 0;

		/**
		 * Updates a texture-3d's pixels. The data is formatted as numbers from 0-255 in
		 * the order: R, G, B, A. The implementation may decide how the pixels are actually
		 * stored in the graphics processor.
		 *
		 * @param pObject Reference to the texture to update.
		 * @param pWidth Width of the texture, in pixels.
		 * @param pHeight Height of the texture, in pixels.
		 * @param pDepth Depth (length) of the texture, in pixels.
		 * @param pDataRgba Pointer to a data buffer, pWidth * pHeight * pDepth in size.
		 * @throws EInvalidRef The texture is not 3-dimensional, or is not a texture
		 * at all.
		 * @throws ETextureSize The texture is larger than the implementation-dependent
		 * maximum texture size. <b>Note:</b> The 3D max texture size is usually different
		 * from 1D/2D.
		 * @throws std::runtime_error Other implementation-specific errors.
		 * @warning This method may cause a segmentation fault if not enough data is provided in
		 * the buffer.
		 */
		virtual void
		updateTexture3DData(ObjRefBase *pObject, int pWidth, int pHeight, int pDepth, const uint8_t *pDataRgba) = 0;

		/**
		 * (Re)generates the mipmap of a texture-3d. The data should be copied to the buffer
		 * before this call.
		 *
		 * @param pObject Reference to the texture to update.
		 * @throws EInvalidRef The texture is not 3-dimensional, or is not a texture
		 * at all.
		 * @throws std::runtime_error Other implementation-specific errors.
		 */
		virtual void updateTexture3DMipmap(ObjRefBase *pObject) = 0;

		// FRAMEBUFFERS

		/**
		 * Creates an new framebuffer object. The resource is usable immediately after
		 * construction.
		 *
		 * @param pWidth Width of the framebuffer, in <b>pixels</b>.
		 * @param pHeight Height of the framebuffer, in <b>pixels</b>.
		 * @return Reference to the newly created resource.
		 * @throws std::runtime_error Possible implementation-dependent errors.
		 */
		virtual ObjRefBase *createFramebuffer(int pWidth, int pHeight) = 0;

		/**
		 * <p> Destroys and creates the specified framebuffer with the new size.
		 * If the framebuffer is bound, the default framebuffer will be rebound.
		 * The default framebuffer cannot be passed to this method. </p>
		 *
		 * <p> This method exists because a pointer may be passed to multiple
		 * things already, plus forcing them to check if the owner wants to create
		 * a new framebuffer and automatically updating it's own pointer does not
		 * sound like a very good idea. </p>
		 *
		 * @param pObject Reference to the existing framebuffer resource.
		 * @param pWidth Width of the new framebuffer, in <b>pixels</b>.
		 * @param pHeight Height of the new framebuffer, in <b>pixels</b>
		 * @throws EInvalidRef The texture is not 3-dimensional, or is not a texture
		 * at all.
		 * @throws std::runtime_error Possible implementation-dependent errors.
		 * @warning Somewhat experimental, and possibly not actually viable for
		 * some graphics APIs to manage in an effective way.
		 */
		virtual void reinitializeFramebuffer(ObjRefBase *pObject, int pWidth, int pHeight) = 0;

		/**
		 * Destroys the passed resource by reference. This method will never throw
		 * an exception.
		 *
		 * @param pObject The framebuffer object to destroy.
		 */
		virtual void destroyFramebuffer(ObjRefBase *pObject) noexcept = 0;

		/**
		 * Gets the color texture from the specified framebuffer resource.
		 *
		 * @param pObject Reference to the framebuffer to get the texture from.
		 * @return Texture-2d reference when present; nullptr if the framebuffer
		 * implementation does not create color textures, or uses a non-texture
		 * compatible implementation.
		 * @throws EInvalidRef The reference does not refer to a framebuffer.
		 * @throws EInvalidRef The default framebuffer was passed.
		 * @throws std::runtime_error Possible implementation-dependent errors.
		 */
		virtual ObjRefBase *getFramebufferColorTexture2D(ObjRefBase *pObject) = 0;

		/**
		 * Gets the depth and stencil texture from the specified framebuffer resource.
		 * Both components are combined into one.
		 *
		 * @todo Unsupported: Depth-stencil textures.
		 * @param pObject Reference to the framebuffer to get the texture from.
		 * @return Texture-2d reference when present; nullptr if the framebuffer
		 * implementation does not create depth-stencil textures, or uses a non-texture
		 * compatible implementation. The texture-2d returned should use red for depth
		 * and green for stencil.
		 * @throws EInvalidRef The reference does not refer to a framebuffer.
		 * @throws EInvalidRef The default framebuffer was passed.
		 * @throws std::runtime_error Possible implementation-dependent errors.
		 */
		virtual ObjRefBase *getFramebufferDepthStencilTexture2D(ObjRefBase *pObject) = 0;

		/**
		 * Copies the data from one framebuffer to another. This expects that pTarget
		 * is equal to or larger than pSource in size.
		 *
		 * @param pSource The source of the copy. The default framebuffer <b>cannot</b> be passed here.
		 * @param pTarget The target of the copy. The default framebuffer can be passed here.
		 * @throws EInvalidRef The reference does not refer to a framebuffer.
		 * @throws EInvalidRef The default framebuffer was passed to pSource.
		 * @throws std::runtime_error Possible implementation-dependent errors.
		 */
		virtual void performBlitFramebuffer(ObjRefBase *pSource, ObjRefBase *pTarget) = 0;

		/**
		 * Copys part of the data from one framebuffer to another. This expects that
		 * pSource and pTarget are at least (pStartX+pWidth)x(pStartY+pHeight) in size.
		 *
		 * @param pSource The source of the copy. The default framebuffer <b>cannot</b> be passed here.
		 * @param pTarget The target of the copy. The default framebuffer can be passed here.
		 * @param pSourceStartX The X value (in pixels) to copy from.
		 * @param pSourceStartY The Y value (in pixels) to copy from.
		 * @param pTargetStartX The X value (in pixels) to copy to.
		 * @param pTargetStartY The Y value (in pixels) to copy to.
		 * @param pWidth The width of the segment to copy, in pixels.
		 * @param pHeight The height of the segment to copy, in pixels.
		 * @throws EInvalidRef The reference does not refer to a framebuffer.
		 * @throws EInvalidRef The default framebuffer was passed to pSource.
		 * @throws std::runtime_error Possible implementation-dependent errors.
		 */
		virtual void
		performBlitFramebuffer(ObjRefBase *pSource, ObjRefBase *pTarget, int pSourceStartX, int pSourceStartY,
		                       int pTargetStartX, int pTargetStartY, int pWidth, int pHeight) = 0;

		/**
		 * Gets a reference that can be passed to represent the default framebuffer
		 * to other methods. The default framebuffer is the one that is actually
		 * rendered when the frame ends.
		 *
		 * @return Special framebuffer reference. It cannot be used in some usual
		 * framebuffer methods.
		 */
		virtual ObjRefBase *getDefaultFramebuffer() = 0;

		/**
		 * "Activates" the specified framebuffer. Subsequent calls to perform*() methods
		 * will affect this framebuffer.
		 *
		 * @param pObject The reference to the framebuffer. This can be the default
		 * framebuffer.
		 * @throws EInvalidRef The reference does not refer to a framebuffer or the default
		 * framebuffer.
		 * @throws std::runtime_error Possible implementation-dependent errors.
		 */
		virtual void activateFramebuffer(ObjRefBase *pObject) = 0;

		// DRAW OBJECTS

		/**
		 * Creates a new draw object from a set of options. This resource can then
		 * be used in performDraw() to draw vertices to the currently bound
		 * framebuffer.
		 *
		 * @param pOptions Options to create the draw object. This contains many
		 * required fields, so just passing an empty struct is not going to do
		 * any good.
		 * @return Reference to the newly created resource.
		 * @throws EInvalidRef The vertex buffer, index buffer, and shader values
		 * contain an invalid reference. Also thrown if a texture is not nullptr
		 * but the pointer is not a valid reference.
		 * @throws std::runtime_error A field of the struct has an invalid value.
		 * @throws std::runtime_error Possible implementation-dependent errors.
		 */
		virtual ObjRefBase *createDrawObject(const DrawObjectOptions &pOptions) = 0;

		/**
		 * Destroys the passed draw object by reference. This method will never
		 * throw an exception.
		 *
		 * @param pObject Resource to destroy.
		 */
		virtual void destroyDrawObject(ObjRefBase *pObject) noexcept = 0;

		/**
		 * Utilizes the passed draw object and the set of matrices to draw an
		 * object to the currently bound framebuffer.
		 *
		 * @param pDrawObject The object to draw.
		 * @param pMatrices Transformation matrices. Technically not required
		 * for every shader, but the built-in ones won't get very far without
		 * them. Will be applied in a certain order, so the names are important
		 * indeed.
		 */
		virtual void performDraw(ObjRefBase *pDrawObject, const MatrixSet &pMatrices) = 0;

		// WINDOW

		/**
		 * Performs some setup before a window is created to hint to GLFW about
		 * what this implementation requires of it.
		 */
		virtual void setupWindowHints() = 0;

		/**
		 * Called after the window is created and bound. Used to initialize the
		 * rest of the implementation that is dependent on a context existing.
		 *
		 * @throws std::runtime_error Possible implementation-dependent errors.
		 */
		virtual void setupWindowPostCreate() = 0;

		/**
		 * Called when the window is resized to resize the actual viewport.
		 *
		 * @param pWidth Width of the new viewport, in pixels.
		 * @param pHeight Height of the new viewport, in pixels.
		 * @throws std::runtime_error Possible implementation-dependent errors.
		 */
		virtual void updateViewportSize(int pWidth, int pHeight) = 0;

		/**
		 * Swaps the buffers and "ends" the frame. Should be called only at
		 * the end of a frame, after everything else has been done.
		 *
		 * @param pWindow Window to swap to. Should be the currently bound
		 * window.
		 */
		virtual void performFinishFrame(Window *pWindow) = 0;
	};

}// namespace aurora

#endif// AURORA_IMPLEMENTATION_H
