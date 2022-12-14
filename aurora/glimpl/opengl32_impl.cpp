/*
 * This file is part of Aurora Game Engine.
 * https://github.com/liam-lightchild/aurora
 */

#include <GL/glew.h>
#include <sail-c++/sail-c++.h>
#include "opengl_impl.h"
#include <GLFW/glfw3.h>
#include <boost/log/trivial.hpp>
#include <memory>
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>

namespace aurora {
	std::unordered_map<std::string, ShaderUniformType> uniformTypes {
		{"Texture0", ShaderUniformType::Texture0},
		{"Texture1", ShaderUniformType::Texture1},
		{"Texture2", ShaderUniformType::Texture2},
		{"Texture3", ShaderUniformType::Texture3},
		{"Texture4", ShaderUniformType::Texture4},
		{"Texture5", ShaderUniformType::Texture5},
		{"Texture6", ShaderUniformType::Texture6},
		{"Texture7", ShaderUniformType::Texture7},
		{"Texture8", ShaderUniformType::Texture8},
		{"Texture9", ShaderUniformType::Texture9},
		{"Texture10", ShaderUniformType::Texture10},
		{"Texture11", ShaderUniformType::Texture11},
		{"Texture12", ShaderUniformType::Texture12},
		{"Texture13", ShaderUniformType::Texture13},
		{"Texture14", ShaderUniformType::Texture14},
		{"Texture15", ShaderUniformType::Texture15},
		{"Texture1D0", ShaderUniformType::Texture1D0},
		{"Texture1D1", ShaderUniformType::Texture1D1},
		{"Texture1D2", ShaderUniformType::Texture1D2},
		{"Texture1D3", ShaderUniformType::Texture1D3},
		{"Texture1D4", ShaderUniformType::Texture1D4},
		{"Texture1D5", ShaderUniformType::Texture1D5},
		{"Texture1D6", ShaderUniformType::Texture1D6},
		{"Texture1D7", ShaderUniformType::Texture1D7},
		{"Texture3D0", ShaderUniformType::Texture3D0},
		{"Texture3D1", ShaderUniformType::Texture3D1},
		{"Texture3D2", ShaderUniformType::Texture3D2},
		{"Texture3D3", ShaderUniformType::Texture3D3},
		{"Texture3D4", ShaderUniformType::Texture3D4},
		{"Texture3D5", ShaderUniformType::Texture3D5},
		{"Texture3D6", ShaderUniformType::Texture3D6},
		{"Texture3D7", ShaderUniformType::Texture3D7},
		{"MatrixObject", ShaderUniformType::MatrixObject},
		{"MatrixView", ShaderUniformType::MatrixView},
		{"MatrixPerspective", ShaderUniformType::MatrixPerspective}
	};

	ObjRefBase *OpenGLImplementation<3, 2>::createShader(const aether::Shader &pShader) {
		int program = glCreateProgram();

		for(const auto &item: pShader.parts) {
			GLenum stage;
			switch(item.stage) {
				case aether::Shader::Vertex: stage = GL_VERTEX_SHADER;
					break;
				case aether::Shader::Pixel: stage = GL_FRAGMENT_SHADER;
					break;
			}

			auto shader = glCreateShader(stage);

			const char *strs[2] = {
				"#version 150 core\n\n",
				item.source.c_str()
			};

			glShaderSource(shader, 2, strs, nullptr);
			glCompileShader(shader);

			int compileStatus;
			glGetShaderiv(shader, GL_COMPILE_STATUS, &compileStatus);

			char *log = new char[65536];
			int length;
			glGetShaderInfoLog(shader, 65536, &length, log);

			if(length > 0 && !compileStatus) { throw std::runtime_error(std::string(log, length)); }
			else if(length > 0) BOOST_LOG_TRIVIAL(warning) << std::string(log, length);

			glAttachShader(program, shader);
		}

		glLinkProgram(program);

		int linkStatus;
		glGetProgramiv(program, GL_LINK_STATUS, &linkStatus);

		char *log = new char[65536];
		int length;
		glGetProgramInfoLog(program, 65536, &length, log);

		if(length > 0 && !linkStatus) { throw std::runtime_error(std::string(log, length)); }
		else if(length > 0) BOOST_LOG_TRIVIAL(warning) << std::string(log, length);

		auto ref = new ShaderReference(program);

		for(const auto &item: pShader.uniforms) {
			ref->uniforms[item.name] = uniformTypes.at(item.purpose);
		}

		return ref;
	}

	void OpenGLImplementation<3, 2>::destroyShader(ObjRefBase *pObject) {
		glDeleteProgram(dynamic_cast<Reference *>(pObject)->resource);
	}

	void OpenGLImplementation<3, 2>::updateViewportSize(int pWidth, int pHeight) {
		glViewport(0, 0, pWidth, pHeight);
	}

	void OpenGLImplementation<3, 2>::setupWindowHints() {
		glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
		glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, true);
		glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
		glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 2);
	}

	void OpenGLImplementation<3, 2>::setupWindowPostCreate() {
		glewExperimental = true;
		glewInit();

		glEnable(GL_CULL_FACE);
		glEnable(GL_DEPTH_TEST);
	}

	void OpenGLImplementation<3, 2>::performFinishFrame(Window *pWindow) {
		glfwSwapBuffers(pWindow->getGlfw());
	}

	void OpenGLImplementation<3, 2>::setClearColor(float pRed, float pGreen, float pBlue, float pAlpha) {
		glClearColor(pRed, pGreen, pBlue, pAlpha);
	}

	void OpenGLImplementation<3, 2>::performClear(ClearOptions pOptions) {
		GLenum bufs = 0;
		if(pOptions.color) { bufs |= GL_COLOR_BUFFER_BIT; }
		if(pOptions.depth) { bufs |= GL_DEPTH_BUFFER_BIT; }
		if(pOptions.stencil) { bufs |= GL_STENCIL_BUFFER_BIT; }
		glClear(bufs);
	}

	ObjRefBase *OpenGLImplementation<3, 2>::createBuffer(BufferType pType) {
		GLuint buf;
		glGenBuffers(1, &buf);
		return new BufferReference(buf, pType);
	}

	void OpenGLImplementation<3, 2>::destroyBuffer(ObjRefBase *pObject) {
		auto ref = dynamic_cast<BufferReference*>(pObject);
		if(ref == nullptr) throw std::runtime_error("invalid buffer reference");
		glDeleteBuffers(1, &ref->resource);
		delete ref;
	}

	void OpenGLImplementation<3, 2>::updateBufferData(ObjRefBase *pObject, void *pData, size_t pSize) {
		auto ref = dynamic_cast<BufferReference*>(pObject);
		if(ref == nullptr) throw std::runtime_error("invalid buffer reference");
		glBindBuffer(GL_ARRAY_BUFFER, ref->resource);
		glBufferData(GL_ARRAY_BUFFER, static_cast<GLsizeiptr>(pSize), pData, GL_STATIC_DRAW);
	}

	void OpenGLImplementation<3, 2>::updateBufferData(ObjRefBase *pObject, void *pData, size_t pSize, size_t pOffset) {
		auto ref = dynamic_cast<BufferReference*>(pObject);
		if(ref == nullptr) throw std::runtime_error("invalid buffer reference");
		glBindBuffer(GL_ARRAY_BUFFER, ref->resource);
		glBufferSubData(GL_ARRAY_BUFFER, static_cast<GLsizeiptr>(pOffset), static_cast<GLsizeiptr>(pSize), pData);
	}

	void
	OpenGLImplementation<3, 2>::retrieveBufferData(ObjRefBase *pObject, void *pData, size_t pSize, size_t pOffset) {
		auto ref = dynamic_cast<BufferReference*>(pObject);
		if(ref == nullptr) throw std::runtime_error("invalid buffer reference");
		glBindBuffer(GL_ARRAY_BUFFER, ref->resource);
		glGetBufferSubData(GL_ARRAY_BUFFER, static_cast<GLsizeiptr>(pOffset), static_cast<GLsizeiptr>(pSize), pData);
	}

	ObjRefBase *OpenGLImplementation<3, 2>::createTexture2D() {
		uint32_t tex;
		glGenTextures(1, &tex);
		return new Reference(tex);
	}

	void OpenGLImplementation<3, 2>::destroyTexture2D(ObjRefBase *pObject) {
		auto ref = dynamic_cast<Reference*>(pObject);
		if(ref == nullptr) throw std::runtime_error("invalid texture reference");

		glDeleteTextures(1, &ref->resource);
		delete ref;
	}

	void OpenGLImplementation<3, 2>::setTexture2DWrapProperty(ObjRefBase *pObject, TextureWrapType pWrap) {
		auto ref = dynamic_cast<Reference*>(pObject);
		if(ref == nullptr) throw std::runtime_error("invalid texture reference");

		GLenum e;
		switch(pWrap) {
			case TextureWrapType::Repeat: e = GL_REPEAT; break;
			case TextureWrapType::ClampToEdge: e = GL_CLAMP_TO_EDGE; break;
			case TextureWrapType::BorderColor: throw std::runtime_error("cannot use this method to set border color");
		}

		glBindTexture(GL_TEXTURE_2D, ref->resource);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, e);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, e);
	}

	void OpenGLImplementation<3, 2>::setTexture2DWrapPropertyBorder(ObjRefBase *pObject, glm::vec3 pColor) {
		auto ref = dynamic_cast<Reference*>(pObject);
		if(ref == nullptr) throw std::runtime_error("invalid texture reference");

		glBindTexture(GL_TEXTURE_2D, ref->resource);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);
		float color[4] = {pColor.r, pColor.g, pColor.b, 1};
		glTexParameterfv(GL_TEXTURE_2D, GL_TEXTURE_BORDER_COLOR, color);
	}

	void
	OpenGLImplementation<3, 2>::setTexture2DFilter(ObjRefBase *pObject, TextureMinFilter pMin, TextureMagFilter pMag) {
		auto ref = dynamic_cast<Reference*>(pObject);
		if(ref == nullptr) throw std::runtime_error("invalid texture reference");

		GLenum min, mag;

		switch(pMin) {
			case TextureMinFilter::Nearest: min = GL_NEAREST; break;
			case TextureMinFilter::Linear: min = GL_LINEAR; break;
			case TextureMinFilter::NearestMipmap: min = GL_NEAREST_MIPMAP_NEAREST; break;
			case TextureMinFilter::LinearMipmap: min = GL_LINEAR_MIPMAP_LINEAR; break;
		}

		switch(pMag) {
			case TextureMagFilter::Nearest: mag = GL_NEAREST; break;
			case TextureMagFilter::Linear: mag = GL_LINEAR; break;
		}

		glBindTexture(GL_TEXTURE_2D, ref->resource);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, min);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, mag);
	}

	void OpenGLImplementation<3, 2>::updateTexture2DData(ObjRefBase *pObject, const sail::image &pImage) {
		auto ref = dynamic_cast<Reference*>(pObject);
		if(ref == nullptr) throw std::runtime_error("invalid texture reference");

		auto img = pImage.convert_to(SAIL_PIXEL_FORMAT_BPP32_RGBA);
		glBindTexture(GL_TEXTURE_2D, ref->resource);
		glTexImage2D(GL_TEXTURE_2D,
					 0,
					 GL_RGBA8,
					 static_cast<GLsizei>(img.width()),
					 static_cast<GLsizei>(img.height()),
					 0,
					 GL_RGBA,
					 GL_UNSIGNED_BYTE,
					 img.pixels());
	}

	void OpenGLImplementation<3, 2>::updateTexture2DMipmap(ObjRefBase *pObject) {
		auto ref = dynamic_cast<Reference*>(pObject);
		if(ref == nullptr) throw std::runtime_error("invalid texture reference");

		glBindTexture(GL_TEXTURE_2D, ref->resource);
		glGenerateMipmap(GL_TEXTURE_2D);
	}

	ObjRefBase *OpenGLImplementation<3, 2>::createDrawObject(const DrawObjectOptions &pOptions) {
		uint32_t vao;
		glCreateVertexArrays(1, &vao);
		glBindVertexArray(vao);
		glBindBuffer(GL_ARRAY_BUFFER, dynamic_cast<Reference*>(pOptions.vertexBuffer)->resource);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, dynamic_cast<Reference*>(pOptions.indexBuffer)->resource);

		auto prog = dynamic_cast<Reference*>(pOptions.shader)->resource;

		int stride = 0, offset = 0;

		for(auto &a : pOptions.arrangement) {
			int size;

			switch(a.type) {
				case VertexInputType::Float:
					size = sizeof(float);
					break;
				case VertexInputType::Int:
					size = sizeof(int);
					break;
				case VertexInputType::Boolean:
					size = sizeof(bool);
					break;
			}

			stride += size * a.count;
		}

		for(auto &a : pOptions.arrangement) {
			int size;
			GLenum e;

			switch(a.type) {
				case VertexInputType::Float:
					size = sizeof(float);
					e = GL_FLOAT;
					break;
				case VertexInputType::Int:
					size = sizeof(int);
					e = GL_INT;
					break;
				case VertexInputType::Boolean:
					size = sizeof(bool);
					e = GL_BOOL;
					break;
			}

			auto attr = glGetAttribLocation(prog, a.name.c_str());
			glVertexAttribPointer(attr, a.count, e, false, stride, reinterpret_cast<void*>(offset));
			glEnableVertexAttribArray(attr);
			offset += size * a.count;
		}

		for(int i = 0; i < 16; ++i) {
			auto item = pOptions.textures[i];
			if(item == nullptr) continue;

			auto dyn = dynamic_cast<Reference*>(item);
			if(dyn == nullptr) throw std::runtime_error("invalid texture reference");

			glActiveTexture(GL_TEXTURE0 + i);
			glBindTexture(GL_TEXTURE_2D, dyn->resource);
		}

		for(int i = 0; i < 8; ++i) {
			auto item = pOptions.textures1D[i];
			if(item == nullptr) continue;

			auto dyn = dynamic_cast<Reference*>(item);
			if(dyn == nullptr) throw std::runtime_error("invalid texture reference");

			glActiveTexture(GL_TEXTURE16 + i);
			glBindTexture(GL_TEXTURE_1D, dyn->resource);
		}

		for(int i = 0; i < 8; ++i) {
			auto item = pOptions.textures3D[i];
			if(item == nullptr) continue;

			auto dyn = dynamic_cast<Reference*>(item);
			if(dyn == nullptr) throw std::runtime_error("invalid texture reference");

			glActiveTexture(GL_TEXTURE24 + i);
			glBindTexture(GL_TEXTURE_3D, dyn->resource);
		}

		glBindVertexArray(0);
		return new DrawObjectReference(vao, pOptions);
	}

	void OpenGLImplementation<3, 2>::destroyDrawObject(ObjRefBase *pObject) {
		auto ref = dynamic_cast<DrawObjectReference*>(pObject);
		if(ref == nullptr) throw std::runtime_error("invalid draw object reference");

		glDeleteVertexArrays(1, &ref->resource);
	}

	void OpenGLImplementation<3, 2>::performDraw(ObjRefBase *pDrawObject, const MatrixSet &pMatrices) {
		auto ref = dynamic_cast<DrawObjectReference*>(pDrawObject);
		if(ref == nullptr) throw std::runtime_error("invalid draw object reference");

		glBindVertexArray(ref->resource);
		auto sh = ref->shader;
		glUseProgram(sh->resource);

		for(const auto &item: sh->uniforms) {
			auto loc = glGetUniformLocation(sh->resource, item.first.c_str());

			switch(item.second) {
				case ShaderUniformType::Texture0: glUniform1i(loc, 0); break;
				case ShaderUniformType::Texture1: glUniform1i(loc, 1); break;
				case ShaderUniformType::Texture2: glUniform1i(loc, 2); break;
				case ShaderUniformType::Texture3: glUniform1i(loc, 3); break;
				case ShaderUniformType::Texture4: glUniform1i(loc, 4); break;
				case ShaderUniformType::Texture5: glUniform1i(loc, 5); break;
				case ShaderUniformType::Texture6: glUniform1i(loc, 6); break;
				case ShaderUniformType::Texture7: glUniform1i(loc, 7); break;
				case ShaderUniformType::Texture8: glUniform1i(loc, 8); break;
				case ShaderUniformType::Texture9: glUniform1i(loc, 9); break;
				case ShaderUniformType::Texture10: glUniform1i(loc, 10); break;
				case ShaderUniformType::Texture11: glUniform1i(loc, 11); break;
				case ShaderUniformType::Texture12: glUniform1i(loc, 12); break;
				case ShaderUniformType::Texture13: glUniform1i(loc, 13); break;
				case ShaderUniformType::Texture14: glUniform1i(loc, 14); break;
				case ShaderUniformType::Texture15: glUniform1i(loc, 15); break;
				case ShaderUniformType::Texture1D0: glUniform1i(loc, 16); break;
				case ShaderUniformType::Texture1D1: glUniform1i(loc, 17); break;
				case ShaderUniformType::Texture1D2: glUniform1i(loc, 18); break;
				case ShaderUniformType::Texture1D3: glUniform1i(loc, 19); break;
				case ShaderUniformType::Texture1D4: glUniform1i(loc, 20); break;
				case ShaderUniformType::Texture1D5: glUniform1i(loc, 21); break;
				case ShaderUniformType::Texture1D6: glUniform1i(loc, 22); break;
				case ShaderUniformType::Texture1D7: glUniform1i(loc, 23); break;
				case ShaderUniformType::Texture3D0: glUniform1i(loc, 24); break;
				case ShaderUniformType::Texture3D1: glUniform1i(loc, 25); break;
				case ShaderUniformType::Texture3D2: glUniform1i(loc, 26); break;
				case ShaderUniformType::Texture3D3: glUniform1i(loc, 27); break;
				case ShaderUniformType::Texture3D4: glUniform1i(loc, 28); break;
				case ShaderUniformType::Texture3D5: glUniform1i(loc, 29); break;
				case ShaderUniformType::Texture3D6: glUniform1i(loc, 30); break;
				case ShaderUniformType::Texture3D7: glUniform1i(loc, 31); break;
				case ShaderUniformType::MatrixObject: glUniformMatrix4fv(loc, 1, false, glm::value_ptr(pMatrices.object)); break;
				case ShaderUniformType::MatrixView: glUniformMatrix4fv(loc, 1, false, glm::value_ptr(pMatrices.view)); break;
				case ShaderUniformType::MatrixPerspective: glUniformMatrix4fv(loc, 1, false, glm::value_ptr(pMatrices.perspective)); break;
			}
		}

		GLenum eFmt;
		switch(ref->indexBufferItemType) {
			case IndexBufferItemType::UnsignedInt: eFmt = GL_UNSIGNED_INT; break;
			case IndexBufferItemType::UnsignedByte: eFmt = GL_UNSIGNED_BYTE; break;
			case IndexBufferItemType::UnsignedShort: eFmt = GL_UNSIGNED_SHORT; break;
		}

		glDrawElements(GL_TRIANGLES, static_cast<GLsizei>(ref->vertexCount), eFmt, nullptr);
		glBindVertexArray(0);
	}

	ObjRefBase *OpenGLImplementation<3, 2>::createTexture1D() {
		uint32_t tex;
		glGenTextures(1, &tex);
		return new Reference(tex);
	}

	void OpenGLImplementation<3, 2>::destroyTexture1D(ObjRefBase *pObject) {
		auto ref = dynamic_cast<Reference*>(pObject);
		if(ref == nullptr) throw std::runtime_error("invalid texture reference");

		glDeleteTextures(1, &ref->resource);
		delete ref;
	}

	void OpenGLImplementation<3, 2>::setTexture1DWrapProperty(ObjRefBase *pObject, TextureWrapType pWrap) {
		auto ref = dynamic_cast<Reference*>(pObject);
		if(ref == nullptr) throw std::runtime_error("invalid texture reference");

		GLenum e;
		switch(pWrap) {
			case TextureWrapType::Repeat: e = GL_REPEAT; break;
			case TextureWrapType::ClampToEdge: e = GL_CLAMP_TO_EDGE; break;
			case TextureWrapType::BorderColor: throw std::runtime_error("cannot use this method to set border color");
		}

		glBindTexture(GL_TEXTURE_1D, ref->resource);
		glTexParameteri(GL_TEXTURE_1D, GL_TEXTURE_WRAP_S, e);
	}

	void OpenGLImplementation<3, 2>::setTexture1DWrapPropertyBorder(ObjRefBase *pObject, glm::vec3 pColor) {
		auto ref = dynamic_cast<Reference*>(pObject);
		if(ref == nullptr) throw std::runtime_error("invalid texture reference");

		glBindTexture(GL_TEXTURE_1D, ref->resource);
		glTexParameteri(GL_TEXTURE_1D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
		float color[4] = {pColor.r, pColor.g, pColor.b, 1};
		glTexParameterfv(GL_TEXTURE_1D, GL_TEXTURE_BORDER_COLOR, color);
	}

	void
	OpenGLImplementation<3, 2>::setTexture1DFilter(ObjRefBase *pObject, TextureMinFilter pMin, TextureMagFilter pMag) {
		auto ref = dynamic_cast<Reference*>(pObject);
		if(ref == nullptr) throw std::runtime_error("invalid texture reference");

		GLenum min, mag;

		switch(pMin) {
			case TextureMinFilter::Nearest: min = GL_NEAREST; break;
			case TextureMinFilter::Linear: min = GL_LINEAR; break;
			case TextureMinFilter::NearestMipmap: min = GL_NEAREST_MIPMAP_NEAREST; break;
			case TextureMinFilter::LinearMipmap: min = GL_LINEAR_MIPMAP_LINEAR; break;
		}

		switch(pMag) {
			case TextureMagFilter::Nearest: mag = GL_NEAREST; break;
			case TextureMagFilter::Linear: mag = GL_LINEAR; break;
		}

		glBindTexture(GL_TEXTURE_1D, ref->resource);
		glTexParameteri(GL_TEXTURE_1D, GL_TEXTURE_MIN_FILTER, min);
		glTexParameteri(GL_TEXTURE_1D, GL_TEXTURE_MAG_FILTER, mag);
	}

	void OpenGLImplementation<3, 2>::updateTexture1DData(ObjRefBase *pObject, int pWidth, const uint8_t *pDataRgba) {
		auto ref = dynamic_cast<Reference*>(pObject);
		if(ref == nullptr) throw std::runtime_error("invalid texture reference");

		glBindTexture(GL_TEXTURE_1D, ref->resource);
		glTexImage1D(GL_TEXTURE_1D,
		             0,
		             GL_RGBA8,
		             static_cast<GLsizei>(pWidth),
		             0,
		             GL_RGBA,
		             GL_UNSIGNED_BYTE,
		             pDataRgba);
	}

	void OpenGLImplementation<3, 2>::updateTexture1DMipmap(ObjRefBase *pObject) {
		auto ref = dynamic_cast<Reference*>(pObject);
		if(ref == nullptr) throw std::runtime_error("invalid texture reference");

		glBindTexture(GL_TEXTURE_1D, ref->resource);
		glGenerateMipmap(GL_TEXTURE_1D);
	}

	ObjRefBase *OpenGLImplementation<3, 2>::createTexture3D() {
		uint32_t tex;
		glGenTextures(1, &tex);
		return new Reference(tex);
	}

	void OpenGLImplementation<3, 2>::destroyTexture3D(ObjRefBase *pObject) {
		auto ref = dynamic_cast<Reference*>(pObject);
		if(ref == nullptr) throw std::runtime_error("invalid texture reference");

		glDeleteTextures(1, &ref->resource);
		delete ref;
	}

	void OpenGLImplementation<3, 2>::setTexture3DWrapProperty(ObjRefBase *pObject, TextureWrapType pWrap) {
		auto ref = dynamic_cast<Reference*>(pObject);
		if(ref == nullptr) throw std::runtime_error("invalid texture reference");

		GLenum e;
		switch(pWrap) {
			case TextureWrapType::Repeat: e = GL_REPEAT; break;
			case TextureWrapType::ClampToEdge: e = GL_CLAMP_TO_EDGE; break;
			case TextureWrapType::BorderColor: throw std::runtime_error("cannot use this method to set border color");
		}

		glBindTexture(GL_TEXTURE_3D, ref->resource);
		glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_WRAP_S, e);
		glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_WRAP_T, e);
		glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_WRAP_R, e);
	}

	void OpenGLImplementation<3, 2>::setTexture3DWrapPropertyBorder(ObjRefBase *pObject, glm::vec3 pColor) {
		auto ref = dynamic_cast<Reference*>(pObject);
		if(ref == nullptr) throw std::runtime_error("invalid texture reference");

		glBindTexture(GL_TEXTURE_3D, ref->resource);
		glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
		glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);
		glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_BORDER);
		float color[4] = {pColor.r, pColor.g, pColor.b, 1};
		glTexParameterfv(GL_TEXTURE_3D, GL_TEXTURE_BORDER_COLOR, color);
	}

	void
	OpenGLImplementation<3, 2>::setTexture3DFilter(ObjRefBase *pObject, TextureMinFilter pMin, TextureMagFilter pMag) {
		auto ref = dynamic_cast<Reference*>(pObject);
		if(ref == nullptr) throw std::runtime_error("invalid texture reference");

		GLenum min, mag;

		switch(pMin) {
			case TextureMinFilter::Nearest: min = GL_NEAREST; break;
			case TextureMinFilter::Linear: min = GL_LINEAR; break;
			case TextureMinFilter::NearestMipmap: min = GL_NEAREST_MIPMAP_NEAREST; break;
			case TextureMinFilter::LinearMipmap: min = GL_LINEAR_MIPMAP_LINEAR; break;
		}

		switch(pMag) {
			case TextureMagFilter::Nearest: mag = GL_NEAREST; break;
			case TextureMagFilter::Linear: mag = GL_LINEAR; break;
		}

		glBindTexture(GL_TEXTURE_3D, ref->resource);
		glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_MIN_FILTER, min);
		glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_MAG_FILTER, mag);
	}

	void OpenGLImplementation<3, 2>::updateTexture3DData(ObjRefBase *pObject, int pWidth, int pHeight, int pDepth,
	                                                     const uint8_t *pDataRgba) {
		auto ref = dynamic_cast<Reference*>(pObject);
		if(ref == nullptr) throw std::runtime_error("invalid texture reference");

		glBindTexture(GL_TEXTURE_3D, ref->resource);
		glTexImage3D(GL_TEXTURE_3D,
		             0,
		             GL_RGBA8,
		             static_cast<GLsizei>(pWidth),
		             static_cast<GLsizei>(pHeight),
		             static_cast<GLsizei>(pDepth),
		             0,
		             GL_RGBA,
		             GL_UNSIGNED_BYTE,
		             pDataRgba);
	}

	void OpenGLImplementation<3, 2>::updateTexture3DMipmap(ObjRefBase *pObject) {
		auto ref = dynamic_cast<Reference*>(pObject);
		if(ref == nullptr) throw std::runtime_error("invalid texture reference");

		glBindTexture(GL_TEXTURE_3D, ref->resource);
		glGenerateMipmap(GL_TEXTURE_3D);
	}

	void OpenGLImplementation<3, 2>::updateTexture2DData(ObjRefBase *pObject, int pWidth, int pHeight,
	                                                     const uint8_t *pDataRgba) {
		auto ref = dynamic_cast<Reference*>(pObject);
		if(ref == nullptr) throw std::runtime_error("invalid texture reference");

		glBindTexture(GL_TEXTURE_2D, ref->resource);
		glTexImage2D(GL_TEXTURE_2D,
		             0,
		             GL_RGBA8,
		             static_cast<GLsizei>(pWidth),
		             static_cast<GLsizei>(pHeight),
		             0,
		             GL_RGBA,
		             GL_UNSIGNED_BYTE,
		             pDataRgba);
	}

	ObjRefBase *OpenGLImplementation<3, 2>::createFramebuffer(int pWidth, int pHeight) {
		uint32_t resource;
		glGenFramebuffers(1, &resource);
		auto ref = new FramebufferReference(resource, nullptr, nullptr, pWidth, pHeight);
		reinitializeFramebuffer(ref, pWidth, pHeight);
		return ref;
	}

	void OpenGLImplementation<3, 2>::reinitializeFramebuffer(ObjRefBase *pObject, int pWidth, int pHeight) {
		auto ref = dynamic_cast<FramebufferReference*>(pObject);
		if(ref == nullptr) { throw std::runtime_error("invalid framebuffer reference"); }

		glBindFramebuffer(GL_FRAMEBUFFER, ref->resource);

		if(ref->colorTexture != nullptr) glDeleteTextures(1, &ref->colorTexture->resource);
		if(ref->depthStencilRendernode != nullptr) glDeleteRenderbuffers(1, &ref->depthStencilRendernode->resource);

		delete ref->colorTexture;
		delete ref->depthStencilRendernode;

		uint32_t colTex;
		glGenTextures(1, &colTex);

		glBindTexture(GL_TEXTURE_2D, colTex);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, pWidth, pHeight, 0, GL_RGBA, GL_FLOAT, nullptr);
		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, colTex, 0);

		uint32_t depthStencilRbo;
		glGenRenderbuffers(1, &depthStencilRbo);
		glBindRenderbuffer(GL_RENDERBUFFER, depthStencilRbo);
		glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, pWidth, pHeight);
		glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, depthStencilRbo);

		auto status = glCheckFramebufferStatus(GL_FRAMEBUFFER);

		if(status != GL_FRAMEBUFFER_COMPLETE) {
			switch(status) {
				case GL_FRAMEBUFFER_UNDEFINED: throw std::runtime_error("OpenGL 3.2: framebuffer undefined");
				case GL_FRAMEBUFFER_INCOMPLETE_ATTACHMENT: throw std::runtime_error("OpenGL 3.2: framebuffer incomplete attachment");
				case GL_FRAMEBUFFER_INCOMPLETE_MISSING_ATTACHMENT: throw std::runtime_error("OpenGL 3.2: framebuffer has no images");
				case GL_FRAMEBUFFER_INCOMPLETE_DRAW_BUFFER: throw std::runtime_error("OpenGL 3.2: framebuffer incomplete draw buffer");
				case GL_FRAMEBUFFER_INCOMPLETE_READ_BUFFER: throw std::runtime_error("OpenGL 3.2: framebuffer incomplete read buffer");
				case GL_FRAMEBUFFER_UNSUPPORTED: throw std::runtime_error("OpenGL 3.2: framebuffer configuration unsupported");
				case GL_FRAMEBUFFER_INCOMPLETE_MULTISAMPLE: throw std::runtime_error("OpenGL 3.2: framebuffer incomplete multisample");
				case GL_FRAMEBUFFER_INCOMPLETE_LAYER_TARGETS: throw std::runtime_error("OpenGL 3.2: framebuffer incomplete layer targets");
				default: throw std::runtime_error("OpenGL 3.2: framebuffer (unknown error)");
			}
		}

		ref->colorTexture = new Reference(colTex);
		ref->depthStencilRendernode = new Reference(depthStencilRbo);
		ref->width = pWidth;
		ref->height = pHeight;
	}

	void OpenGLImplementation<3, 2>::destroyFramebuffer(ObjRefBase *pObject) {
		auto ref = dynamic_cast<FramebufferReference*>(pObject);
		if(ref == nullptr) { return; }

		if(ref->colorTexture != nullptr) glDeleteTextures(1, &ref->colorTexture->resource);
		if(ref->depthStencilRendernode != nullptr) glDeleteRenderbuffers(1, &ref->depthStencilRendernode->resource);

		delete ref->colorTexture;
		delete ref->depthStencilRendernode;

		glDeleteFramebuffers(1, &ref->resource);
	}

	ObjRefBase *OpenGLImplementation<3, 2>::getFramebufferColorTexture2D(ObjRefBase *pObject) {
		auto ref = dynamic_cast<FramebufferReference*>(pObject);
		if(ref == nullptr) { throw std::runtime_error("invalid framebuffer reference"); }

		return ref->colorTexture;
	}

	ObjRefBase *OpenGLImplementation<3, 2>::getFramebufferDepthStencilTexture2D(ObjRefBase *pObject) {
		BOOST_LOG_TRIVIAL(error) << "Not implemented: Depth + Stencil textures on framebuffers in Aurora OpenGL 3.2";
		return nullptr;
	}

	void OpenGLImplementation<3, 2>::performBlitFramebuffer(ObjRefBase *pSource, ObjRefBase *pTarget) {
		auto refs = dynamic_cast<FramebufferReference*>(pSource);
		if(refs == nullptr) { throw std::runtime_error("invalid framebuffer reference"); }

		glBindFramebuffer(GL_READ_FRAMEBUFFER, refs->resource);

		if(dynamic_cast<DefaultFramebufferReference*>(pTarget)) {
			glBindFramebuffer(GL_DRAW_FRAMEBUFFER, 0);
			glBlitFramebuffer(0, 0, refs->width, refs->height, 0, 0, refs->width, refs->height, GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT, GL_NEAREST);
		} else {
			auto reft = dynamic_cast<FramebufferReference*>(pTarget);
			if(reft == nullptr) { throw std::runtime_error("invalid framebuffer reference"); }
			glBindFramebuffer(GL_DRAW_FRAMEBUFFER, reft->resource);
			glBlitFramebuffer(0, 0, refs->width, refs->height, 0, 0, reft->width, reft->height, GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT, GL_NEAREST);
		}
	}

	void OpenGLImplementation<3, 2>::performBlitFramebuffer(ObjRefBase *pSource, ObjRefBase *pTarget, int pStartX,
	                                                        int pStartY, int pWidth, int pHeight) {
		auto refs = dynamic_cast<FramebufferReference*>(pSource);
		if(refs == nullptr) { throw std::runtime_error("invalid framebuffer reference"); }

		glBindFramebuffer(GL_READ_FRAMEBUFFER, refs->resource);

		if(dynamic_cast<DefaultFramebufferReference*>(pTarget)) {
			glBindFramebuffer(GL_DRAW_FRAMEBUFFER, 0);
			glBlitFramebuffer(pStartX, pStartX, pWidth, pHeight, 0, 0, refs->width, refs->height, GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT, GL_NEAREST);
		} else {
			auto reft = dynamic_cast<FramebufferReference*>(pTarget);
			if(reft == nullptr) { throw std::runtime_error("invalid framebuffer reference"); }
			glBindFramebuffer(GL_DRAW_FRAMEBUFFER, reft->resource);
			glBlitFramebuffer(pStartX, pStartX, pWidth, pHeight, 0, 0, reft->width, reft->height, GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT, GL_NEAREST);
		}
	}

	ObjRefBase *OpenGLImplementation<3, 2>::getDefaultFramebuffer() {
		return &m_DefaultFramebufferRef;
	}

	void OpenGLImplementation<3, 2>::activateFramebuffer(ObjRefBase *pObject) {
		if(dynamic_cast<DefaultFramebufferReference*>(pObject)) {
			glBindFramebuffer(GL_FRAMEBUFFER, 0);
		} else {
			auto ref = dynamic_cast<FramebufferReference*>(pObject);
			if(ref == nullptr) { throw std::runtime_error("invalid framebuffer reference"); }
			glBindFramebuffer(GL_FRAMEBUFFER, ref->resource);
		}
	}
}
