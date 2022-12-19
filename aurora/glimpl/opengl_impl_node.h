/*
 * This file is part of Aurora Game Engine.
 * https://github.com/liam-lightchild/aurora
 */

#ifndef AURORA_OPENGL_IMPL_NODE_H
#define AURORA_OPENGL_IMPL_NODE_H

#include "../graphics/implementation.h"
#include "opengl_impl.h"

namespace aurora {
	bool checkOpenGlContext(int pMajor, int pMinor);

	template<int major, int minor>
	class OpenGlImplementationNode : public Implementation::Node {
	public:
		~OpenGlImplementationNode() override = default;

		int getPriority() override {
			return major * 10 + minor;
		}

		bool isValid() override {
			return checkOpenGlContext(major, minor);
		}

		Implementation *create() override {
			return new OpenGlImplementation<major, minor>;
		}
	};
}// namespace aurora

#endif// AURORA_OPENGL_IMPL_NODE_H
