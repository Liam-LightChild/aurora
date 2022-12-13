/*
 * This file is part of Aurora Game Engine.
 * https://github.com/liam-lightchild/aurora
 */

#include "shaders.h"
#include "aurora/graphics/implementation.h"

namespace aurora::shaders {
	const VertexArrangement testA {
		A_VNODE(v_position, Float, 3),
		A_VNODE(v_color, Float, 3),
		A_VNODE(v_texCoords, Float, 2)
	};
}
