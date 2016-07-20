//
// Copyright (c) 2016, Applied Informatics Software Engineering GmbH.
// and Contributors.
//

#include <iostream>
#include <algorithm>
#include "cpp/TransitionForCpp.h"
#include "cpp/ParameterForCpp.h"
#include "cpp/FSMForCpp.h"
#include "cpp/MapForCpp.h"
#include "cpp/StateForCpp.h"
#include "cpp/GuardForCpp.h"
#include "cpp/ActionForCpp.h"
#include "cpp/ExpressionForCpp.h"
#include "parser/IndentStream.h"
#include "model/Parameter.h"
#include "model/Action.h"

using Poco::FSM::MODEL::Parameter;
using namespace std;

static const bool RAPP = false;

namespace Poco
{
namespace FSM
{
namespace PARSER
{
namespace CPP
{

const string TransitionForCpp::name() const
{
    string cppname = Element::name();
    replace(cppname.begin(), cppname.end(), '-', '_');
    return cppname;
}
void TransitionForCpp::generateVirtualTransitions(ostream& inc, bool debug) const
{
    if (begstate()->entry())
    {
        inc << "virtual void " << "Entry" << "(" << begstate()->map()->fsm()->context() << "& context);";
    }
    if (begstate()->exit())
    {
        inc << "virtual void " << "Exit" << "(" << begstate()->map()->fsm()->context() << "& context);";
    }
    inc << "virtual void " << name() << "(" << begstate()->map()->fsm()->context() << "& context";
    if (!_parameters.empty())
    {
        inc << ", ";
        for (int no = 0; no < _parameters.size(); ++no)
        {
            dynamic_cast<ParameterForCpp*>(_parameters[no])->generateInclude(inc, debug);
            if (no + 1 < _parameters.size())
                inc << ", ";
        }
    }
    inc << ");" << endl;
}
void TransitionForCpp::generateDefinition(ostream& inc, bool debug) const
{
    inc << "void " << name() << "(" << begstate()->map()->fsm()->context() << "& context";
    if (!_parameters.empty())
    {
        inc << ", ";
        for (int no = 0; no < _parameters.size(); ++no)
        {
            dynamic_cast<ParameterForCpp*>(_parameters[no])->generateInclude(inc, debug);
            if (no + 1 < _parameters.size())
                inc << ", ";
        }
    }
    inc << ");" << endl;
}
void TransitionForCpp::generateInclude(ostream& inc, bool debug) const
{
}
void TransitionForCpp::generateCode(ostream& cpp, bool debug) const
{
    cpp << endl;
    cpp << "// ----------------------------------------------------" << endl;
    cpp << "void " << _begstate->map()->name() << '_'  << _begstate->name() << "::";
    Parameter context("context");
    context.type() = begstate()->map()->fsm()->context() + '&';
    cpp << declaration(&context) << " {"<< endl;
//					cpp << "(" << begstate()->map()->fsm()->context() << "&context) {" << endl ;
    cpp << tab;

    cpp << _begstate->map()->fsm()->klass() << "& ctxt = context.getOwner();" << endl;
    cpp << endl;


	List<GuardPtr>::const_iterator guard;
	for (guard = guards().begin(); guard != guards().end(); ++guard)
    {
        if ((*guard) != NULL)
        {
            if ((*guard)->endstate() == NULL)
            {
				List<MODEL::ActionPtr>::const_iterator action;
				for (action = (*guard)->actions().begin(); action != (*guard)->actions().end(); ++action)
                    if (*action != NULL)
                    {
                        ActionForCpp* afc = static_cast<ActionForCpp*>(*action);
                        cpp << "ctxt." << (*action)->display() << endl;
                    }
                continue;
            }

            if ((*guard)->condition())
            {
                ExpressionForCpp* efc = dynamic_cast<ExpressionForCpp*>((*guard)->condition());
                cpp << "if (" << efc->display("ctxt.") << ") {" << endl << tab;
            }
            cpp << "(context.getState()).Exit(context);" << endl;

            if ((*guard)->actions().size() > 0)
            {
                if (RAPP)
                {
                    cpp << "context.clearState();" << endl;
                }
                if (RAPP)
                {
                    cpp << "try {" << endl << tab;
                }
            }

            cpp << "context.setState(context." << (*guard)->endstate()->name() << ");" << endl;
            if (!RAPP)
            {
                cpp << "(context.getState()).Entry(context);" << endl;
            }

			List<MODEL::ActionPtr>::const_iterator action;
			for (action = (*guard)->actions().begin(); action != (*guard)->actions().end(); ++action)
                if (*action != NULL)
                {
                    ActionForCpp* afc = static_cast<ActionForCpp*>(*action);
                    cpp << "ctxt." << (*action)->display() << endl;
                }

            if ((*guard)->actions().size() > 0)
            {
                if (RAPP)
                {
                    cpp << back << "} catch (...) {" << endl << tab;
                    cpp << "context.setState(context." << (*guard)->endstate()->name() << ");" << endl;
                    cpp << "throw;" << endl << back;
                    cpp << "}" << endl;
                }
            }
            if (RAPP)
            {
                cpp << "(context.getState()).Entry(context);" << endl;
            }
            if ((*guard)->condition())
                cpp << back << "} else " << endl;
        }
    }
    if (_guards[0]->condition())
    {
        Parameter context("context");
        cpp << "{" << endl;
        MapForCpp* mfcpp = dynamic_cast<MapForCpp*>(_begstate->map());
        cpp << "     " << mfcpp->defaultStateName() << "::" << call("context") << ";" << endl;
        cpp << "}" << endl;
    }
    cpp << "return;" << back << endl;
    cpp << "};" << endl;
}
}
}
}
}
