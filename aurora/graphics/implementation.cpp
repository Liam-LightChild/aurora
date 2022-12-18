/*
 * This file is part of Aurora Game Engine.
 * https://github.com/liam-lightchild/aurora
 */

#include "implementation.h"

namespace aurora {
	Exception::Exception() = default;

	Exception::Exception(const char *const pMessage) : exception(pMessage) {}

	Exception::Exception(const std::string &pMessage) : Exception(pMessage.c_str()) {}

	Exception::Exception(const char *const pMessage, int pI) : exception(pMessage, pI) {}

	EInvalidRef::EInvalidRef() = default;

	EInvalidRef::EInvalidRef(const char *pMessage) : Exception(pMessage) {}

	EInvalidRef::EInvalidRef(const std::string &pMessage) : Exception(pMessage) {}

	EInvalidRef::EInvalidRef(const char *pMessage, int pI) : Exception(pMessage, pI) {}

	EShaderCompile::EShaderCompile() = default;

	EShaderCompile::EShaderCompile(const char *pMessage) : Exception(pMessage) {}

	EShaderCompile::EShaderCompile(const std::string &pMessage) : Exception(pMessage) {}

	EShaderCompile::EShaderCompile(const char *pMessage, int pI) : Exception(pMessage, pI) {}

	EBufferUnderflow::EBufferUnderflow() = default;

	EBufferUnderflow::EBufferUnderflow(const char *pMessage) : Exception(pMessage) {}

	EBufferUnderflow::EBufferUnderflow(const std::string &pMessage) : Exception(pMessage) {}

	EBufferUnderflow::EBufferUnderflow(const char *pMessage, int pI) : Exception(pMessage, pI) {}

	EBufferOverflow::EBufferOverflow() = default;

	EBufferOverflow::EBufferOverflow(const char *pMessage) : Exception(pMessage) {}

	EBufferOverflow::EBufferOverflow(const std::string &pMessage) : Exception(pMessage) {}

	EBufferOverflow::EBufferOverflow(const char *pMessage, int pI) : Exception(pMessage, pI) {}

	ETextureSize::ETextureSize() = default;

	ETextureSize::ETextureSize(const char *pMessage) : Exception(pMessage) {}

	ETextureSize::ETextureSize(const std::string &pMessage) : Exception(pMessage) {}

	ETextureSize::ETextureSize(const char *pMessage, int pI) : Exception(pMessage, pI) {}
}// namespace aurora