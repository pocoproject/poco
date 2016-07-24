#ifndef cpp_ex4_Stoplight_sm_H
#define cpp_ex4_Stoplight_sm_H
#include "Poco/FSM/statemap.h"
namespace cpp_ex4 {
	// Forward declarations.--------------------------
	class StopMap;
	class StopMap_EastWestGreen;
	class StopMap_EastWestYellow;
	class StopMap_NorthSouthGreen;
	class StopMap_NorthSouthYellow;
	class StopMap_Default;
	class StoplightState;
	template<typename Derived> class StoplightContext;
	class Stoplight;
	
	
	// Class State.-----------------------------------
	class StoplightState : public statemap::State {
	public:
		StoplightState(const char *name, int no) : statemap::State(name, no) {};
		
		virtual void Entry(StoplightContext<Stoplight>&) {};
		virtual void Exit(StoplightContext<Stoplight>&) {};
		
		virtual void Timeout(StoplightContext<Stoplight>& context);
	protected:
		virtual void Default(StoplightContext<Stoplight>& context);
	};
	
	// FSM map default state class.-------------------
	class StopMap_Default : public StoplightState {
	public:
		StopMap_Default (const char *name, int stateId) : StoplightState(name, stateId)	{}
	};
	
	// FSM map states class.--------------------------
	// --------------------------------------------------
	class StopMap_EastWestGreen:  public StopMap_Default { 
	public:
		StopMap_EastWestGreen(const char* name, int no) : StopMap_Default(name, no) {}
		
		void Entry(StoplightContext<Stoplight>& context);
		
		void Timeout(StoplightContext<Stoplight>& context);
	};
	
	// --------------------------------------------------
	class StopMap_EastWestYellow:  public StopMap_Default { 
	public:
		StopMap_EastWestYellow(const char* name, int no) : StopMap_Default(name, no) {}
		
		void Entry(StoplightContext<Stoplight>& context);
		
		void Timeout(StoplightContext<Stoplight>& context);
	};
	
	// --------------------------------------------------
	class StopMap_NorthSouthGreen:  public StopMap_Default { 
	public:
		StopMap_NorthSouthGreen(const char* name, int no) : StopMap_Default(name, no) {}
		
		void Entry(StoplightContext<Stoplight>& context);
		
		void Timeout(StoplightContext<Stoplight>& context);
	};
	
	// --------------------------------------------------
	class StopMap_NorthSouthYellow:  public StopMap_Default { 
	public:
		StopMap_NorthSouthYellow(const char* name, int no) : StopMap_Default(name, no) {}
		
		void Entry(StoplightContext<Stoplight>& context);
		
		void Timeout(StoplightContext<Stoplight>& context);
	};
	
	// FSM map class.---------------------------------
	class StopMap {
	public:
		StopMap();
		StopMap_EastWestGreen EastWestGreen;
		StopMap_EastWestYellow EastWestYellow;
		StopMap_NorthSouthGreen NorthSouthGreen;
		StopMap_NorthSouthYellow NorthSouthYellow;
	};
	
	// FSM context class -----------------------------
	template<typename Derived>
	class StoplightContext : public statemap::FSMContext, public StopMap {
	public:
		explicit 
		StoplightContext(statemap::Notifier& notifier) : FSMContext(notifier, StopMap::NorthSouthGreen) {};
		StoplightContext(statemap::Notifier& notifier, const statemap::State& state) : FSMContext(notifier, state) {};
		
		void setStartState(const statemap::State& state) {
			setState(state);
		}
		
		virtual void enterStartState() {
		    getState().Entry(*this);
		    return;
		}
		
		Derived& getOwner() {
		    return (*static_cast<Derived*>(this));
		};
		
		const Derived& getOwner() const {
		    return (*static_cast<const Derived*>(this));
		};
		
		StoplightState& getState() const {
		    if (_state == nullptr) {
		        throw statemap::StateUndefinedException();
		    }
		     return (dynamic_cast<StoplightState&>(*_state));
		};
		
		void Timeout() {
		    setTransition("Timeout");
		    getState().Timeout(*this);
		    setTransition(nullptr);
		};
	};
}
#endif
