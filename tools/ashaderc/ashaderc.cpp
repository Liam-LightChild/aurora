/*
 * This file is part of Aurora Game Engine.
 * https://github.com/liam-lightchild/aurora
 */

#include <aurora/aether/aether.h>
#include <boost/program_options.hpp>
#include <iostream>
#include <xercesc/dom/DOM.hpp>
#include <xercesc/util/PlatformUtils.hpp>
#include <xercesc/util/XMLString.hpp>

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

using namespace xercesc;

namespace po = boost::program_options;

std::string fixUpShaderSrc(const std::string &pSource) {
	std::string src, line;

	for(const auto &item: pSource) {
		if((line.empty() || src.ends_with(' ')) && item == ' ') { continue; }

		if(item == '\n') {
			auto preprocessor = line.starts_with('#');
			line.clear();
			if(!preprocessor) { continue; }
		} else { line += item; }

		src += item;
	}

	return src;
}

int main(int pArgCount, char **pArgs) {
	po::positional_options_description pd;
	pd.add("input-file", 1);
	po::options_description desc("Allowed options");

	desc.add_options()
		    ("help", "Produce help message")
		    ("info", "Produce information message")
		    ("output-file,o", po::value<std::string>(), "Destination path")
		    ("input-file,i", po::value<std::string>(), "Provide Input file")
		    ("encapsulate", po::bool_switch()->default_value(false), "Encapsulates the output as a C++ file")
		    ("encapsulate-var", po::value<std::string>(), "The variable name for --encapsulate (including namespace)");

	po::variables_map vm;
	po::store(po::command_line_parser(pArgCount, pArgs).options(desc).positional(pd).run(), vm);
	po::notify(vm);

	if(vm.count("info")) {
		std::cout << info << std::endl;
		return 0;
	}

	if(vm.count("help") || !vm.count("input-file") || !vm.count("output-file") || (vm["encapsulate"].as<bool>() && !vm.count("encapsulate-var"))) {
		std::cout << desc << std::endl;
		return 1;
	}

	auto inputPath = vm["input-file"].as<std::string>();
	auto outputPath = vm["output-file"].as<std::string>();

	try {
		XMLPlatformUtils::Initialize();
	}
	catch(const XMLException &e) {
		std::cerr << XMLString::transcode(e.getMessage()) << std::endl;
	}

	auto impl = DOMImplementationRegistry::getDOMImplementation(XMLString::transcode("LS"));
	auto parser = impl->createLSParser(xercesc_3_2::DOMImplementationLS::MODE_SYNCHRONOUS, nullptr);

	if(parser->getDomConfig()->canSetParameter(XMLUni::fgDOMNamespaces, true)) {
		parser->getDomConfig()->setParameter(XMLUni::fgDOMNamespaces, true);
	}
	if(parser->getDomConfig()->canSetParameter(XMLUni::fgDOMDatatypeNormalization, true)) {
		parser->getDomConfig()->setParameter(XMLUni::fgDOMDatatypeNormalization, true);
	}

	auto doc = parser->parseURI(inputPath.c_str());
	auto element = doc->getDocumentElement();

	aurora::aether::Shader sh;

	auto aetherNs = XMLString::transcode(aurora::aether::Resource::schemaUri),
		ashdrNs = XMLString::transcode(aurora::aether::Shader::schemaUri);

	sh.id = XMLString::transcode(element->getAttributeNS(aetherNs, XMLString::transcode("id")));

	auto elementNodes = element->getChildNodes();
	for(int i = 0; i < elementNodes->getLength(); ++i) {
		auto shaderPart = dynamic_cast<DOMElement *>(elementNodes->item(i));
		if(shaderPart == nullptr) { continue; }
		auto shaderPartType = std::string(XMLString::transcode(shaderPart->getLocalName()));
		auto shaderPartSrc =
			dynamic_cast<DOMElement *>(shaderPart->getElementsByTagNameNS(ashdrNs, XMLString::transcode("glsl"))
			                                     ->item(0));
		if(shaderPartSrc == nullptr) { throw std::runtime_error("no Shader source?"); }

		aurora::aether::Shader::Stage shaderPartTypeEnum;

		if(shaderPartType == "vertex") { shaderPartTypeEnum = aurora::aether::Shader::Vertex; }
		else if(shaderPartType == "pixel") { shaderPartTypeEnum = aurora::aether::Shader::Pixel; }
		else { throw std::runtime_error("invalid shader part type"); }

		aurora::aether::Shader::Part
			part(shaderPartTypeEnum, fixUpShaderSrc(XMLString::transcode(shaderPartSrc->getTextContent())));
		sh.parts.emplace_back(std::move(part));

		auto childNodes = shaderPart->getChildNodes();
		for(int j = 0; j < childNodes->getLength(); ++j) {
			auto childElement = dynamic_cast<DOMElement *>(childNodes->item(j));
			if(childElement == nullptr) { continue; }

			auto childElementTypeX = XMLString::transcode(childElement->getLocalName());
			auto childElementType = std::string(childElementTypeX);

			if(childElementType == "input" && shaderPartTypeEnum == aurora::aether::Shader::Vertex) {
				auto name = XMLString::transcode(childElement->getAttributeNS(ashdrNs, XMLString::transcode("name")));
				auto purpose = XMLString::transcode(childElement->getAttributeNS(ashdrNs, XMLString::transcode("from")));
				auto elementType = XMLString::transcode(childElement->getAttributeNS(ashdrNs, XMLString::transcode("element_type")));
				auto sizeStr = XMLString::transcode(childElement->getAttributeNS(ashdrNs, XMLString::transcode("size")));
				sh.inputs.emplace_back(name, purpose);
				sh.vertexNodes.emplace_back(name, elementType, purpose, std::stoi(sizeStr));
			}

			if(childElementType == "output" && shaderPartTypeEnum == aurora::aether::Shader::Pixel) {
				auto name = XMLString::transcode(childElement->getAttributeNS(ashdrNs, XMLString::transcode("name")));
				auto color = XMLString::transcode(childElement->getAttributeNS(ashdrNs, XMLString::transcode("to")));
				aurora::aether::Shader::Output output(std::string(name), std::stoi(color));
				XMLString::release(&color);
				sh.outputs.emplace_back(std::move(output));
			}

			if(childElementType == "uniform") {
				auto name = XMLString::transcode(childElement->getAttributeNS(ashdrNs, XMLString::transcode("name")));
				auto
					purpose = XMLString::transcode(childElement->getAttributeNS(ashdrNs, XMLString::transcode("from")));
				aurora::aether::Shader::Uniform uniform(name, purpose);
				sh.uniforms.emplace_back(std::move(uniform));
			}

			XMLString::release(&childElementTypeX);
		}
	}

	auto outPath = std::filesystem::path(outputPath);

	if(outPath.has_parent_path() && !std::filesystem::exists(outPath.parent_path())) {
		std::filesystem::create_directories(outPath.parent_path());
	}

	if(vm["encapsulate"].as<bool>()) {
		auto out = std::ofstream(outPath);

		out << "// Autogenerated source file. Do not edit!\n"
			   "#include <aurora/shaders/shaders.h>\n\n"
			   "const ::aurora::aether::Shader " << vm["encapsulate-var"].as<std::string>()
			<< "(::nlohmann::json::parse(R\"(" << sh.serialize().dump(4) << ")\"));" << std::endl;
	} else {
		auto out = std::ofstream(outPath, std::ios::binary);
		auto data = nlohmann::json::to_cbor(sh.serialize());
		out.write(reinterpret_cast<const char *>(data.data()), static_cast<std::streamsize>(data.size()));
	}

	XMLString::release(&aetherNs);
	XMLString::release(&ashdrNs);

	parser->release();
	XMLPlatformUtils::Terminate();
}
