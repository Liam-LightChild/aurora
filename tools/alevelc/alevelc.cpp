/*
 * This file is part of Aurora Game Engine.
 * https://github.com/liam-lightchild/aurora
 */

#include <aurora/aether/aether.h>
#include <boost/program_options.hpp>
#include <iostream>
#include <fstream>
#include <xercesc/dom/DOM.hpp>
#include <xercesc/util/PlatformUtils.hpp>
#include <xercesc/util/XMLString.hpp>
#include <glm/gtx/euler_angles.hpp>
#include <functional>

const char *info = R"(
--- Information ---------------------------------------------------------------

alevelc: Compiles level xml files into their binary equivilant, ready to be used.

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

void
parseControllers(DOMElement *pElement, const std::function<void(const aurora::aether::Level::Controller &)> &pInsert) {
	auto elementNodes = pElement->getChildNodes();
	for(int i = 0; i < elementNodes->getLength(); ++i) {
		auto controller = dynamic_cast<DOMElement *>(elementNodes->item(i));
		if(controller == nullptr) { continue; }

		auto cont = aurora::aether::Level::Controller();
		cont.type = std::string(XMLString::transcode(controller->getAttribute(XMLString::transcode("type"))));

		auto childNodes = controller->getChildNodes();
		for(int j = 0; j < childNodes->getLength(); ++j) {
			auto child = dynamic_cast<DOMElement *>(childNodes->item(j));
			if(child == nullptr) { continue; }

			auto nameStr = std::string(XMLString::transcode(child->getAttribute(XMLString::transcode("name"))));
			auto valueStr = std::string(XMLString::transcode(child->getAttribute(XMLString::transcode("value"))));

			cont.properties
			    .insert({
				            nameStr,
				            valueStr
			            });
		}

		pInsert(cont);
	}
}

void parse(DOMElement *pElement, const std::function<void(const aurora::aether::Level::Object &)> &pInsert) {
	auto elementNodes = pElement->getChildNodes();
	for(int i = 0; i < elementNodes->getLength(); ++i) {
		auto object = dynamic_cast<DOMElement *>(elementNodes->item(i));
		if(object == nullptr) { continue; }

		auto obj = aurora::aether::Level::Object();
		obj.name = std::string(XMLString::transcode(object->getAttribute(XMLString::transcode("name"))));

		auto childNodes = object->getChildNodes();
		for(int j = 0; j < childNodes->getLength(); ++j) {
			auto child = dynamic_cast<DOMElement *>(childNodes->item(j));
			if(child == nullptr) { continue; }

			auto childNodeType = XMLString::transcode(child->getLocalName());
			auto childNodeTypeStr = std::string(childNodeType);

			if(childNodeTypeStr == "position") {
				auto posX = std::stod(XMLString::transcode(child->getAttribute(XMLString::transcode("x"))));
				auto posY = std::stod(XMLString::transcode(child->getAttribute(XMLString::transcode("y"))));
				auto posZ = std::stod(XMLString::transcode(child->getAttribute(XMLString::transcode("z"))));
				obj.position = {
					posX,
					posY,
					posZ
				};
			} else if(childNodeTypeStr == "rotation") {
				auto posX = std::stod(XMLString::transcode(child->getAttribute(XMLString::transcode("yaw"))));
				auto posY = std::stod(XMLString::transcode(child->getAttribute(XMLString::transcode("pitch"))));
				auto posZ = std::stod(XMLString::transcode(child->getAttribute(XMLString::transcode("roll"))));
				auto q = glm::dquat(glm::dvec3(
					glm::radians(posX),
					glm::radians(posY),
					glm::radians(posZ)));
				obj.rotation = q;
			} else if(childNodeTypeStr == "rotation-quat") {
				auto posW = std::stod(XMLString::transcode(child->getAttribute(XMLString::transcode("w"))));
				auto posX = std::stod(XMLString::transcode(child->getAttribute(XMLString::transcode("x"))));
				auto posY = std::stod(XMLString::transcode(child->getAttribute(XMLString::transcode("y"))));
				auto posZ = std::stod(XMLString::transcode(child->getAttribute(XMLString::transcode("z"))));
				obj.rotation = {
					posW,
					posX,
					posY,
					posZ
				};
			} else if(childNodeTypeStr == "children") {
				parse(child, [&obj](const aurora::aether::Level::Object &pObj) {
					obj.objects
					   .insert({
						           pObj.name,
						           pObj
					           });
				});
			} else if(childNodeTypeStr == "controllers") {
				parseControllers(child, [&obj](const aurora::aether::Level::Controller &pCont) {
					obj.controllers.emplace_back(pCont);
				});
			}

			XMLString::release(&childNodeType);
		}

		pInsert(obj);
	}
}

int main(int pArgCount, char **pArgs) {
	po::positional_options_description pd;
	pd.add("input-file", 1);
	po::options_description desc("Allowed options");

	desc.add_options()
		    ("help", "Produce help message")
		    ("info", "Produce information message")
		    ("output-file,o", po::value<std::string>(), "Destination path")
		    ("input-file,i", po::value<std::string>(), "Provide Input file");

	po::variables_map vm;
	po::store(po::command_line_parser(pArgCount, pArgs).options(desc).positional(pd).run(), vm);
	po::notify(vm);

	if(vm.count("info")) {
		std::cout << info << std::endl;
		return 0;
	}

	if(vm.count("help") || !vm.count("input-file") || !vm.count("output-file")) {
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

	aurora::aether::Level level;

	auto aetherNs = XMLString::transcode(aurora::aether::Resource::schemaUri),
		alvlNs = XMLString::transcode(aurora::aether::Level::schemaUri);

	level.id = XMLString::transcode(element->getAttributeNS(aetherNs, XMLString::transcode("id")));

	parse(element, [&level](const aurora::aether::Level::Object &pObj) {
		level.objects
		     .insert({
			             pObj.name,
			             pObj
		             });
	});

	auto outPath = std::filesystem::path(outputPath);

	if(outPath.has_parent_path() && !std::filesystem::exists(outPath.parent_path())) {
		std::filesystem::create_directories(outPath.parent_path());
	}

	auto out = std::ofstream(outPath, std::ios::binary);
	auto data = nlohmann::json::to_cbor(level.serialize());
	out.write(reinterpret_cast<const char *>(data.data()), static_cast<std::streamsize>(data.size()));

	XMLString::release(&aetherNs);
	XMLString::release(&alvlNs);

	parser->release();
	XMLPlatformUtils::Terminate();
}
