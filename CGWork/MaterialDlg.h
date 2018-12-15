#pragma once

// CMaterialDlg dialog
#include "ALMath.h"

class CMaterialDlg : public CDialog
{
	DECLARE_DYNAMIC(CMaterialDlg)

public:
	CMaterialDlg(CWnd* pParent = NULL);   // standard constructor
	virtual ~CMaterialDlg();

	Vec4 GetAmbientCoeffs() const;
	void SetAmbientCoeffs(const Vec4& ka);

	Vec4 GetDiffuseCoeffs() const;
	void SetDiffuseCoeffs(const Vec4& kd);

	Vec4 GetShininessCoeffs() const;
	void SetShininessCoeffs(const Vec4& ks);

	int GetSpecularCoeff() const;
	void SetSpecularCoeff(int spec);

// Dialog Data
	enum { IDD = IDD_MATERIAL_DLG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
    // ambient property of the material
    double m_ambient_r, m_ambient_g, m_ambient_b;
    // Diffuse property of the material
    double m_diffuse_r, m_diffuse_g, m_diffuse_b;
    // shininess property of the material
    double m_shininess_r, m_shininess_g, m_shininess_b;
    // specular property of the material (integer values only)
    int m_specular;
};
