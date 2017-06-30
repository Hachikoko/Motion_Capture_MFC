#pragma once
#include "SuperGridCtrl.h"
#include "MyButton.h"
class CMySuperGrid :
	public CSuperGridCtrl
{
public:
	CMySuperGrid();
	~CMySuperGrid();

public:
	void InitializeGrid(void);
	void _DeleteAll(void);
	void DynamicUpdateSomeItems(int nItem);
	void SetNewImage(int nItem);
	CImageList *CreateDragImageEx(int nItem);
	BOOL m_bDrag;

	void OnControlLButtonDown(UINT nFlags, CPoint point, LVHITTESTINFO& ht);
	void OnUpdateListViewItem(CTreeItem* lpItem, LV_ITEM *plvItem);
	CItemInfo* CopyData(CItemInfo* lpSrc);
	int GetIcon(const CTreeItem* pItem);
	COLORREF GetCellRGB(void);
	BOOL OnItemExpanding(CTreeItem *pItem, int iItem);
	BOOL OnItemExpanded(CTreeItem* pItem, int iItem);
	BOOL OnCollapsing(CTreeItem *pItem);
	BOOL OnItemCollapsed(CTreeItem *pItem);
	BOOL OnDeleteItem(CTreeItem* pItem, int nIndex);

	BOOL OnVkReturn(void);
protected:
	CImageList m_image;
	CComboBox* ShowList(int nItem, int nCol, CStringList *lstItems);
	CMyButton* ShowButton(int nItem, int nCol);
	int CalcHorzExtent(CWnd* pWnd, CStringList *pList);
public:
	DECLARE_MESSAGE_MAP()
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnShowWindow(BOOL bShow, UINT nStatus);
};

