#include "stdafx.h"
#include "MySuperGrid.h"
#include "ComboInListView.h"
#include "resource.h"



CMySuperGrid::CMySuperGrid()
{
}


CMySuperGrid::~CMySuperGrid()
{
}

void CMySuperGrid::InitializeGrid(void)
{
	SetExtendedStyle(LVS_EX_GRIDLINES);
	LPTSTR lpszCols[] = { _T("绝对节点号"),_T("关节名称"),0 };
	LV_COLUMN   lvColumn;
	lvColumn.mask = LVCF_FMT | LVCF_WIDTH | LVCF_TEXT | LVCF_SUBITEM;
	lvColumn.fmt = LVCFMT_LEFT;
	lvColumn.cx = 100;

	for (int x = 0; lpszCols[x] != NULL; x++)
	{
		lvColumn.pszText = lpszCols[x];
		InsertColumn(x, &lvColumn);
	}

//	CItemInfo* lp = new CItemInfo();
//	lp->SetItemText(_T("节点5"));
//	lp->SetImage(0);
//	lp->AddSubItemText(_T("肩关节"));
//	lp->SetControlType(lp->CONTROLTYPE::combobox, 0);
//	CStringList list;
//	list.AddTail(_T("肩关节"));
//	list.AddTail(_T("肘关节"));
//	list.AddTail(_T("肘关节"));
//	list.AddTail(_T("肘关节"));
//	list.AddTail(_T("肘关节"));
//	list.AddTail(_T("肘关节"));
//	list.AddTail(_T("肘关节"));
//	list.AddTail(_T("肘关节"));
//	list.AddTail(_T("肘关节"));
//	lp->SetListData(0, &list);
//	CTreeItem * pRoot = InsertRootItem(lp);
//	if (pRoot == NULL)
//		return;
//
//	CItemInfo* lp1 = new CItemInfo();
//	lp1->SetItemText(_T("节点6"));
//	lp1->SetImage(1);
//	lp1->AddSubItemText(_T("肘关节"));
//	lp1->SetControlType(lp1->CONTROLTYPE::combobox, 0);
//	CStringList list1;
//	list1.AddTail(_T("肩关节"));
//	list1.AddTail(_T("肘关节"));
//	list1.AddTail(_T("肘关节"));
//	list1.AddTail(_T("肘关节"));
//	list1.AddTail(_T("肘关节"));
//	list1.AddTail(_T("肘关节"));
//	list1.AddTail(_T("肘关节"));
//	lp1->SetListData(0, &list1);
//	CTreeItem * pRoot1 = InsertRootItem(lp1);
//	if (pRoot1 == NULL)
//		return;
//	//could now expand one level
//	Expand(pRoot, 0 /*listview index 0*/);
//	Expand(pRoot1, 1 /*listview index 0*/);
////	UINT uflag = LVIS_SELECTED | LVIS_FOCUSED;
////	SetItemState(0, uflag, uflag);
}

void CMySuperGrid::_DeleteAll(void)
{
	DeleteAll();
}

void CMySuperGrid::DynamicUpdateSomeItems(int nItem)
{

}

void CMySuperGrid::SetNewImage(int nItem)
{
	CTreeItem*pItem = GetTreeItem(nItem);
	CItemInfo *lp = GetData(pItem);
	if (lp != NULL)
	{
		lp->SetSubItemImage(3/*subitem*/, 4/*image index*/);
		lp->SetSubItemImage(2/*subitem*/, 4/*image index*/);
		//update internal node
		UpdateData(pItem, lp, TRUE);
		InvalidateItemRect(nItem);
	}
}

CImageList * CMySuperGrid::CreateDragImageEx(int nItem)
{
	if (m_bDrag)
		return CSuperGridCtrl::CreateDragImageEx(GetDragItem());
	else
		return NULL;
}

void CMySuperGrid::OnControlLButtonDown(UINT nFlags, CPoint point, LVHITTESTINFO& ht)
{
	CTreeItem*pSelItem = GetTreeItem(ht.iItem);
	if (pSelItem != NULL)
	{
		CItemInfo* pInfo = GetData(pSelItem);
		CItemInfo::CONTROLTYPE ctrlType;
		if (pInfo->GetControlType(ht.iSubItem - 1, ctrlType))
		{
			if (ctrlType == pInfo->CONTROLTYPE::combobox)
			{
				CStringList* list = NULL;
				pInfo->GetListData(ht.iSubItem - 1, list);
				CComboBox * pList = ShowList(ht.iItem, ht.iSubItem, list);
			}
		}
	}
}

void CMySuperGrid::OnUpdateListViewItem(CTreeItem* lpItem, LV_ITEM *plvItem)
{
	//put some extra validation here 
	CString str = (CString)plvItem->pszText;
	if (!str.Compare(_T("Bugs: Impossible")))
	{
		str += _T(" ( ya right )");
		CItemInfo *lp = GetData(lpItem);
		if (lp != NULL)
		{
			if (plvItem->iSubItem == 0)
				lp->SetItemText(str);
			else //subitem data 
				lp->SetSubItemText(plvItem->iSubItem - 1, str);
			UpdateData(lpItem, lp);
			//update internal nodes, don磘 use bUpdateRow here hence we know the item and
			//can optimize the darn thing a little bit.
			SetItemText(plvItem->iItem, plvItem->iSubItem, str.GetBuffer(1));
		}
	}
	else
		CSuperGridCtrl::OnUpdateListViewItem(lpItem, plvItem);
}

CItemInfo * CMySuperGrid::CopyData(CItemInfo * lpSrc)
{
	ASSERT(lpSrc != NULL);
	CItemInfo* lpDest = new CItemInfo;
	//well okay I put all the copy thing in one function, located in CItemInfo class, 
	//so you should check out this function, remember to modify this function each time you add new data to CItemInfo class.
	lpDest->CopyObjects(lpSrc);
	return lpDest;
}

int CMySuperGrid::GetIcon(const CTreeItem* pItem)
{
	if (pItem != NULL)
	{
		int n = GetData(pItem)->GetImage();
		if (n != -1)
			return n;

		int iImage = 0;
		if (ItemHasChildren(pItem))
		{
			IsCollapsed(pItem) ? iImage = 1/*close icon*/ : iImage = 0;/*open icon*/
		}
		else
			iImage = 2;//doc icon
		return iImage;
	}
	return 0;
}

COLORREF CMySuperGrid::GetCellRGB(void)
{
	return RGB(192, 0, 0);
}

BOOL CMySuperGrid::OnItemExpanding(CTreeItem *pItem, int iItem)
{
	return 1;
}

BOOL CMySuperGrid::OnItemExpanded(CTreeItem* pItem, int iItem)
{
	return 1;
}

BOOL CMySuperGrid::OnCollapsing(CTreeItem *pItem)
{
	return 1;
}

BOOL CMySuperGrid::OnItemCollapsed(CTreeItem *pItem)
{
	return 1;
}

BOOL CMySuperGrid::OnDeleteItem(CTreeItem* pItem, int nIndex)
{
	return 1;
}

BOOL CMySuperGrid::OnVkReturn(void)
{
	BOOL bResult = FALSE;
	int iItem = GetNextItem(-1, LVNI_ALL | LVNI_SELECTED);
	if (GetCurSubItem() != -1 && iItem != -1)
	{
		CTreeItem*pSelItem = GetTreeItem(iItem);
		if (pSelItem != NULL)
		{
			CHeaderCtrl* pHeader = (CHeaderCtrl*)GetDlgItem(0);
			int iSubItem = Header_OrderToIndex(pHeader->m_hWnd, GetCurSubItem());
			CItemInfo* pInfo = GetData(pSelItem);
			CItemInfo::CONTROLTYPE ctrlType;
			if (pInfo->GetControlType(iSubItem - 1, ctrlType))
			{
				switch (ctrlType)
				{
					/*put in your own control here*/
				case pInfo->CONTROLTYPE::datecontrol:break;
				case pInfo->CONTROLTYPE::button:
				{
					//CMyButton * pButton = ShowButton(iItem, iSubItem);
					//bResult = TRUE;
				}break;
				case pInfo->CONTROLTYPE::dropdownlistviewwhatevercontrol:break;
				case pInfo->CONTROLTYPE::combobox:
				{

					CStringList* list = NULL;
					pInfo->GetListData(iSubItem - 1, list);
					CComboBox * pList = ShowList(iItem, iSubItem, list);
					bResult = TRUE; //I'll handle it from here
				}break;
				default:break;
				}
			}
		}
	}
	return bResult;
}

#define IDC_COMBOBOXINLISTVIEW 0x1235
CComboBox* CMySuperGrid::ShowList(int nItem, int nCol, CStringList *lstItems)
{
	CString strFind = GetItemText(nItem, nCol);

	//basic code start
	CRect rect;
	int offset = 0;
	// Make sure that the item is visible
	if (!EnsureVisible(nItem, TRUE)) return NULL;
	GetSubItemRect(nItem, nCol, LVIR_BOUNDS, rect);
	// Now scroll if we need to expose the column
	CRect rcClient;
	GetClientRect(rcClient);
	if (offset + rect.left < 0 || offset + rect.left > rcClient.right)
	{
		CSize size;
		size.cx = offset + rect.left;
		size.cy = 0;
		Scroll(size);
		rect.left -= size.cx;
	}

	rect.left += offset;
	rect.right = rect.left + GetColumnWidth(nCol);
	if (rect.right > rcClient.right)
		rect.right = rcClient.right;
	//basic code end

	rect.bottom += 10 * rect.Height();//dropdown area
	DWORD dwStyle = WS_CHILD | WS_VISIBLE | WS_VSCROLL | WS_HSCROLL | CBS_DROPDOWNLIST | CBS_DISABLENOSCROLL;
	CComboBox *pList = new CComboInListView(nItem, nCol, lstItems);
	pList->Create(dwStyle, rect, this, IDC_COMBOBOXINLISTVIEW);
	pList->ModifyStyleEx(0, WS_EX_CLIENTEDGE);//can we tell at all
	pList->SetHorizontalExtent(CalcHorzExtent(pList, lstItems));
	pList->ShowDropDown();
	pList->SelectString(-1, strFind.GetBuffer(1));
	// The returned pointer should not be saved
	return pList;
}

#define IDC_BUTTONINLISTVIEW 0x1234
CMyButton* CMySuperGrid::ShowButton(int nItem, int nCol)
{
	CRect rect;
	int offset = 0;
	if (!EnsureVisible(nItem, TRUE)) return NULL;
	GetSubItemRect(nItem, nCol, LVIR_BOUNDS, rect);
	CRect rcClient;
	GetClientRect(rcClient);


	DWORD dwStyle = WS_CHILD | WS_VISIBLE;
	CMyButton *pButton = new CMyButton();
	pButton->Create(_T("确定分配"), dwStyle, rect, this, IDC_BUTTONINLISTVIEW);
	pButton->ShowWindow(SW_SHOWNORMAL);
	return pButton;
}

int CMySuperGrid::CalcHorzExtent(CWnd* pWnd, CStringList *pList)
{
	int nExtent = 0;
	if (pWnd != NULL)
	{
		CDC* pDC = pWnd->GetDC();
		HFONT hFont = (HFONT)pWnd->SendMessage(WM_GETFONT); //why not pWnd->GetFont();..I like the send thing alot and
		CFont *pFont = CFont::FromHandle(hFont);			//this way I get to use this function..cool :)
		if (pFont != NULL)										//ya what ever makes me happy,.right :}
		{
			CFont* pOldFont = pDC->SelectObject(pFont);
			CSize newExtent;
			POSITION pos = pList->GetHeadPosition();
			while (pos != NULL)
			{
				CString str(pList->GetNext(pos));
				newExtent = pDC->GetTextExtent(str);
				newExtent.cx += 6;
				if (newExtent.cx > nExtent)
				{
					nExtent = newExtent.cx;
				}
			}
			pDC->SelectObject(pOldFont);
		}
		pWnd->ReleaseDC(pDC);
	}
	return nExtent;
}
BEGIN_MESSAGE_MAP(CMySuperGrid, CSuperGridCtrl)
	ON_WM_CREATE()
	ON_WM_SHOWWINDOW()
END_MESSAGE_MAP()


int CMySuperGrid::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CSuperGridCtrl::OnCreate(lpCreateStruct) == -1)
		return -1;

	// TODO:  在此添加您专用的创建代码
	//m_image.Create(16, 16, ILC_COLOR32 | ILC_MASK, 2, 2);
	//m_image.Add(AfxGetApp()->LoadIcon(IDB_GREENLIGHT));
	//m_image.Add(AfxGetApp()->LoadIcon(IDB_REDLIGHT));
	if (!m_image.Create(IDB_GREENREDLIGHT, 16, 1, RGB(0, 255, 255)))
		return -1;

	SetImageList(&m_image, LVSIL_SMALL);
	CImageList *pImageList = GetImageList(LVSIL_SMALL);
	if (pImageList)
		ImageList_GetIconSize(pImageList->m_hImageList, &m_cxImage, &m_cyImage);
	else
		return -1;

	//If you want to assiciate images to subitems, create an imagelist
	//and call SetSubItemImageList
	//now the next time you want to map an image from this imagelist to a
	//particular subitem call SetSubItemText(col,text,image-index);

	//CImageList imageSubItem;
	//if (!imageSubItem.Create(IDB_SUBITEMS, 16, 1, RGB(0, 255, 255)))
	//	return -1;

	//if (!SetSubItemImageList(&imageSubItem))
	//	return -1;
	return 0;
}


void CMySuperGrid::OnShowWindow(BOOL bShow, UINT nStatus)
{
	CSuperGridCtrl::OnShowWindow(bShow, nStatus);

	// TODO: 在此处添加消息处理程序代码

}
