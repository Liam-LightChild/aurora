#include "aurora/application.h"
#include "aurora/resources.h"
#include "aurora/level/object.h"
#include <boost/log/trivial.hpp>

class MainApplication : public aurora::Application {
private:
	aurora::level::Level *m_Level;

public:
	MainApplication() : m_Level(getInstance()->getAssetLoader()->load<aurora::level::Level>("test:test.level")) {
		m_Level->setCurrentCamera(1);
		setLevel(m_Level);
	}

	~MainApplication() override = default;

	void render() override {
		Application::render();
	}

	void update() override {
		Application::update();
	}
};

int main() {
	MainApplication *a = nullptr;
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
