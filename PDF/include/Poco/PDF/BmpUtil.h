//
// BmpUtil.h
//


#ifndef PDF_BmpUtil_INCLUDED
#define PDF_BmpUtil_INCLUDED

#include "hpdf_doc.h"

namespace Poco {
	namespace PDF {

		HPDF_Image LoadBMPImageFromFile(HPDF_Doc pdf,	const char    *filename);

	}
} // namespace Poco::PDF


#endif // PDF_BmpUtil_INCLUDED