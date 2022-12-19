/*
 * This file is part of Aurora Game Engine.
 * https://github.com/liam-lightchild/aurora
 */

#include <aurora/aether/aether.h>
#include <boost/program_options.hpp>
#include <iostream>
#include <fstream>
#include <nlohmann/json.hpp>

const char *info = R"(
--- Information ---------------------------------------------------------------

amkindex: Produces an assets.idx.aet file based on the files provided by
the command line.

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
	pd.add("input-file", -1);
	po::options_description desc("Allowed options");

	desc.add_options()
		    ("help", "Produce help message")
		    ("info", "Produce information message")
		    ("output-file,o", po::value<std::string>(), "Destination path")
		    ("input-files,i", po::value<std::vector<std::string>>()->multitoken(), "Provide Input files");

	po::variables_map vm;
	po::store(po::command_line_parser(pArgCount, pArgs).options(desc).positional(pd).run(), vm);
	po::notify(vm);

	if(vm.count("info")) {
		std::cout << info << std::endl;
		return 0;
	}

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
