//
// Created by liam on 12/2/22.
//

#ifndef AURORA_ASSET_LOADER_H
#define AURORA_ASSET_LOADER_H

#include <string>
#include <filesystem>
#include <unordered_map>
#include <utility>
#include <optional>

namespace aurora {

	template<typename T>
	class Asset {
		std::filesystem::path m_Path;
		std::optional<std::shared_ptr<T>> m_Value;

	public:
		explicit Asset(std::filesystem::path pPath) : m_Path(std::move(pPath)) {}

		std::shared_ptr<T> value() {
			if(!m_Value) { m_Value = std::make_shared<T>(m_Path); }
			return m_Value.value();
		}

		/**
		 * This function does not necessarily guarantee the unloading of the Resource,
		 * which will only be performed when all references to it are kaput. This method
		 * just removes the reference contained in this Asset, which will in turn <b>allow</b>
		 * the data to be unloaded.
		 */
		void unload() {
			m_Value = std::nullopt;
		}
	};

	class AssetLoader {
		std::unordered_map<std::string, std::filesystem::path> m_Index;

	public:
		explicit AssetLoader(const std::filesystem::path &pPath);

		template<typename T>
		Asset<T> get(const std::string &pAssetId) {
			return Asset<T>(m_Index[pAssetId]);
		}
	};

}// namespace aurora

#endif// AURORA_ASSET_LOADER_H
