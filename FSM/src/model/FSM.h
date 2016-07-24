/*
 * FSM.h
 *
 *  Created on: 16 janv. 2016
 *      Author: FrancisANDRE
 */

#ifndef POCO_FSM_MODEL_FSM_H_
#define POCO_FSM_MODEL_FSM_H_
#include "Poco/FileStream.h"
#include "Poco/Path.h"
using Poco::Path;

#include "SMC.h"
#include "Compilable.h"

namespace Poco {
	namespace FSM {
		namespace PARSER {
			class Parser;
		}
	}
}
using Poco::FSM::PARSER::Parser;

namespace Poco {
	namespace FSM {
		namespace MODEL {

			class FSM  : public Compilable {
			public:
				FSM(PARSER::Parser* parser) : Compilable(), _parser(parser) {
				}
				virtual ~FSM() {
				}


				void    setcontext()					{ _context = _fsmClass + "<" + _klass + ">"; }
				string& startState()					{ return _startState; }
				string& source()						{ return _source; }
//				string& context()						{ return _fsmClass; }
				string& fsmfile()						{ return _fsmFile; }
				string& fsmclass()						{ return _fsmClass; }
				string& klass()							{ return _klass; }
				string& header()						{ return _header; }
				string& returnt()						{ return _return; }
				List<string>& packages()				{ return _packages; }
				List<string>& includes()				{ return _includes; }
				List<string>& imports()					{ return _imports; }
				List<string>& declares()				{ return _declares; }
				bool& updated()							{ return _updated; }
				Parser* parser() const					{ return _parser; }

				const string& startState() const		{ return _startState; }
				const string& context()const 			{ return _context; }
				const string& fsmfile()const 			{ return _fsmFile; }
				const string& fsmclass()const 			{ return _fsmClass; }
				const string& klass()const 				{ return _klass; }
				const string& header()const 			{ return _header; }
				const List<string>& packages()const		{ return _packages; }
				const List<string>& includes()const		{ return _includes; }
				const List<string>& imports()const 		{ return _imports; }
				const List<string>& declares()const		{ return _declares; }
				const List<MapPtr>& maps()const			{ return _maps; }
				bool updated() const					{ return _updated; }

				void add(MapPtr map);
				void addPackage(const string& package);
				void addImport(const string& import);
				void addInclude(const string& inc);
				void addDeclare(const string& declare);
				void print(Print& print) const;

				virtual bool updated(const Path& out) = 0;

			protected:
				Parser*		_parser = nullptr;

				// The state map's initial state.
				string _startState;

				// Raw source code appearing at the beginning of the state
				// map source file.
				string _source;

				// This state map is associated with this class.
				string _context;
				string _fsmClass;

				// generated finite state machine class file name.
				string _fsmFile;

				// This map is implemented in the class with given name
				string _klass;

				// Where the associated class is defined.
				string _header;

				// For C++ only. List of include files. Will be output to
				// the .cpp file in the same order as they appear in the
				// .sm file.
				List<string> _includes;

				// This code is placed in this package/namespace.
				List<string> _packages;

				// Place names of imports in this list.
				List<string> _imports;

				// Place forward declarations in this list.
				List<string> _declares;

				// The context class access level. Empty string by default.
				string _accessLevel;

				// The line where %start, etc. should appear.
				// Used in error messages.
				int _headerLine;

				// The state maps.
				List<MapPtr> _maps;

				string _return;

				bool _updated = false;

			};

		}
	}
}
#endif
