//
// Created by liam on 12/2/22.
//

#pragma clang diagnostic push
#pragma ide diagnostic ignored "OCInconsistentNamingInspection"

#ifndef AURORA_OPENGL_IMPL_NODE_H
#define AURORA_OPENGL_IMPL_NODE_H

#include "aurora/graphics/implementation.h"
#include "opengl_impl.h"

namespace aurora {
	bool checkOpenGLContext(int pMajor, int pMinor);

	template<int major, int minor>
	class OpenGLImplementationNode : public Implementation::Node {
	public:
		~OpenGLImplementationNode() override = default;

		int getPriority() override {
			return major * 10 + minor;
		}

		bool isValid() override {
			return checkOpenGLContext(major, minor);
		}

		Implementation *create() override {
			return new OpenGLImplementation<major, minor>;
		}
	};
}// namespace aurora

#endif// AURORA_OPENGL_IMPL_NODE_H

#pragma clang diagnostic pop
