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
	class AssetLoader {
		struct Ref {
			void* ptr;
			int refs;
		};

		std::unordered_map<std::string, std::filesystem::path> m_Index;
		std::unordered_map<std::string, Ref*> m_Refs;
		std::unordered_map<void*, Ref*> m_RefsByPtr;

	public:
		explicit AssetLoader(const std::filesystem::path &pPath);

		template<typename T>
		T *load(const std::string &pAssetId) {
			if(m_Refs.contains(pAssetId)) {
				auto ref = m_Refs[pAssetId];
				ref->refs++;
				return reinterpret_cast<T*>(ref->ptr);
			} else {
				T* ptr = new T(this, m_Index[pAssetId], pAssetId);

				auto r = new Ref {
					.ptr = ptr,
					.refs = 1
				};

				m_Refs[pAssetId] = r;
				m_RefsByPtr[ptr] = r;

				return ptr;
			}
		}

		/**
		 * Tells the asset loader that the caller is no longer using the asset pointed to
		 * by pPointer. If the asset is unloaded by this call or does not exist, this function
		 * returns false. If the asset lives past this call, it returns true.
		 *
		 * @tparam T Required due to delete restrictions.
		 */
		template<typename T>
		bool unload(void* pPointer) {
			if(!m_RefsByPtr.contains(pPointer)) return false;
			auto ref = m_RefsByPtr[pPointer];

			if(ref->refs != -1 && --ref->refs <= 0) {
				delete reinterpret_cast<T*>(ref->ptr);
				delete ref;
				return false;
			} else {
				return true;
			}
		}

		template<typename T>
		void inject(const std::string &pAssetId, T* pPointer) {
			auto r = new Ref {
				.ptr = pPointer,
				.refs = -1
			};

			m_Refs[pAssetId] = r;
			m_RefsByPtr[pPointer] = r;
		}
	};

	void injectBuiltinAssets(AssetLoader *pLoader);

}// namespace aurora

#endif// AURORA_ASSET_LOADER_H
