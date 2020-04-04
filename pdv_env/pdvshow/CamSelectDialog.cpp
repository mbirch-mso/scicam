// CamSelectDialog.cpp : implementation file
//

#include "stdafx.h"

#include "resource.h"

#include "CamSelectDialog.h"

#include "ConfigDataSet.h"

#include "PdvInput.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#define	CAMDIRNAME			_T("camera_config")
#define	CAMSETUPNAME		_T(".\\initcam -u %s -f \"%s\" > camsetup.out");
#define	CAMTYPEFIELD		_T("cameratype:")
#define	CAMFILEMAX			32000
#define	CAMFILESUFFIX		_T("cfg")

/////////////////////////////////////////////////////////////////////////////
// CCamSelectDialog dialog


CCamSelectDialog::CCamSelectDialog(CWnd* pParent /*=NULL*/,
				   LPCSTR szConfigPath /* = NULL */,
				   LPCSTR szCamFile /* = NULL */)
				   : CDialog(CCamSelectDialog::IDD, pParent)
{
    //{{AFX_DATA_INIT(CCamSelectDialog)
    m_sConfigPath = _T("");
    m_sCamType = _T("");
    m_sCamFile = _T("");
    //}}AFX_DATA_INIT

    if (szCamFile)
	m_sCamFile = szCamFile;

    if (szConfigPath)
	m_sConfigPath = szConfigPath;

    m_bAllowNewEntries = FALSE;
    m_bEdited = FALSE;

    m_pCamera = NULL;

}


CCamSelectDialog::~CCamSelectDialog()
{
    ClearCamList();

}

void CCamSelectDialog::DoDataExchange(CDataExchange* pDX)
{
    CDialog::DoDataExchange(pDX);
    //{{AFX_DATA_MAP(CCamSelectDialog)
    DDX_Control(pDX, IDC_CAMLIST, m_wndCameraList);
    DDX_Control(pDX, IDC_CAMTYPE, m_wndCamType);
    DDX_Control(pDX, IDC_CAMCONFIG, m_wndCamConfig);
    DDX_Text(pDX, IDC_CONFIG_PATH, m_sConfigPath);
    DDX_Text(pDX, IDC_CAMTYPE, m_sCamType);
    DDX_Text(pDX, IDC_CAMCONFIG, m_sCamFile);
    //}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CCamSelectDialog, CDialog)
    //{{AFX_MSG_MAP(CCamSelectDialog)
    ON_BN_CLICKED(IDC_BROWSE_PATH, OnBrowsePath)
    ON_LBN_KILLFOCUS(IDC_CAMLIST, OnKillfocusCamlist)
    ON_LBN_DBLCLK(IDC_CAMLIST, OnDblclkCamlist)
    ON_WM_CREATE()
    ON_EN_UPDATE(IDC_CAMCONFIG, OnUpdateCamconfig)
    ON_EN_UPDATE(IDC_CAMTYPE, OnUpdateCamtype)
    ON_NOTIFY(TVN_SELCHANGED, IDC_CAMLIST, OnSelchangedCamlist)
    //}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CCamSelectDialog message handlers

void CCamSelectDialog::OnBrowsePath() 
{

    // TODO: Add your control notification handler code here

    UpdateData(TRUE);


    BuildCameraList();

    UpdateData(FALSE);


}



void CCamSelectDialog::OnKillfocusCamlist() 
{

    UpdateFromList();

}

void CCamSelectDialog::OnOK() 
{
    // TODO: Add extra validation here

    // Get the selection

    if (!m_bEdited)
	UpdateFromList();

    CDialog::OnOK();
}

// Scan the camara directory and build a list of supported cameras

int __cdecl CompareCamClass(const void *elem1, const void *elem2)

{
    CCameraRecord **p1 = (CCameraRecord **) elem1;
    CCameraRecord **p2 = (CCameraRecord **) elem2;

    int rc =  strcmp((*p1)->szCamClass, (*p2)->szCamClass);

    TRACE("Compare %s - %s = %d\n", (*p1)->szCamClass, (*p2)->szCamClass, rc);

    return rc;

}

// Strip leading and ending quotes 

void StripQuotes(char *instr, CString &out)
{


    if (instr[0] == '"' && instr[1])
    {
	int j;
	char buffer[_MAX_PATH];

	strcpy(buffer,instr+1);

	j = (int) strlen(buffer)-1;

	while (j > 0 && isspace(buffer[j]) || buffer[j] == '"')
	{
	    j--;

	}

	if (buffer[j+1] == '"')
	    buffer[j+1] = '\0';

	out = buffer;
    }

    else

	out = instr;

}

void CCamSelectDialog::BuildCameraList()
{


    // Scan all the files in the installed camaras directory and build a list
    HANDLE hFile;
    char szWildCard[_MAX_PATH];
    int cldev, foxdev;

    sprintf(szWildCard, "%s\\*.%s", m_sConfigPath, CAMFILESUFFIX);

    WIN32_FIND_DATA FindFileData;
    hFile = FindFirstFile(szWildCard, &FindFileData);
    if (hFile == INVALID_HANDLE_VALUE)
    {	return;
    }

    cldev = IsClink(m_pCamera);
    foxdev = IsFox(m_pCamera);

    if (m_pCamera->GetDevice() == NULL)
    {
        char msg[256];
        sprintf(msg, "%s%d_%d not found or not a framegrabber", m_pCamera->GetDevName(), m_pCamera->GetUnitNumber(), m_pCamera->GetChannelNumber());
        AfxMessageBox(msg, MB_OK|MB_ICONEXCLAMATION,0);
        exit(1);
    }

    ClearCamList();
    bool bMoreToRead = TRUE;
    while (bMoreToRead)
    {

	CString strCamType;
	CString strCamClass;
	CString strCamModel;
	CString strCamInfo;

	char szCamFileName[_MAX_PATH];
	sprintf(szCamFileName, "%s\\%s", m_sConfigPath, FindFileData.cFileName);

	ConfigDataSet cfg;

	cfgset_init(&cfg);
	cfgset_load(&cfg, szCamFileName);

	char *test;
	int clflag;

	if (test = cfgset_lookup(&cfg, "cameratype"))
	    StripQuotes(test,strCamType);
	else strCamType = "";

	if (test = cfgset_lookup(&cfg, "camera_class"))
	    StripQuotes(test,strCamClass);
	else strCamClass = "";

	if (test = cfgset_lookup(&cfg, "camera_model"))
	    StripQuotes(test,strCamModel);
	else strCamModel = "";

	if (test = cfgset_lookup(&cfg, "camera_info"))
	    StripQuotes(test,strCamInfo);
	else strCamInfo = "";

	if (test = cfgset_lookup(&cfg, "cameralink"))
	    clflag = atoi(test);
	else if (cfgset_lookup(&cfg, "CL_CFG_NORM") || cfgset_lookup(&cfg, "CL_DATA_PATH_NORM"))
	    clflag = 1;
	else clflag = 0;


	//TRACE("File %s Type %s Clases %s\n", 
	//	szCamFileName, strCamType, strCamClass);

	cfgset_destroy(&cfg);

	if ((foxdev || (cldev && clflag) || (!cldev && !clflag))
	    && ((strCamType.GetLength()
	    || ((strCamModel.GetLength()) && (strCamClass.GetLength())))))
	{
	    CCameraRecord *pCamRec = NULL;
	    TRY
	    {	
		pCamRec = new CCameraRecord;
	    }
	    CATCH_ALL (e)
	    {
		break;
	    }
	    END_CATCH_ALL

		strcpy(pCamRec->szCamFile, szCamFileName);
	    strcpy(pCamRec->szCamClass, strCamClass);
	    if (*(strCamType))
		strcpy(pCamRec->szCamType, strCamType);
	    else
	    {
		sprintf(pCamRec->szCamType, "%s %s", strCamModel, strCamInfo);
	    }


	    CCameraRecord **pClasses = (CCameraRecord **) m_ClassList.GetData();

	    bool found = FALSE;

	    int i;

	    for (i=(int) m_ClassList.GetSize()-1;i>= 0; i--)
	    {
		if (pClasses)
		{
		    if (!strcmp(pClasses[i]->szCamClass, strCamClass))
		    {
			found = TRUE;
			break;
		    }
		}
	    }

	    if (!found)
		m_ClassList.Add(pCamRec);


	    TRY
	    {	
		m_CamList.Add(pCamRec);
	    }
	    CATCH_ALL (e)
	    {	
		delete pCamRec; 
		break;
	    }
	    END_CATCH_ALL

	}
	bMoreToRead = (FindNextFile(hFile, &FindFileData) != 0);
    }

    // Sort array by classes

    qsort(m_ClassList.GetData(), m_ClassList.GetSize(), sizeof (CCameraRecord *),
	CompareCamClass);

    SetListBox();

    FindClose(hFile);
}



// Locate a camera record by camera type
CCameraRecord* CCamSelectDialog::LookupCamera(const CString& strCameraType)
{
    if (strCameraType.IsEmpty())
    {	
	return NULL;
    }

    CCameraRecord* pCamRec = NULL;
    for (int idx = 0; idx < m_CamList.GetSize(); idx++)
    {
	pCamRec = (CCameraRecord*) m_CamList.GetAt(idx);
	if (pCamRec && strCameraType == pCamRec->szCamType)
	{
	    return pCamRec;
	}
    }

    return NULL;
}

void CCamSelectDialog::SetCameraPath(LPCSTR szNewPath)
{

    m_sConfigPath = szNewPath;

    BuildCameraList();

    if (m_hWnd)
        UpdateData(FALSE);

}

BOOL CCamSelectDialog::OnInitDialog() 
{
    CDialog::OnInitDialog();

    BuildCameraList();

    if (m_bAllowNewEntries)
    {

	m_wndCamConfig.SetReadOnly(FALSE );
	m_wndCamType.SetReadOnly(FALSE );
    }

    m_wndCameraList.SetIndent(32);

    UpdateData(TRUE);

    // TODO: Add extra initialization here

    return TRUE;  // return TRUE unless you set the focus to a control
    // EXCEPTION: OCX Property Pages should return FALSE
}

void CCamSelectDialog::SetListBox()
{
    // Add list of supported camera types
    //
    CPtrArray* pCameraList = &m_CamList;

    // Clear the list box

    m_wndCameraList.DeleteAllItems();

    HTREEITEM root =  NULL;

    //	root = m_wndCameraList.InsertItem("Cameras");

    //	m_wndCameraList.SetItemState(root, TVIS_EXPANDED, TVIS_EXPANDED);


    if (pCameraList)
    {   
	int idx;

	for ( idx = 0; idx < m_ClassList.GetSize(); idx++)
	{	
	    CCameraRecord* pCamRec = (CCameraRecord*) m_ClassList.GetAt(idx);


	    HTREEITEM parent = NULL;

	    parent = m_wndCameraList.InsertItem(pCamRec->szCamClass,root);

	    m_wndCameraList.SetItemData(parent, (LPARAM) pCamRec->szCamClass);
	}

	for ( idx = 0; idx < pCameraList->GetSize(); idx++)
	{	
	    CCameraRecord* pCamRec = (CCameraRecord*) pCameraList->GetAt(idx);	 
	    if (pCamRec)
	    {	
		HTREEITEM parent = NULL;
		if (strlen(pCamRec->szCamClass))
		{
		    if (!(parent = LookupClassString(pCamRec->szCamClass)))
		    {
			parent = m_wndCameraList.InsertItem(pCamRec->szCamClass,root);

			m_wndCameraList.SetItemData(parent, (LPARAM) pCamRec->szCamClass);

		    }
		}

		HTREEITEM nIndex = m_wndCameraList.InsertItem((const char*) pCamRec->szCamType,
		    parent);

		if (nIndex != NULL)
		{	
		    m_wndCameraList.SetItemData(nIndex, idx);

		    if (pCamRec->szCamFile == m_sCamFile)
		    {
			m_wndCameraList.Select(nIndex, TVGN_CARET);
			m_sCamType = pCamRec->szCamType;
		    }
		}
	    }
	}
    }

    SortAllEntries();


}

void CCamSelectDialog::ClearCamList()
{

    CCameraRecord* pCamRec = NULL;

    for (int idx = 0; idx < m_CamList.GetSize(); idx++)
    {
	pCamRec = (CCameraRecord*) m_CamList.GetAt(idx);
	if (pCamRec)
	{
	    delete pCamRec;
	}
    }

    m_CamList.RemoveAll();

    m_ClassList.RemoveAll();

}

void CCamSelectDialog::OnDblclkCamlist() 
{
    // TODO: Add your control notification handler code here
    OnOK();
}

void CCamSelectDialog::UpdateFromList()
{
    HTREEITEM item = m_wndCameraList.GetSelectedItem();

    if (item != NULL && !m_wndCameraList.ItemHasChildren(item))
    {
	int idx = (int) m_wndCameraList.GetItemData(item);

	if (idx < m_CamList.GetSize())
	{
	    CCameraRecord *pCamRec = (CCameraRecord *)m_CamList.GetAt(idx);

	    m_sCamFile = pCamRec->szCamFile;
	    m_sCamType = pCamRec->szCamType;

	    UpdateData(FALSE);

	    m_bEdited = FALSE;
	}
    }
}

int CCamSelectDialog::OnCreate(LPCREATESTRUCT lpCreateStruct) 
{
    if (CDialog::OnCreate(lpCreateStruct) == -1)
	return -1;


    // TODO: Add your specialized creation code here

    return 0;
}

void CCamSelectDialog::OnUpdateCamconfig() 
{
    // TODO: If this is a RICHEDIT control, the control will not
    // send this notification unless you override the CDialog::OnInitDialog()
    // with the ENM_UPDATE flag ORed into the lParam mask.

    // TODO: Add your control notification handler code here

    m_bEdited = TRUE;

}

void CCamSelectDialog::OnUpdateCamtype() 
{
    // TODO: If this is a RICHEDIT control, the control will not
    // send this notification unless you override the CDialog::OnInitDialog()
    // function to send the EM_SETEVENTMASK message to the control
    // with the ENM_UPDATE flag ORed into the lParam mask.

    // TODO: Add your control notification handler code here

    m_bEdited = TRUE;
}

void CCamSelectDialog::OnSelchangedCamlist(NMHDR* pNMHDR, LRESULT* pResult) 
{
    NM_TREEVIEW* pNMTreeView = (NM_TREEVIEW*)pNMHDR;
    // TODO: Add your control notification handler code here
    UpdateFromList();

    *pResult = 0;
}

HTREEITEM CCamSelectDialog::LookupClassString(const char *s)
{
    HTREEITEM root;
    HTREEITEM item;

    root = m_wndCameraList.GetRootItem();

    if (!root)
	return NULL;

    //	item = m_wndCameraList.GetChildItem(root);

    item = root;

    while (item)
    {
	CString test;

	test = m_wndCameraList.GetItemText(item);

	if (!strcmp(s,test))
	    return item;

	item = m_wndCameraList.GetNextSiblingItem(item);

    }

    return NULL;

}

int CALLBACK CamCompare(LPARAM param1, LPARAM param2, LPARAM lparamsort)

{
    int index1 = (int) param1;
    int index2 = (int) param2;

    CCamSelectDialog* pDlg = (CCamSelectDialog *) lparamsort;

    CCameraRecord *pCamRec1 = (CCameraRecord *)pDlg->m_CamList.GetAt(index1);
    CCameraRecord *pCamRec2 = (CCameraRecord *)pDlg->m_CamList.GetAt(index2);


    int rc = strcmp(pCamRec1->szCamType, pCamRec2->szCamType);

    //	TRACE("Compare %s - %s = %d\n", pCamRec1->szCamType, pCamRec2->szCamType, rc);

    return rc;
}

int CALLBACK CamClassCompare(LPARAM param1, LPARAM param2, LPARAM lparamsort)

{
    char * s1 = (char *)param1;
    char * s2 = (char *) param2;

    int rc = strcmp(s1, s2);

    //	TRACE("Compare %s - %s = %d\n", s1, s2, rc);

    return rc;
}


void CCamSelectDialog::SortAllEntries()
{
    HTREEITEM root;
    HTREEITEM item;

    TV_SORTCB tvs;

    root = m_wndCameraList.GetRootItem();

    if (!root)
	return ;

    //	tvs.hParent = root;
    //	tvs.lpfnCompare = CamClassCompare;
    //	tvs.lParam = (LPARAM) this;

    //	CString st = m_wndCameraList.GetItemText(root);

    //	TRACE("Tracing Root %s Has Children %d\n",st , m_wndCameraList.ItemHasChildren(root));

    //	m_wndCameraList.SortChildrenCB(&tvs);

    //	item = m_wndCameraList.GetChildItem(root);

    item = root;

    while (item)
    {
	tvs.hParent = item;
	tvs.lpfnCompare = CamCompare;
	tvs.lParam = (LPARAM) this;

	CString st = m_wndCameraList.GetItemText(item);

	TRACE("Tracing item %s Has Children %d\n",st , m_wndCameraList.ItemHasChildren(item));


	m_wndCameraList.SortChildrenCB(&tvs);

	item = m_wndCameraList.GetNextSiblingItem(item);

    }

}

bool CCamSelectDialog::SelectConfigFile(PdvInput *pCamera, bool bAllowNew)
{

    if (pCamera)
	SetCamera(pCamera);

    if (!m_pCamera)
	return FALSE;

    SetAllowNewEntries(bAllowNew);

    m_sConfigPath = pCamera->GetCamConfigPath();
    m_sCamFile = pCamera->GetCamFile();
    m_sCamType = pCamera->GetCameraType();
    if (DoModal() == IDOK)
    {	

	CString sOldCamera = m_pCamera->GetCameraSelect();

	m_pCamera->SetCameraStrings(m_sCamType, m_sCamFile, m_sConfigPath);

	return TRUE;

    }

    return FALSE;

}

bool CCamSelectDialog::GetConfigFile(PdvInput *pCamera)
{
    if (SelectConfigFile(pCamera, FALSE))
    {
	if (!m_pCamera->Setup(m_sConfigPath, m_sCamFile))
  {
        AfxMessageBox(m_pCamera->lastError().c_str(), MB_OK|MB_ICONEXCLAMATION,0);
        exit(1);
  }

	return TRUE;
    }

    return FALSE;


}

bool CCamSelectDialog::IsClink(PdvInput *pCamera)
{
    pCamera->Initialize(EDT_INTERFACE, pCamera->GetUnitNumber(), pCamera->GetChannelNumber());
    return (pCamera->IsClink());
}

bool CCamSelectDialog::IsFox(PdvInput *pCamera)
{
    pCamera->Initialize(EDT_INTERFACE, pCamera->GetUnitNumber(), pCamera->GetChannelNumber());
    return (pCamera->IsFox());
}

