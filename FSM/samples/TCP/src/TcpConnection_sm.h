#ifndef TcpConnection_sm_H
#define TcpConnection_sm_H
#include "Poco/FSM/statemap.h"
// Forward declarations.--------------------------
class TCP;
class TCP_CLOSED;
class TCP_CLOSE_WAIT;
class TCP_CLOSING;
class TCP_ClientOpening;
class TCP_Default;
class TCP_ESTABLISHED;
class TCP_FIN_WAIT_1;
class TCP_FIN_WAIT_2;
class TCP_LAST_ACK;
class TCP_LISTEN;
class TCP_SYN_RCVD;
class TCP_SYN_SENT;
class TCP_ServiceOpening;
class TCP_TIME_WAIT;
class TCP_Transmitting;
class TCP_Default;
class TcpConnectionState;
template<typename Derived> class TcpConnectionContext;
class TcpConnection;


// Class State.-----------------------------------
class TcpConnectionState : public statemap::State {
public:
	TcpConnectionState(const char *name, int no) : statemap::State(name, no) {};
	
	virtual void Entry(TcpConnectionContext<TcpConnection>&) {};
	virtual void Exit(TcpConnectionContext<TcpConnection>&) {};
	
	virtual void ACK(TcpConnectionContext<TcpConnection>& context, const TcpSegment& segment);
	virtual void AcceptOpen(TcpConnectionContext<TcpConnection>& context, const TcpSegment& segment);
	virtual void AckTimeout(TcpConnectionContext<TcpConnection>& context);
	virtual void ActiveOpen(TcpConnectionContext<TcpConnection>& context, const sockaddr_in* address);
	virtual void ClientOpened(TcpConnectionContext<TcpConnection>& context, const sockaddr_in* address);
	virtual void Close(TcpConnectionContext<TcpConnection>& context);
	virtual void CloseAckTimeout(TcpConnectionContext<TcpConnection>& context);
	virtual void CloseTimeout(TcpConnectionContext<TcpConnection>& context);
	virtual void ConnAckTimeout(TcpConnectionContext<TcpConnection>& context);
	virtual void FIN(TcpConnectionContext<TcpConnection>& context, const TcpSegment& segment);
	virtual void FIN_ACK(TcpConnectionContext<TcpConnection>& context, const TcpSegment& segment);
	virtual void OpenFailed(TcpConnectionContext<TcpConnection>& context, const char* reason);
	virtual void PSH(TcpConnectionContext<TcpConnection>& context, const TcpSegment& segment);
	virtual void PSH_ACK(TcpConnectionContext<TcpConnection>& context, const TcpSegment& segment);
	virtual void PassiveOpen(TcpConnectionContext<TcpConnection>& context, unsigned short port);
	virtual void RST(TcpConnectionContext<TcpConnection>& context, const TcpSegment& segment);
	virtual void RST_ACK(TcpConnectionContext<TcpConnection>& context, const TcpSegment& segment);
	virtual void SYN(TcpConnectionContext<TcpConnection>& context, const TcpSegment& segment);
	virtual void SYN_ACK(TcpConnectionContext<TcpConnection>& context, const TcpSegment& segment);
	virtual void ServerOpened(TcpConnectionContext<TcpConnection>& context);
	virtual void TransAckTimeout(TcpConnectionContext<TcpConnection>& context);
	virtual void Transmit(TcpConnectionContext<TcpConnection>& context, const char* data, int offset, int size);
	virtual void UNDEF(TcpConnectionContext<TcpConnection>& context, const TcpSegment& segment);
	virtual void URG(TcpConnectionContext<TcpConnection>& context, const TcpSegment& segment);
protected:
	virtual void Default(TcpConnectionContext<TcpConnection>& context);
};

// FSM map default state class.-------------------
class TCP_Default:  public TcpConnectionState { 
public:
	TCP_Default(const char* name, int no) : TCP_Default(name, no) {}


	void PassiveOpen(TcpConnectionContext<TcpConnection>& context, unsigned short port);
	void ActiveOpen(TcpConnectionContext<TcpConnection>& context, const sockaddr_in* address);
	void Transmit(TcpConnectionContext<TcpConnection>& context, const char* data, int offset, int size);
	void FIN(TcpConnectionContext<TcpConnection>& context, const TcpSegment& segment);
	void SYN(TcpConnectionContext<TcpConnection>& context, const TcpSegment& segment);
	void PSH(TcpConnectionContext<TcpConnection>& context, const TcpSegment& segment);
	void ACK(TcpConnectionContext<TcpConnection>& context, const TcpSegment& segment);
	void URG(TcpConnectionContext<TcpConnection>& context, const TcpSegment& segment);
	void FIN_ACK(TcpConnectionContext<TcpConnection>& context, const TcpSegment& segment);
	void SYN_ACK(TcpConnectionContext<TcpConnection>& context, const TcpSegment& segment);
	void PSH_ACK(TcpConnectionContext<TcpConnection>& context, const TcpSegment& segment);
	void UNDEF(TcpConnectionContext<TcpConnection>& context, const TcpSegment& segment);
	void RST(TcpConnectionContext<TcpConnection>& context, const TcpSegment& segment);
	void RST_ACK(TcpConnectionContext<TcpConnection>& context, const TcpSegment& segment);
	void ConnAckTimeout(TcpConnectionContext<TcpConnection>& context);
	void TransAckTimeout(TcpConnectionContext<TcpConnection>& context);
	void CloseAckTimeout(TcpConnectionContext<TcpConnection>& context);
	void CloseTimeout(TcpConnectionContext<TcpConnection>& context);
};

// FSM map states class.--------------------------
// --------------------------------------------------
class TCP_CLOSED:  public TCP_Default { 
public:
	TCP_CLOSED(const char* name, int no) : TCP_Default(name, no) {}
	
	
	void PassiveOpen(TcpConnectionContext<TcpConnection>& context, unsigned short port);
	void ActiveOpen(TcpConnectionContext<TcpConnection>& context, const sockaddr_in* address);
	void AcceptOpen(TcpConnectionContext<TcpConnection>& context, const TcpSegment& segment);
	void Close(TcpConnectionContext<TcpConnection>& context);
};

// --------------------------------------------------
class TCP_CLOSE_WAIT:  public TCP_Default { 
public:
	TCP_CLOSE_WAIT(const char* name, int no) : TCP_Default(name, no) {}
	
	
	void Close(TcpConnectionContext<TcpConnection>& context);
};

// --------------------------------------------------
class TCP_CLOSING:  public TCP_Default { 
public:
	TCP_CLOSING(const char* name, int no) : TCP_Default(name, no) {}
	
	void Entry(TcpConnectionContext<TcpConnection>& context);
	void Exit(TcpConnectionContext<TcpConnection>& context);
	
	void ACK(TcpConnectionContext<TcpConnection>& context, const TcpSegment& segment);
	void CloseAckTimeout(TcpConnectionContext<TcpConnection>& context);
	void UNDEF(TcpConnectionContext<TcpConnection>& context, const TcpSegment& segment);
};

// --------------------------------------------------
class TCP_ClientOpening:  public TCP_Default { 
public:
	TCP_ClientOpening(const char* name, int no) : TCP_Default(name, no) {}
	
	
	void ClientOpened(TcpConnectionContext<TcpConnection>& context, const sockaddr_in* address);
	void OpenFailed(TcpConnectionContext<TcpConnection>& context, const char* reason);
};

// --------------------------------------------------

// --------------------------------------------------
class TCP_ESTABLISHED:  public TCP_Default { 
public:
	TCP_ESTABLISHED(const char* name, int no) : TCP_Default(name, no) {}
	
	
	void FIN(TcpConnectionContext<TcpConnection>& context, const TcpSegment& segment);
	void PSH(TcpConnectionContext<TcpConnection>& context, const TcpSegment& segment);
	void Transmit(TcpConnectionContext<TcpConnection>& context, const char* data, int offset, int size);
	void Close(TcpConnectionContext<TcpConnection>& context);
};

// --------------------------------------------------
class TCP_FIN_WAIT_1:  public TCP_Default { 
public:
	TCP_FIN_WAIT_1(const char* name, int no) : TCP_Default(name, no) {}
	
	void Entry(TcpConnectionContext<TcpConnection>& context);
	void Exit(TcpConnectionContext<TcpConnection>& context);
	
	void ACK(TcpConnectionContext<TcpConnection>& context, const TcpSegment& segment);
	void FIN(TcpConnectionContext<TcpConnection>& context, const TcpSegment& segment);
	void FIN_ACK(TcpConnectionContext<TcpConnection>& context, const TcpSegment& segment);
	void CloseAckTimeout(TcpConnectionContext<TcpConnection>& context);
};

// --------------------------------------------------
class TCP_FIN_WAIT_2:  public TCP_Default { 
public:
	TCP_FIN_WAIT_2(const char* name, int no) : TCP_Default(name, no) {}
	
	void Entry(TcpConnectionContext<TcpConnection>& context);
	void Exit(TcpConnectionContext<TcpConnection>& context);
	
	void FIN(TcpConnectionContext<TcpConnection>& context, const TcpSegment& segment);
	void CloseAckTimeout(TcpConnectionContext<TcpConnection>& context);
	void UNDEF(TcpConnectionContext<TcpConnection>& context, const TcpSegment& segment);
};

// --------------------------------------------------
class TCP_LAST_ACK:  public TCP_Default { 
public:
	TCP_LAST_ACK(const char* name, int no) : TCP_Default(name, no) {}
	
	void Entry(TcpConnectionContext<TcpConnection>& context);
	void Exit(TcpConnectionContext<TcpConnection>& context);
	
	void ACK(TcpConnectionContext<TcpConnection>& context, const TcpSegment& segment);
	void CloseAckTimeout(TcpConnectionContext<TcpConnection>& context);
};

// --------------------------------------------------
class TCP_LISTEN:  public TCP_Default { 
public:
	TCP_LISTEN(const char* name, int no) : TCP_Default(name, no) {}
	
	
	void SYN(TcpConnectionContext<TcpConnection>& context, const TcpSegment& segment);
	void Close(TcpConnectionContext<TcpConnection>& context);
	void RST(TcpConnectionContext<TcpConnection>& context, const TcpSegment& segment);
	void UNDEF(TcpConnectionContext<TcpConnection>& context, const TcpSegment& segment);
};

// --------------------------------------------------
class TCP_SYN_RCVD:  public TCP_Default { 
public:
	TCP_SYN_RCVD(const char* name, int no) : TCP_Default(name, no) {}
	
	void Entry(TcpConnectionContext<TcpConnection>& context);
	void Exit(TcpConnectionContext<TcpConnection>& context);
	
	void RST(TcpConnectionContext<TcpConnection>& context, const TcpSegment& segment);
	void ACK(TcpConnectionContext<TcpConnection>& context, const TcpSegment& segment);
	void Close(TcpConnectionContext<TcpConnection>& context);
	void AckTimeout(TcpConnectionContext<TcpConnection>& context);
};

// --------------------------------------------------
class TCP_SYN_SENT:  public TCP_Default { 
public:
	TCP_SYN_SENT(const char* name, int no) : TCP_Default(name, no) {}
	
	void Entry(TcpConnectionContext<TcpConnection>& context);
	void Exit(TcpConnectionContext<TcpConnection>& context);
	
	void SYN_ACK(TcpConnectionContext<TcpConnection>& context, const TcpSegment& segment);
	void Close(TcpConnectionContext<TcpConnection>& context);
	void ConnAckTimeout(TcpConnectionContext<TcpConnection>& context);
};

// --------------------------------------------------
class TCP_ServiceOpening:  public TCP_Default { 
public:
	TCP_ServiceOpening(const char* name, int no) : TCP_Default(name, no) {}
	
	
	void ServerOpened(TcpConnectionContext<TcpConnection>& context);
	void OpenFailed(TcpConnectionContext<TcpConnection>& context, const char* reason);
};

// --------------------------------------------------
class TCP_TIME_WAIT:  public TCP_Default { 
public:
	TCP_TIME_WAIT(const char* name, int no) : TCP_Default(name, no) {}
	
	void Entry(TcpConnectionContext<TcpConnection>& context);
	void Exit(TcpConnectionContext<TcpConnection>& context);
	
	void FIN_ACK(TcpConnectionContext<TcpConnection>& context, const TcpSegment& segment);
	void CloseTimeout(TcpConnectionContext<TcpConnection>& context);
	void UNDEF(TcpConnectionContext<TcpConnection>& context, const TcpSegment& segment);
};

// --------------------------------------------------
class TCP_Transmitting:  public TCP_Default { 
public:
	TCP_Transmitting(const char* name, int no) : TCP_Default(name, no) {}
	
	void Entry(TcpConnectionContext<TcpConnection>& context);
	void Exit(TcpConnectionContext<TcpConnection>& context);
	
	void ACK(TcpConnectionContext<TcpConnection>& context, const TcpSegment& segment);
	void PSH_ACK(TcpConnectionContext<TcpConnection>& context, const TcpSegment& segment);
	void PSH(TcpConnectionContext<TcpConnection>& context, const TcpSegment& segment);
	void FIN(TcpConnectionContext<TcpConnection>& context, const TcpSegment& segment);
	void Close(TcpConnectionContext<TcpConnection>& context);
	void TransAckTimeout(TcpConnectionContext<TcpConnection>& context);
};

// FSM map class.---------------------------------
class TCP {
public:
	TCP();
	TCP_CLOSED CLOSED;
	TCP_CLOSE_WAIT CLOSE_WAIT;
	TCP_CLOSING CLOSING;
	TCP_ClientOpening ClientOpening;
	TCP_Default Default;
	TCP_ESTABLISHED ESTABLISHED;
	TCP_FIN_WAIT_1 FIN_WAIT_1;
	TCP_FIN_WAIT_2 FIN_WAIT_2;
	TCP_LAST_ACK LAST_ACK;
	TCP_LISTEN LISTEN;
	TCP_SYN_RCVD SYN_RCVD;
	TCP_SYN_SENT SYN_SENT;
	TCP_ServiceOpening ServiceOpening;
	TCP_TIME_WAIT TIME_WAIT;
	TCP_Transmitting Transmitting;
};

// FSM context class -----------------------------
template<typename Derived>
class TcpConnectionContext : public statemap::FSMContext, public TCP {
public:
	explicit 
	TcpConnectionContext(statemap::Notifier* notifier = NULL) : FSMContext(TCP::CLOSED, notifier) {};
	TcpConnectionContext(const statemap::State& state, statemap::Notifier* notifier = NULL) : FSMContext(state, notifier) {};
	
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
	
	TcpConnectionState& getState() const {
	    if (_state == NULL) {
	        throw statemap::StateUndefinedException();
	    }
	     return (dynamic_cast<TcpConnectionState&>(*_state));
	};
	
	void ACK(const TcpSegment& segment) {
	    setTransition("ACK");
	    getState().ACK(*this, segment);
	    setTransition(NULL);
	};
	void AcceptOpen(const TcpSegment& segment) {
	    setTransition("AcceptOpen");
	    getState().AcceptOpen(*this, segment);
	    setTransition(NULL);
	};
	void AckTimeout() {
	    setTransition("AckTimeout");
	    getState().AckTimeout(*this);
	    setTransition(NULL);
	};
	void ActiveOpen(const sockaddr_in* address) {
	    setTransition("ActiveOpen");
	    getState().ActiveOpen(*this, address);
	    setTransition(NULL);
	};
	void ClientOpened(const sockaddr_in* address) {
	    setTransition("ClientOpened");
	    getState().ClientOpened(*this, address);
	    setTransition(NULL);
	};
	void Close() {
	    setTransition("Close");
	    getState().Close(*this);
	    setTransition(NULL);
	};
	void CloseAckTimeout() {
	    setTransition("CloseAckTimeout");
	    getState().CloseAckTimeout(*this);
	    setTransition(NULL);
	};
	void CloseTimeout() {
	    setTransition("CloseTimeout");
	    getState().CloseTimeout(*this);
	    setTransition(NULL);
	};
	void ConnAckTimeout() {
	    setTransition("ConnAckTimeout");
	    getState().ConnAckTimeout(*this);
	    setTransition(NULL);
	};
	void FIN(const TcpSegment& segment) {
	    setTransition("FIN");
	    getState().FIN(*this, segment);
	    setTransition(NULL);
	};
	void FIN_ACK(const TcpSegment& segment) {
	    setTransition("FIN_ACK");
	    getState().FIN_ACK(*this, segment);
	    setTransition(NULL);
	};
	void OpenFailed(const char* reason) {
	    setTransition("OpenFailed");
	    getState().OpenFailed(*this, reason);
	    setTransition(NULL);
	};
	void PSH(const TcpSegment& segment) {
	    setTransition("PSH");
	    getState().PSH(*this, segment);
	    setTransition(NULL);
	};
	void PSH_ACK(const TcpSegment& segment) {
	    setTransition("PSH_ACK");
	    getState().PSH_ACK(*this, segment);
	    setTransition(NULL);
	};
	void PassiveOpen(unsigned short port) {
	    setTransition("PassiveOpen");
	    getState().PassiveOpen(*this, port);
	    setTransition(NULL);
	};
	void RST(const TcpSegment& segment) {
	    setTransition("RST");
	    getState().RST(*this, segment);
	    setTransition(NULL);
	};
	void RST_ACK(const TcpSegment& segment) {
	    setTransition("RST_ACK");
	    getState().RST_ACK(*this, segment);
	    setTransition(NULL);
	};
	void SYN(const TcpSegment& segment) {
	    setTransition("SYN");
	    getState().SYN(*this, segment);
	    setTransition(NULL);
	};
	void SYN_ACK(const TcpSegment& segment) {
	    setTransition("SYN_ACK");
	    getState().SYN_ACK(*this, segment);
	    setTransition(NULL);
	};
	void ServerOpened() {
	    setTransition("ServerOpened");
	    getState().ServerOpened(*this);
	    setTransition(NULL);
	};
	void TransAckTimeout() {
	    setTransition("TransAckTimeout");
	    getState().TransAckTimeout(*this);
	    setTransition(NULL);
	};
	void Transmit(const char* data, int offset, int size) {
	    setTransition("Transmit");
	    getState().Transmit(*this, data, offset, size);
	    setTransition(NULL);
	};
	void UNDEF(const TcpSegment& segment) {
	    setTransition("UNDEF");
	    getState().UNDEF(*this, segment);
	    setTransition(NULL);
	};
	void URG(const TcpSegment& segment) {
	    setTransition("URG");
	    getState().URG(*this, segment);
	    setTransition(NULL);
	};
};
#endif
