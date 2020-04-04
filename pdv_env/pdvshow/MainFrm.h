// MainFrm.h : interface of the CMainFrame class
//
/////////////////////////////////////////////////////////////////////////////

#if !defined(AFX_MAINFRM_H__7E2B5362_2BE6_11D2_8DF1_00A0C932D513__INCLUDED_)
#define AFX_MAINFRM_H__7E2B5362_2BE6_11D2_8DF1_00A0C932D513__INCLUDED_

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000

#include "ContrastDlg.h"

#include "CamProgramDlg.h"
#include "CamControlDlg.h"
#include "HistDialog.h"
#include "ProfileDialog.h"
#include "EdtImageTextDlg.h"
#include "ZoomDialog.h"

#include "DisplaySkipDlg.h"


#include "BinningDlg.h"


#include "LiveDisplay.h"



#include "RoiDlg.h"
#include "SoftwareRGBDlg.h"


#ifdef USE_PLUGINS
#include "PluginBar.h"
#endif

class CPdvshowDoc;
class CPdvshowView;

struct UpdateImageRec {

	bool bOnce;
	void *pTarget;
	int (*pUpdateImage)(void *pTarget, EdtImage *pImage);

	UpdateImageRec(void *pt, int (*pUpdate)(void *, EdtImage *), bool Once = TRUE)
	{
		pTarget = pt;
		pUpdateImage = pUpdate;
		bOnce = Once;
	}
	UpdateImageRec()
	{
		pTarget = NULL;
		pUpdateImage = NULL;
		bOnce = TRUE;
	}
};


#define WM_STATUS_UPDATE (WM_USER + 100)

class CMainFrame : public CMDIFrameWnd
{
	DECLARE_DYNAMIC(CMainFrame)
public:
	CMainFrame();

// Attributes
public:

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CMainFrame)
	public:
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
	virtual BOOL OnCmdMsg(UINT nID, int nCode, void* pExtra, AFX_CMDHANDLERINFO* pHandlerInfo);
	protected:
	virtual BOOL OnCommand(WPARAM wParam, LPARAM lParam);
	//}}AFX_VIRTUAL

// Implementation
public:

#ifdef USE_PLUGINS

	CPluginBar m_PluginBar;

#endif

	void UpdateDocSecondaryViews();
	void UpdateSecondaryWindows(EdtImage *pNewImage, ImageWindowData *pWindowData);
	int m_nTimer;
	int GetZoomMode();

	void SetViewNormal();
	void SetZoomMode(int nNewMode);
	bool m_bFoiCycle;
	bool m_bLiveStopping;
	void CloseViewHooks(CPdvshowView *pView);
	CEdtImageWin * GetActiveImageWindow();
	void OnViewLButtonUp(CPdvshowView * pView, UINT nFlags, CPoint point);
	void OnViewLButtonDown(CPdvshowView * pView, UINT nFlags, CPoint point);
	void OnViewMouseMove(CPdvshowView * pView, UINT nFlags, CPoint point);
	EdtImage * GetBaseImage();
	EdtImage * GetActiveImage();
	CContrastDlg * m_pContrastDialog;
	CCamProgramDlg * m_pCamProgramDlg;
	CCamControlDlg * m_pCamControlDlg;

	CHistDialog * m_pHistDialog;
	CProfileDialog * m_pProfileDialog;
	CEdtImageTextDlg * m_pEdtImageTextDlg;
	CDisplaySkipDlg *m_pDisplaySkipDlg;

	CRoiDlg * m_pRoiDlg;
	CSoftwareRGBDlg * m_pSoftwareRGB;
	CBinningDlg *m_pBinDlg;


	EdtImage *m_pLastBufImage;

	virtual afx_msg LRESULT OnStatusUpdate(WPARAM wParam, LPARAM lparam);
	static int UpdateOnNewImage(void * target, void * pNewImage, void *pWindowData, int nFrameNumber);
	CPdvshowDoc * GetActiveDoc();
	virtual ~CMainFrame();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

#ifdef USE_TOUCAN
//	CToucanColorDialog *m_pToucanColorDialog;
#endif

	CZoomDialog *m_pZoomDialog;

protected:  // control bar embedded members
	CStatusBar  m_wndStatusBar;
	CToolBar    m_wndToolBar;
	CToolBar	m_wndFoiToolBar;

	LiveDisplay *m_pPump;
	PdvInput * m_pCamera;

// Generated message map functions
public:
	static void NotifyDisplaySkip(void * target, int value);
	void SetupSoftwareRGB();
	double m_dMaxDelay;
	double m_dMeanDelay;
	CToolBar m_wndPluginBar;
	CToolBarCtrl m_wndPluginBarCtrl;

	int AnswerImageRequest(int (*pUpdateImage)(void *, EdtImage *), void *pSrc, int bOnce);
	static int RequestImage(void *pObj, int (*pUpdateImage)(void *, EdtImage*), void *pSrc, int bOnce);

	/* callbacks for linking to external code */

	static int RoiChanged(void *pTarget);
	static int SnapImage(void *pTarget);
	static int StartLive(void *pTarget);
	static int CBIsLive(void *pTarget);

	bool m_bStopPending;

	double m_dFPS;
	double m_dZoom;
	int m_nFrames;
	int m_nTimeouts;
	int m_nX;
	int m_nY;
	CString m_sPixelValue;
	CString m_sStatus;

	CString m_sDebugString;

	EdtListHead<UpdateImageRec> m_UpdateImageList;


	// Plugin Callbacks
	static void RequestImageCB(void *pTarget, void *pSrc, bool bOnce, bool onoff);

	static void UpdateResultCB (void *pTarget,EdtImage *pImage);

	static void AcquireControlCB(void *pTarget, int command);

	static EdtImage * GetCurrentImageCB(void *pTarget);

	static void RequestEventCB(void *pTarget, uint event_id, int onoff);

	void AnswerPluginImageRequest(void *pSrc, int bOnce);




	void UpdateFrameStatusValues();

	void SetDisplayContinuous();
	void AcquireSingle();
	void AcquireWhiteBalance();
	void AcquireBlack();

	static void OnSetDisplayContinuous(void *pThis);
	static void OnAcquireSingle(void *pThis);
	static void OnAcquireWhiteBalance(void *pThis);
	static void OnAcquireBlack(void *pThis);

	bool IsLive();
	void SetPump(LiveDisplay *pPump);
	//{{AFX_MSG(CMainFrame)
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnCameraCapture();
	afx_msg void OnCameraContinuous();
	afx_msg void OnUpdateCameraContinuous(CCmdUI* pCmdUI);
	afx_msg void OnCameraCommand();
	afx_msg void OnUpdateCameraCommand(CCmdUI* pCmdUI);
	afx_msg void OnCameraDecspeed();
	afx_msg void OnCameraIncspeed();
	afx_msg void OnCameraSetup();
	afx_msg void OnContrast();
	afx_msg void OnViewHistogram();
	afx_msg void OnViewProfile();
	afx_msg void OnMask();
	afx_msg void OnFilter();
	afx_msg void OnFill();
	afx_msg void OnArithmetic();
	afx_msg void OnImageText();
	afx_msg void OnViewSelectregion();
	afx_msg void OnUpdateViewSelectregion(CCmdUI* pCmdUI);
	afx_msg void OnUpdateViewProfile(CCmdUI* pCmdUI);
	afx_msg void OnDrawLine();
	afx_msg void OnUpdateDrawLine(CCmdUI* pCmdUI);
	afx_msg void OnCameraSettings();
	afx_msg void OnClearLine();
	afx_msg void OnViewClearregion();
	afx_msg void OnCameraProgram();
	afx_msg void OnUpdateDirectDraw(CCmdUI* pCmdUI);
	afx_msg void OnGamma();
	afx_msg void OnFoiunit0();
	afx_msg void OnFoiunit1();
	afx_msg void OnFoiunit2();
	afx_msg void OnFoiunit3();
	afx_msg void OnFoiunit4();
	afx_msg void OnFoiunit5();
	afx_msg void OnUpdateFoiunit0(CCmdUI* pCmdUI);
	afx_msg void OnUpdateFoiunit1(CCmdUI* pCmdUI);
	afx_msg void OnUpdateFoiunit2(CCmdUI* pCmdUI);
	afx_msg void OnUpdateFoiunit3(CCmdUI* pCmdUI);
	afx_msg void OnUpdateFoiunit4(CCmdUI* pCmdUI);
	afx_msg void OnUpdateFoiunit5(CCmdUI* pCmdUI);
	afx_msg void OnFoiCycle();
	afx_msg void OnUpdateFoiCycle(CCmdUI* pCmdUI);
	afx_msg void OnRgb();
	afx_msg void OnViewZoominX2();
	afx_msg void OnViewZoomoutX2();
	afx_msg void OnViewCenterhere();
	afx_msg void OnViewNormal();
	afx_msg void OnUpdateViewCenterhere(CCmdUI* pCmdUI);
	afx_msg void OnUpdateViewNormal(CCmdUI* pCmdUI);
	afx_msg void OnUpdateViewZoomoutX2(CCmdUI* pCmdUI);
	afx_msg void OnUpdateViewZoominX2(CCmdUI* pCmdUI);
	afx_msg void OnViewNormalReset();
	afx_msg void OnUpdateViewNormalReset(CCmdUI* pCmdUI);
	afx_msg void OnZoomPanel();
#if _MSC_VER >= 1300
	afx_msg void OnTimer(UINT_PTR nIDEvent);
#else
	afx_msg void OnTimer(UINT nIDEvent);
#endif
	afx_msg void OnFreerun();
	afx_msg void OnUpdateFreerun(CCmdUI* pCmdUI);
	afx_msg void OnRoiButton();
	afx_msg void OnUpdateRoiButton(CCmdUI* pCmdUI);
	afx_msg void OnUpdateIndicatorFrames(CCmdUI * pCmdUI);
	afx_msg void OnUpdateIndicatorFPS(CCmdUI * pCmdUI);
	afx_msg void OnUpdateIndicatorTimeouts(CCmdUI * pCmdUI);
	afx_msg void OnUpdateIndicatorZoom(CCmdUI * pCmdUI);
	afx_msg void OnUpdateIndicatorX(CCmdUI * pCmdUI);
	afx_msg void OnUpdateIndicatorY(CCmdUI * pCmdUI);
	afx_msg void OnUpdateIndicatorPixelValue(CCmdUI * pCmdUI);
	afx_msg void OnUpdateIndicatorDebug(CCmdUI * pCmdUI);
	afx_msg void OnUpdateIndicatorStatus(CCmdUI * pCmdUI);
	afx_msg void OnDirectDraw();
	afx_msg void OnKeepUp();
	afx_msg void OnUpdateKeepUp(CCmdUI* pCmdUI);
	afx_msg void OnDrawFromThread();
	afx_msg void OnUpdateDrawFromThread(CCmdUI* pCmdUI);
	afx_msg void OnBinning();
	afx_msg void OnUpdateBinning(CCmdUI* pCmdUI);
	afx_msg void OnDisplaySkip();
	afx_msg void OnClose();
	afx_msg void OnCameraCheckforoverrun();
	afx_msg void OnUpdateCameraCheckforoverrun(CCmdUI* pCmdUI);
	afx_msg void OnShowSingleFrame();
	afx_msg void OnUpdateShowSingleFrame(CCmdUI* pCmdUI);
	afx_msg void OnAutonumber();
	afx_msg void OnUpdateAutonumber(CCmdUI* pCmdUI);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Developer Studio will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_MAINFRM_H__7E2B5362_2BE6_11D2_8DF1_00A0C932D513__INCLUDED_)
