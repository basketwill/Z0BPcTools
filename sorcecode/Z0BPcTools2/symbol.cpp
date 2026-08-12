#include "stdafx.h"
#include "symbol.h"

CSymbol::CSymbol()
	:m_SymbolMoudleSize(0)
{
	ZeroMemory(m_SymbolTable64,sizeof(CHashList*)*MAX_SYMBOL_HASH_ARRAY);

	ZeroMemory(m_SymbolMoudle,MAX_SYMBOL_MOUDLE*sizeof(tagSymbolMoudle));
}

CSymbol::~CSymbol()
{
	
}

void CSymbol::SetSymbol(uint64_t u64ptr,LPTSTR SymbolName,BOOL bForceUpdate)
{
	
	uint8_t  temp8ptr  = (u64ptr%MAX_SYMBOL_HASH_ARRAY);
	CHashList* pHashList = &m_SymbolTable64[temp8ptr];
	if(pHashList)
	{
		SymbolTable* Sym = pHashList->GetSymbol(u64ptr);
		if ( Sym )
		{
			if (!bForceUpdate)
			{
				return;
			}

			Sym->SetSymbol(u64ptr,SymbolName);

			return;
		}
		
		pHashList->SetSymbol(u64ptr,SymbolName,bForceUpdate);
	}	
}

void CSymbol::AddSymbolMoudle(
						uint64_t u64ptr,
						uint64_t uSize,
						LPTSTR lpMoudleName)
{
	if ( m_SymbolMoudleSize >= 0 &&
		m_SymbolMoudleSize < MAX_SYMBOL_MOUDLE &&
		lpMoudleName &&
		u64ptr &&
		uSize)
	{
		m_SymbolMoudle[m_SymbolMoudleSize].mBase = u64ptr;
		m_SymbolMoudle[m_SymbolMoudleSize].mSize = uSize;
		StringCchCopy(m_SymbolMoudle[m_SymbolMoudleSize].mName,
			         MAX_SYMBOL_NAME,
					 lpMoudleName);

		m_SymbolMoudleSize++;
	}
}

const LPTSTR CSymbol::GetMoudleSymbolName(uint64_t u64ptr,int64_t *offset)
{
	if ( m_SymbolMoudleSize >= 0 &&
		m_SymbolMoudleSize < MAX_SYMBOL_MOUDLE &&
		offset)
	{
		for ( int i = 0; i < m_SymbolMoudleSize ; i++)
		{
			if ( u64ptr >= m_SymbolMoudle[i].mBase &&
				u64ptr <= (m_SymbolMoudle[i].mBase + m_SymbolMoudle[i].mSize))
			{
				*offset = u64ptr;
				return m_SymbolMoudle[i].mName;
			}
		}
	}

	return NULL;
}

const LPTSTR CSymbol::GetSymbol(uint64_t u64ptr)
{
	uint8_t  temp8ptr  = (u64ptr%MAX_SYMBOL_HASH_ARRAY);
	CHashList* pHashList = &m_SymbolTable64[temp8ptr];

	if(pHashList)
	{
		SymbolTable* Sym = pHashList->GetSymbol(u64ptr);

		if (Sym)
		{
			return Sym->GetSymbolName();
		}
		
	}

	return NULL;
}