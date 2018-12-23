#pragma once


// CExportDialog dialog

class CExportDialog : public CDialogEx
{
	DECLARE_DYNAMIC(CExportDialog)

public:
	CExportDialog(CWnd* pParent = nullptr);   // standard constructor
	virtual ~CExportDialog();

	void SetHeight(unsigned int height);
	unsigned int GetHeight();

	void SetWidth(unsigned int width);
	unsigned int GetWidth();


// Dialog Data
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_SAVE_IMAGE_DLG };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
private:
	unsigned int m_height;
	unsigned int m_width;
};
