// EdtLiveFrame.h: interface for the EdtLiveFrame class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_EDTLIVEFRAME_H__1352298A_A66E_4DBF_83FE_8E4B8F08CF6F__INCLUDED_)
#define AFX_EDTLIVEFRAME_H__1352298A_A66E_4DBF_83FE_8E4B8F08CF6F__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "EdtImageFrame.h"

#include "LiveDisplay.h"

class DLLEXPORT EdtLiveFrame : public CEdtImageFrame  
{
	DECLARE_DYNCREATE(EdtLiveFrame)

public:

	EdtImage *m_pImage;
	LiveDisplay *m_pGrabber;
	BOOL m_bOwnGrabber;
	BOOL m_bOwnImage;

	EdtLiveFrame(LiveDisplay *pGrabber = NULL);

	EdtLiveFrame(const char *devname, const int unit = 0, const int channel = 0);

	virtual ~EdtLiveFrame();

	// factory method
	static EdtLiveFrame *NewLiveViewer(LiveDisplay *pGrabber = NULL);

	void CreateLiveViewer(LiveDisplay *pGrabber = NULL);

	void StartLive();

	void StopLive();

	void Snap();

	void SetGrabber(LiveDisplay *pGrabber);

	LiveDisplay *GetGrabber()
	{
		return m_pGrabber;
	}

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CEdtImageFrame)
	//}}AFX_VIRTUAL

protected:
	// Generated message map functions
	//{{AFX_MSG(EdtLiveFrame)
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

#endif // !defined(AFX_EDTLIVEFRAME_H__1352298A_A66E_4DBF_83FE_8E4B8F08CF6F__INCLUDED_)
