/*
 * This file is part of Aurora Game Engine.
 * https://github.com/liam-lightchild/aurora
 */

#include "aether.h"
#include <fstream>

namespace aurora::aether {
	Mesh::Mesh(const nlohmann::json &pJson) : Resource(pJson) {
		for(const auto &item: pJson[".p"]) {
			positions.emplace_back(item[0], item[1], item[2]);
		}

		for(const auto &item: pJson[".t"]) {
			texCoords.emplace_back(item[0], item[1]);
		}

		for(const auto &item: pJson[".n"]) {
			normals.emplace_back(item[0], item[1], item[2]);
		}

		for(const auto &item: pJson[".tris"]) {
			MeshTri tri{};
			tri.vertices[0] = item[0];
			tri.texVertices[0] = item[1];
			tri.normalVertices[0] = item[2];
			tri.vertices[1] = item[3];
			tri.texVertices[1] = item[4];
			tri.normalVertices[1] = item[5];
			tri.vertices[2] = item[6];
			tri.texVertices[2] = item[7];
			tri.normalVertices[2] = item[8];
			tris.emplace_back(tri);
		}
	}

	nlohmann::json Mesh::serialize() {
		auto j = Resource::serialize();
		auto p = nlohmann::json::array();
		auto t = nlohmann::json::array();
		auto n = nlohmann::json::array();
		auto ta = nlohmann::json::array();

		for(const auto &item: positions) {
			p.emplace_back(nlohmann::json::array({
				                                     item.x,
				                                     item.y,
				                                     item.z
			                                     }));
		}
		for(const auto &item: texCoords) {
			t.emplace_back(nlohmann::json::array({
				                                     item.x,
				                                     item.y
			                                     }));
		}
		for(const auto &item: normals) {
			n.emplace_back(nlohmann::json::array({
				                                     item.x,
				                                     item.y,
				                                     item.z
			                                     }));
		}

		for(const auto &item: tris) {
			ta.emplace_back(nlohmann::json::array({
				                                      item.vertices[0],
				                                      item.texVertices[0],
				                                      item.normalVertices[0],
				                                      item.vertices[1],
				                                      item.texVertices[1],
				                                      item.normalVertices[1],
				                                      item.vertices[2],
				                                      item.texVertices[2],
				                                      item.normalVertices[2],
			                                      }));
		}

		j[".p"] = p;
		j[".t"] = t;
		j[".n"] = n;
		j[".tris"] = ta;

		return j;
	}

	Mesh::Mesh(AssetLoader *, const std::filesystem::path &pPath, const std::string &)
		: Mesh(nlohmann::json::from_cbor(std::ifstream(pPath))) {}
}
