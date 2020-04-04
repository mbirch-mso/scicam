// pdvshow.cpp : Defines the class behaviors for the application.
//

#include "stdafx.h"
#include "pdvshow.h"

#include "MainFrm.h"
#include "ChildFrm.h"
#include "pdvshowDoc.h"
#include "pdvshowView.h"
#include "LimitSingleInstance.h"

#include "CamSelectDialog.h"

#include <direct.h>

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CPdvshowApp

BEGIN_MESSAGE_MAP(CPdvshowApp, CWinApp)
    //{{AFX_MSG_MAP(CPdvshowApp)
    ON_COMMAND(ID_APP_ABOUT, OnAppAbout)
ON_COMMAND(ID_FILE_OPEN, OnFileOpen)
    //}}AFX_MSG_MAP
    // Standard file based document commands
    ON_COMMAND(ID_FILE_NEW, CWinApp::OnFileNew)
ON_COMMAND(ID_FILE_OPEN, CWinApp::OnFileOpen)
    // Standard print setup command
    ON_COMMAND(ID_FILE_PRINT_SETUP, CWinApp::OnFilePrintSetup)
END_MESSAGE_MAP()

    /////////////////////////////////////////////////////////////////////////////
    // CPdvshowApp construction

CPdvshowApp::CPdvshowApp()
{
    // TODO: add construction code here,
    // Place all significant initialization in InitInstance
    m_pPump = NULL;
    m_pCamera = NULL;

    m_dDefaultZoom = 1.0;

    m_bAutoNumber = FALSE;
    m_strFileName = "__NOFILENAME__";
}

class CPdvshowDocManager : public CDocManager

{

    public:
        virtual BOOL DoPromptFileName(CString& fileName, UINT nIDSTitle, DWORD lFlags,
                BOOL bOpenFileDialog, CDocTemplate* pTemplate);

};

/////////////////////////////////////////////////////////////////////////////
// The one and only CPdvshowApp object

CPdvshowApp theApp;

// This identifier was generated to be statistically unique for your app.
// You may change it if you prefer to choose a specific identifier.

// {7E2B5358-2BE6-11D2-8DF1-00A0C932D513}
static const CLSID clsid =
{ 0x7e2b5358, 0x2be6, 0x11d2, { 0x8d, 0xf1, 0x0, 0xa0, 0xc9, 0x32, 0xd5, 0x13 } };


/////////////////////////////////////////////////////////////////////////////
// CPdvshowApp initialization

BOOL CPdvshowApp::InitInstance()
{

    // Initialize OLE libraries
    if (!AfxOleInit())
    {
        AfxMessageBox(IDP_OLE_INIT_FAILED);
        return FALSE;
    }

    AfxEnableControlContainer();

    // Standard initialization
    // If you are not using these features and wish to reduce the size
    //  of your final executable, you should remove from the following
    //  the specific initialization routines you do not need.

#ifdef _AFXDLL
    Enable3dControls();			// Call this when using MFC in a shared DLL
#else
    Enable3dControlsStatic();	// Call this when linking to MFC statically
#endif

    // Change the registry key under which our settings are stored.
    // You should modify this string to be something appropriate
    // such as the name of your company or organization.
    //	SetRegistryKey(_T("EDT"));

    LoadStdProfileSettings();  // Load standard INI file options (including MRU)

    // get the device name

    if (!InitPump())
        return FALSE;


    m_pDocManager = new CPdvshowDocManager;

    // Register the application's document templates.  Document templates
    //  serve as the connection between documents, frame windows and views.

    //CMultiDocTemplate* pDocTemplate;
    m_pDocTemplate = new CMultiDocTemplate(
            IDR_PDVSHOTYPE,
            RUNTIME_CLASS(CPdvshowDoc),
            RUNTIME_CLASS(CChildFrame), // custom MDI child frame
            RUNTIME_CLASS(CPdvshowView));
    AddDocTemplate(m_pDocTemplate);

    // Connect the COleTemplateServer to the document template.
    //  The COleTemplateServer creates new documents on behalf
    //  of requesting OLE containers by using information
    //  specified in the document template.
    m_server.ConnectTemplate(clsid, m_pDocTemplate, FALSE);

    // Register all OLE server factories as running.  This enables the
    //  OLE libraries to create objects from other applications.
    COleTemplateServer::RegisterAll();
    // Note: MDI applications register all server objects without regard
    //  to the /Embedding or /Automation on the command line.

    // create main MDI Frame window
    CMainFrame* pMainFrame = new CMainFrame;

    pMainFrame->SetPump(m_pPump);
    if (!pMainFrame->LoadFrame(IDR_MAINFRAME))
        return FALSE;
    m_pMainWnd = pMainFrame;

    // Parse command line for standard shell commands, DDE, file open
    CCommandLineInfo cmdInfo;
    ParseCommandLine(cmdInfo);

    // Check to see if launched as OLE server
    if (cmdInfo.m_bRunEmbedded || cmdInfo.m_bRunAutomated)
    {
        // Application was run with /Embedding or /Automation.  Don't show the
        //  main window in this case.
        return TRUE;
    }

    // When a server application is launched stand-alone, it is a good idea
    //  to update the system registry in case it has been damaged.
    //m_server.UpdateRegistry(OAT_DISPATCH_OBJECT);
    //COleObjectFactory::UpdateRegistryAll();

    // Dispatch commands specified on the command line
    if (!ProcessShellCommand(cmdInfo))
        return FALSE;

    PdvInput *pCamera = m_pPump->GetCamera();

    if (pCamera->GetDevice() == NULL)
    {
        char msg[128];

        sprintf(msg, "%s%d_%d not found or not a framegrabber", pCamera->GetDevName(), pCamera->GetUnitNumber(), pCamera->GetChannelNumber());
        AfxMessageBox(msg, MB_OK|MB_ICONEXCLAMATION,0);
        return FALSE;
    }

    // Attempt to show the full image size
    if (pCamera && pCamera->GetWidth())
    {
        pMainFrame->SetWindowPos(NULL,0,0,pCamera->GetWidth() + 60,
                pCamera->GetHeight() + 60,SWP_NOSIZE | SWP_NOZORDER | SWP_NOACTIVATE);
    }

    // The main window has been initialized, so show and update it.
    pMainFrame->ShowWindow(m_nCmdShow);
    pMainFrame->UpdateWindow();

    return TRUE;
}



/////////////////////////////////////////////////////////////////////////////
// CAboutDlg dialog used for App About

class CAboutDlg : public CDialog
{
    public:
        CAboutDlg();

        // Dialog Data
        //{{AFX_DATA(CAboutDlg)
        enum { IDD = IDD_ABOUTBOX };
        //}}AFX_DATA

        // ClassWizard generated virtual function overrides
        //{{AFX_VIRTUAL(CAboutDlg)
    protected:
        virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
        //}}AFX_VIRTUAL

        // Implementation
    protected:
        //{{AFX_MSG(CAboutDlg)
        // No message handlers
        //}}AFX_MSG
        DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialog(CAboutDlg::IDD)
{
    //{{AFX_DATA_INIT(CAboutDlg)
    //}}AFX_DATA_INIT
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
    CDialog::DoDataExchange(pDX);
    //{{AFX_DATA_MAP(CAboutDlg)
    //}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialog)
    //{{AFX_MSG_MAP(CAboutDlg)
    // No message handlers
    //}}AFX_MSG_MAP
END_MESSAGE_MAP()

    // App command to run the dialog
void CPdvshowApp::OnAppAbout()
{
    CAboutDlg aboutDlg;
    aboutDlg.DoModal();
}

/////////////////////////////////////////////////////////////////////////////
// CPdvshowApp commands


void CPdvshowApp::GoLive()
{
    ImageWindowData *windows[MAX_LIVE_WINDOWS];
    int nWindows = 0;

    POSITION pos = m_pDocTemplate->GetFirstDocPosition();

    while (pos != NULL && nWindows < MAX_LIVE_WINDOWS)
    {
        CPdvshowDoc* pDoc = (CPdvshowDoc*)m_pDocTemplate->GetNextDoc(pos);

        pDoc->AlignWithCamera();

        CPdvshowView *pView;

        POSITION viewpos = pDoc->GetFirstViewPosition();
        pView = (CPdvshowView *) pDoc->GetNextView( viewpos );

        // should check for right class

        windows[nWindows] = &pView->m_wndImageWindow.m_wData;
        nWindows++;


    }

    m_pPump->PrepareBuffers();

    m_pPump->StartLive(windows, nWindows, TRUE);

}

CEdtImageWin * CPdvshowApp::FindImageWindow(EdtImage * pImage)
{

    POSITION pos = m_pDocTemplate->GetFirstDocPosition();

    while (pos != NULL)
    {
        CDocument* pDoc = m_pDocTemplate->GetNextDoc(pos);

        CPdvshowView *pView;

        POSITION viewpos = pDoc->GetFirstViewPosition();
        pView = (CPdvshowView *) pDoc->GetNextView( viewpos );

        // should check for right class

        if (pImage == pView->m_wndImageWindow.GetBaseImage())
            return &pView->m_wndImageWindow;


    }

    return NULL;

}
// Prompt for file name - used for open and save as
BOOL CPdvshowDocManager::DoPromptFileName(CString& fileName, UINT nIDSTitle, DWORD lFlags,
        BOOL bOpenFileDialog, CDocTemplate* pTemplate)
// if pTemplate==NULL => all document templates
{
    CFileDialog dlgFile(bOpenFileDialog);

    CString title;
    char imagetype[MAX_PATH];


    VERIFY(title.LoadString(nIDSTitle));


    if (theApp.m_strFileName == "__NOFILENAME__")
    {
        if (theApp.GetAutoNumber())
            fileName = theApp.m_strFileName = "pdvshow.000.bmp";
        else fileName = "pdvshow.bmp";
    }
    theApp.GetNextFileName(fileName);

    GetFileSuffix(fileName,imagetype,511);

#if 0
    if (theApp.GetAutoNumber())
    {
        char pathname[MAX_PATH];
        char basename[MAX_PATH];

        int index = theApp.SplitFileName(fileName, basename, imagetype, MAX_PATH);

        sprintf(pathname, "%s.%03d.%s", basename, index+1, imagetype);
        fileName = theApp.m_strFileName = pathname;
    }
#endif

    dlgFile.m_ofn.Flags |= lFlags;
    dlgFile.m_ofn.nMaxCustFilter = 0;

    // construct the various file filters
    CString strFilter = _T("");
    strFilter += _T("Bitmap image files (*.bmp)");
    strFilter += (TCHAR)'\0';
    strFilter += _T("*.bmp");
    strFilter += (TCHAR)'\0';
    dlgFile.m_ofn.nMaxCustFilter++;
    if (!bOpenFileDialog)
    {	// only allow these files types on a save
        strFilter += _T("TIF image files (*.tif)");
        strFilter += (TCHAR)'\0';
        strFilter += _T("*.tif");
        strFilter += (TCHAR)'\0';
        dlgFile.m_ofn.nMaxCustFilter++;
        strFilter += _T("Raw image files (*.raw)");
        strFilter += (TCHAR)'\0';
        strFilter += _T("*.raw");
        strFilter += (TCHAR)'\0';
        dlgFile.m_ofn.nMaxCustFilter++;
    }
    strFilter += _T("All files (*.*)");
    strFilter += (TCHAR)'\0';
    strFilter += _T("*.*");
    strFilter += (TCHAR)'\0';
    dlgFile.m_ofn.nMaxCustFilter++;

    dlgFile.m_ofn.lpstrDefExt = _T("bmp");
    dlgFile.m_ofn.nFilterIndex = 1;
    if (!bOpenFileDialog)
    {	// on a save see if the user entered a suffix and set the default appropriately
        TCHAR szSuffix[4];
        int nFileSuffix = GetFileSuffix(fileName, szSuffix, 4);
        CString strSuffix = szSuffix;
        if (strSuffix.GetLength() == 3)
        {	strSuffix.MakeLower();
            if (strSuffix == _T("bmp"))
            {	dlgFile.m_ofn.lpstrDefExt = _T("bmp");
                dlgFile.m_ofn.nFilterIndex = 1;
                fileName = fileName.Left(nFileSuffix-1);
            }
            else if (strSuffix == _T("tif"))
            {	dlgFile.m_ofn.lpstrDefExt = _T("tif");
                dlgFile.m_ofn.nFilterIndex = 2;
                fileName = fileName.Left(nFileSuffix-1);
            }
            else if (strSuffix == _T("raw"))
            {	dlgFile.m_ofn.lpstrDefExt = _T("raw");
                dlgFile.m_ofn.nFilterIndex = 3;
                fileName = fileName.Left(nFileSuffix-1);
            }
        }
    }

    dlgFile.m_ofn.lpstrFilter = strFilter;
    dlgFile.m_ofn.lpstrTitle = title;

    dlgFile.m_ofn.lpstrFile = fileName.GetBuffer(_MAX_PATH);
    int nDirPath= GetDirPath(dlgFile.m_ofn.lpstrFile, NULL, 0);
    dlgFile.m_ofn.nFileOffset = nDirPath;
    int nFileSuffix = GetFileSuffix(dlgFile.m_ofn.lpstrFile, NULL, 0);
    dlgFile.m_ofn.nFileExtension = nFileSuffix;

    BOOL bResult = dlgFile.DoModal() == IDOK ? TRUE : FALSE;
    fileName.ReleaseBuffer();

    return bResult;
}

void CPdvshowApp::OnFileOpen() 
{
    // TODO: Add your command handler code here
    /////////////////////////////////////////////////////////////////////////////
    CString newName;
    if (!DoPromptFileName(newName, AFX_IDS_OPENFILE,
                OFN_HIDEREADONLY | OFN_FILEMUSTEXIST, TRUE, NULL))
        return; // open cancelled

    OpenDocumentFile(newName);
    // if returns NULL, the user has already been alerted

}

void CPdvshowApp::UpdateAllDocViews()
{

    POSITION pos = m_pDocTemplate->GetFirstDocPosition();

    while (pos != NULL)
    {
        CDocument* pDoc = m_pDocTemplate->GetNextDoc(pos);

        pDoc->UpdateAllViews(NULL);
    }

}



CEdtImageWin * CPdvshowApp::FindImageWindow(ImageWindowData *pWindowData)
{
    POSITION pos = m_pDocTemplate->GetFirstDocPosition();

    while (pos != NULL)
    {
        CDocument* pDoc = m_pDocTemplate->GetNextDoc(pos);

        CPdvshowView *pView;

        POSITION viewpos = pDoc->GetFirstViewPosition();
        pView = (CPdvshowView *) pDoc->GetNextView( viewpos );

        // should check for right class

        if (pWindowData == &pView->m_wndImageWindow.m_wData)
            return &pView->m_wndImageWindow;


    }

    return NULL;
}

BOOL CPdvshowApp::InitPump()
{

    BOOL nBufsSet = FALSE;
    BOOL wait_for_screen = FALSE;
    BOOL bNoReset = FALSE;

    BOOL check_overrun = FALSE;
    BOOL pingpong = false;


    char devname[256];

    int ncameras = 0;
    int curcamera = 0;
    int i;

    int unit[MAX_CAMERAS];
    int channel[MAX_CAMERAS];

    memset(unit,0, sizeof(unit));
    memset(channel,0, sizeof(unit));

    char szModulePath[_MAX_PATH];

    ::GetModuleFileName(AfxGetInstanceHandle(), szModulePath, _MAX_PATH);
    char szDirPath[_MAX_PATH];

    GetDirPath(szModulePath, szDirPath, _MAX_PATH);
    m_strModulePath = szDirPath;

    int nBufs ;

    strcpy(devname,EDT_INTERFACE);

    // Select the camera card unit
    unit[0] = 0;
    for (i = 1; i < __argc; i++)
    {	
        LPCTSTR pszParam = __targv[i];
        if (pszParam[0] == '-' || pszParam[0] == '/')
        {	
            if (!strncmp(pszParam+1, "pdv", 3) || !strncmp(pszParam+1, "PDV", 3))
            {	
                char szUnit[256];
                strcpy(szUnit,pszParam+1);

                unit[curcamera] = edt_parse_unit_channel(szUnit,
                        devname,
                        EDT_INTERFACE,
                        &channel[curcamera]);

                ncameras ++;
                curcamera ++;

                m_Dbg = 0 ;
            }
            else if (strncmp(pszParam+1, "dmy", 3) == 0)
            {
                char szUnit[64];
                strcpy(szUnit, pszParam+4);
                unit[curcamera] = atoi(szUnit);
                strcpy(devname,"dmy");

                ncameras ++;
                curcamera ++;

                m_Dbg = 1 ;
            }
            else if (strcmp(pszParam+1, "wait") == 0)
            {	
                wait_for_screen = TRUE;

            }
            else if (strncmp(pszParam+1, "noreset", 5) == 0)
            {
                bNoReset = TRUE;
            }
            else if (strncmp(pszParam+1, "over", 4) == 0)
            {
                check_overrun = TRUE;
            }
            else if (strncmp(pszParam+1, "pp", 4) == 0)
            {
                pingpong = TRUE;
            }
            else if (strncmp(pszParam+1, "nb", 2) == 0)
            {
                char szBufs[64];
                strcpy(szBufs, pszParam+3);
                nBufsSet = TRUE;
                nBufs = atoi(szBufs);
            }
            else if (strncmp(pszParam+1, "z", 1) == 0)
            {
                char szBufs[64];

                strcpy(szBufs, pszParam+2);

                m_dDefaultZoom = atof(szBufs);


            }
        }
    }

    if (!ncameras)
    {
        ncameras = 1;
        unit[0] = 0;
        channel[0] = 0;
    }

    // The one and NOT necessarily only CLimitSingleInstance object
    // (modified from the example to handle possible multiple pdvshows
    // connected to different devices
    CLimitSingleInstance g_SingleInstanceObj("PdvShowIdHowCanThisNotBeUniqueFor", 
            devname, unit[0],channel[0]); 

    if (g_SingleInstanceObj.IsAnotherInstanceRunning())
    {
        char msg[128];
        sprintf(msg, "Pdvshow -%s%d already running!", devname, unit[0]);
        AfxMessageBox(msg, MB_OK|MB_ICONEXCLAMATION,0);
        return FALSE; 
    }

    m_pPump = new LiveDisplay((m_Dbg)?"dmy":devname, 
            unit[0], 
            channel[0]);

    m_pCamera = m_pPump->GetCamera();

    if (check_overrun)
        m_pCamera->SetCheckOverrun(true);

    if (!m_pCamera)
    {
        char msg[128];

        sprintf(msg, "Could not access device <%s>, unit %d -- exiting", devname, unit[0]);
        AfxMessageBox(msg, MB_OK|MB_ICONEXCLAMATION,0);
        return FALSE;
    }

#if 0
    // Make sure we have a capture directory
    char szCaptureDirectory[_MAX_PATH];
    sprintf(szCaptureDirectory, "%s\\%s", m_strModulePath, "camera_capture");
    m_strDefaultCaptureDir = szCaptureDirectory;
    _mkdir(m_strDefaultCaptureDir);
#endif

    RestoreCaptureProfile();


    char cwd_path[_MAX_PATH+1];


    if (nBufsSet)
    {
        if (nBufs == 0)
        {	
            m_pCamera->SetDoBufs(FALSE) ;
        }
        else
        {	
            m_pCamera->SetNBufs(nBufs) ;
        }
    }

    if (wait_for_screen)
    {


    }

    m_pCamera->SetNCameras(ncameras);
    if (ncameras > 1)
        m_pCamera->SetCycleCamera(true);
    else
        m_pCamera->SetCycleCamera(false);

    if (pingpong)
        m_pCamera->SetPingPong(true);

    for (i=0;i<ncameras;i++)
    {
        m_pCamera->SetCameraIndex(i);

        CString s;

        s.Format("%s\\camera_config",getcwd(cwd_path,_MAX_PATH));

        m_pCamera->SetCamConfigPath(s);


        if (m_pCamera->Initialize((m_Dbg)? "dmy":EDT_INTERFACE, unit[i],
                    channel[i]) && 
                m_pCamera->GetWidth() && m_pCamera->GetHeight())
        {	

        }
        else
        {

            CCamSelectDialog dlg;

            dlg.GetConfigFile(m_pCamera);

        }

    }

    m_pCamera->SetCameraIndex(0);

    return TRUE;
}

void CPdvshowApp::RestoreCaptureProfile()
{

}

PdvInput * CPdvshowApp::GetCamera()
{
    return m_pCamera;
}

LiveDisplay * CPdvshowApp::GetPump()
{
    return m_pPump;
}	

void CPdvshowApp::ResetAllDocs()
{

    // first delete all the existing documents

    CloseAllDocuments(FALSE);

    // now create a single new one

    AfxGetMainWnd()->PostMessage(WM_COMMAND, ID_FILE_NEW);




}


int CPdvshowApp::SplitFileName(LPCTSTR fname, 
        char *basename, 
        char *extension, 
        int max_size)

{
    char * lastperiod, 
    * penultperiod;
    int index = 0;

    strcpy(basename, fname);

    lastperiod = strrchr(basename, '.');
    if (lastperiod)
    {
        *lastperiod = 0;
        lastperiod++;
        strcpy(extension, lastperiod);
    }

    penultperiod = strrchr(basename, '.');
    if (penultperiod && isdigit(penultperiod[1]))
    {
        *penultperiod = 0;
        penultperiod++;
        index = atoi(penultperiod);
    }

    return index;
}

void CPdvshowApp::GetNextFileName(LPCTSTR& lpszPathName)
{

    char imagetype[MAX_PATH];
    CPdvshowApp * pApp =(CPdvshowApp *) AfxGetApp();

    if (pApp->GetAutoNumber())
    {
        char pathname[MAX_PATH];
        char basename[MAX_PATH];

        int index = pApp->SplitFileName(lpszPathName, basename, imagetype, MAX_PATH);

        sprintf(pathname, "%s.%03d.%s", basename, index+1, imagetype);
        lpszPathName = pApp->m_strFileName = pathname;
    }
}



void CPdvshowApp::GetNextFileName(CString& lpszPathName)
{

    char imagetype[MAX_PATH];
    CPdvshowApp * pApp =(CPdvshowApp *) AfxGetApp();

    if (pApp->GetAutoNumber())
    {
        char pathname[MAX_PATH];
        char basename[MAX_PATH];

        int index = pApp->SplitFileName(lpszPathName, basename, imagetype, MAX_PATH);

        sprintf(pathname, "%s.%03d.%s", basename, index+1, imagetype);
        lpszPathName = pApp->m_strFileName = pathname;
    }
}
