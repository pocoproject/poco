//
// Copyright (c) 2016, Applied Informatics Software Engineering GmbH.
// and Contributors.
//

#include "parser/IndentStream.h"

std::ostream& tab(std::ostream& stream)
{
    IndentStream* pIndentStream = dynamic_cast<IndentStream*>(&stream);
    if (pIndentStream != NULL)
    {
        pIndentStream->ib.tab();
    }
    return stream;
}

std::ostream& back(std::ostream& stream)
{
    IndentStream* pIndentStream = dynamic_cast<IndentStream*>(&stream);
    if (pIndentStream != NULL)
    {
        pIndentStream->ib.back();
    }
    return stream;
}

