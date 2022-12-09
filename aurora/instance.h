#ifndef AURORA_INSTANCE_H
#define AURORA_INSTANCE_H

#include "asset_loader.h"
#include "graphics/graphics.h"
#include "graphics/implementation.h"
#include "graphics/implementation_finder.h"

namespace aurora {

	class Instance {
	private:
		Implementation *m_Implementation;
		AssetLoader *m_AssetLoader;
		Graphics *m_Graphics;

	public:
		Instance(ImplementationFinder *pFinder, const std::filesystem::path &pAssetPath);

		[[nodiscard]] inline Implementation *getImpl() const { return m_Implementation; }
		[[nodiscard]] inline AssetLoader *getAssetLoader() const { return m_AssetLoader; }
		[[nodiscard]] inline Graphics *getGraphics() const { return m_Graphics; }
	};

}// namespace aurora

#endif// AURORA_INSTANCE_H
