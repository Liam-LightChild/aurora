#include <aurora/aether/aether.h>
#include <boost/program_options.hpp>
#include <iostream>
#include <nlohmann/json.hpp>

namespace po = boost::program_options;

int main(int pArgCount, char **pArgs) {
	po::positional_options_description pd;
	pd.add("input-file", -1);
	po::options_description desc("Allowed options");

	desc.add_options()
		    ("help", "Produce help message")
		    ("output-file,o", po::value<std::string>(), "Destination path")
		    ("input-files,i", po::value<std::vector<std::string>>()->multitoken(), "Provide Input files");

	po::variables_map vm;
	po::store(po::command_line_parser(pArgCount, pArgs).options(desc).positional(pd).run(), vm);
	po::notify(vm);

	if(vm.count("help") || !vm.count("input-files") || !vm.count("output-file")) {
		std::cout << desc << std::endl;
		return 0;
	}

	auto index = nlohmann::json::object();

	for(const auto &item: vm["input-files"].as<std::vector<std::string>>()) {
		if(item.ends_with(".aet")) {
			aurora::aether::Resource r(aurora::aether::Resource::readFromFile(item));
			index[r.id] = item;
		}
	}

	auto outPath = std::filesystem::path(vm["output-file"].as<std::string>());

	if(outPath.has_parent_path() && !std::filesystem::exists(outPath.parent_path())) {
		std::filesystem::create_directories(outPath.parent_path());
	}

	std::ofstream out(outPath);
	auto cbor = nlohmann::json::to_cbor(index);
	out.write(reinterpret_cast<char *>(cbor.data()), static_cast<std::streamsize>(cbor.size()));
}
