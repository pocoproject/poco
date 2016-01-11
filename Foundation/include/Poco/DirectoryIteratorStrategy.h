//
// RecursiveDirectoryIteratorStategies.h
//
// $Id$
//
// Library: Foundation
// Package: Filesystem
// Module:  RecursiveDirectoryIterator
//
// Definitions of the RecursiveDirectoryIterator stategy classes.
//
// Copyright (c) 2012, Applied Informatics Software Engineering GmbH.
// and Contributors.
//
// SPDX-License-Identifier:	BSL-1.0
//


#ifndef Foundation_RecursiveDirectoryIteratorStrategy_INCLUDED
#define Foundation_RecursiveDirectoryIteratorStrategy_INCLUDED


#include "Poco/Foundation.h"
#include "Poco/DirectoryIterator.h"
#include <stack>
#include <queue>
#include <functional>


namespace Poco {


class Foundation_API AbstractTraverseErrorCallback
	/// This is the base class for all instantiations of
	/// the TraverseErrorCallback template.
{
public:
	AbstractTraverseErrorCallback()
	{
	}

	AbstractTraverseErrorCallback(const AbstractTraverseErrorCallback& callback)
	{
	}

	virtual ~AbstractTraverseErrorCallback()
	{
	}
	
	AbstractTraverseErrorCallback& operator = (const AbstractTraverseErrorCallback& callback)
	{
		return *this;
	}

	virtual void invoke(const std::string& path) const = 0;
	virtual AbstractTraverseErrorCallback* clone() const = 0;
};


template <class C> 
class TraverseErrorCallback: public AbstractTraverseErrorCallback
	/// This template class implements an adapter that sits between
	/// a TraverseBase iterator and an object's method invoked during a
	/// directory read error.
{
public:
	typedef void (C::*Callback)(const std::string& path);

	TraverseErrorCallback(C& object, Callback method)
        : _pObject(&object), 
          _method(method)
	{
	}

	TraverseErrorCallback(const TraverseErrorCallback& callback)
        : _pObject(callback._pObject), 
          _method(callback._method)
	{
	}

	~TraverseErrorCallback()
	{
	}

	TraverseErrorCallback& operator = (const TraverseErrorCallback& callback)
	{
		if (&callback != this)
		{
			_pObject = callback._pObject;
			_method  = callback._method;
		}
		return *this;
	}

	void invoke(const std::string& path) const
	{
		(_pObject->*_method)(path);
	}

	AbstractTraverseErrorCallback* clone() const
	{
		return new TraverseErrorCallback(*this);
	}

private:
	TraverseErrorCallback();

	C*       _pObject;
	Callback _method;
};


class Foundation_API TraverseBase
{
public:
	typedef std::stack<DirectoryIterator> Stack;
	typedef std::pointer_to_unary_function<const Stack&, UInt16> DepthFunPtr;

	enum
	{
		D_INFINITE = 0 /// Special value for infinite traverse depth.
	};

	TraverseBase(DepthFunPtr depthDeterminer, UInt16 maxDepth = D_INFINITE);

    TraverseBase& setOnError(const AbstractTraverseErrorCallback& cb);
        /// Binds the option to the given method.
        ///
        /// The callback method will be called if the Traverse class fails
        /// to read a directory. 
        ///
        /// Usage:
        ///     onError(TraverseErrorCallback<MyClass>(this, &MyClass::myCallback));

protected:
	bool isFiniteDepth();
	bool isDirectory(Poco::File& file);
	void invokeOnError(const std::string& path) const;

	DepthFunPtr _depthDeterminer;
	UInt16 _maxDepth;
	DirectoryIterator _itEnd;
	AbstractTraverseErrorCallback* _pCallback;

private:
	TraverseBase();
	TraverseBase(const TraverseBase&);
	TraverseBase& operator=(const TraverseBase&);
};


class Foundation_API ChildrenFirstTraverse: public TraverseBase
{
public:
	ChildrenFirstTraverse(DepthFunPtr depthDeterminer, UInt16 maxDepth = D_INFINITE);

	const std::string next(Stack* itStack, bool* isFinished);

private:
	ChildrenFirstTraverse();
	ChildrenFirstTraverse(const ChildrenFirstTraverse&);
	ChildrenFirstTraverse& operator=(const ChildrenFirstTraverse&);
};


class Foundation_API SiblingsFirstTraverse: public TraverseBase
{
public:
	SiblingsFirstTraverse(DepthFunPtr depthDeterminer, UInt16 maxDepth = D_INFINITE);

	const std::string next(Stack* itStack, bool* isFinished);

private:
	SiblingsFirstTraverse();
	SiblingsFirstTraverse(const SiblingsFirstTraverse&);
	SiblingsFirstTraverse& operator=(const SiblingsFirstTraverse&);

	std::stack<std::queue<std::string> > _dirsStack;
};


} // namespace Poco


#endif // Foundation_RecursiveDirectoryIteratorStrategy_INCLUDED
