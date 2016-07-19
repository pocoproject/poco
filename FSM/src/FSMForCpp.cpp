/*
 * FSMForCpp.cpp
 *
 *  Created on: 19 janv. 2016
 *      Author: FrancisANDRE
 */

#include <iostream>
#include "Poco/Timestamp.h"
#include "Poco/File.h"
#include "Poco/FileStream.h"
using Poco::FileInputStream;
using Poco::FileOutputStream;
using Poco::File;
using Poco::Timestamp;

#include "cpp/FSMForCpp.h"
#include "cpp/MapForCpp.h"
#include "parser/Parser.h"

using namespace std;

namespace Poco
{
namespace FSM
{
namespace PARSER
{
namespace CPP
{
void FSMForCpp::openNamespaces(IndentStream& os) const
{
    for (const auto& package : packages())
        if (!package.empty())
            os << "namespace " << package << " {" << endl << tab;
}
void FSMForCpp::closeNamespaces(IndentStream& os) const
{
    for (const auto& package : packages())
        if (!package.empty())
            os << back << '}' << endl;
}
void FSMForCpp::generate(const Path& out, bool debug) const
{
    {
        Path incPath(out, fsmfile() + ".h");
        FileOutputStream incfos(incPath.toString());
        IndentStream inc(incfos);
        string define;
        for (const auto& ns : packages())
        {
            define += ns;
            define += "_";
        }
        define += fsmfile();
        inc << "#ifndef " << define << "_H" << endl;
        inc << "#define " << define << "_H" << endl;

        inc << "#include \"Poco/FSM/statemap.h\"" << endl;

        openNamespaces(inc);

        for (auto map : _maps)
        {
            dynamic_cast<MapForCpp*>(map)->generateInclude(inc, debug);
        }
        closeNamespaces(inc);
        inc << "#endif" << endl;
    }
    {
        Path cppPath(out, fsmfile() + ".cpp");
        FileOutputStream cppfos(cppPath.toString());
        IndentStream cpp(cppfos);

        for (const auto& include : includes())
            if (!include.empty())
                cpp << "#include \"" << include << "\"" << endl;

        if (!header().empty())
            cpp << "#include \"" << header() << "\"" << endl;

        cpp << "#include \"" << fsmfile() << ".h\"" << endl;

        cpp << "using namespace statemap;" << endl;

        openNamespaces(cpp);
        for (const auto& map : _maps)
        {
            dynamic_cast<MapForCpp*>(map)->generateCode(cpp, debug);
        }
        closeNamespaces(cpp);
    }
}
bool FSMForCpp::updated(const Path& out)
{
    Path incPath(out, fsmfile() + ".h");
    File incFile(incPath);
    if (incFile.exists())
    {
        Timestamp incFilets = incFile.getLastModified();
        Timestamp fsmfilets = parser()->file().getLastModified();
        if (incFilets > fsmfilets)
            return true;
    }

    return false;
}

}
}
}
}
