#include <aurora/aether/aether.h>
#include <boost/program_options.hpp>
#include <iostream>
#include <nlohmann/json.hpp>

namespace po = boost::program_options;

int main(int pArgCount, char **pArgs) {
	po::positional_options_description pd;
	pd.add("input-file", 1);
	po::options_description desc("Allowed options");

	desc.add_options()
		    ("help", "Produce help message")
		    ("output-file,o", po::value<std::string>()->required(), "Destination path")
		    ("input-file,i", po::value<std::string>()->required(), "Provide Input file")
		    ("texture-path,t", po::value<std::string>()->required(), "Provide texture path");

	po::variables_map vm;
	po::store(po::command_line_parser(pArgCount, pArgs).options(desc).positional(pd).run(), vm);
	po::notify(vm);

	if(vm.count("help") || !vm.count("input-file") || !vm.count("output-file") || !vm.count("texture-path")) {
		std::cout << desc << std::endl;
		return 1;
	}

	auto inputPath = vm["input-file"].as<std::string>();
	auto outputPath = vm["output-file"].as<std::string>();
	auto texturePath = vm["texture-path"].as<std::string>();

	auto outPath = std::filesystem::path(outputPath);
	auto texPath = std::filesystem::path(texturePath);

	if(outPath.has_parent_path() && !std::filesystem::exists(outPath.parent_path())) {
		std::filesystem::create_directories(outPath.parent_path());
	}

	auto relPath = relative(texPath, outPath.parent_path());

	std::ifstream in(inputPath);
	std::ofstream out(outputPath);

	auto parse = nlohmann::json::parse(in);
	parse["path"] = relPath.string();
	aurora::aether::TextureMeta meta(parse);
	nlohmann::json::to_cbor(meta.serialize(), out);
}
