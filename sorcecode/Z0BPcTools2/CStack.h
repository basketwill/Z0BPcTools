/*____________________________________________________________________________
		Copyright (C) 2002 PGP Corporation
        All rights reserved.

        $Id: CStack.h 9679 2002-08-06 20:11:24Z dallen $
____________________________________________________________________________*/

#ifndef Included_CStack_h	// [
#define Included_CStack_h

#include "CList.h"

// Class CStack


template <typename T, bool e = false> class CStack
{
	NOT_COPYABLE(CStack)

public:
	CStack() { }
	~CStack() { }

	uint32_t	Size() const {return mList.Count();}

	void	Push(T *pItem);
	T *		Pop();
	T *     First();
	T *		Next(T* pItem);
	T *     Remove(T* pItem);
	void	Empty() {mList.Empty();}
	void	EmptyWithDelete() {mList.EmptyWithDelete();}

private:
	CList<T>	mList;
};

// Class CStack template member functions.

template <typename T, bool e>
void 
CStack<T, e>::Push(T *pItem)
{
	mList.AddHead(pItem);
}

template <typename T, bool e >
T * 
CStack<T, e>::Pop()
{
	if (Size() == 0)
		return NULL;
	else
		return mList.Remove(mList.Head());
}

template <typename T, bool e >
T * 
CStack<T, e>::First()
{
	if (Size() == 0)
		return NULL;
	else
		return mList.Head();
}

template <typename T, bool e >
T *    
CStack<T, e>::Remove(T* pItem)
{
	return mList.Remove(pItem);
}

template <typename T, bool e >
T *		
CStack<T, e>::Next(T* pItem)
{
	return mList.Next(pItem);
}

#endif
