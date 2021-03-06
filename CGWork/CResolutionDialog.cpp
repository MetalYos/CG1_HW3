// CResolutionDialog.cpp : implementation file
//

#include "stdafx.h"
#include "CGWork.h"
#include "CResolutionDialog.h"
#include "afxdialogex.h"


// CResolutionDialog dialog

IMPLEMENT_DYNAMIC(CResolutionDialog, CDialogEx)

CResolutionDialog::CResolutionDialog(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_RESOLUTION_DLG, pParent)
	, Resolution(20)
{

}

CResolutionDialog::~CResolutionDialog()
{
}

void CResolutionDialog::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT_TESS, Resolution);
	DDV_MinMaxInt(pDX, Resolution, 2, 20);
}


BEGIN_MESSAGE_MAP(CResolutionDialog, CDialogEx)
END_MESSAGE_MAP()


// CResolutionDialog message handlers
