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
		Texture1D0,
		Texture1D1,
		Texture1D2,
		Texture1D3,
		Texture1D4,
		Texture1D5,
		Texture1D6,
		Texture1D7,
		Texture3D0,
		Texture3D1,
		Texture3D2,
		Texture3D3,
		Texture3D4,
		Texture3D5,
		Texture3D6,
		Texture3D7,
	};

	enum class IndexBufferItemType {
		UnsignedInt,
		UnsignedShort,
		UnsignedByte,
	};
}

#endif //AURORA_ENUMS_H
