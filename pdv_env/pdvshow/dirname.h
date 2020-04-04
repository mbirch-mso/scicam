// DirName.h: interface for the CDirName class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_DIRNAME_H__6DB27E14_2E08_11D2_97DD_00104B357776__INCLUDED_)
#define AFX_DIRNAME_H__6DB27E14_2E08_11D2_97DD_00104B357776__INCLUDED_

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000

_declspec(dllexport) int GetDirPath(LPCTSTR lpszPathName, LPTSTR lpsDirPath, UINT nMax);
_declspec(dllexport)  int GetFileName(LPCTSTR lpszPathName, LPTSTR lpszFileName, UINT nMax);
_declspec(dllexport)  int GetFileSuffix(LPCTSTR lpszPathName, LPTSTR lpszFileSuffix, UINT nMax);


#endif // !defined(AFX_DIRNAME_H__6DB27E14_2E08_11D2_97DD_00104B357776__INCLUDED_)
