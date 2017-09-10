// RightView.cpp : implementation file
//

#include "stdafx.h"
#include "sp.h"
#include "RightView.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CRightView
#include "MainFrm.h"

IMPLEMENT_DYNCREATE(CRightView, CListView)

CRightView::CRightView():m_list(GetListCtrl())
{
}

CRightView::~CRightView()
{
}


BEGIN_MESSAGE_MAP(CRightView, CListView)
	//{{AFX_MSG_MAP(CRightView)
	ON_NOTIFY_REFLECT(NM_DBLCLK, OnDblclk)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CRightView drawing

void CRightView::OnDraw(CDC* pDC)
{
	CDocument* pDoc = GetDocument();
	// TODO: add draw code here
}

/////////////////////////////////////////////////////////////////////////////
// CRightView diagnostics

#ifdef _DEBUG
void CRightView::AssertValid() const
{
	CListView::AssertValid();
}

void CRightView::Dump(CDumpContext& dc) const
{
	CListView::Dump(dc);
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CRightView message handlers

/**************************************************
* void CRightView::OnInitialUpdate() 
* ����:�ұ�ListVIew�ĳ�ʼ������������ͼ���б�
* ************************************************/
void CRightView::OnInitialUpdate() 
{
	CListView::OnInitialUpdate();
	
	// TODO: Add your specialized code here and/or call the base class
	if(m_list.GetStyle()&LVS_REPORT)
		return;

	//ΪListViee����ͼ��
	int nIDs[]={IDB_DOC,IDB_DIR_UNSEL};
	const int nCount=sizeof(nIDs)/sizeof(nIDs[0]);
	CBitmap bmp[nCount];
	for(int i=0;i<nCount;i++)
		bmp[i].LoadBitmap(nIDs[i]);
	m_iList.Create(16,16,ILC_COLOR32|ILC_MASK,8,8);
	for(i=0;i<nCount;i++)
	{
		m_iList.Add(&bmp[i],RGB(255,255,255));
		bmp[i].DeleteObject();
	}
	m_list.SetImageList(&m_iList,LVSIL_SMALL);

	m_list.ModifyStyle(0,LVS_REPORT);
	m_list.InsertColumn(0,_T("����"),0,200);
	m_list.InsertColumn(1,_T("��С"),0,120);
	m_list.InsertColumn(2,_T("����"),0,120);
	m_list.InsertColumn(3,_T("�޸�����"),0,120);
}


/******************************************
* void CRightView::Refresh(CString szPath)
* ����:�ұ�ListView��ˢ�º���
* ������szPath,���TreeViewѡ�нڵ������·��
*******************************************/
void CRightView::Refresh(CString szPath)
{
	m_list.DeleteAllItems();
	if(szPath.IsEmpty())
		return;
	CFileFind ff;
	BOOL b=ff.FindFile(szPath+_T("/*.*"));
	int i=0;
	while(b)
	{
		b=ff.FindNextFile();
		if(ff.IsDots()||ff.IsHidden())
			continue;

		if(ff.IsDirectory())
		{
			m_list.InsertItem(i,ff.GetFileName(),1);
			m_list.SetItemText(i,2,_T("�ļ���"));
		}
		else
		{	
			m_list.InsertItem(i,ff.GetFileName(),0);
			int nLen=ff.GetLength()/1024;
			if(!nLen)
				nLen=1;
			szPath.Format("%d kb",nLen);
			m_list.SetItemText(i,1,szPath);
			m_list.SetItemText(i,2,_T("�ļ�"));
		}
		FILETIME ft;
		ff.GetLastWriteTime(&ft);
		COleDateTime time=ft;
		szPath.Format("%d-%d-%d %d:%d",time.GetYear(),time.GetMonth(),
			time.GetDay(),time.GetHour(),time.GetMinute());
		m_list.SetItemText(i,3,szPath);
		++i;
	}
}


/***********************************************************
* void CRightView::OnDblclk(NMHDR* pNMHDR, LRESULT* pResult) 
* ���ܣ��ұ��б���ͼ�ģ���Ϣ��Ӧ������˫���б����ĳ��ʱ��
		�����ͼչ�����Զ�ѡ�е���Ӧ�Ľڵ�
* �����������MSDN
**********************************************************/
void CRightView::OnDblclk(NMHDR* pNMHDR, LRESULT* pResult) 
{
	// TODO: Add your control notification handler code here
	LPNMITEMACTIVATE p = (LPNMITEMACTIVATE) pNMHDR;
	CMainFrame* pFrame=(CMainFrame*)AfxGetMainWnd();
	CLeftView* pLeftView=pFrame->m_pLeftView;	
	CTreeCtrl& tree=pLeftView->m_tree;
	tree.Expand(pLeftView->m_hSelItem,TVE_EXPAND);

	HTREEITEM hParent=pLeftView->m_hSelItem;
	hParent=tree.GetChildItem(hParent);
	CString listSelText=m_list.GetItemText(p->iItem,p->iSubItem);
	CString szPath;
	while(hParent)
	{
		szPath=tree.GetItemText(hParent);
		if(szPath==listSelText)
		{
			szPath=pLeftView->GetPath(hParent);
			tree.Expand(hParent,TVE_EXPAND);
			tree.SelectItem(hParent);
		}
		hParent=tree.GetNextSiblingItem(hParent);
	}
	*pResult = 0;
}