#include "aurora/application.h"
#include "aurora/resources/shader.h"
#include "aurora/resources/buffer.h"
#include "aurora/resources/draw_object.h"

class MainApplication : public aurora::Application {
private:
	aurora::Shader *m_Shader;
	aurora::Buffer *m_VertexBuffer, *m_IndexBuffer;
	aurora::DrawObject *m_DrawObject;
	aurora::VertexArrangement m_TestShaderArrange = {
		{"v_position", aurora::VertexInputType::Float, 3},
		{"v_color", aurora::VertexInputType::Float, 3}
	};

public:
	MainApplication() : m_Shader(getInstance()->getAssetLoader()->load<aurora::Shader>("test:test.shader")),
	                    m_VertexBuffer(new aurora::Buffer(aurora::VertexBuffer)),
						m_IndexBuffer(new aurora::Buffer(aurora::IndexBuffer)) {
		float vertexBuf[] = {
			0, 0, 0, 1, 0, 0,
			0, 1, 0, 0, 1, 0,
			1, 0, 0, 0, 0, 1,
		};

		uint32_t indexBuf[] = {
			0, 1, 2
		};

		m_VertexBuffer->update(vertexBuf, sizeof(vertexBuf));
		m_IndexBuffer->update(indexBuf, sizeof(indexBuf));

		m_DrawObject = new aurora::DrawObject(aurora::DrawObjectOptions {
			.shader = m_Shader->getReference(),
			.vertexBuffer = m_VertexBuffer->getReference(),
			.indexBuffer = m_IndexBuffer->getReference(),
			.vertexCount = 3,
			.indexBufferItemType = aurora::IndexBufferItemType::UnsignedInt,
			.arrangement = m_TestShaderArrange
		});
	}

	~MainApplication() override {
		delete m_DrawObject;
		delete m_VertexBuffer;
		delete m_IndexBuffer;
		getInstance()->getAssetLoader()->unload<aurora::Shader>(m_Shader);
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
	auto a = new MainApplication;
	a->run();
	delete a;
}
