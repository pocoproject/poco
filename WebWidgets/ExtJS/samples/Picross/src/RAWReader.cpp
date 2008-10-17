#include "RAWReader.h"
#include "Poco/FileStream.h"
#include "Poco/StringTokenizer.h"
#include "Poco/NumberParser.h"


RAWReader::RAWReader(std::istream& in, int width, int height):
	_data(),
	_width(width),
	_height(height)
{
	init(in);
}


RAWReader::RAWReader(const Poco::File& f, int width, int height):
	_data(),
	_width(width),
	_height(height)
{
	if (_width <= 0 || _height <= 0)
		poco_assert (extractFromName(f.path(), _width, _height));
	poco_assert (f.exists());
	Poco::FileInputStream fis(f.path());
	init(fis);
}


RAWReader::RAWReader(const std::string& file, int width, int height):
	_data(),
	_width(width),
	_height(height)
{
	if (_width <= 0 || _height <= 0)
		poco_assert (extractFromName(file, _width, _height));
	Poco::FileInputStream fis(file);
	init(fis);
}

	
void RAWReader::init(std::istream& in)
{
	poco_assert (in.good());
	poco_assert (_width > 0);
	poco_assert (_height > 0);
	Poco::BinaryReader br(in, Poco::BinaryReader::LITTLE_ENDIAN_BYTE_ORDER);
	
	for (int i = 0; i < _height; ++i)
		readRow(br);
}


void RAWReader::readRow(Poco::BinaryReader& in)
{
	poco_assert (_width > 0);
	unsigned char r,g,b;
	for (int j = 0; j < _width; ++j)
	{
		in >> r;
		in >> g;
		in >> b;
		// we shoul do a conversion to YUV and then judge depending on the luminace value
		// simple check r only
		if (r <= 0x80u)
			_data.push_back(true);
		else
			_data.push_back(false);
	}
}


bool RAWReader::extractFromName(const std::string& fileName, int& width, int& height) const
{
	std::size_t pos = fileName.rfind('_');
	if (pos == std::string::npos)
		return false;

	std::string dim(fileName.substr(pos+1));
	Poco::StringTokenizer tok(dim, "xX. ", Poco::StringTokenizer::TOK_IGNORE_EMPTY | Poco::StringTokenizer::TOK_TRIM);
	if (tok.count() < 2)
		return false;
	bool ret = Poco::NumberParser::tryParse(tok[0], width);
	ret &= Poco::NumberParser::tryParse(tok[1], height);
	return ret;
}