//
// Document.h
//
// Library: PDF
// Package: PDFCore
// Module:  Document
//
// Definition of the Document class.
//
// Copyright (c) 2006, Applied Informatics Software Engineering GmbH.
// and Contributors.
//
// SPDX-License-Identifier:	BSL-1.0
//


#ifndef PDF_Document_INCLUDED
#define PDF_Document_INCLUDED


#include "Poco/PDF/PDF.h"
#include "Poco/PDF/Declarations.h"
#include "Poco/PDF/Page.h"
#include "Poco/PDF/Outline.h"
#include <deque>
#include <map>


namespace Poco {


class LocalDateTime;


namespace PDF {


class PDF_API Document
	/// A Document represents a PDF document object.
{
public:
	using DataPtr = HPDF_BYTE*;
	using SizeType = HPDF_UINT32;
	using PageContainer = std::deque<Page>;
	using OutlineContainer = std::deque<Outline>;
	using FontContainer = std::map<std::string, Font>;
	using EncoderContainer = std::map<std::string, Encoder>;
	using ImageContainer = std::map<std::string, Image>;

	enum Info
	{
		INFO_CREATION_DATE = 0,
		INFO_MOD_DATE,
		INFO_AUTHOR,
		INFO_CREATOR,
		INFO_PRODUCER,
		INFO_TITLE,
		INFO_SUBJECT,
		INFO_KEYWORDS
	};

	enum Permission
	{
		ENABLE_READ = HPDF_ENABLE_READ,
			/// User can read the document.
		ENABLE_PRINT = HPDF_ENABLE_PRINT,
			/// User can print the document.
		ENABLE_EDIT_ALL = HPDF_ENABLE_EDIT_ALL,
			/// User can edit the contents of the document other than annotations, form fields.
		ENABLE_COPY = HPDF_ENABLE_COPY,
			/// User can copy the text and the graphics of the document.
		ENABLE_EDIT = HPDF_ENABLE_EDIT
			/// User can add or modify the annotations and form fields of the document.
	};

	enum PageLayout
	{
		PAGE_LAYOUT_SINGLE = 0,
			/// Only one page is displayed.
		PAGE_LAYOUT_ONE_COLUMN,
			/// Display the pages in one column.
		PAGE_LAYOUT_TWO_COLUMN_LEFT,
			/// Display the pages in two column. The page of the odd number is displayed left.
		PAGE_LAYOUT_TWO_COLUMN_RIGHT
			/// Display the pages in two column. The page of the odd number is displayed right.
	};


	enum PageMode
	{
		PAGE_MODE_USE_NONE = 0,
			/// Display the document with neither outline nor thumbnail.
		PAGE_MODE_USE_OUTLINE,
			/// Display the document with outline pain.
		PAGE_MODE_USE_THUMBS,
			///Display the document with thumbnail pain.
		PAGE_MODE_FULL_SCREEN
			/// Display the document with full screen mode.
	};

	enum Compression
	{
		COMPRESSION_NONE = HPDF_COMP_NONE,
			/// All contents are not compressed.
		COMPRESSION_TEXT = HPDF_COMP_TEXT,
			/// Compress the contents stream of the page.
		COMPRESSION_IMAGE = HPDF_COMP_IMAGE,
			/// Compress the streams of the image objects.
		COMPRESSION_METADATA = HPDF_COMP_METADATA,
			/// Other stream datas (fonts, cmaps and so on)  are compressed.
		COMPRESSION_ALL = HPDF_COMP_ALL
			/// All stream datas are compressed.
			/// (Same as HPDF_COMP_TEXT | HPDF_COMP_IMAGE | HPDF_COMP_METADATA)
	};

	enum Encryption
	{
		ENCRYPT_R2 = 2,
			/// Use "Revision 2" algorithm.
			/// The length of key is automatically set to 5(40bit).
		ENCRYPT_R3 = 3
			/// Use "Revision 3" algorithm.
			/// Between 5(40bit) and 16(128bit) can be specified for length of the key.
	};

	enum PageNumberStyle
	{
		PAGE_NUM_STYLE_DECIMAL = 0,
			/// Page label is displayed by Arabic numerals.
		PAGE_NUM_STYLE_UPPER_ROMAN,
			/// Page label is displayed by Uppercase roman numerals.
		PAGE_NUM_STYLE_LOWER_ROMAN,
			/// Page label is displayed by Lowercase roman numerals.
		PAGE_NUM_STYLE_UPPER_LETTERS,
			/// Page label is displayed by Uppercase letters (using A to Z).
		PAGE_NUM_STYLE_LOWER_LETTERS,
			/// Page label is displayed by Lowercase letters (using a to z).
	};

	Document(const std::string fileName = "",
		Poco::UInt32 pageCount = 1,
		Page::Size pageSize = Page::PAGE_SIZE_LETTER,
		Page::Orientation orientation = Page::ORIENTATION_PORTRAIT);
		/// Creates the Document, sets the file name to the specified value and
		/// creates the specified number of pages.

	Document(Poco::UInt32 pageCount,
		Page::Size pageSize = Page::PAGE_SIZE_LETTER,
		Page::Orientation orientation = Page::ORIENTATION_PORTRAIT);
		/// Creates the Document, sets the file name to the specified value and
		/// creates the specified number of pages.

	virtual ~Document();
		/// Destroys the Document.

	void createNew(bool resetAll = false);
		/// Resets the current document and creates a new one.
		/// If resetAll is true, the loaded resources are unloaded
		/// prior to creating the new document.

	void save(const std::string fileName = "");
		/// Saves the document to the specified file.
		/// If fileName is empty string, the member variable
		/// _fileName is used. If member variable is empty string,
		/// document is saved to the memory stream.

	DataPtr data(SizeType& sz);
		/// Returns the document content as raw data and data size in
		/// the sz argument.

	SizeType size();
		/// Resets the document stream, reads the document into the stream and
		/// returns the document data size.

	void setPages(std::size_t pagePerPages);
		/// Sets the number of pages per page.
		/// See HARU library HPDF_SetPagesConfiguration API call
		/// documentation for detailed explanation.

	void setPageLayout(PageLayout pageLayout);
		/// Sets the page layout.

	PageLayout getPageLayout() const;
		/// Returns the current page layout.

	void setPageMode(PageMode pageMode);
		/// Sets the page mode.

	PageMode getPageMode() const;
		/// Returns the current page mode.

	const Page& getPage(int index);
		/// Returns the page at index position.

	const Page& operator [] (int index);
		/// Returns the page at index position.

	const Page& getCurrentPage();
		/// Returns the current page.

	const Page& addPage(Page::Size pageSize= Page::PAGE_SIZE_LETTER,
		Page::Orientation orientation = Page::ORIENTATION_PORTRAIT);
		/// Adds a page at the end of the document and returns the reference to it.

	const Page& insertPage(int index,
		Page::Size pageSize= Page::PAGE_SIZE_LETTER,
		Page::Orientation orientation = Page::ORIENTATION_PORTRAIT);
		/// Inserts the page before the page at index position and returns the reference to it.

	//void openAction();
		/// Sets the first page that appears when document is opened.

	const Font& loadFont(const std::string& name, const std::string& encoding);

	const Font& font(const std::string& name, const std::string& encoding = "");
		/// Looks for the font with specified name in the font container.
		/// If the font is not found, it is created.
		/// Returns the reference to the requested font.

	std::string loadType1Font(const std::string& afmFileName, const std::string& pfmFileName);
		/// Loads type 1 font from file. Returns font name.

	std::string loadTTFont(const std::string& fileName, bool embed, int index = -1);
		/// Loads true type font from file. Returns font name.
		/// If the embed parameter is true, the glyph data of the font is embedded,
		/// otherwise only the matrix data is included in PDF file.

	const Image& loadPNGImage(const std::string& fileName);
		/// Loads the specified PNG image from the file and returns reference to it.

	const Image& loadPNGImageInfo(const std::string& fileName);
		/// Loads the specified PNG image information from the file and returns reference to it.
		/// Unlike loadPNGImage, this function does not load the whole data immediately.
		/// Only size and color properties are loaded. The image data is loaded just before the
		/// image object is written to PDF, and the loaded data is deleted immediately.

	const Image& loadJPEGImage(const std::string& fileName);
		/// Loads the specified PNG image from the file and returns reference to it.

	void compression(Compression mode);
		/// Sets the compression mode.

	void encryption(Encryption mode, Poco::UInt32 keyLength);
		/// Sets the encryption mode.

	const Encoder& loadEncoder(const std::string& name);
		/// Loads the encoder.

	const Encoder& getCurrentEncoder();
		/// Returns the current encoder.

	const Encoder& setCurrentEncoder(const std::string& name);
		/// Set the encoder as current and returns a reference to it.

	void addPageLabel(int pageNum, PageNumberStyle style, int firstPage, const std::string& prefix = "");
		/// adds page labeling range for the document.

	void useUTF8Encoding();
		/// Enables use of UTF-8 encoding (default enabled).

	void useJapaneseFonts();
		/// Enables use of Japanese fonts.

	void useKoreanFonts();
		/// Enables use of Korean fonts.

	void useChineseFonts();
		/// Enables use of Chinese fonts.

	void useChineseTraditionalFonts();
		/// Enables use of Chinese Traditional fonts.

	void useJapaneseEncodings();
		/// Enables use of Japanese encodings.

	void useKoreanEncodings();
		/// Enables use of Korean encodings.

	void useChineseEncodings();
		/// Enables use of Chinese encodings.

	void useChineseTraditionalEncodings();
		/// Enables use of Chinese Traditional encodings.

	void extendedGraphicState();
		/// Creates extended graphic state object.
		/// Bumps up the version of PDF to 1.4.
		/// NOTE:
		/// In Acrobat Reader 5.0, when ExtGState object is used combined with HPDF_Page_Concat(),
		/// there is a case that cannot be correctly displayed.

	const Outline& createOutline(const std::string& title, const Outline& outline, const Encoder& encoder);
		/// Creates the outline.

	void setInfo(Info info, const std::string& value);
		/// Sets the document info.

	void setInfo(Info info, const LocalDateTime& dt);
		/// Sets the document creation or moidification date.

	std::string getInfo(Info info);
		/// Returns the document info.

	void setPassword(const std::string& ownerPassword, const std::string& userPassword);
		/// Sets the document owner and user passwords.

	void setPermission(Permission perm);
		/// Sets the permission on the document.

	std::size_t pageCount() const;
		/// Returns number of pages in the document.

private:
	HPDF_Doc& handle();

	void init(Poco::UInt32 pageCount,
		Page::Size pageSize, Page::Orientation orientation);

	void reset(bool all = false);
		/// Resets the current document. If all is true, the loaded
		/// resources (e.g. fonts, encodings ...)are unloaded. Otherwise
		/// the resources are not unloaded.

	const Image& loadPNGImageImpl(const std::string& fileName, bool doLoad);

	HPDF_Doc         _pdf;
	std::string      _fileName;
	DataPtr          _pRawData;
	SizeType         _size;
	PageContainer    _pages;
	FontContainer    _fonts;
	EncoderContainer _encoders;
	OutlineContainer _outlines;
	ImageContainer   _images;

	friend class Page;
};


} } // namespace Poco::PDF


#endif // PDF_Document_INCLUDED
