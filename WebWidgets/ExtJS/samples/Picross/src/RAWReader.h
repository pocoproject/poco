// RAWReader
// Copyright 2008 by Peter Schojer


#ifndef RAWReader_H_INCLUDED
#define RAWReader_H_INCLUDED


#include "Poco/BinaryReader.h"
#include "Poco/File.h"
#include <istream>
#include <vector>

class RAWReader
	/// class to read raw files in format RGB
{
public:
	RAWReader(std::istream& in, int width, int height);

	RAWReader(const Poco::File& f, int width = -1, int height = -1);
		/// tries to extract from the filename a string _{width}x{height] to get the
		/// dimension

	RAWReader(const std::string& file, int width = -1, int height = -1);
		/// tries to extract from the filename a string _{width}x{height] to get the
		/// dimension
	
	bool get(int posX, int posY) const;

	int width() const;

	int height() const;

private:
	int pos(int x, int y) const;

	void init(std::istream& in);

	void readRow(Poco::BinaryReader& in);
		/// Reads a complete row from in
		/// skips bytes 

	bool extractFromName(const std::string& fileName, int& width, int& height) const;

private:
	std::vector<bool> _data;
	int _width;
	int _height;
};


inline bool RAWReader::get(int posX, int posY) const
{
	return _data[pos(posX, posY)];
}


inline int RAWReader::pos(int x, int y) const
{
	// stored column after column!
	poco_assert_dbg (x < _width);
	poco_assert_dbg (y < _height);
	return x + (_width*y);
}


inline int RAWReader::width() const
{
	return _width;
}


inline int RAWReader::height() const
{
	return _height;
}


#endif