#pragma once


// CDialogSilhouette dialog

class CDialogSilhouette : public CDialogEx
{
	DECLARE_DYNAMIC(CDialogSilhouette)

public:
	CDialogSilhouette(CWnd* pParent = nullptr);   // standard constructor
	virtual ~CDialogSilhouette();

	int Thickness;
	COLORREF Color;

// Dialog Data
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DIALOG_SILHOUETTE };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	COLORREF m_oldColor;

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnClickedButtonSilColor();
};
