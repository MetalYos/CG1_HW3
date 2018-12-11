#pragma once


// CPerspectiveDialog dialog

class CPerspectiveDialog : public CDialogEx
{
	DECLARE_DYNAMIC(CPerspectiveDialog)

public:
	CPerspectiveDialog(CWnd* pParent = nullptr);   // standard constructor
	virtual ~CPerspectiveDialog();

	double ProjPlane;
	double FOV;

// Dialog Data
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_PRESPECTIVE_DIALOG };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
};
