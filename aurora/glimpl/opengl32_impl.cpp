#include <GL/glew.h>
#include <sail-c++/sail-c++.h>
#include "opengl_impl.h"
#include <GLFW/glfw3.h>
#include <aurora/aether/aether.h>
#include <boost/log/trivial.hpp>
#include <memory>

namespace aurora {
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

		return new Reference(program);
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
			case RepeatTexture: e = GL_REPEAT; break;
			case ClampToEdge: e = GL_CLAMP_TO_EDGE; break;
			case BorderColor: throw std::runtime_error("cannot use this method to set border color");
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

		auto img = pImage.convert_to(SAIL_PIXEL_FORMAT_BPP64_RGBA);
		glBindTexture(GL_TEXTURE_2D, ref->resource);
		glTexImage2D(GL_TEXTURE_2D,
					 0,
					 GL_RGBA32F,
					 static_cast<GLsizei>(img.width()),
					 static_cast<GLsizei>(img.height()),
					 0,
					 GL_RGBA,
					 GL_UNSIGNED_SHORT,
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

		for(int i = 0; i < 32; ++i) {
			auto item = pOptions.textures[i];
			if(item == nullptr) continue;

			auto dyn = dynamic_cast<Reference*>(item);
			if(dyn == nullptr) throw std::runtime_error("invalid texture reference");

			glActiveTexture(GL_TEXTURE0 + i);
			glBindTexture(GL_TEXTURE_2D, dyn->resource); // TODO other kinds of texture
		}

		glBindVertexArray(0);
		return new DrawObjectReference(vao, pOptions);
	}

	void OpenGLImplementation<3, 2>::destroyDrawObject(ObjRefBase *pObject) {
		auto ref = dynamic_cast<DrawObjectReference*>(pObject);
		if(ref == nullptr) throw std::runtime_error("invalid draw object reference");

		glDeleteVertexArrays(1, &ref->resource);
	}

	void OpenGLImplementation<3, 2>::performDraw(ObjRefBase *pDrawObject) {
		auto ref = dynamic_cast<DrawObjectReference*>(pDrawObject);
		if(ref == nullptr) throw std::runtime_error("invalid draw object reference");

		glBindVertexArray(ref->resource);
		glUseProgram(ref->shader->resource);

		GLenum eFmt;
		switch(ref->indexBufferItemType) {
			case IndexBufferItemType::UnsignedInt: eFmt = GL_UNSIGNED_INT; break;
			case IndexBufferItemType::UnsignedByte: eFmt = GL_UNSIGNED_BYTE; break;
			case IndexBufferItemType::UnsignedShort: eFmt = GL_UNSIGNED_SHORT; break;
		}

		glDrawElements(GL_TRIANGLES, static_cast<GLsizei>(ref->vertexCount), eFmt, nullptr);
		glBindVertexArray(0);
	}
}
