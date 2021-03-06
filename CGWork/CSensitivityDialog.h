#pragma once

#include "Vec4.h"

// CSensitivityDialog dialog

class CSensitivityDialog : public CDialogEx
{
	DECLARE_DYNAMIC(CSensitivityDialog)

public:
	CSensitivityDialog(CWnd* pParent = nullptr);   // standard constructor
	virtual ~CSensitivityDialog();

// Dialog Data
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_SENSITIVITY_DLG };
#endif
public:
	Vec4 GetSensitivity();
protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
private:
	double m_sensitivityRotate;
	double m_sensitivityTranslate;
	double m_sensitivityScale;
};
