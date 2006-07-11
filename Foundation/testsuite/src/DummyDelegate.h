//
// DummyDelegate.h
//

#ifndef DummyDelegate_INCLUDED
#define DummyDelegate_INCLUDED

class DummyDelegate
{
public:
	DummyDelegate ();
	virtual ~DummyDelegate ();

	void onSimple ( const void* pSender, int& i );
	void onSimple2 ( const void* pSender, int& i );
};

#endif // DummyDelegate_INCLUDED
