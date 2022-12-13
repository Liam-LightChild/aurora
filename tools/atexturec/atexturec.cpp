#include <aurora/aether/aether.h>
#include <boost/program_options.hpp>
#include <iostream>
#include <nlohmann/json.hpp>

const char *info = R"(
--- Information ---------------------------------------------------------------

ashaderc: Produces a .ashdr.aet file based on an xml input, intended to be read
at runtime by an Aurora application.

This tool is part of the Aurora Game Engine. Use --help to view the help message.

--- License -------------------------------------------------------------------

BSD 3-Clause License

Copyright (c) 2022, der_frühling

Redistribution and use in source and binary forms, with or without
modification, are permitted provided that the following conditions are met:

1. Redistributions of source code must retain the above copyright notice, this
   list of conditions and the following disclaimer.

2. Redistributions in binary form must reproduce the above copyright notice,
   this list of conditions and the following disclaimer in the documentation
   and/or other materials provided with the distribution.

3. Neither the name of the copyright holder nor the names of its
   contributors may be used to endorse or promote products derived from
   this software without specific prior written permission.

THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE
FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.

-------------------------------------------------------------------------------
)";

namespace po = boost::program_options;

int main(int pArgCount, char **pArgs) {
	po::positional_options_description pd;
	pd.add("input-file", 1);
	po::options_description desc("Allowed options");

	desc.add_options()
		    ("help", "Produce help message")
		    ("info", "Produce information message")
		    ("output-file,o", po::value<std::string>()->required(), "Destination path")
		    ("input-file,i", po::value<std::string>()->required(), "Provide Input file")
		    ("texture-path,t", po::value<std::string>()->required(), "Provide texture path");

	po::variables_map vm;
	po::store(po::command_line_parser(pArgCount, pArgs).options(desc).positional(pd).run(), vm);
	po::notify(vm);

	if(vm.count("info")) {
		std::cout << info << std::endl;
	}

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
