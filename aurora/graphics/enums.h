//
// Created by liam on 12/8/22.
//

#ifndef AURORA_ENUMS_H
#define AURORA_ENUMS_H

namespace aurora {
	enum BufferType {
		VertexBuffer,
		IndexBuffer,
	};

	enum class TextureWrapType {
		Repeat,
		ClampToEdge,
		BorderColor,
	};

	enum class TextureMinFilter {
		Nearest,
		Linear,
		NearestMipmap,
		LinearMipmap
	};

	enum class TextureMagFilter {
		Nearest,
		Linear
	};

	enum class VertexInputType {
		Float,
		Int,
		Boolean,
	};

	enum class ShaderUniformType {
		Texture0,
		Texture1,
		Texture2,
		Texture3,
		Texture4,
		Texture5,
		Texture6,
		Texture7,
		Texture8,
		Texture9,
		Texture10,
		Texture11,
		Texture12,
		Texture13,
		Texture14,
		Texture15,
		Texture16,
		Texture17,
		Texture18,
		Texture19,
		Texture20,
		Texture21,
		Texture22,
		Texture23,
		Texture24,
		Texture25,
		Texture26,
		Texture27,
		Texture28,
		Texture29,
		Texture30,
		Texture31,
	};

	enum class IndexBufferItemType {
		UnsignedInt,
		UnsignedShort,
		UnsignedByte,
	};
}

#endif //AURORA_ENUMS_H
