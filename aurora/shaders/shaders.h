#ifndef AURORA_SHADERS_H
#define AURORA_SHADERS_H


#include "../aether/aether.h"
#include "../graphics/implementation.h"

/*
 * This file defines the variables in which the built-in shaders are stored.
 * See the shaders.cmake file for how they should be referenced.
 */

namespace aurora::shaders {
	extern const aether::Shader test;
	extern const VertexArrangement testA;

	struct TestVertex {
		glm::vec3 position;
		glm::vec3 color;
		glm::vec2 textureCoords;
	};
}

#endif //AURORA_SHADERS_H
