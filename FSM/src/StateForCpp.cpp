/*
 * StateForCpp.cpp
 *
 *  Created on: 19 janv. 2016
 *      Author: FrancisANDRE
 */

#include <iostream>
#include "cpp/MapForCpp.h"
#include "cpp/StateForCpp.h"
#include "cpp/EntryForCpp.h"
#include "cpp/ExitForCpp.h"
#include "cpp/ActionForCpp.h"
#include "cpp/TransitionForCpp.h"
#include "parser/IndentStream.h"
#include "model/FSM.h"

using namespace std;

namespace Poco
{
namespace FSM
{
namespace PARSER
{
namespace CPP
{
string StateForCpp::statename() const
{
    return map()->name() + '_' + name();
}
void StateForCpp::generateForwarDeclaration(ostream& inc, bool debug) const
{
    inc << "class " << statename() << ';' << endl;
}
void StateForCpp::generateVirtualTransitions(ostream& inc, bool debug) const
{
    for (const auto& transition : _transitions)
    {
        TransitionForCpp* tfc = dynamic_cast<TransitionForCpp*>(transition);
        if (tfc)
            tfc->generateVirtualTransitions(inc, debug);
    }
}
void StateForCpp::generateDefinition(ostream& inc, bool debug) const
{
    inc << "// --------------------------------------------------" << endl;
    inc << "class " << statename() << ":  public " << map()->name() <<"_Default { " << endl;
    inc << "public:" << endl << tab;
    inc << statename() << "(const char* name, int no) : " << map()->name() << "_Default(name, no) {}" << endl;

    inc << endl;
    if (entry())
    {
        inc << "void " << "Entry" << "(" << map()->fsm()->context() << "& context);" << endl;
    }
    if (exit())
    {
        inc << "void " << "Exit" << "(" << map()->fsm()->context() << "& context);" << endl;
    }
    inc << endl;

    for (auto transition : _transitions)
    {
        TransitionForCpp* tfc = dynamic_cast<TransitionForCpp*>(transition);
        if (tfc)
            tfc->generateDefinition(inc, debug);
    }
    inc << back << "};" << endl;
    inc << endl;
}


void StateForCpp::generateInclude(ostream& inc, bool debug) const
{
}
void StateForCpp::generateDeclaration(ostream& cpp, int no) const
{
//					cpp << name() << "\t\t" << map()->name() << "::" << State::name();
    cpp << State::name() << '(' << '"' << State::name() << '"' << ", " << no << ")";
}
void StateForCpp::generateCode(ostream& cpp, bool debug) const
{
    if (entry())
    {
        cpp << "void " << statename() << "::Entry" << "(" << map()->fsm()->context() << "& context) {" << endl;
        cpp << "    " << map()->fsm()->klass() << "& ctxt = context.getOwner();" << endl;
        for (const auto& action : entry()->actions())
            if (action)
            {
                ActionForCpp* afc = static_cast<ActionForCpp*>(action);
                cpp << tab << "ctxt." << afc->display() << endl << back;
            }
        cpp << "}" << endl;
    }
    if (exit())
    {
        cpp << "void " << statename() << "::Exit" << "(" << map()->fsm()->context() << "& context) {" << endl;
        cpp << "    " << map()->fsm()->klass() << "& ctxt = context.getOwner();" << endl;
        for (const auto& action : exit()->actions())
            if (action)
            {
                ActionForCpp* afc = static_cast<ActionForCpp*>(action);
                cpp << tab << "ctxt." << afc->display() << endl << back;
            }
        cpp << "}" << endl;
    }
    for (const auto& transition : _transitions)
    {
        TransitionForCpp* tfc = dynamic_cast<TransitionForCpp*>(transition);
        if (tfc)
            tfc->generateCode(cpp, debug);
    }
}
}
}
}
}