#include <aurora/aether/aether.h>
#include <boost/program_options.hpp>
#include <iostream>
#include <xercesc/dom/DOM.hpp>
#include <xercesc/util/PlatformUtils.hpp>
#include <xercesc/util/XMLString.hpp>

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
		    ("output-file,o", po::value<std::string>(), "Destination path")
		    ("input-file,i", po::value<std::string>(), "Provide Input file");

	po::variables_map vm;
	po::store(po::command_line_parser(pArgCount, pArgs).options(desc).positional(pd).run(), vm);
	po::notify(vm);

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
				auto
					purpose = XMLString::transcode(childElement->getAttributeNS(ashdrNs, XMLString::transcode("from")));
				aurora::aether::Shader::Input input(name, purpose);
				sh.inputs.emplace_back(std::move(input));
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

	auto data = nlohmann::json::to_cbor(sh.serialize());
	auto outPath = std::filesystem::path(outputPath);

	if(outPath.has_parent_path() && !std::filesystem::exists(outPath.parent_path())) {
		std::filesystem::create_directories(outPath.parent_path());
	}

	auto out = std::ofstream(outPath);
	out.write(reinterpret_cast<const char *>(data.data()), static_cast<std::streamsize>(data.size()));

	XMLString::release(&aetherNs);
	XMLString::release(&ashdrNs);

	parser->release();
	XMLPlatformUtils::Terminate();
}
