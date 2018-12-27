// MaterialDlg.cpp : implementation file
//

#include "stdafx.h"
#include "CGWork.h"
#include "MaterialDlg.h"


// CMaterialDlg dialog

IMPLEMENT_DYNAMIC(CMaterialDlg, CDialog)

CMaterialDlg::CMaterialDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CMaterialDlg::IDD, pParent)
	, m_ambient_r(0.2), m_ambient_g(0.2), m_ambient_b(0.2)
	, m_diffuse_r(0.8), m_diffuse_g(0.8), m_diffuse_b(0.8)
	, m_shininess_r(1.0), m_shininess_g(1.0), m_shininess_b(1.0)
	, m_specular(32)
{

}

CMaterialDlg::~CMaterialDlg()
{
}

Vec4 CMaterialDlg::GetAmbientCoeffs() const
{
	return Vec4(m_ambient_r, m_ambient_g, m_ambient_b);
}

void CMaterialDlg::SetAmbientCoeffs(const Vec4 & ka)
{
	m_ambient_r = ka[0];
	m_ambient_g = ka[1];
	m_ambient_b = ka[2];
}

Vec4 CMaterialDlg::GetDiffuseCoeffs() const
{
	return Vec4(m_diffuse_r, m_diffuse_g, m_diffuse_b);
}

void CMaterialDlg::SetDiffuseCoeffs(const Vec4 & kd)
{
	m_diffuse_r = kd[0];
	m_diffuse_g = kd[1];
	m_diffuse_b = kd[2];
}

Vec4 CMaterialDlg::GetShininessCoeffs() const
{
	return Vec4(m_shininess_r, m_shininess_g, m_shininess_b);
}

void CMaterialDlg::SetShininessCoeffs(const Vec4 & ks)
{
	m_shininess_r = ks[0];
	m_shininess_g = ks[1];
	m_shininess_b = ks[2];
}

int CMaterialDlg::GetSpecularCoeff() const
{
	return m_specular;
}

void CMaterialDlg::SetSpecularCoeff(int spec)
{
	m_specular = spec;
}

void CMaterialDlg::DoDataExchange(CDataExchange* pDX)
{
    CDialog::DoDataExchange(pDX);
    DDX_Text(pDX, IDC_MATERIAL_AMBIENT_R, m_ambient_r);
    DDV_MinMaxDouble(pDX, m_ambient_r, 0.0, 1.0);
	DDX_Text(pDX, IDC_MATERIAL_AMBIENT_G, m_ambient_g);
	DDV_MinMaxDouble(pDX, m_ambient_g, 0.0, 1.0);
	DDX_Text(pDX, IDC_MATERIAL_AMBIENT_B, m_ambient_b);
	DDV_MinMaxDouble(pDX, m_ambient_b, 0.0, 1.0);

    DDX_Text(pDX, IDC_MATERIAL_DIFFUSE_R, m_diffuse_r);
    DDV_MinMaxDouble(pDX, m_diffuse_r, 0.0, 1.0);
	DDX_Text(pDX, IDC_MATERIAL_DIFFUSE_G, m_diffuse_g);
	DDV_MinMaxDouble(pDX, m_diffuse_g, 0.0, 1.0);
	DDX_Text(pDX, IDC_MATERIAL_DIFFUSE_B, m_diffuse_b);
	DDV_MinMaxDouble(pDX, m_diffuse_b, 0.0, 1.0);

    DDX_Text(pDX, IDC_MATERIAL_SHININESS_R, m_shininess_r);
    DDV_MinMaxDouble(pDX, m_shininess_r, 0.0, 1.0);
	DDX_Text(pDX, IDC_MATERIAL_SHININESS_G, m_shininess_g);
	DDV_MinMaxDouble(pDX, m_shininess_g, 0.0, 1.0);
	DDX_Text(pDX, IDC_MATERIAL_SHININESS_B, m_shininess_b);
	DDV_MinMaxDouble(pDX, m_shininess_b, 0.0, 1.0);

    DDX_Text(pDX, IDC_MATERIAL_SPECULAR, m_specular);
	DDV_MinMaxDouble(pDX, m_specular, 0, 1024);
}


BEGIN_MESSAGE_MAP(CMaterialDlg, CDialog)
END_MESSAGE_MAP()


// CMaterialDlg message handlers
