// file      : examples/roundtrip/driver.cxx
// copyright : not copyrighted - public domain

#include <string>
#include <fstream>
#include <iostream>

#include "Poco/XML/XMLStreamParser.h"

using namespace std;
using namespace Poco::XML;

int main(int argc, char* argv[])
{
	if (argc != 2)
	{
		cerr << "usage: " << argv[0] << " <xml-file>" << endl;
		return 1;
	}

	try
	{
		// Enable stream exceptions so that io failures are reported
		// as stream rather than as parsing exceptions.
		//
		ifstream ifs;
		ifs.exceptions(ifstream::badbit | ifstream::failbit);
		ifs.open(argv[1], ifstream::in | ifstream::binary);

		// Configure the parser to receive attributes as events as well
		// as to receive prefix-namespace mappings (namespace declarations
		// in XML terminology).
		//
		XMLStreamParser p(ifs, argv[1],
				XMLStreamParser::RECEIVE_DEFAULT | XMLStreamParser::RECEIVE_ATTRIBUTES_EVENT | XMLStreamParser::RECEIVE_NAMESPACE_DECLS);

		// Configure serializer not to perform indentation. Existing
		// indentation, if any, will be preserved.
		//
		XMLStreamSerializer s(cout, "out", 0);

		for (XMLStreamParser::EventType e(p.next()); e != XMLStreamParser::Eof; e = p.next())
		{
			switch (e)
			{
			case XMLStreamParser::StartElement:
			{
				s.startElement(p.getQName());
				break;
			}
			case XMLStreamParser::EndElement:
			{
				s.endElement();
				break;
			}
			case XMLStreamParser::StartNamespaceDecl:
			{
				s.namespaceDecl(p.namespace_(), p.prefix());
				break;
			}
			case XMLStreamParser::EndNamespaceDecl:
			{
				// There is nothing in XML that indicates the end of namespace
				// declaration since it is scope-based.
				//
				break;
			}
			case XMLStreamParser::StartAttribute:
			{
				s.startAttribute(p.getQName());
				break;
			}
			case XMLStreamParser::EndAttribute:
			{
				s.endAttribute();
				break;
			}
			case XMLStreamParser::Characters:
			{
				s.characters(p.value());
				break;
			}
			case XMLStreamParser::Eof:
			{
				// Handled in the for loop.
				//
				break;
			}
			}
		}
	} catch (const ios_base::failure&)
	{
		cerr << "io failure" << endl;
		return 1;
	} catch (const Poco::Exception& e)
	{
		cerr << e.displayText() << endl;
		return 1;
	}
}
