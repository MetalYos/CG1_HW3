// CDialogSilhouette.cpp : implementation file
//

#include "stdafx.h"
#include "CGWork.h"
#include "CDialogSilhouette.h"
#include "afxdialogex.h"


// CDialogSilhouette dialog

IMPLEMENT_DYNAMIC(CDialogSilhouette, CDialogEx)

CDialogSilhouette::CDialogSilhouette(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_DIALOG_SILHOUETTE, pParent)
{

}

CDialogSilhouette::~CDialogSilhouette()
{
}

void CDialogSilhouette::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT_SIL_THICKNESS, Thickness);
	DDV_MinMaxInt(pDX, Thickness, 1, 3);
}


BEGIN_MESSAGE_MAP(CDialogSilhouette, CDialogEx)
	ON_BN_CLICKED(IDC_BUTTON_SIL_COLOR, &CDialogSilhouette::OnClickedButtonSilColor)
END_MESSAGE_MAP()


// CDialogSilhouette message handlers


void CDialogSilhouette::OnClickedButtonSilColor()
{
	m_oldColor = Color;
	CColorDialog colorDialog(Color);
	if (colorDialog.DoModal() == IDOK)
	{
		Color = colorDialog.GetColor();
	}
}
