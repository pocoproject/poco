/*
 * MapForCpp.cpp
 *
 *  Created on: 19 janv. 2016
 *      Author: FrancisANDRE
 */

#include <iostream>
#include "parser/IndentStream.h"
#include "model/Parameter.h"
#include "cpp/MapForCpp.h"
#include "cpp/StateForCpp.h"
#include "cpp/FSMForCpp.h"
#include "cpp/TransitionForCpp.h"

using Poco::FSM::MODEL::Parameter;
using Poco::FSM::MODEL::StatePtr;

using namespace std;

namespace Poco
{
namespace FSM
{
namespace PARSER
{
namespace CPP
{

string MapForCpp::defaultStateName() const
{
    return name() + '_' + defaultState()->name();
}

string MapForCpp::startStateName() const
{
    return fsm()->startState();
}

static const int LL = 50;

void MapForCpp::generateInclude(ostream& inc, bool debug) const
{
    string comment;
    {
        comment = "// Forward declarations.";
        comment.resize(LL, '-');
        inc << comment << endl;
        inc << "class " << name() << ";" << endl;

        for (const auto& state : states())
        {
            dynamic_cast<StateForCpp*>(state.second)->generateForwarDeclaration(inc, debug);
        }

        inc << "class " << defaultStateName() << ";" << endl;
        inc << "class " << fsm()->klass() << "State;" << endl;
        inc << "template<typename Derived> class " << fsm()->fsmclass() << ";" << endl;
        inc << "class " << fsm()->klass() << ";" << endl;
        inc << endl;
        inc << endl;
    }

    {
        comment = "// Class State.";
        comment.resize(LL, '-');
        inc << comment << endl;
        inc << "class " << fsm()->klass() << "State : public statemap::State {" << endl;
        inc << "public:" << endl << tab;

        inc << fsm()->klass() << "State(const char *name, int no) : statemap::State(name, no) {};" << endl;
        inc << endl;
        inc << "virtual void Entry(" << fsm()->context() << "&) {};" << endl;
        inc << "virtual void Exit(" << fsm()->context() << "&) {};" << endl;
        inc << endl;

#if 0
        for (const auto& state : states())
        {
            StateForCpp* sfc = dynamic_cast<StateForCpp*>(state.second);
            sfc->generateVirtualTransitions(inc);
        }
#else
        for (const auto& transition : transitions())
            if (transition.second)
            {
                Parameter context("context");
                context.type() = fsm()->context() + '&';
                TransitionForCpp* tfc = static_cast<TransitionForCpp*>(transition.second);
                inc << "virtual void " << tfc->declaration(&context) << ";" << endl;
            }

#endif
        inc << back << "protected:" << endl << tab;
        inc << "virtual void Default(" << fsm()->context() << "& context);" << endl << back;
        inc << "};" << endl;
        inc << endl;
    }

    {
        comment = "// FSM map default state class.";
        comment.resize(LL, '-');
        inc << comment << endl;
        inc << "class " << defaultStateName() << " : public " << fsm()->klass() << "State {" << endl;
        inc << "public:" << endl << tab;
        inc << defaultStateName() << " (const char *name, int stateId) : " << fsm()->klass() << "State(name, stateId)	{}" << endl;
        inc << back << "};";
        inc << endl;
        inc << endl;

        comment = "// FSM map states class.";
        comment.resize(LL, '-');
        inc << comment << endl;
        for (const auto& state : states())
        {
            StateForCpp* sfc = dynamic_cast<StateForCpp*>(state.second);
            sfc->generateDefinition(inc, debug);
        }
    }


    {
        comment = "// FSM map class.";
        comment.resize(LL, '-');
        inc << comment << endl;
        inc << "class " << name() << " {" << endl;
        inc << "public:" << endl << tab;
        inc << name() << "();" << endl;
        for (const auto& state : states())
        {
            const StatePtr sp = state.second;
            inc << name() + '_' + sp->name() << ' ' << sp->name() << ";" << endl;
        }
        inc << back << "};" << endl;
        inc << endl;
    }

    {
        comment = "// FSM context class ";
        comment.resize(LL, '-');
        inc << comment << endl;

        inc << "template<typename Derived>" << endl;
        inc << "class " << fsm()->fsmclass() << " : public statemap::FSMContext, public " << name() << " {" << endl;
        inc << "public:" << endl << tab;

        inc << "explicit " << endl;
        inc << fsm()->fsmclass() << "(statemap::Notifier& notifier) : FSMContext(notifier, " << startStateName() << ") {};" << endl;
        inc << fsm()->fsmclass() << "(statemap::Notifier& notifier, const statemap::State& state) : FSMContext(notifier, state) {};" << endl;
        inc << endl;

        inc << "void setStartState(const statemap::State& state) {" << endl;
        inc << "	setState(state);" << endl;
        inc << "}" << endl;
        inc << endl;

        inc << "virtual void enterStartState() {" << endl;
        inc << "    getState().Entry(*this);" << endl;
        inc << "    return;" << endl;
        inc << "}" << endl;
        inc << endl;

        inc << "Derived& getOwner() {" << endl;
        inc << "    return (*static_cast<Derived*>(this));" << endl;
        inc << "};" << endl;
        inc << endl;

        inc << "const Derived& getOwner() const {" << endl;
        inc << "    return (*static_cast<const Derived*>(this));" << endl;
        inc << "};" << endl;
        inc << endl;

        inc << fsm()->klass() << "State& getState() const {" << endl;
        inc << "    if (_state == nullptr) {" << endl;
        inc << "        throw statemap::StateUndefinedException();" << endl;
        inc << "    }" << endl;
        inc << "     return (dynamic_cast<" << fsm()->klass() << "State&>(*_state));" << endl;
        inc << "};" << endl;
        inc << endl;

        for (const auto& transition : transitions())
            if (transition.second)
            {
                TransitionForCpp* tfc = static_cast<TransitionForCpp*>(transition.second);
                if (fsm()->returnt().empty())
                    inc << "void ";
                else
                    inc << fsm()->returnt() + " ";

                inc << tfc->declaration() << " {" << endl;
                inc << "    setTransition(\"" << tfc->Transition::name() << "\");" << endl;
                inc << "    getState()." << tfc->call("*this") << ";" << endl;
                inc << "    setTransition(nullptr);" << endl;
                inc << "};" << endl;
            }
        inc << back;
        inc << "};" << endl;

    }
}
void MapForCpp::generateCode(ostream& cpp, bool debug) const
{
    string comment;
    cpp << "// FSM Map constructor." << endl;
    cpp << name() << "::" << name() << "() :" << endl << tab;
    int no = 0;
    for (const auto& state : states())
    {
        StateForCpp* sfc = dynamic_cast<StateForCpp*>(state.second);
        sfc->generateDeclaration(cpp, no++);
        if (no < states().size())
            cpp << ",";
        cpp << endl;
    }
    cpp << back << "{}" << endl;

    cpp << endl;
    comment = "// Class state ";
    comment.resize(LL, '-');
    cpp << comment << endl;
    for (const auto& transition : transitions())
        if (transition.second)
        {
            Parameter context("context");
            context.type() = fsm()->context() + '&';
            TransitionForCpp* tfc = static_cast<TransitionForCpp*>(transition.second);
            if (fsm()->returnt().empty())
                cpp << "void ";
            else
                cpp << fsm()->returnt() + " ";

            cpp << fsm()->klass() << "State::" << tfc->declaration(&context) << " {" << endl;
            cpp << "     Default(context);" << endl;
            cpp << "}" << endl;
        }

    cpp << "void " << fsm()->klass() << "State::Default(" << fsm()->context() << "& context) {" << endl;
    cpp << "    throw TransitionUndefinedException(context.getState().getName(), context.getTransition());" << endl;
    cpp << "    return;" << endl;
    cpp << "}" << endl;


    for (const auto& state : states())
    {
        StateForCpp* sfc = dynamic_cast<StateForCpp*>(state.second);
        sfc->generateCode(cpp, debug);
    }
}
}
}
}
}