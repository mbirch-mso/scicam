// MainFrm.cpp : implementation of the CMainFrame class
//

#include "stdafx.h"
#include "resource.h"

#include <afxpriv.h>

#include "pdvshow.h"
#include "pdvshowdoc.h"
#include "pdvshowview.h"

#include "MainFrm.h"
#include "ChildFrm.h"
#include "CamSelectDialog.h"


#ifdef USE_PLUGINS

#include "plugsocket.h"

#endif

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CMainFrame

IMPLEMENT_DYNAMIC(CMainFrame, CMDIFrameWnd)

BEGIN_MESSAGE_MAP(CMainFrame, CMDIFrameWnd)
    //{{AFX_MSG_MAP(CMainFrame)
    ON_WM_CREATE()
    ON_COMMAND(ID_CAMERA_CAPTURE, OnCameraCapture)
    ON_COMMAND(ID_CAMERA_CONTINUOUS, OnCameraContinuous)
    ON_UPDATE_COMMAND_UI(ID_CAMERA_CONTINUOUS, OnUpdateCameraContinuous)
    ON_COMMAND(ID_CAMERA_COMMAND, OnCameraCommand)
    ON_UPDATE_COMMAND_UI(ID_CAMERA_COMMAND, OnUpdateCameraCommand)
    ON_COMMAND(ID_CAMERA_DECSPEED, OnCameraDecspeed)
    ON_COMMAND(ID_CAMERA_INCSPEED, OnCameraIncspeed)
    ON_COMMAND(ID_CAMERA_SETUP, OnCameraSetup)
    ON_COMMAND(ID_CONTRAST, OnContrast)
    ON_COMMAND(ID_VIEW_HISTOGRAM, OnViewHistogram)
    ON_COMMAND(ID_VIEW_PROFILE, OnViewProfile)
    ON_COMMAND(ID_MASK, OnMask)
    ON_COMMAND(ID_FILTER, OnFilter)
    ON_COMMAND(ID_Fill, OnFill)
    ON_COMMAND(ID_ARITHMETIC, OnArithmetic)
    ON_COMMAND(ID_IMAGE_TEXT, OnImageText)
    ON_COMMAND(ID_VIEW_SELECTREGION, OnViewSelectregion)
    ON_UPDATE_COMMAND_UI(ID_VIEW_SELECTREGION, OnUpdateViewSelectregion)
    ON_UPDATE_COMMAND_UI(ID_VIEW_PROFILE, OnUpdateViewProfile)
    ON_COMMAND(ID_DRAW_LINE, OnDrawLine)
    ON_UPDATE_COMMAND_UI(ID_DRAW_LINE, OnUpdateDrawLine)
    ON_COMMAND(ID_CAMERA_SETTINGS, OnCameraSettings)
    ON_COMMAND(ID_CLEAR_LINE, OnClearLine)
    ON_COMMAND(ID_VIEW_CLEARREGION, OnViewClearregion)
    ON_COMMAND(ID_CAMERA_PROGRAM, OnCameraProgram)
    ON_UPDATE_COMMAND_UI(ID_DIRECT_DRAW, OnUpdateDirectDraw)
    ON_COMMAND(ID_GAMMA, OnGamma)
    ON_COMMAND(ID_FOIUNIT_0, OnFoiunit0)
    ON_COMMAND(ID_FOIUNIT_1, OnFoiunit1)
    ON_COMMAND(ID_FOIUNIT_2, OnFoiunit2)
    ON_COMMAND(ID_FOIUNIT_3, OnFoiunit3)
    ON_COMMAND(ID_FOIUNIT_3, OnFoiunit4)
    ON_COMMAND(ID_FOIUNIT_3, OnFoiunit5)
    ON_UPDATE_COMMAND_UI(ID_FOIUNIT_0, OnUpdateFoiunit0)
    ON_UPDATE_COMMAND_UI(ID_FOIUNIT_1, OnUpdateFoiunit1)
    ON_UPDATE_COMMAND_UI(ID_FOIUNIT_2, OnUpdateFoiunit2)
    ON_UPDATE_COMMAND_UI(ID_FOIUNIT_3, OnUpdateFoiunit3)
    ON_UPDATE_COMMAND_UI(ID_FOIUNIT_4, OnUpdateFoiunit4)
    ON_UPDATE_COMMAND_UI(ID_FOIUNIT_5, OnUpdateFoiunit5)
    ON_COMMAND(ID_FOI_CYCLE, OnFoiCycle)
    ON_UPDATE_COMMAND_UI(ID_FOI_CYCLE, OnUpdateFoiCycle)
    ON_COMMAND(ID_RGB, OnRgb)
    ON_COMMAND(ID_VIEW_ZOOMIN_X2, OnViewZoominX2)
    ON_COMMAND(ID_VIEW_ZOOMOUT_X2, OnViewZoomoutX2)
    ON_COMMAND(ID_VIEW_CENTERHERE, OnViewCenterhere)
    ON_COMMAND(ID_VIEW_NORMAL, OnViewNormal)
    ON_UPDATE_COMMAND_UI(ID_VIEW_CENTERHERE, OnUpdateViewCenterhere)
    ON_UPDATE_COMMAND_UI(ID_VIEW_NORMAL, OnUpdateViewNormal)
    ON_UPDATE_COMMAND_UI(ID_VIEW_ZOOMOUT_X2, OnUpdateViewZoomoutX2)
    ON_UPDATE_COMMAND_UI(ID_VIEW_ZOOMIN_X2, OnUpdateViewZoominX2)
    ON_COMMAND(ID_VIEW_NORMAL_RESET, OnViewNormalReset)
    ON_UPDATE_COMMAND_UI(ID_VIEW_NORMAL_RESET, OnUpdateViewNormalReset)
    ON_COMMAND(ID_ZOOM_PANEL, OnZoomPanel)
    ON_WM_TIMER()
    ON_COMMAND(ID_FREERUN, OnFreerun)
    ON_UPDATE_COMMAND_UI(ID_FREERUN, OnUpdateFreerun)
    ON_COMMAND(ID_ROI_BUTTON, OnRoiButton)
    ON_UPDATE_COMMAND_UI(ID_ROI_BUTTON, OnUpdateRoiButton)
    ON_UPDATE_COMMAND_UI(ID_INDICATOR_FRAMES,OnUpdateIndicatorFrames)
    ON_UPDATE_COMMAND_UI(ID_INDICATOR_FPS,OnUpdateIndicatorFPS)
    ON_UPDATE_COMMAND_UI(ID_INDICATOR_TIMEOUTS,OnUpdateIndicatorTimeouts)
    ON_UPDATE_COMMAND_UI(ID_INDICATOR_ZOOM,OnUpdateIndicatorZoom)
    ON_UPDATE_COMMAND_UI(ID_INDICATOR_X,OnUpdateIndicatorX)
    ON_UPDATE_COMMAND_UI(ID_INDICATOR_Y,OnUpdateIndicatorY)
    ON_UPDATE_COMMAND_UI(ID_INDICATOR_PIXEL_VALUE,OnUpdateIndicatorPixelValue)
    ON_UPDATE_COMMAND_UI(ID_INDICATOR_DEBUG,OnUpdateIndicatorDebug)
    ON_UPDATE_COMMAND_UI(ID_INDICATOR_STATUS,OnUpdateIndicatorStatus)
    ON_COMMAND(ID_DIRECT_DRAW, OnDirectDraw)
    ON_COMMAND(ID_KEEP_UP, OnKeepUp)
    ON_UPDATE_COMMAND_UI(ID_KEEP_UP, OnUpdateKeepUp)
    ON_COMMAND(ID_DRAW_FROM_THREAD, OnDrawFromThread)
    ON_UPDATE_COMMAND_UI(ID_DRAW_FROM_THREAD, OnUpdateDrawFromThread)
    ON_COMMAND(ID_BINNING, OnBinning)
    ON_UPDATE_COMMAND_UI(ID_BINNING, OnUpdateBinning)
    ON_COMMAND(ID_DISPLAY_SKIP, OnDisplaySkip)
    ON_WM_CLOSE()
    ON_COMMAND(ID_CAMERA_CHECKFOROVERRUN, OnCameraCheckforoverrun)
    ON_UPDATE_COMMAND_UI(ID_CAMERA_CHECKFOROVERRUN, OnUpdateCameraCheckforoverrun)
    ON_COMMAND(ID_SHOW_SINGLE_FRAME, OnShowSingleFrame)
    ON_UPDATE_COMMAND_UI(ID_SHOW_SINGLE_FRAME, OnUpdateShowSingleFrame)
    ON_MESSAGE(WM_STATUS_UPDATE, OnStatusUpdate)
    ON_COMMAND(ID_AUTONUMBER, OnAutonumber)
    ON_UPDATE_COMMAND_UI(ID_AUTONUMBER, OnUpdateAutonumber)
    //}}AFX_MSG_MAP
END_MESSAGE_MAP()


static UINT indicators[] =
{
    ID_INDICATOR_STATUS,           // status line indicator
    ID_INDICATOR_FRAMES,
    ID_INDICATOR_FPS,
    ID_INDICATOR_TIMEOUTS, 
    ID_INDICATOR_X,
    ID_INDICATOR_Y,
    ID_INDICATOR_PIXEL_VALUE,
    ID_INDICATOR_ZOOM,
    ID_INDICATOR_DEBUG
};

/////////////////////////////////////////////////////////////////////////////
// CMainFrame construction/destruction

CMainFrame::CMainFrame()
{
    // TODO: add member initialization code here
    m_pContrastDialog = NULL;
    m_pCamProgramDlg = NULL;
    m_pCamControlDlg = NULL;

    m_pHistDialog = NULL;
    m_pProfileDialog = NULL;
    m_pEdtImageTextDlg = NULL;
    m_bFoiCycle = FALSE;
    m_pRoiDlg = NULL;
    m_pBinDlg = NULL;
    m_pSoftwareRGB = NULL;
    m_pDisplaySkipDlg = NULL;


    m_pZoomDialog = NULL;

    m_pPump = NULL;
    m_pCamera = NULL;

    m_nTimer = 0;


    m_bStopPending = FALSE;

    m_dFPS = 0;
    m_nFrames = 0;
    m_nTimeouts = 0;
    m_nX = 0;
    m_nY = 0;
    m_sPixelValue = "";

    m_sDebugString = "";

    m_sStatus = "";

    m_dZoom = 1;

    m_dMeanDelay = 0;
    m_dMaxDelay = 0;

}

CMainFrame::~CMainFrame()
{
}

int CMainFrame::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
    if (CMDIFrameWnd::OnCreate(lpCreateStruct) == -1)
        return -1;

    //
    // Select toolbar
    //

    if (!m_wndToolBar.Create(this) ||
        !m_wndToolBar.LoadToolBar(IDR_MAINFRAME))
    {
        TRACE0("Failed to create toolbar\n");
        return -1;      // fail to create
    }

    if (!m_wndStatusBar.Create(this) ||
        !m_wndStatusBar.SetIndicators(indicators,
        sizeof(indicators)/sizeof(UINT)))
    {
        TRACE0("Failed to create status bar\n");
        return -1;      // fail to create
    }


    // TODO: Remove this if you don't want tool tips or a resizeable toolbar
    m_wndToolBar.SetBarStyle(m_wndToolBar.GetBarStyle() |
        CBRS_TOOLTIPS | CBRS_FLYBY | CBRS_SIZE_DYNAMIC);

    // TODO: Delete these three lines if you don't want the toolbar to
    //  be dockable
    m_wndToolBar.EnableDocking(CBRS_ALIGN_ANY);
    EnableDocking(CBRS_ALIGN_ANY);
    DockControlBar(&m_wndToolBar);




#ifdef USE_PLUGINS

    m_PluginBar.Create(this, m_pCamera, ".\\plugins");

    m_wndToolBar.SetBarStyle(m_wndToolBar.GetBarStyle() |
        CBRS_TOOLTIPS | CBRS_FLYBY | CBRS_SIZE_DYNAMIC);

    m_PluginBar.EnableDocking(CBRS_ALIGN_ANY);
    DockControlBar(&m_PluginBar);

    // Set up callbacks

    m_PluginBar.m_PluginManager.SetRequestImageCB(RequestImageCB);

    m_PluginBar.m_PluginManager.SetUpdateResultCB(UpdateResultCB);
    m_PluginBar.m_PluginManager.SetAcquireControlCB(AcquireControlCB);
    m_PluginBar.m_PluginManager.SetGetCurrentImageCB(GetCurrentImageCB);
    m_PluginBar.m_PluginManager.SetRequestEventCB(RequestEventCB);

    m_PluginBar.m_PluginManager.SetOwner(this);


#endif

    return 0;
}

BOOL CMainFrame::PreCreateWindow(CREATESTRUCT& cs)
{
    // TODO: Modify the Window class or styles here by modifying
    //  the CREATESTRUCT cs

    return CMDIFrameWnd::PreCreateWindow(cs);
}

/////////////////////////////////////////////////////////////////////////////
// CMainFrame diagnostics

#ifdef _DEBUG
void CMainFrame::AssertValid() const
{
    CMDIFrameWnd::AssertValid();
}

void CMainFrame::Dump(CDumpContext& dc) const
{
    CMDIFrameWnd::Dump(dc);
}

#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CMainFrame message handlers

void CMainFrame::OnCameraCapture() 
{
    // TODO: Add your command handler code here
    CPdvshowDoc *pDoc = GetActiveDoc();

    if (m_pPump->IsLive())
    {
        OnCameraContinuous();
    }
    else 	if (pDoc)
    {

        pDoc->CaptureImage();

        UpdateDocSecondaryViews();

        m_nTimeouts = m_pPump->GetTimeouts();

        UpdateFrameStatusValues();

    }

}

void CMainFrame::OnCameraContinuous() 
{
    // TODO: Add your command handler code here
    bool bGoing;


    if (m_bStopPending)
        return;

    if (m_pPump->IsLive())
    {

        if (!m_bStopPending)
        {
            m_pPump->Stop();

            if (!m_nTimer)
                m_nTimer = (int) SetTimer(1, 100, NULL);

            bGoing = FALSE;

            m_bStopPending = TRUE;
        }

    }
    else if (!m_bStopPending)
    {
        ((CPdvshowApp *) AfxGetApp())->GoLive();

        m_pPump->AddEventFunc(UpdateOnNewImage,this);

        bGoing = TRUE;

    }



}

void CMainFrame::OnUpdateCameraContinuous(CCmdUI* pCmdUI) 
{
    // TODO: Add your command update UI handler code here
    if (m_pPump->IsLive() || m_bStopPending)
    {
        pCmdUI->SetCheck(TRUE);
    }
    else
    {
        pCmdUI->SetCheck(FALSE);
    }
}

void CMainFrame::OnCameraCommand() 
{


}

void CMainFrame::OnUpdateCameraCommand(CCmdUI* pCmdUI) 
{
    // TODO: Add your command update UI handler code here

}

void CMainFrame::OnCameraDecspeed() 
{
    // TODO: Add your command handler code here
    if (!m_pCamera->DecShutterSpeed())
    {	
        return;
    }

    int nShutterSpeed;
    if (!m_pCamera->GetShutterSpeed(nShutterSpeed))
    {	
        return;
    }

    m_sStatus.Format("Shutter speed is %d.", nShutterSpeed);


}

void CMainFrame::OnCameraIncspeed() 
{
    // TODO: Add your command handler code here
    if (!m_pCamera->IncShutterSpeed())
    {	
        return;
    }

    int nShutterSpeed;
    if (!m_pCamera->GetShutterSpeed(nShutterSpeed))
    {	
        return;
    }

    m_sStatus.Format("Shutter speed is %d.", nShutterSpeed);

}



void CMainFrame::OnCameraSetup() 
{
    CCamSelectDialog dlg;

    if (m_pPump->IsLive())
        OnCameraContinuous();

    int width = m_pCamera->GetWidth();
    int height = m_pCamera->GetHeight();
    int type = m_pCamera->GetType();

    dlg.GetConfigFile(m_pCamera);

    if (width != m_pCamera->GetWidth() ||
        height != m_pCamera->GetHeight() || 
        type != m_pCamera->GetType())
    {
        ((CPdvshowApp *) AfxGetApp())->ResetAllDocs();
    }
    else
        OnCameraCapture();



}

void CMainFrame::OnContrast() 
{

    CEdtImageWin *pWindow;
    pWindow = GetActiveImageWindow();

    if (!m_pContrastDialog)
    {
        m_pContrastDialog = new CContrastDlg;
        m_pContrastDialog->SetImageWindow(pWindow);
        m_pContrastDialog->SetCamera(m_pCamera);
        m_pContrastDialog->Create(IDD_CONTRAST,this);

        m_pContrastDialog->ShowWindow(SW_SHOW);
    }
    else
    {
        m_pContrastDialog->SetImageWindow(pWindow);
        m_pContrastDialog->ShowWindow(SW_RESTORE);
    }
}

void CMainFrame::OnViewHistogram() 
{
    // TODO: Add your command handler code here
    if (!m_pHistDialog)
    {
        m_pHistDialog = new CHistDialog();

        m_pHistDialog->Create(IDD_HIST_DIALOG, this);

        if (GetActiveImage())
        {

            m_pHistDialog->Compute(GetActiveImage());
        }

        m_pHistDialog->ShowWindow(SW_SHOW);

    }
    else
    {
        if (GetActiveImage())
        {

            m_pHistDialog->Compute(GetActiveImage());
        }

        m_pHistDialog->ShowWindow(SW_RESTORE);
    }
}

void CMainFrame::OnViewProfile() 
{
    // TODO: Add your command handler code here
    // TODO: Add your command handler code here

    CEdtImageWin *pWindow;
    pWindow = GetActiveImageWindow();

    if (!m_pProfileDialog)
    {
        m_pProfileDialog = new CProfileDialog();

        m_pProfileDialog->Create(IDD_PROFILE_DIALOG, this);

        if (pWindow && pWindow->IsLineActive())
        {
            CPoint pt1, pt2;

            pWindow->GetEndPoints(pt1, pt2);

            m_pProfileDialog->Compute(pWindow->GetBaseImage(),pt1,pt2);
        }

        m_pProfileDialog->ShowWindow(SW_SHOW);

    }
    else
    {
        if (pWindow && pWindow->IsLineActive())
        {
            CPoint pt1, pt2;

            pWindow->GetEndPoints(pt1, pt2);

            m_pProfileDialog->Compute(pWindow->GetBaseImage(),pt1,pt2);
        }

        m_pProfileDialog->ShowWindow(SW_RESTORE);
    }
}

void CMainFrame::OnMask() 
{
    // TODO: Add your command handler code here

}

void CMainFrame::OnFilter() 
{
    // TODO: Add your command handler code here

}

void CMainFrame::OnFill() 
{
    // TODO: Add your command handler code here

}

void CMainFrame::OnArithmetic() 
{
    // TODO: Add your command handler code here

}

CPdvshowDoc * CMainFrame::GetActiveDoc()
{
    CChildFrame *pChild = (CChildFrame *) GetActiveFrame();

    return (CPdvshowDoc *) pChild->GetActiveDocument();

}

int CMainFrame::UpdateOnNewImage(void * target, void * pNewImage, void * pWindow, 
                                 int nFrameNumber)
{
    CMainFrame *pThis = (CMainFrame *) target;

    if (!pThis || !pNewImage)
        return -1;

    ::SendMessage(pThis->m_hWnd, WM_STATUS_UPDATE,0,0);

    pThis->UpdateSecondaryWindows((EdtImage *)pNewImage, (ImageWindowData *)pWindow);



    return 0;

}

// Update the frame counter on the status bar

LRESULT CMainFrame::OnStatusUpdate(WPARAM wParam, LPARAM lparam)
{
    CString s;
    CString s2;

    if (m_pPump->IsLive())
    {
        m_nFrames = m_pPump->GetFrameCount();
        m_dFPS = m_pPump->GetFrameRate();

        CEdtImageWin * pWindow = GetActiveImageWindow();

        if (!pWindow)
            return 0;

        m_dMeanDelay = pWindow->GetMeanDelay();
        m_dMaxDelay = pWindow->GetMaxDelay();

#if 0
#ifdef _DEBUG

        if (m_pCamera)
        {
            m_sDebugString.Format("Started %5d | Todo %5d | Donecount %5d | Skipped %5d", 
                m_pCamera->m_nStarted, 
                m_pCamera->m_nTodo,
                edt_done_count(m_pCamera->m_hPdv),
                m_pCamera->m_nSkipped);

        }

#endif
#endif
        m_nTimeouts = m_pPump->GetTimeouts();
        UpdateFrameStatusValues();

    }
    else	
    {
        m_nTimeouts = m_pPump->GetTimeouts();
        UpdateFrameStatusValues();
    }

    return 0;
}

// Get the current active image from the current view . Cannot be called from
// another thread


EdtImage * CMainFrame::GetActiveImage()
{
    CChildFrame *pChild = (CChildFrame *) GetActiveFrame();

    CPdvshowDoc *pDoc =  (CPdvshowDoc *) pChild->GetActiveDocument();

    if (pDoc)
        return pDoc->GetImage();

    return NULL;

}

EdtImage * CMainFrame::GetBaseImage()
{
    CChildFrame *pChild = (CChildFrame *) GetActiveFrame();

    CPdvshowDoc *pDoc =  (CPdvshowDoc *) pChild->GetActiveDocument();

    if (pDoc)
        return pDoc->GetBaseImage();

    return NULL;
}

void CMainFrame::OnViewMouseMove(CPdvshowView * pView, UINT nFlags, CPoint point)
{
    if (pView)
    {

        pView->m_wndImageWindow.ConvertToImageCoordinates(point);

        if (m_pEdtImageTextDlg && m_pEdtImageTextDlg->m_hWnd &&
            m_pEdtImageTextDlg->IsWindowVisible() &&
            m_pEdtImageTextDlg->m_bFollowCursor)
        {
            m_pEdtImageTextDlg->m_wndImageText.SetImageHandle( pView->m_wndImageWindow.GetBaseImage());
            m_pEdtImageTextDlg->m_wndImageText.SetCenterPoint(point);
            m_pEdtImageTextDlg->m_wndImageText.RefreshData();

            m_pEdtImageTextDlg->m_nStartX = point.x;
            m_pEdtImageTextDlg->m_nStartY = point.y;	

            m_pEdtImageTextDlg->UpdateData(FALSE);
        }

        // Get the cursor values
        CString s;

        EdtImage *pImage = pView->m_wndImageWindow.GetBaseImage();

        

        m_nX = point.x;
        m_nY = point.y;

        if (pImage && point.x <= pImage->GetWidth() &&
            point.y <= pImage->GetHeight()) 
        {
            int Value = 0;
            pImage->GetPixel(point.x, point.y, Value);

            if (pImage->GetNColors() == 1)
                m_sPixelValue.Format("%5d",  Value);
            else
                m_sPixelValue.Format("(%3d,%3d,%3d)", 
                GetRValue(Value), GetGValue(Value), GetBValue(Value));

        }
        else
        {
            m_sPixelValue = "";

        }

        m_dZoom = pView->m_wndImageWindow.m_wData.m_Zoom.GetZoomX();
    }
}

void CMainFrame::OnImageText() 
{
    // TODO: Add your command handler code here
    // TODO: Add your command handler code here

    if (!m_pEdtImageTextDlg)
    {
        m_pEdtImageTextDlg = new CEdtImageTextDlg();

        m_pEdtImageTextDlg->Create(IDD_PIXELTEXT, this);

        if (GetActiveImage())
        {

            m_pEdtImageTextDlg->m_wndImageText.SetImageHandle(GetActiveImage());
        }

        m_pEdtImageTextDlg->ShowWindow(SW_SHOW);

    }
    else
    {
        if (GetActiveImage())
        {

            m_pEdtImageTextDlg->m_wndImageText.SetImageHandle(GetActiveImage());
        }

        m_pEdtImageTextDlg->ShowWindow(SW_RESTORE);
    }
}

void CMainFrame::OnViewSelectregion() 
{
    // TODO: Add your command handler code here
    // TODO: Add your command handler code here
    CEdtImageWin *pWindow = GetActiveImageWindow();

    if (pWindow)
    {

        pWindow->SetRectTracking(!pWindow->IsRectTracking());

        if (pWindow->IsLineTracking())
            pWindow->SetLineTracking(FALSE);

    }


}

void CMainFrame::OnUpdateViewSelectregion(CCmdUI* pCmdUI) 
{
    // TODO: Add your command update UI handler code here
    CEdtImageWin *pWindow = GetActiveImageWindow();

    if (pWindow)
    {

        if (pWindow->IsRectTracking())
        {
            pCmdUI->SetCheck(TRUE);
        }
        else
        {
            pCmdUI->SetCheck(FALSE);
        }

    }
}

void CMainFrame::OnUpdateViewProfile(CCmdUI* pCmdUI) 
{
    // TODO: Add your command update UI handler code here

}

void CMainFrame::OnDrawLine() 
{
    // TODO: Add your command handler code here
    CEdtImageWin *pWindow = GetActiveImageWindow();

    if (pWindow)
    {

        pWindow->SetLineTracking(!pWindow->IsLineTracking());
        if (pWindow->IsRectTracking())
            pWindow->SetRectTracking(FALSE);

    }

}

void CMainFrame::OnUpdateDrawLine(CCmdUI* pCmdUI) 
{
    // TODO: Add your command update UI handler code here
    CEdtImageWin *pWindow = GetActiveImageWindow();

    if (pWindow)
    {

        if (pWindow->IsLineTracking())
        {
            pCmdUI->SetCheck(TRUE);
        }
        else
        {
            pCmdUI->SetCheck(FALSE);
        }

    }

}

void CMainFrame::OnViewLButtonUp(CPdvshowView * pView, UINT nFlags, CPoint point)
{
    if (pView)
    {
        CEdtImageWin *pWindow = &pView->m_wndImageWindow;

        if (m_pProfileDialog && m_pProfileDialog->m_bVisible &&
            pWindow->IsLineTracking())
        {
            // 
            CPoint pt1, pt2;

            pWindow->GetEndPoints(pt1, pt2);

            m_pProfileDialog->Compute(pWindow->GetBaseImage(),pt1,pt2);

        }

        if (m_pHistDialog && m_pHistDialog->m_bVisible &&
            pWindow->IsRectTracking())
        {

            OnViewHistogram();
        }

    }
}

void CMainFrame::OnViewLButtonDown(CPdvshowView * pView, UINT nFlags, CPoint point)
{
    if (pView)
    {
        CEdtImageWin *pWindow = &pView->m_wndImageWindow;



    }
}

CEdtImageWin * CMainFrame::GetActiveImageWindow()
{
    CPdvshowDoc *pDoc = GetActiveDoc();

    if (pDoc)
        return pDoc->GetImageWindow();
    else
        return NULL;
}

#include <process.h>

void CMainFrame::OnCameraSettings() 
{
    // TODO: Add your command handler code here

    // check for existence of camera_setup executable
    // FIX - deal with camera type and multiple boards
    // 

    if (m_pCamera->GetCameraModel())
    {
        CString cam_control;

        cam_control.Format("./camera_control/%s.exe", m_pCamera->GetCameraModel());

        if (_access(cam_control,0) == 0)
        {
            _spawnl(P_NOWAIT,cam_control, cam_control, NULL);

            return;
        } 
        cam_control.Format(".\\camera_control\\%s.bat", m_pCamera->GetCameraModel());

        if (_access(cam_control,0) == 0)
        {
            _spawnl(P_WAIT,cam_control, cam_control, NULL);

            return;
        } 


    }

    if (!m_pCamControlDlg)
    {
        m_pCamControlDlg = new CCamControlDlg();

        m_pCamControlDlg->SetCamera(m_pCamera);

        m_pCamControlDlg->Create(IDD_CAM_CONTROL, this);

        m_pCamControlDlg->ShowWindow(SW_SHOW);

    }
    else
    {
        m_pCamControlDlg->UpdateData(FALSE);

        m_pCamControlDlg->ShowWindow(SW_RESTORE);
    }

}

void CMainFrame::OnClearLine() 
{
    // TODO: Add your command handler code here
    CEdtImageWin *pWindow = GetActiveImageWindow();

    if (pWindow)
    {

        pWindow->SetLineTracking(FALSE);
        pWindow->SetLineActive(FALSE);

    }
}

void CMainFrame::OnViewClearregion() 
{
    // TODO: Add your command handler code here
    CEdtImageWin *pWindow = GetActiveImageWindow();

    if (pWindow)
    {

        pWindow->SetRectTracking(FALSE);
        pWindow->SetRectActive(FALSE);

    }
}

void CMainFrame::CloseViewHooks(CPdvshowView *pView)
{
    // Check for possible problems
    ASSERT(pView);

    if (m_pPump->IsLive())

    {
        m_pPump->Stop(); // Fix - this might still fail

    }

    if (m_pContrastDialog)
    {

        if (m_pContrastDialog->m_pImageWindow == &pView->m_wndImageWindow)
            m_pContrastDialog->SetImageWindow(NULL);

    }
}

void CMainFrame::OnCameraProgram() 
{
    // TODO: Add your command handler code here
    // TODO: Add your command handler code here
    // TODO: Add your command handler code here
    if (!m_pCamProgramDlg)
    {
        m_pCamProgramDlg = new CCamProgramDlg();

        m_pCamProgramDlg->SetCamera(m_pCamera);


        m_pCamProgramDlg->Create(IDD_CAM_PROGRAM, this);

        m_pCamProgramDlg->ShowWindow(SW_SHOW);

    }
    else
    {
        m_pCamProgramDlg->UpdateData(FALSE);

        m_pCamProgramDlg->ShowWindow(SW_RESTORE);
    }
}



void CMainFrame::OnUpdateDirectDraw(CCmdUI* pCmdUI) 
{

#if 0
    CWindowFactory *MainWindowFactory = GetMainWindowFactory();

    // TODO: Add your command update UI handler code here
    // TODO: Add your command handler code here
    if (MainWindowFactory->UseDirectDraw())
        pCmdUI->SetCheck(TRUE);
    else
        pCmdUI->SetCheck(FALSE);


#endif

}

#include "GammaDialog.h"

void CMainFrame::OnGamma() 
{
    // TODO: Add your command handler code here
    CGammaDialog dlg;

    dlg.DoModal();

}

void CMainFrame::OnFoiunit0() 
{
    // TODO: Add your command handler code here
    if (!m_pPump->IsLive())
        OnCameraCapture();}

void CMainFrame::OnFoiunit1() 
{
    // TODO: Add your command handler code here
    if (!m_pPump->IsLive())
        OnCameraCapture();
}

void CMainFrame::OnFoiunit2() 
{
    // TODO: Add your command handler code here
    if (!m_pPump->IsLive())
        OnCameraCapture();
}

void CMainFrame::OnFoiunit3() 
{
    // TODO: Add your command handler code here
    if (!m_pPump->IsLive())
        OnCameraCapture();
}
void CMainFrame::OnFoiunit4() 
{
    // TODO: Add your command handler code here
    if (!m_pPump->IsLive())
        OnCameraCapture();
}
void CMainFrame::OnFoiunit5() 
{
    // TODO: Add your command handler code here
    if (!m_pPump->IsLive())
        OnCameraCapture();
}
void CMainFrame::OnUpdateFoiunit0(CCmdUI* pCmdUI) 
{
    // TODO: Add your command update UI handler code here

}

void CMainFrame::OnUpdateFoiunit1(CCmdUI* pCmdUI) 
{
    // TODO: Add your command update UI handler code here

}

void CMainFrame::OnUpdateFoiunit2(CCmdUI* pCmdUI) 
{
    // TODO: Add your command update UI handler code here
    // TODO: Add your command update UI handler code here

}

void CMainFrame::OnUpdateFoiunit3(CCmdUI* pCmdUI) 
{
    // TODO: Add your command update UI handler code here
}

void CMainFrame::OnUpdateFoiunit4(CCmdUI* pCmdUI) 
{

}

void CMainFrame::OnUpdateFoiunit5(CCmdUI* pCmdUI) 
{
}
void CMainFrame::OnFoiCycle() 
{
    // TODO: Add your command handler code here

}

void CMainFrame::OnUpdateFoiCycle(CCmdUI* pCmdUI) 
{
    // TODO: Add your command update UI handler code here
}

void CMainFrame::OnRgb() 
{
    // TODO: Add your command handler code here

    //	return ;


    if (m_pCamera->GetInterlaceType()  == PDV_BGGR ||
        m_pCamera->GetInterlaceType()  == PDV_BGGR_WORD ||
        m_pCamera->GetInterlaceType() == PDV_ES10_BGGR ||
        m_pCamera->GetInterlaceType() == PDV_INVERT_RIGHT_BGGR_INTLV)
    {

        SetupSoftwareRGB();

        m_pSoftwareRGB->ShowWindow(SW_SHOW);

    }


}

void CMainFrame::OnViewZoominX2() 
{
    // TODO: Add your command handler code here

    SetZoomMode(1);
}

void CMainFrame::OnViewZoomoutX2() 
{
    // TODO: Add your command handler code here
    SetZoomMode(2);	
}

void CMainFrame::OnViewCenterhere() 
{
    // TODO: Add your command handler code here
    SetZoomMode(3);	
}

void CMainFrame::OnViewNormal() 
{
    // TODO: Add your command handler code here
    SetZoomMode(0);

}

void CMainFrame::OnUpdateViewCenterhere(CCmdUI* pCmdUI) 
{
    // TODO: Add your command update UI handler code here
    pCmdUI->Enable(TRUE);
    if (GetZoomMode() == 3)
        pCmdUI->SetCheck(TRUE);
    else
        pCmdUI->SetCheck(FALSE);


}

void CMainFrame::OnUpdateViewNormal(CCmdUI* pCmdUI) 
{
    // TODO: Add your command update UI handler code here
    pCmdUI->Enable(TRUE);
    if (GetZoomMode() == 0)
        pCmdUI->SetCheck(TRUE);
    else
        pCmdUI->SetCheck(FALSE);

}

void CMainFrame::OnUpdateViewZoomoutX2(CCmdUI* pCmdUI) 
{
    // TODO: Add your command update UI handler code here
    pCmdUI->Enable(TRUE);
    if (GetZoomMode() == 2)
        pCmdUI->SetCheck(TRUE);
    else
        pCmdUI->SetCheck(FALSE);

}

void CMainFrame::OnUpdateViewZoominX2(CCmdUI* pCmdUI) 
{
    // TODO: Add your command update UI handler code here
    pCmdUI->Enable(TRUE);
    if (GetZoomMode() == 1)
        pCmdUI->SetCheck(TRUE);
    else
        pCmdUI->SetCheck(FALSE);

}

void CMainFrame::SetZoomMode(int nNewMode)
{

    // TODO: Add your command update UI handler code here
    CEdtImageWin *pWindow = GetActiveImageWindow();

    if (pWindow)
    {

        pWindow->SetMouseZoomMode(nNewMode);


    }

}

void CMainFrame::OnViewNormalReset() 
{
    // TODO: Add your command handler code here
    // TODO: Add your command update UI handler code here
    CEdtImageWin *pWindow = GetActiveImageWindow();

    if (pWindow)
    {

        pWindow->ResetZoom();
        SetZoomMode(0);

    }
}

void CMainFrame::OnUpdateViewNormalReset(CCmdUI* pCmdUI) 
{
    // TODO: Add your command update UI handler code here
    pCmdUI->Enable(TRUE);
    pCmdUI->SetCheck(FALSE);
}

void CMainFrame::SetViewNormal()
{
    SetZoomMode(ZOOM_MODE_NORMAL);
}

int CMainFrame::GetZoomMode()

{
    // TODO: Add your command update UI handler code here
    CEdtImageWin *pWindow = GetActiveImageWindow();

    if (pWindow)
    {
        return pWindow->GetZoomMode();

    }

    return ZOOM_MODE_NORMAL;
}

void CMainFrame::OnZoomPanel() 
{
    // TODO: Add your command handler code here
    CEdtImageWin *pWindow = GetActiveImageWindow();

    if (!m_pZoomDialog)
    {
        m_pZoomDialog = new CZoomDialog;

        m_pZoomDialog->SetWindow(pWindow);

        m_pZoomDialog->Create(IDD_ZOOM_DIALOG,this);

        m_pZoomDialog->ShowWindow(SW_SHOW);
    }
    else
    {
        m_pZoomDialog->SetWindow(pWindow);
        m_pZoomDialog->ShowWindow(SW_RESTORE);
    }



}

#if _MSC_VER >= 1300
void CMainFrame::OnTimer(UINT_PTR nIDEvent)
#else
void CMainFrame::OnTimer(UINT nIDEvent)
#endif
{
    // TODO: Add your message handler code here and/or call default
    switch(nIDEvent) 
    {

    case 1:
        if (!m_pPump->IsLive())
        {

            CPdvshowDoc *pDoc = GetActiveDoc();

            EdtImage *pImage1 = m_pCamera->GetLastImage();

            if (pDoc)
            {
                pDoc->CaptureLastImage();
            }

            ((CPdvshowApp *) AfxGetApp())->UpdateAllDocViews();

            UpdateDocSecondaryViews();

            UpdateFrameStatusValues();

            KillTimer(1);

            m_nTimer =  0;

            m_bStopPending = FALSE;

            TRACE("Capture stopped\n");


        }
        break;

    case 2:
        if (!m_bStopPending)
        {

            // Sample

            TRACE("Going to Capture\n");
            OnCameraCapture();

            CEdtImageWin *pWindow = GetActiveImageWindow();

            if (!pWindow)
                return;

            EdtImage *pImage1 = pWindow->GetImage();			

            UpdateImageRec *pTest;

            pTest = m_UpdateImageList.GetFirst();

            while (!m_UpdateImageList.EndOfList() && pTest)
            {
                if (pTest->pUpdateImage)
                    pTest->pUpdateImage(pTest->pTarget, pImage1);

                delete pTest;

                m_UpdateImageList.DeleteCurrent();

                pTest = m_UpdateImageList.GetFirst();

            }			

            TRACE("Going back to continuous\n");

            OnCameraContinuous();

            KillTimer(2);

        }

        break;


    }


    CMDIFrameWnd::OnTimer(nIDEvent);
}

void CMainFrame::UpdateSecondaryWindows(EdtImage * pNewImage, ImageWindowData *pWindowData)
{

    CEdtImageWin *pWindow = NULL;

    if (m_pEdtImageTextDlg)
    {
        m_pEdtImageTextDlg->m_wndImageText.SetImageHandle(
            pNewImage);
        m_pEdtImageTextDlg->m_wndImageText.RefreshData();
    }

    pWindow = ((CPdvshowApp *)AfxGetApp())->FindImageWindow(pWindowData);

    if (m_pHistDialog && m_pHistDialog->m_bVisible)
    {
        // We need to look up the image in the list of
        // views


        if (pWindow)

        {
            if (pWindow->IsRectActive())
            {
                EdtImage SubImage;
                CRect SubRegion;
                pWindow->GetRegion(SubRegion);

                SubImage.AttachToParent(pNewImage, SubRegion.left, 
                    SubRegion.top,
                    SubRegion.Width(), 
                    SubRegion.Height());

                m_pHistDialog->Compute(&SubImage);

            }
            else
                m_pHistDialog->Compute(pNewImage);

        }

        else
            m_pHistDialog->Compute(pNewImage);
    }

    if (m_pProfileDialog && m_pProfileDialog->m_bVisible)
    {
        // We need to look up the image in the list of
        // views


        if (pWindow)
        {

            CPoint pt1, pt2;

            if (pWindow->GetEndPoints(pt1, pt2))

                m_pProfileDialog->Compute(pNewImage,pt1,pt2);

        }

    }

#ifdef USE_PLUGINS

    m_PluginBar.m_PluginManager.UpdateImage(pNewImage);

#endif

    m_pLastBufImage = pNewImage;
}

void CMainFrame::UpdateDocSecondaryViews()
{
    CPdvshowDoc *pDoc = GetActiveDoc();

    if (pDoc)
    {
        CEdtImageWin * pWindow = GetActiveImageWindow();

        if (pWindow)
        {
            UpdateSecondaryWindows(pDoc->GetBaseImage(), &pWindow->m_wData);
        }

    }
}

void CMainFrame::SetPump(LiveDisplay * pPump)
{
    m_pPump = pPump;

    if (m_pPump)
    {
        m_pCamera = m_pPump->GetCamera();

        m_pCamera->m_bKeepUp = TRUE;

        // Set to direct draw in acquire thread
    }
    else
        m_pCamera = NULL;

}

bool CMainFrame::IsLive()
{
    if (m_pPump)
        return m_pPump->IsLive();
    else
        return FALSE;
}




void CMainFrame::OnFreerun() 
{
    // TODO: Add your command handler code here
    if (m_pCamera)
    {
        m_pCamera->SetFreeRun(!m_pCamera->GetFreeRun());
    }


}

void CMainFrame::OnUpdateFreerun(CCmdUI* pCmdUI) 
{
    // TODO: Add your command update UI handler code here
    if (m_pCamera)
    {
        pCmdUI->SetCheck(m_pCamera->GetFreeRun());
    }

}

void CMainFrame::SetDisplayContinuous() 
{
    // TODO: Add your control notification handler code here
    TRACE("Main Frame Display COntinuous\n");
    OnCameraContinuous();

}

void CMainFrame::AcquireSingle() 
{
    // TODO: Add your control notification handler code here
    TRACE("Main Frame Capture Single \n");
    OnCameraCapture();

}

void CMainFrame::AcquireWhiteBalance() 
{
    // TODO: Add your control notification handler code here
    TRACE("Main Frame White Balance\n");
    OnCameraCapture();


}

void CMainFrame::AcquireBlack() 
{
    // TODO: Add your control notification handler code here
    TRACE("Main Frame Black\n");
    OnCameraCapture();

    CEdtImageWin *pWindow = GetActiveImageWindow();

    if (!pWindow)
        return;

    EdtImage *pImage = pWindow->GetImage();

}

void CMainFrame::OnSetDisplayContinuous(void *pThis) 
{
    // TODO: Add your control notification handler code here

    CMainFrame *pObj = (CMainFrame *) pThis;

    pObj->SetDisplayContinuous();

}

void CMainFrame::OnAcquireSingle(void *pThis) 
{
    CMainFrame *pObj = (CMainFrame *) pThis;

    pObj->AcquireSingle();	
}

void CMainFrame::OnAcquireWhiteBalance(void *pThis) 
{
    CMainFrame *pObj = (CMainFrame *) pThis;

    pObj->AcquireWhiteBalance();
}

void CMainFrame::OnAcquireBlack(void *pThis) 
{
    CMainFrame *pObj = (CMainFrame *) pThis;

    pObj->AcquireBlack();


}

void CMainFrame::OnRoiButton() 
{
    // TODO: Add your command handler code here

    if (!IsLive())
    {
        // TODO: Add your command handler code here
        if (!m_pRoiDlg)
        {
            m_pRoiDlg = new CRoiDlg();

            m_pRoiDlg->Create(IDD_ROI_DIALOG, this);

            m_pRoiDlg->UpdateFromCamera(m_pCamera);

            m_pRoiDlg->AttachTarget(RoiChanged, this);

            m_pRoiDlg->ShowWindow(SW_SHOW);

        }
        else
        {
            m_pRoiDlg->UpdateFromCamera(m_pCamera);

            m_pRoiDlg->ShowWindow(SW_RESTORE);
        }		

    }
    else
    {

        AfxMessageBox("Unable to Set ROI When video is active! Stop Live and try again.");

    }
}

void CMainFrame::OnUpdateRoiButton(CCmdUI* pCmdUI) 
{
    // TODO: Add your command update UI handler code here
    if (m_pCamera)
#define USE_ROI
#ifdef USE_ROI
        pCmdUI->Enable(m_pCamera->HasROI());
#else
        pCmdUI->Enable(FALSE);
#endif


    else
        pCmdUI->Enable(FALSE);
}

int CMainFrame::RoiChanged(void * pTarget)
{
    CMainFrame *pObj = (CMainFrame *) pTarget;

    ((CPdvshowApp *) AfxGetApp())->ResetAllDocs();

    return 0;

}

int CMainFrame::SnapImage(void * pTarget)
{

    CMainFrame *pObj = (CMainFrame *) pTarget;

    pObj->OnCameraCapture();

    return 0;
}

int CMainFrame::StartLive(void * pTarget)
{

    CMainFrame *pObj = (CMainFrame *) pTarget;

    pObj->OnCameraContinuous();

    return 0;
}

int CMainFrame::CBIsLive(void * pTarget)
{

    CMainFrame *pObj = (CMainFrame *) pTarget;

    return pObj->IsLive();

}

void CMainFrame::OnUpdateIndicatorFrames(CCmdUI* pCmdUI) 
{
    // TODO: Add your command update UI handler code here
    CString s;

    s.Format("Frames = %5d", m_nFrames);

    pCmdUI->SetText(s);
    pCmdUI->Enable(TRUE);


}

void CMainFrame::OnUpdateIndicatorFPS(CCmdUI* pCmdUI) 
{
    // TODO: Add your command update UI handler code here
    CString s;

    s.Format("%5.2f fps", m_dFPS);

    pCmdUI->SetText(s);
    pCmdUI->Enable(TRUE);


}


void CMainFrame::OnUpdateIndicatorTimeouts(CCmdUI* pCmdUI) 
{
    // TODO: Add your command update UI handler code here
    CString s;

    if (m_nTimeouts)
        s.Format("Timeouts = %d", m_nTimeouts);
    else 
        s = "";

    pCmdUI->SetText(s);
    pCmdUI->Enable(TRUE);


}

void CMainFrame::OnUpdateIndicatorX(CCmdUI* pCmdUI) 
{
    // TODO: Add your command update UI handler code here
    CString s;

    s.Format("X = %4d", m_nX);

    pCmdUI->SetText(s);
    pCmdUI->Enable(TRUE);


}


void CMainFrame::OnUpdateIndicatorY(CCmdUI* pCmdUI) 
{
    // TODO: Add your command update UI handler code here
    CString s;

    s.Format("Y = %4d", m_nY);

    pCmdUI->SetText(s);
    pCmdUI->Enable(TRUE);


}


void CMainFrame::OnUpdateIndicatorZoom(CCmdUI* pCmdUI) 
{
    // TODO: Add your command update UI handler code here
    CString s;

    int pct = (int) (m_dZoom * 100);

    s.Format("Zoom = %5d %%", pct);

    pCmdUI->SetText(s);
    pCmdUI->Enable(TRUE);


}


void CMainFrame::OnUpdateIndicatorDebug(CCmdUI* pCmdUI) 
{
    // TODO: Add your command update UI handler code here
    pCmdUI->SetText(m_sDebugString);
    pCmdUI->Enable(TRUE);


}


void CMainFrame::OnUpdateIndicatorPixelValue(CCmdUI* pCmdUI) 
{
    // TODO: Add your command update UI handler code here
    pCmdUI->SetText(m_sPixelValue);
    pCmdUI->Enable(TRUE);


}


void CMainFrame::OnUpdateIndicatorStatus(CCmdUI* pCmdUI) 
{
    // TODO: Add your command update UI handler code here
    pCmdUI->SetText(m_sStatus);
    pCmdUI->Enable(TRUE);


}

void CMainFrame::UpdateFrameStatusValues()

{
    CString s;	

    s.Format("Frames = %5d", m_nFrames);
    m_wndStatusBar.SetPaneText(1,s);

    s.Format("%5.2f fps", m_dFPS);

    m_wndStatusBar.SetPaneText(2,s);
    if (m_nTimeouts)
        s.Format("Timeouts = %d", m_nTimeouts);
    else
        s = "";

    m_wndStatusBar.SetPaneText(3,s);


#if 1
    m_sDebugString.Format("Delay Mean = %7.0f | Max = %7.0f | Skipped %4d %7.0f %%", m_dMeanDelay * 1000.0, m_dMaxDelay * 1000.0,
        m_pCamera->GetSkipped(), 
        (m_nFrames)? ((double) m_pCamera->GetSkipped()  / ((double) m_nFrames+ m_pCamera->GetSkipped())) * 100.0 : 0.0);
#else
    m_sDebugString = "";

#endif

    m_wndStatusBar.SetPaneText(8,m_sDebugString);
}


int CMainFrame::RequestImage(void *pObj, 
                             int (__cdecl *pUpdateImage)(void *,EdtImage *), void *pSrc, int bOnce)
{

    return ((CMainFrame *) pObj)->AnswerImageRequest(pUpdateImage, pSrc, bOnce);

}

int CMainFrame::AnswerImageRequest(int (__cdecl *UpdateImage)(void *,EdtImage *), void *pSrc, int bOnce)
{
    TRACE("Main Frame Answer Image Request\n");

    if (m_pPump->IsLive())
    {


        OnCameraContinuous();

        m_UpdateImageList.Append(new UpdateImageRec(pSrc, UpdateImage));

        SetTimer(2, 200, NULL);

        return 0;
    }
    else
    {
        OnCameraCapture();

        CEdtImageWin *pWindow = GetActiveImageWindow();

        if (!pWindow)
            return -1;

        EdtImage *pImage = pWindow->GetImage();

        UpdateImage(pSrc, pImage);

        OnCameraCapture();

        return 0;


    }

}

BOOL CMainFrame::OnCommand(WPARAM wParam, LPARAM lParam) 
{
    // TODO: Add your specialized code here and/or call the base class

    return CMDIFrameWnd::OnCommand(wParam, lParam);
}

void CMainFrame::OnDirectDraw() 
{
#if 0
    // TODO: Add your command handler code here
    CWindowFactory *MainWindowFactory = GetMainWindowFactory();

    bool old = MainWindowFactory->UseDirectDraw();
    MainWindowFactory->SetDirectDraw(!old);
#endif

}

void CMainFrame::OnKeepUp() 
{
    // TODO: Add your command handler code here
    if (m_pCamera)
        m_pCamera->m_bKeepUp = !m_pCamera->m_bKeepUp;
}

void CMainFrame::OnUpdateKeepUp(CCmdUI* pCmdUI) 
{
    // TODO: Add your command update UI handler code here
    if (m_pCamera)
    {
        pCmdUI->SetCheck(m_pCamera->m_bKeepUp);
    }	
}

BOOL CMainFrame::OnCmdMsg(UINT nID, int nCode, void* pExtra, AFX_CMDHANDLERINFO* pHandlerInfo) 
{
#ifdef USE_PLUGINS

    // TODO: Add your specialized code here and/or call the base class
    if (m_PluginBar.CheckCommand(nID, nCode, pExtra, pHandlerInfo))
        return TRUE;

#endif

    return CMDIFrameWnd::OnCmdMsg(nID, nCode, pExtra, pHandlerInfo);
}

void CMainFrame::OnDrawFromThread() 
{
    // TODO: Add your command handler code here
    //CImagePumpDisplay *pD = m_pPump->m_pDisplay;

    if (m_pPump)
        m_pPump->m_bWndDirect = !m_pPump->m_bWndDirect;


}

void CMainFrame::OnUpdateDrawFromThread(CCmdUI* pCmdUI) 
{
    // TODO: Add your command update UI handler code here

    //CImagePumpDisplay *pD = m_pPump->m_pDisplay;

    if (m_pPump)
        pCmdUI->SetCheck(m_pPump->m_bWndDirect);
}

void CMainFrame::OnBinning() 
{
    // TODO: Add your command handler code here

    if (!IsLive())
    {
        // TODO: Add your command handler code here
        if (!m_pBinDlg)
        {
            m_pBinDlg = new CBinningDlg;
            m_pBinDlg->Create(IDD_BINNING_DIALOG, this);

            m_pBinDlg->UpdateFromCamera(m_pCamera);

            m_pBinDlg->AttachTarget(RoiChanged, this);

            m_pBinDlg->ShowWindow(SW_SHOW);

        }
        else
        {
            m_pBinDlg->UpdateFromCamera(m_pCamera);

            m_pBinDlg->ShowWindow(SW_RESTORE);
        }		

    }
    else
    {

        AfxMessageBox("Unable to Set Binning When video is active! Stop Live and try again.");

    }

}

void CMainFrame::OnUpdateBinning(CCmdUI* pCmdUI) 
{
    // TODO: Add your command update UI handler code here
    if (m_pCamera)
        pCmdUI->Enable(m_pCamera->HasBinning());

}


#ifdef USE_PLUGINS

// Callbacks for plugins

void CMainFrame::AnswerPluginImageRequest(void *pSrc, int bOnce)
{
    TRACE("Main Frame Answer Image Request\n");

    if (m_pPump->IsLive())
    {


        OnCameraContinuous();

        SetTimer(2, 200, NULL);


    }
    else
    {
        OnCameraCapture();

        CEdtImageWin *pWindow = GetActiveImageWindow();

        if (!pWindow)
            return;

        EdtImage *pImage = pWindow->GetImage();



        CPlugSocket *pSocket = (CPlugSocket *) pSrc;

        pSocket->UpdateImage(pImage);

        OnCameraCapture();

    }

}

// Ask for 
void CMainFrame::RequestImageCB(void *pTarget, void *pSrc, bool bOnce, bool onoff)

{
    CMainFrame *pThis = (CMainFrame *) pTarget;

    if (onoff)
    {

        pThis->AnswerPluginImageRequest(pSrc, bOnce);
    }


}

// Tell the owner that there's new image data to display
// Passing NULL for pData means refresh the current image

void CMainFrame::UpdateResultCB(void *pTarget,EdtImage *pImage)

{

}

void CMainFrame::AcquireControlCB(void *pTarget, int command)

{

}


EdtImage * CMainFrame::GetCurrentImageCB(void *pTarget)
{

    CMainFrame *pFrm = (CMainFrame *) pTarget;

    CEdtImageWin *pWindow = pFrm->GetActiveImageWindow();

    if (!pWindow)
        return NULL;

    EdtImage *pImage = pWindow->GetImage();

    return pImage;

}


// Request a particular windows event 

void CMainFrame::RequestEventCB(void *pTarget, u_long event_id, int onoff)

{


}

#endif

void CMainFrame::SetupSoftwareRGB()
{

    if (!m_pSoftwareRGB)
    {

        m_pSoftwareRGB = new CSoftwareRGBDlg;

        m_pSoftwareRGB->Create(IDD_SOFTWARE_RGB, this);

    }

    bool redrowfirst;
    bool greenpixelfirst;

    int extended_depth = m_pCamera->GetExtendedDepth();
    m_pCamera->GetBayerOrder(redrowfirst, greenpixelfirst);

    m_pSoftwareRGB->SetCameraValues(extended_depth,
        redrowfirst,
        greenpixelfirst);

    m_pSoftwareRGB->SetCamera(m_pCamera);

    m_pSoftwareRGB->m_RequestImage = RequestImage;
    m_pSoftwareRGB->m_pRequestTarget = this;

}

void CMainFrame::OnDisplaySkip() 
{
    // TODO: Add your command handler code here
    // TODO: Add your command handler code here
    if (!m_pDisplaySkipDlg)
    {
        m_pDisplaySkipDlg = new CDisplaySkipDlg;
        m_pDisplaySkipDlg->Create(IDD_DISPLAY_SKIP_DLG, this);

        m_pDisplaySkipDlg->m_nDisplaySkip = m_pPump->GetDisplaySkip();

        m_pDisplaySkipDlg->SetOwnerNotification(this, NotifyDisplaySkip);

        m_pDisplaySkipDlg->ShowWindow(SW_SHOW);

    }
    else
    {
        m_pDisplaySkipDlg->m_nDisplaySkip = m_pPump->GetDisplaySkip();

        m_pDisplaySkipDlg->ShowWindow(SW_RESTORE);
    }			
}

void CMainFrame::NotifyDisplaySkip(void *target, int value)
{
    CMainFrame *pThis = (CMainFrame *) target;

    if (value != pThis->m_pPump->GetDisplaySkip())
        pThis->m_pPump->SetDisplaySkip(value);
}

void CMainFrame::OnClose() 
{
    // TODO: Add your message handler code here and/or call default

    // shut down live capture
    if (m_pPump->IsLive())
    {
        m_pPump->Abort();
    }


    CMDIFrameWnd::OnClose();
}

void CMainFrame::OnCameraCheckforoverrun() 
{
    // TODO: Add your command handler code here
    if (m_pCamera)
        m_pCamera->SetCheckOverrun(!m_pCamera->GetCheckOverrun());

}

void CMainFrame::OnUpdateCameraCheckforoverrun(CCmdUI* pCmdUI) 
{
    // TODO: Add your command update UI handler code here
    // TODO: Add your command update UI handler code here
    if (m_pCamera)
        pCmdUI->SetCheck(m_pCamera->GetCheckOverrun());

}

void CMainFrame::OnShowSingleFrame() 
{
    // TODO: Add your command handler code here
    if (m_pCamera && m_pCamera->IsMultiFrame())
    {
        m_pCamera->SetUseFrameHeight(!m_pCamera->GetUseFrameHeight());

        ((CPdvshowApp *) AfxGetApp())->ResetAllDocs();

    }

}

void CMainFrame::OnUpdateShowSingleFrame(CCmdUI* pCmdUI) 
{
    // TODO: Add your command update UI handler code here
    if (m_pCamera)
    {
        if (m_pCamera->IsMultiFrame())
            pCmdUI->SetCheck(m_pCamera->GetUseFrameHeight());
        else
            pCmdUI->Enable(FALSE);
    }
}

void CMainFrame::OnAutonumber() 
{
    // TODO: Add your command handler code here
    CPdvshowApp * pApp =(CPdvshowApp *) AfxGetApp();

    pApp->SetAutoNumber(!pApp->GetAutoNumber());

}

void CMainFrame::OnUpdateAutonumber(CCmdUI* pCmdUI) 
{
    // TODO: Add your command update UI handler code here

    CPdvshowApp * pApp =(CPdvshowApp *) AfxGetApp();

    pCmdUI->SetCheck(pApp->GetAutoNumber());

}
