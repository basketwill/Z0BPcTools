#ifndef _symbol_h
#define _symbol_h
#include "CList.h"
#include <stdint.h>
#include <strsafe.h>

#define MAX_SYMBOL_HASH_ARRAY 256
#define MAX_SYMBOL_NAME    64
#define MAX_SYMBOL_MOUDLE  64

class SymbolTable
{
	friend class CSymbol;
	friend class CList<SymbolTable>;
	friend class CHashList;
public:
	SymbolTable()
		:mSymbolPtr(NULL)
		,_mNext(NULL)
		,_mPrev(NULL)
	{
		ZeroMemory(mSymbolName,sizeof(TCHAR)*MAX_SYMBOL_NAME);
	}

	~SymbolTable()
	{
		
	}

	void SetSymbol(uint64_t& u64Ptr ,LPTSTR SymbolName)
	{
		StringCchCopy(mSymbolName,MAX_SYMBOL_NAME,SymbolName);
		mSymbolPtr = u64Ptr;
	}

	void SetSymbol(uint32_t& u32Ptr )
	{

	}

	void SetSymbol(uint16_t& u16Ptr )
	{

	}

	void SetSymbol(uint8_t& u8Ptr )
	{

	}

	const LPTSTR GetSymbolName()
	{
		return mSymbolName;
	}

private:
	SymbolTable*		_mNext;
	SymbolTable*		_mPrev;
	uint64_t			mSymbolPtr;//µØÖ·
	TCHAR				mSymbolName[MAX_SYMBOL_NAME];
};


class CHashList
{
	friend class CSymbol;
public:
	CHashList()
	{
		
	}

	void SetSymbol(uint64_t& u64Ptr,LPTSTR SymbolName,BOOL bForceUpdate = FALSE )
	{
		if (u64Ptr && SymbolName)
		{
			SymbolTable* Sym = new SymbolTable();
			if (Sym)
			{
				Sym->SetSymbol(u64Ptr,SymbolName);
				SymbolHash.AddHead(Sym);
			}
		}
	}

	void SetSymbol(uint32_t& u32Ptr,LPTSTR SymbolName,BOOL bForceUpdate = FALSE )
	{

	}

	SymbolTable* GetSymbol(uint64_t u64ptr)
	{
		if (SymbolHash.Count())
		{
			SymbolTable* SymbolFirst = SymbolHash.Head();
			while (SymbolFirst)
			{
				if (SymbolFirst->mSymbolPtr == u64ptr)
				{
					return SymbolFirst;
				}

				SymbolFirst = SymbolHash.Next(SymbolFirst);
			}
		}
	 
		return NULL;
	}

	~CHashList()
	{
		if (SymbolHash.Count())
		{
// 			SymbolTable* SymbolFirst = SymbolHash.Head();
// 			while (SymbolFirst)
// 			{
// 				delete SymbolFirst;
// 				SymbolFirst = SymbolHash.Next(SymbolFirst);
// 			}
			SymbolHash.EmptyWithDelete();
		}
	}

private:
	CList<SymbolTable> SymbolHash;
};

typedef struct _tagSymbolMoudle
{
	uint64_t mBase;
	uint64_t mSize;
	TCHAR    mName[MAX_SYMBOL_NAME];
}tagSymbolMoudle;

class CSymbol
{
public:
	CSymbol();
	~CSymbol();

	void SetSymbol(uint64_t u64ptr,LPTSTR SymbolName,BOOL bForceUpdate = FALSE);
	const LPTSTR GetSymbol(uint64_t u64ptr);
	void AddSymbolMoudle(uint64_t u64ptr,uint64_t uSize , LPTSTR lpMoudleName);
	const LPTSTR GetMoudleSymbolName(uint64_t u64ptr,int64_t *offset);
private:
	CHashList		m_SymbolTable64[MAX_SYMBOL_HASH_ARRAY];
	tagSymbolMoudle m_SymbolMoudle[MAX_SYMBOL_MOUDLE];
	int				m_SymbolMoudleSize;
};

#endif