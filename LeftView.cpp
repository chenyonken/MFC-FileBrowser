// LeftView.cpp : implementation file
//

#include "stdafx.h"
#include "sp.h"
#include "LeftView.h"
#include "MainFrm.h"
#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CLeftView
IMPLEMENT_DYNCREATE(CLeftView, CTreeView)

CLeftView::CLeftView():m_tree(GetTreeCtrl())
{
	m_treeSelPath=_T("");
}

CLeftView::~CLeftView()
{
}


BEGIN_MESSAGE_MAP(CLeftView, CTreeView)
	//{{AFX_MSG_MAP(CLeftView)
	ON_NOTIFY_REFLECT(TVN_SELCHANGED, OnSelchanged)
	ON_NOTIFY_REFLECT(TVN_ITEMEXPANDED, OnItemexpanded)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CLeftView drawing

void CLeftView::OnDraw(CDC* pDC)
{
	CDocument* pDoc = GetDocument();
	// TODO: add draw code here
}

/////////////////////////////////////////////////////////////////////////////
// CLeftView diagnostics

#ifdef _DEBUG
void CLeftView::AssertValid() const
{
	CTreeView::AssertValid();
}

void CLeftView::Dump(CDumpContext& dc) const
{
	CTreeView::Dump(dc);
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CLeftView message handlers


/******************************************
* void CLeftView::OnInitialUpdate() 
* ���ܣ����TreeView�ĳ�ʼ������,����ͼ���б�
*******************************************/
void CLeftView::OnInitialUpdate() 
{
	CTreeView::OnInitialUpdate();
	if(m_tree.GetChildItem(NULL))//
		return;
	//Ϊ���TreeView���ͼ���б�
	int nIDs[]={IDB_ROOT,IDB_DISK,IDB_DIR_SEL,IDB_DIR_UNSEL};
	const int nCount=sizeof(nIDs)/sizeof(nIDs[0]);
	CBitmap bmp[nCount];
	m_iList.Create(16,16,ILC_COLOR32|ILC_MASK,8,8);
	for(int i=0;i<nCount;i++)
		bmp[i].LoadBitmap(nIDs[i]);
	for(i=0;i<sizeof(nIDs)/sizeof(nIDs[0]);i++)
	{
		m_iList.Add(&bmp[i],RGB(255,255,255));
		bmp[i].DeleteObject();
	}
	m_tree.SetImageList(&m_iList,TVSIL_NORMAL);

	m_tree.ModifyStyle(0,TVS_HASLINES|TVS_LINESATROOT|TVS_HASBUTTONS|
		TVS_SHOWSELALWAYS);
	HTREEITEM hRoot=m_tree.InsertItem("�ҵĵ���",0,0);
	char buf[100];
	DWORD len=GetLogicalDriveStrings(sizeof(buf)/sizeof(char),buf);
	char* p=buf;
	CString str;
	while(*p)
	{
		str=p;
		int nLen=str.GetLength();
		if(nLen<1)
			break;
		if(str[nLen-1]=='\\')
			str=str.Left(nLen-1);
		HTREEITEM hItem=m_tree.InsertItem(str,1,1,hRoot);
		m_tree.InsertItem(_T(""),hItem);
		p+=nLen+1;
	}
	m_tree.Expand(hRoot,TVE_EXPAND);
	// TODO: Add your specialized code here and/or call the base class
	
}


/***********************************************************
* void CLeftView::OnSelchanged(NMHDR* pNMHDR, LRESULT* pResult) 
* ���ܣ���������οؼ��ڵ��ϵ�����ʱ����Ϣ��Ӧ����������
		�ұ�CListView��ĵ�refresh����ˢ��CListView�������
* �����������MSDN
************************************************************/
void CLeftView::OnSelchanged(NMHDR* pNMHDR, LRESULT* pResult) 
{
	NM_TREEVIEW* p = (NM_TREEVIEW*)pNMHDR;
	// TODO: Add your control notification handler code here
	HTREEITEM hItem=p->itemNew.hItem;
	CString szPath;
	if(hItem!=m_tree.GetRootItem())//�ڽ�����ȫ����֮ǰҲ�ᷢ��һ��SelChange��Ϣ�������Ϣ������
//		szPath=m_tree.GetItemText(hItem);
//	CMainFrame* pFrame=(CMainFrame*)AfxGetMainWnd();
//	pFrame->m_pRightView->Refresh(szPath);
	szPath=GetPath(hItem);
	int nLen=szPath.GetLength();
	if(nLen&&szPath[nLen-1]=='\\')
		szPath=szPath.Left(nLen-1);	

	CMainFrame* pFrame=(CMainFrame*)AfxGetMainWnd();
	m_treeSelPath=szPath;//����TreeView��ǰѡ�нڵ������·��
	m_hSelItem=hItem;
	pFrame->m_pRightView->Refresh(szPath);
	*pResult = 0;
}


/******************************************
* int CLeftView::GetDepth(HTREEITEM hItem)
* ���ܣ���ȡ�ڵ�����
* ��������Ҫ��ȡ��ȵĽڵ�ľ��
* ����ֵ���ڵ�����
********************************************/
int CLeftView::GetDepth(HTREEITEM hItem)
{
	int i=0;
	while(hItem)
	{
		hItem=m_tree.GetParentItem(hItem);
		++i;
	}
	return i;
}



/**********************************************
* CString CLeftView::GetPath(HTREEITEM hItem)
* ���ܣ���ȡĳ���ڵ������·���������ڵ�·��������\����
		�ں�����ʹ��ʱ�ض�����»���Ҫȥ�����ġ�\����
* ������hItem,��Ҫ�������·���Ľڵ�ľ��
* ����ֵ���ڵ������·��
**********************************************/
CString CLeftView::GetPath(HTREEITEM hItem)
{
	CString str;
	while(hItem)
	{
		str=m_tree.GetItemText(hItem)+_T("\\")+str;
		hItem=m_tree.GetParentItem(hItem);
		if(hItem==m_tree.GetRootItem())
			break;
	}
	return str;
}

/**************************************************************
* void CLeftView::OnItemexpanded(NMHDR* pNMHDR, LRESULT* pResult) 
* ���ܣ���������οؼ������ԱߵļӺ�ʱ����Ϣ��Ӧ������
		��ȡѡ�нڵ��·���;��������LoadItem���ظýڵ����Ŀ¼
* �����������MSDN
****************************************************************/
void CLeftView::OnItemexpanded(NMHDR* pNMHDR, LRESULT* pResult) 
{
	NM_TREEVIEW* p = (NM_TREEVIEW*)pNMHDR;
	// TODO: Add your control notification handler code here
	HTREEITEM hItem=p->itemNew.hItem;
	CString szPath;
	if(hItem!=m_tree.GetRootItem())//�ڽ�����ȫ����֮ǰҲ�ᷢ��һ��SelChange��Ϣ�������Ϣ������
//		szPath=m_tree.GetItemText(hItem);
		szPath=GetPath(hItem);

	int nLen=szPath.GetLength();
	if(nLen&&szPath[nLen-1]=='\\')
		szPath=szPath.Left(nLen-1);	
	LoadItem(szPath,hItem);
	*pResult = 0;
}



/*******************************************************
* void CLeftView::LoadItem(CString szPath,HTREEITEM hItem)
* ���ܣ����������οؼ���ĳһ��ʱ�������οؼ��м�������Ŀ¼
* ������szPath,����Ľڵ������·��
		hItem,����Ľڵ�ľ��
********************************************************/
void CLeftView::LoadItem(CString szPath,HTREEITEM hItem)
{
	if(szPath.IsEmpty())
		return;
	m_tree.DeleteItem(m_tree.GetChildItem(hItem));
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
			CString str;
			str=ff.GetFileName();
			HTREEITEM hParent=m_tree.InsertItem(str,3,2,hItem);

			//ʹ����Ŀ¼���ļ����ܹ���ʾ�Ӻ�
			CFileFind tmpff;
			BOOL tmpb2=tmpff.FindFile(szPath+_T("\\")+str+_T("/*.*"));
			while(tmpb2)
			{
				tmpb2=tmpff.FindNextFile();
				if(tmpff.IsDots())
					continue;
				if(tmpff.IsDirectory())
				{
					m_tree.InsertItem(_T(""),3,2,hParent);
					break;
				}
				else
					continue;	
			}
		}
		else
			continue;
	}
}
