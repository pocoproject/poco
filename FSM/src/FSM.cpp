/*
 * FSM.cpp
 *
 *  Created on: 16 janv. 2016
 *      Author: FrancisANDRE
 */

#include "Poco/Format.h"
using Poco::format;

#include "parser/Keyword.h"
#include "model/FSM.h"
#include "model/Map.h"
using Poco::FSM::PARSER::Keyword;

namespace Poco
{
namespace FSM
{
namespace MODEL
{
void FSM::add(MapPtr map)
{
    _maps.push_back(map);
    map->fsm() = this;
}
void FSM::addPackage(const string& package)
{
    _packages.push_back(package);
}
void FSM::addImport(const string& import)
{
    _imports.push_back(import);
}
void FSM::addInclude(const string& inc)
{
    _includes.push_back(inc);
}
void FSM::addDeclare(const string& declare)
{
    _declares.push_back(declare);
}
void FSM::print(Print& print) const
{
    print(format("%s\t%s", string(Keyword::asString(Keyword::START)), startState()));
    print(format("%s\t%s", string(Keyword::asString(Keyword::FSMFILE)), fsmfile()));
    print(format("%s\t%s", string(Keyword::asString(Keyword::FFSMLASS)), fsmclass()));
    print(format("%s\t%s", string(Keyword::asString(Keyword::CLASS)), klass()));
//				print(format("%s\t%s", string(Keyword::asString(Keyword::DECLARE)), declare()));
//				print(format("%s\t%s", string(Keyword::asString(Keyword::ACCESS)), access()));
    print(format("%s\t%s", string(Keyword::asString(Keyword::HEADER)), header()));
    print(format("%s\t%s", string(Keyword::asString(Keyword::PACKAGE)), packages()[0]));

    for (const auto& map : maps())
    {
        print(format("%s\t%s", string(Keyword::asString(Keyword::MAP)), map->Element::name()));
        map->print(print);
    }
}
}
}
}
