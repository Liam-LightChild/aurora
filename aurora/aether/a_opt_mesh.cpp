/*
 * This file is part of Aurora Game Engine.
 * https://github.com/liam-lightchild/aurora
 */

#include "aether.h"

namespace aurora::aether {
	OptimisedMesh::OptimisedMesh(const Mesh &pMesh, const Shader &pShader, const glm::vec4 &pColor) {
		struct Vertex {
			glm::vec3 position;
			glm::vec2 texCoord;
			glm::vec3 normal;

			Vertex(const glm::vec3 &pPosition, const glm::vec2 &pTexCoord, const glm::vec3 &pNormal)
				: position(pPosition), texCoord(pTexCoord), normal(pNormal) {}

			bool operator==(const Vertex &pRhs) const {
				return position == pRhs.position &&
				       texCoord == pRhs.texCoord &&
				       normal == pRhs.normal;
			}

			bool operator!=(const Vertex &pRhs) const {
				return !(pRhs == *this);
			}
		};

		std::vector<Vertex> vertices;
		for(const auto &item: pMesh.tris) {
			for(int i = 0; i < 3; ++i) {
				auto vtx = Vertex(pMesh.positions[item.vertices[i]],
				                  pMesh.texCoords[item.texVertices[i]],
				                  pMesh.normals[item.normalVertices[i]]);
				auto iter = vertices.begin(), end = vertices.end();

				bool done = false;

				for(int j = 0; iter != end; iter++, j++) {
					if(*iter == vtx) {
						indexData.emplace_back(j);
						done = true;
						break;
					}
				}

				if(!done) {
					vertices.emplace_back(vtx);
					indexData.emplace_back((uint32_t) vertices.size() - 1);
				}
			}
		}

		for(const auto &item: vertices) {
			for(const auto &node: pShader.vertexNodes) {
				if(node.type != "Float") {
					throw std::runtime_error("Non-float input values are not supported by this class");
				}

				if(node.from == "position2") {
					vertexData.emplace_back(item.position.x);
					vertexData.emplace_back(item.position.y);
				} else if(node.from == "position3") {
					vertexData.emplace_back(item.position.x);
					vertexData.emplace_back(item.position.y);
					vertexData.emplace_back(item.position.z);
				} else if(node.from == "color3_rgb") {
					vertexData.emplace_back(pColor.r);
					vertexData.emplace_back(pColor.g);
					vertexData.emplace_back(pColor.b);
				} else if(node.from == "color4_rgba") {
					vertexData.emplace_back(pColor.r);
					vertexData.emplace_back(pColor.g);
					vertexData.emplace_back(pColor.b);
					vertexData.emplace_back(pColor.a);
				} else if(node.from == "tex1") {
					vertexData.emplace_back(item.texCoord.s);
				} else if(node.from == "tex2") {
					vertexData.emplace_back(item.texCoord.s);
					vertexData.emplace_back(item.texCoord.t);
				} else if(node.from == "tex3") {
					vertexData.emplace_back(item.texCoord.s);
					vertexData.emplace_back(item.texCoord.t);
					vertexData.emplace_back(item.texCoord.r);
				} else { throw std::runtime_error("Unsupported mesh input value " + node.from); }
			}
		}
	}
}
