#include "aurora/application.h"
#include "aurora/resources.h"
#include <boost/log/trivial.hpp>

struct vertex {
	glm::vec3 position;
	glm::vec3 color;
	glm::vec2 textureCoords;
};

class MainApplication : public aurora::Application {
private:
	aurora::Shader *m_Shader;
	aurora::Buffer *m_VertexBuffer, *m_IndexBuffer;
	aurora::DrawObject *m_DrawObject;
	aurora::Texture2D *m_Texture;
	aurora::VertexArrangement m_TestShaderArrange {
		A_VNODE(v_position, Float, 3),
		A_VNODE(v_color, Float, 3),
		A_VNODE(v_texCoords, Float, 2)
	};

public:
	MainApplication() : m_Shader(getInstance()->getAssetLoader()->load<aurora::Shader>("test:test.shader")),
	                    m_VertexBuffer(new aurora::Buffer(aurora::VertexBuffer)),
						m_IndexBuffer(new aurora::Buffer(aurora::IndexBuffer)),
						m_Texture(getInstance()->getAssetLoader()->load<aurora::Texture2D>("test:test.texture")){
		vertex vertices[] = {
			{{0, 0, 0}, {1, 0, 0}, {0, 1}},
			{{0, 1, 0}, {0, 1, 0}, {0, 0}},
			{{1, 0, 0}, {0, 0, 1}, {1, 1}},
			{{1, 1, 0}, {1, 1, 1}, {1, 0}}
		};

		uint32_t indexBuf[] = {
			0, 1, 2,
			1, 3, 2
		};

		m_VertexBuffer->update(vertices, sizeof(vertices));
		m_IndexBuffer->update(indexBuf, sizeof(indexBuf));

		aurora::DrawObjectOptions options;
		options.shader = m_Shader->getReference();
		options.vertexBuffer = m_VertexBuffer->getReference();
		options.indexBuffer = m_IndexBuffer->getReference();
		options.vertexCount = 6;
		options.indexBufferItemType = aurora::IndexBufferItemType::UnsignedInt;
		options.arrangement = m_TestShaderArrange;
		options.textures[0] = m_Texture->getReference();

		m_DrawObject = new aurora::DrawObject(options);
	}

	~MainApplication() override {
		delete m_DrawObject;
		delete m_VertexBuffer;
		delete m_IndexBuffer;

		auto a = getInstance()->getAssetLoader();
		a->unload<aurora::Shader>(m_Shader);
		a->unload<aurora::Texture2D>(m_Texture);
	}

	void render() override {
		Application::render();
		m_DrawObject->draw();
	}

	void update() override {
		Application::update();
	}
};

int main() {
	MainApplication* a = nullptr;
	try {
		a = new MainApplication;
		a->run();
		delete a;
	} catch(const std::exception &e) {
		delete a;
		BOOST_LOG_TRIVIAL(error) << "Exception encountered";
		BOOST_LOG_TRIVIAL(error) << typeid(e).name() << ": " << e.what();
	} catch(...) {
		delete a;
		std::rethrow_exception(std::current_exception());
	}
}
