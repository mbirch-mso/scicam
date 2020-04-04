// DirName.cpp: implementation of the CDirName class.
//
//////////////////////////////////////////////////////////////////////

#include "afxwin.h"

#include "DirName.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

// Get a directory path from a full file path
int GetDirPath(LPCTSTR lpszPathName, LPTSTR lpsDirPath, UINT nMax)
{
	LPTSTR lpszTemp = (LPTSTR)lpszPathName;
	for (LPCTSTR lpsz = lpszPathName; *lpsz != '\0'; lpsz = _tcsinc(lpsz))
	{
		// remember last directory/drive separator
		if (*lpsz == '\\' || *lpsz == '/' || *lpsz == ':')
			lpszTemp = (LPTSTR)_tcsinc(lpsz);
	}

	// copy it into the buffer if one provided
	nMax = lstrlen(lpszPathName) - lstrlen(lpszTemp);
	if (lpsDirPath)
	{	lstrcpyn(lpsDirPath, lpszPathName, nMax);
	}

	return nMax;
}

// Get a file name from a full file path
int GetFileName(LPCTSTR lpszPathName, LPTSTR lpszFileName, UINT nMax)
{
	// always capture the complete file name including extension (if present)
	LPTSTR lpszTemp = (LPTSTR)lpszPathName;
	for (LPCTSTR lpsz = lpszPathName; *lpsz != '\0'; lpsz = _tcsinc(lpsz))
	{
		// remember last directory/drive separator
		if (*lpsz == '\\' || *lpsz == '/' || *lpsz == ':')
			lpszTemp = (LPTSTR)_tcsinc(lpsz);
	}

	// copy it into the buffer if provided
	if (lpszFileName)
	{	lstrcpyn(lpszFileName, lpszTemp, nMax);
	}
	return lstrlen(lpszTemp)+1;
}

// Get a file suffix from a full file path
int GetFileSuffix(LPCTSTR lpszPathName, LPTSTR lpszFileSuffix, UINT nMax)
{
	// always capture the complete file name including extension (if present)
	LPTSTR lpszTemp = (LPTSTR)lpszPathName;
	for (LPCTSTR lpsz = lpszPathName; *lpsz != '\0'; lpsz = _tcsinc(lpsz))
	{
		// remember last suffix separator
		if (*lpsz == '.')
			lpszTemp = (LPTSTR)_tcsinc(lpsz);
	}

	// lpszFileSuffix can be NULL which just returns its position in the path
	if (lpszFileSuffix == NULL)
	{	if (lpszTemp == lpszPathName)
		{	return lstrlen(lpszPathName);
		}
		else
		{	return lpszTemp - lpszPathName;
		}
	}

	// copy it into the buffer if provided
	if (lpszFileSuffix)
	{	if (lpszTemp == lpszPathName)
		{	lstrcpy(lpszFileSuffix, "");
		}
		else
		{	lstrcpyn(lpszFileSuffix, lpszTemp, nMax);
		}
	}
	if (lpszTemp == lpszPathName)
	{	return lstrlen(lpszPathName);
	}
	else
	{	return lpszTemp - lpszPathName;
	}
}