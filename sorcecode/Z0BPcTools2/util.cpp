#include "stdafx.h"
#include "util.h"

TCHAR *
CPSkipWhitespace(
	LPTSTR lpszIn
	)
{
	
	while (*lpszIn == ' ' || *lpszIn == '\t') {
		lpszIn++;
	}
	return (TCHAR *) lpszIn;
}                   /* CPSkipWhiteSpace() */


ULONG GetTextSize(LPTSTR lpText)
{
	int len = 0;
	if (lpText)
	{
		len = lstrlen(lpText);
		int TotleLen = 0;
		for(int i = 0; i < len;i++)
		{
			if (lpText[i] > 0xFE )
			{
				TotleLen += 2;
			}
			else
			{
				TotleLen++;
			}
		}

		if (TotleLen )
		{
			len = TotleLen;
		}
	}

	return len;
}