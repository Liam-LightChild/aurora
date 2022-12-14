#include <boost/program_options.hpp>
#include <iostream>
#include <filesystem>
#include <fstream>
#include <aurora/aether/aether.h>

namespace po = boost::program_options;

void split2(const std::string &pString, std::string &p1, std::string &p2, char pChar = ' ') {
	auto firstSpace = pString.find(pChar);

	p1 = pString.substr(0, firstSpace);
	p2 = pString.substr(firstSpace + 1);
}

void split3(const std::string &pString, std::string &p1, std::string &p2, std::string &p3, char pChar = ' ') {
	auto firstSpace = pString.find(pChar);
	auto secondSpace = pString.find(pChar, firstSpace + 1);

	p1 = pString.substr(0, firstSpace);
	p2 = pString.substr(firstSpace + 1, secondSpace - firstSpace - 1);
	p3 = pString.substr(secondSpace + 1);
}

int main(int pArgCount, char** pArgs) {
	po::positional_options_description pd;
	pd.add("input-file", 1);
	po::options_description desc("Allowed options");

	desc.add_options()
		    ("help", "Produce help message")
		    ("output-file,o", po::value<std::string>()->required(), "Destination path")
		    ("input-file,i", po::value<std::string>()->required(), "Provide Input file")
		    ("mesh-path,m", po::value<std::string>()->required(), "Provide mesh path");

	po::variables_map vm;
	po::store(po::command_line_parser(pArgCount, pArgs).options(desc).positional(pd).run(), vm);
	po::notify(vm);

	if(vm.count("help") || !vm.count("input-file") || !vm.count("output-file") || !vm.count("mesh-path")) {
		std::cerr << desc << std::endl;
		return 1;
	}

	auto inputPath = vm["input-file"].as<std::string>();
	auto outputPath = vm["output-file"].as<std::string>();
	auto meshPath = vm["mesh-path"].as<std::string>();

	auto outPath = std::filesystem::path(outputPath);
	if(outPath.has_parent_path() && !std::filesystem::exists(outPath.parent_path())) {
		std::filesystem::create_directories(outPath.parent_path());
	}

	std::ifstream in(inputPath), meshIn(meshPath);
	std::ofstream out(outputPath);

	nlohmann::json meta = nlohmann::json::parse(in);

	aurora::aether::Mesh mesh;
	mesh.id = meta["@id"];

	std::string line, a, b, c;
	while(!meshIn.eof()) {
		std::getline(meshIn, line);

		if(line.find('#') != std::string::npos) {
			line = line.substr(0, line.find('#'));
		}

		auto begin = line.find_first_not_of(' ');
		auto end = line.find_last_not_of(' ');
		if(begin == end) continue;
		auto cmd = line.substr(begin, ++end - begin);

		if(cmd.empty()) continue;
		auto cmdBaseEnd = cmd.find_first_of(' ');

		auto cmdBase = cmd.substr(0, cmdBaseEnd);
		auto cmdArgs = cmd.substr(cmdBaseEnd + 1);

		if(cmdBase == "o" || cmdBase == "s") {
			// Ignored; irrelevant to pure mesh loading.
		} else if(cmdBase == "v") {
			split3(cmdArgs, a, b, c);
			glm::vec3 vec(std::stof(a), std::stof(b), std::stof(c));

			mesh.positions.emplace_back(vec);
		} else if(cmdBase == "vn") {
			split3(cmdArgs, a, b, c);
			glm::vec3 vec(std::stof(a), std::stof(b), std::stof(c));

			mesh.normals.emplace_back(vec);
		} else if(cmdBase == "vt") {
			split2(cmdArgs, a, b);
			glm::vec2 vec(std::stof(a), std::stof(b));

			mesh.texCoords.emplace_back(vec);
		} else if(cmdBase == "f") {
			split3(cmdArgs, a, b, c);

			std::string aa, ab, ac, ba, bb, bc, ca, cb, cc;
			split3(a, aa, ab, ac, '/');
			split3(b, ba, bb, bc, '/');
			split3(c, ca, cb, cc, '/');

			aurora::aether::MeshTri tri{};
			tri.vertices[0] = std::stoi(aa) - 1;
			tri.vertices[1] = std::stoi(ba) - 1;
			tri.vertices[2] = std::stoi(ca) - 1;
			tri.texVertices[0] = std::stoi(ab) - 1;
			tri.texVertices[1] = std::stoi(bb) - 1;
			tri.texVertices[2] = std::stoi(cb) - 1;
			tri.normalVertices[0] = std::stoi(ac) - 1;
			tri.normalVertices[1] = std::stoi(bc) - 1;
			tri.normalVertices[2] = std::stoi(cc) - 1;
			mesh.tris.emplace_back(tri);
		} else throw std::runtime_error("invalid OBJ directive: " + cmdBase);
	}

	nlohmann::json::to_cbor(mesh.serialize(), out);
}
