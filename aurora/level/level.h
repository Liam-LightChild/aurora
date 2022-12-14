/*
 * This file is part of Aurora Game Engine.
 * https://github.com/liam-lightchild/aurora
 */

#ifndef AURORA_LEVEL_H
#define AURORA_LEVEL_H

#include "../asset_loader.h"
#include "../aether/aether.h"
#include "../resources/framebuffer.h"

namespace aurora::level {

	class Object;
	class CameraController;

	class Level {
	private:
		std::vector<Object*> m_Objects;
		std::unordered_map<int, CameraController*> m_Cameras;
		int m_CurrentCamera = -1;

	public:
		Level();
		virtual ~Level();
		explicit Level(const aether::Level &pAether);
		Level(AssetLoader*, const std::filesystem::path &pPath, const std::string&);

		[[nodiscard]] const std::vector<Object*> &getObjects() const {
			return m_Objects;
		}

		virtual void render();
		virtual void update();

		virtual Framebuffer* renderCamera(int pCameraId);
		int getCurrentCamera() const;
		CameraController *getCurrentCameraController() const;
		void setCurrentCamera(int pCurrentCamera);
		void registerCamera(int pId, CameraController *pController);
		void unregisterCamera(int pId);
		const std::unordered_map<int, CameraController *> &getCameras() const;
	};

} // aurora::level

#endif //AURORA_LEVEL_H
