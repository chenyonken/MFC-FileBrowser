#if !defined(AFX_LEFTVIEW_H__DBFE50A0_5530_4296_814B_5E2C4933ED85__INCLUDED_)
#define AFX_LEFTVIEW_H__DBFE50A0_5530_4296_814B_5E2C4933ED85__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// LeftView.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CLeftView view
#include<afxcview.h>
class CLeftView : public CTreeView
{
public:
	CTreeCtrl& m_tree;					//���TreeView�Ŀؼ�����
	CImageList m_iList;					//TreeView�ؼ������ͼ���б�
	CString m_treeSelPath;				//��ǰTreeCtrl��ѡ�нڵ������·��
	HTREEITEM m_hSelItem;				//��ǰTreeCtrl��ѡ�нڵ�ľ��
protected:
	CLeftView();           // protected constructor used by dynamic creation
	DECLARE_DYNCREATE(CLeftView)

// Attributes
public:

// Operations
public:
	void LoadItem(CString szPath,HTREEITEM hItem);
	CString GetPath(HTREEITEM hItem);
	int GetDepth(HTREEITEM hItem);

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CLeftView)
	public:
	virtual void OnInitialUpdate();
	protected:
	virtual void OnDraw(CDC* pDC);      // overridden to draw this view
	//}}AFX_VIRTUAL

// Implementation
protected:
	virtual ~CLeftView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

	// Generated message map functions
protected:
	//{{AFX_MSG(CLeftView)
	afx_msg void OnSelchanged(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnItemexpanded(NMHDR* pNMHDR, LRESULT* pResult);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_LEFTVIEW_H__DBFE50A0_5530_4296_814B_5E2C4933ED85__INCLUDED_)
