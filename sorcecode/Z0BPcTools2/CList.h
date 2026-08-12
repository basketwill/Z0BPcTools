/*____________________________________________________________________________
		Copyright (C) 2002 PGP Corporation
        All rights reserved.

        $Id: CList.h 9679 2002-08-06 20:11:24Z dallen $
____________________________________________________________________________*/

#ifndef Included_CList_h	// [
#define Included_CList_h

typedef unsigned int uint32_t;

#define IsntNull( p )	( (BOOL) ( (p) != NULL ) )
#define IsNull( p )		( (BOOL) ( (p) == NULL ) )
// Types
#define UNUSED(a) a;

#define NOT_COPYABLE(type)												\
	private:															\
	type(const type& rhs);											\
	type&	operator=(const type& rhs);

#define DECLARE_MODALSTATE(type)										\
	NOT_COPYABLE(type)													\
	\
	protected:															\
	type() { }														\
	~type() { }


template <typename T> class CList;


// Class CListableObject

// Inherit from this class to make your object usable with CList.
template <typename T> class CListableObject
{
	friend class CList<T>;

public:
	CListableObject() : _mPrev(NULL), _mNext(NULL) { }
	~CListableObject() { }

private:
	T	*_mPrev, *_mNext;
};


// Class CList

template <typename T> class CList
{
	NOT_COPYABLE(CList)

public:
	CList() : mHead(NULL), mTail(NULL), mCount(0) { }
	~CList() { }

	uint32_t	Count() const {return mCount;}
	bool		IsEmpty() const {return (mCount == 0);}

	bool		Contains(const T *pItem) const;

	T *		Next(T *pItem) const {return pItem->_mNext;}
	T *		Prev(T *pItem) const {return pItem->_mPrev;}
	T *		Head() const {return mHead;}
	T *		Tail() const {return mTail;}

	void	AddHead(T *pItem);
	void	AddTail(T *pItem);

	void	InsertAfter(T *pItem, T *pNewItem);
	void	InsertBefore(T *pItem, T *pNewItem);

	T *		Remove(T *pItem);

	void	Empty();
	void	EmptyWithDelete();

private:
	T	*mHead;
	T	*mTail;

	uint32_t	mCount;
};


// Class CList template member functions.

template <typename T> 
bool 
CList<T>::Contains(const T *pItem) const
{
	//pgpAssertAddrValid(pItem, T);

	T	*pCurItem = mHead;

	while (IsntNull(pCurItem))
	{
		if (pCurItem == pItem)
			return TRUE;

		pCurItem = pCurItem->_mNext;
	}

	return FALSE;
}

template <typename T> 
void 
CList<T>::AddHead(T *pItem)
{
	//pgpAssertAddrValid(pItem, T);
	//pgpAssert(!Contains(pItem));

	if (IsNull(mHead))
	{
		mHead = mTail = pItem;
		pItem->_mNext = pItem->_mPrev = NULL;
	}
	else
	{
		mHead->_mPrev = pItem;
		pItem->_mNext = mHead;
		pItem->_mPrev = NULL;

		mHead = pItem;
	}

	mCount++;
}

template <typename T> 
void 
CList<T>::AddTail(T *pItem)
{
	//pgpAssertAddrValid(pItem, T);
	//pgpAssert(!Contains(pItem));

	if (IsNull(mTail))
	{
		mHead = mTail = pItem;
		pItem->_mNext = pItem->_mPrev = NULL;
	}
	else
	{
		mTail->_mNext = pItem;
		pItem->_mPrev = mTail;
		pItem->_mNext = NULL;

		mTail = pItem;
	}

	mCount++;
}

template <typename T> 
void 
CList<T>::InsertAfter(T *pItem, T *pNewItem)
{
	//pgpAssertAddrValid(pItem, T);
	//pgpAssertAddrValid(pNewItem, T);
	//pgpAssert(Contains(pItem) && !Contains(pNewItem));
	
	pNewItem->_mPrev = pItem;
	pNewItem->_mNext = pItem->_mNext;

	if (IsntNull(pNewItem->_mNext))
		pNewItem->_mNext->_mPrev = pNewItem;

	pItem->_mNext = pNewItem;

	if (mTail == pItem)
		mTail = pNewItem;

	mCount++;
}

template <typename T> 
void 
CList<T>::InsertBefore(T *pItem, T *pNewItem)
{	
	//pgpAssertAddrValid(pItem, T);
	//pgpAssertAddrValid(pNewItem, T);
	//pgpAssert();
	//Contains(pItem) && !Contains(pNewItem);
	pNewItem->_mPrev = pItem->_mPrev;
	pNewItem->_mNext = pItem;

	if (IsntNull(pNewItem->_mPrev))
		pNewItem->_mPrev->_mNext = pNewItem;

	pItem->_mPrev = pNewItem;

	if (mHead == pItem)
		mHead = pNewItem;

	mCount++;
}

template <typename T> 
T * 
CList<T>::Remove(T *pItem)
{
	Contains(pItem);

	if (IsntNull(pItem->_mPrev))
		pItem->_mPrev->_mNext = pItem->_mNext;

	if (IsntNull(pItem->_mNext))
		pItem->_mNext->_mPrev = pItem->_mPrev;

	if (mHead == pItem)
		mHead = pItem->_mNext;

	if (mTail == pItem)
		mTail = pItem->_mPrev;

	pItem->_mNext = pItem->_mPrev = NULL;
	mCount--;

	return pItem;
}

template <typename T> 
void 
CList<T>::Empty()
{
	mHead = NULL;
	mTail = NULL;
	mCount = 0;
}

template <typename T> 
void 
CList<T>::EmptyWithDelete()
{
	while (IsntNull(mHead))
	{
		delete Remove(mHead);
	}
}

#endif	// ] Included_CList_h
