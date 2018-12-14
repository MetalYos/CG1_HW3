// CGWorkView.h : interface of the CCGWorkView class
//
/////////////////////////////////////////////////////////////////////////////

#if !defined(AFX_CGWORKVIEW_H__5857316D_EA60_11D5_9FD5_00D0B718E2CD__INCLUDED_)
#define AFX_CGWORKVIEW_H__5857316D_EA60_11D5_9FD5_00D0B718E2CD__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000


#include "gl\gl.h"    // Include the standard CGWork  headers
#include "gl\glu.h"   // Add the utility library


#include "Light.h"

#include <vector>
#include "CColorsDialog.h"
#include "CPerspectiveDialog.h"
#include "ALMath.h"
#include "CSensitivityDialog.h"
#include "CResolutionDialog.h"
#include "Vec4.h"
#include "Geometry.h"

struct Edge {
	CPoint a, b;
	COLORREF color;
};

class CCGWorkView : public CView
{
protected: // create from serialization only
	CCGWorkView();
	DECLARE_DYNCREATE(CCGWorkView)

// Attributes
public:
	CCGWorkDoc* GetDocument();

// Operations
public:

private:
	bool m_isAxis_X;
	bool m_isAxis_Y;
	bool m_isAxis_Z;
	int m_nAction;				// Rotate, Translate, Scale
	int m_nView;				// Orthographic, perspective
	bool m_bIsPerspective;			// is the view perspective
	
	CString m_strItdFileName;		// file name of IRIT data

	int m_nLightShading;			// shading: Flat, Gouraud.

	double m_lMaterialAmbient;		// The Ambient in the scene
	double m_lMaterialDiffuse;		// The Diffuse in the scene
	double m_lMaterialSpecular;		// The Specular in the scene
	int m_nMaterialCosineFactor;		// The cosine factor for the specular

	LightParams m_lights[MAX_LIGHT];	//configurable lights array
	LightParams m_ambientLight;		//ambient light (only RGB is used)

	int m_nCoordSpace;		// The selected Coord space to perform transformations in

	// Orthographic projection height
	double orthoHeight;
	// BBOX parameters
	bool isBBoxOn;
	// Color dialog flag
	bool isCColorDialogOpen;
	
	// Mouse parameters
	bool mouseClicked;
	CPoint prevMousePos;
	CPoint mouseClickPos;
	
	// Fine tuning parameters
	double normalSizeFactor;
	bool showGeos;
	bool aroundEye;

	// Poly fill selection parameter
	enum PolyFillSelection
	{
		WIREFRAME,
		SOLID_SCREEN,
		SOLID_FILE
	};
	PolyFillSelection currentPolySelection;

	// Quick hack
	std::vector< std::vector<Edge> > selectedPolys;

	// Drawing functions
	int GetOctant(CPoint a, CPoint b);
	void DrawPointOctant(CDC * pDC, int x, int y, COLORREF color, const CPoint& origA, int oct);
	CPoint TranslateBToFirstOctant(const CPoint& a, const CPoint& b, int oct);
	CPoint TranslatePointFrom8th(CPoint p, int oct);
	CPoint TranslatePointTo8th(CPoint p, int oct);
	void SetSelectedPoly(CPoint mousePos, Poly* p, std::vector<Vec4Line> poly);
	void DrawSelectedPolys(CDC* pDC);
	void DrawBoundingBox(CDC* pDC, const std::vector<Poly*>& polys, const Mat4& modelTransform,
		const Mat4& camTransform, const Mat4& projection, const Mat4& toView, COLORREF color);
	bool IsClippedZ(const Vec4& p1, const Vec4& p2);

	//Dialogs
	CColorsDialog m_colorDialog; 
	CPerspectiveDialog m_perspDialog;
	CSensitivityDialog m_sensitivityDialog;
	CResolutionDialog m_resolutionDialog;

	Vec4 m_sensitivity;


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CCGWorkView)
public:

	virtual void OnDraw(CDC* pDC);  // overridden to draw this view
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
	void DrawLine(CDC* pDC, COLORREF color, CPoint a, CPoint b);
	void DrawPoly(CDC* pDc, std::vector<Edge>);
	void ScanConvert(CDC* pDc, std::vector<Edge> poly, COLORREF color);
	protected:
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CCGWorkView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:
	BOOL InitializeCGWork();
	BOOL SetupViewingFrustum(void);
	BOOL SetupViewingOrthoConstAspect(void);

	virtual void RenderScene();


	HGLRC    m_hRC;			// holds the Rendering Context
	CDC*     m_pDC;			// holds the Device Context
	int m_WindowWidth;		// hold the windows width
	int m_WindowHeight;		// hold the windows height
	double m_AspectRatio;		// hold the fixed Aspect Ration
	bool isFirstDraw;

	HBITMAP m_pDbBitMap;
	CDC* m_pDbDC;

// Generated message map functions
protected:
	//{{AFX_MSG(CCGWorkView)
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnDestroy();
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnFileLoad();
	afx_msg void OnViewOrthographic();
	afx_msg void OnUpdateViewOrthographic(CCmdUI* pCmdUI);
	afx_msg void OnViewPerspective();
	afx_msg void OnUpdateViewPerspective(CCmdUI* pCmdUI);
	afx_msg void OnActionRotate();
	afx_msg void OnUpdateActionRotate(CCmdUI* pCmdUI);
	afx_msg void OnActionScale();
	afx_msg void OnUpdateActionScale(CCmdUI* pCmdUI);
	afx_msg void OnActionTranslate();
	afx_msg void OnUpdateActionTranslate(CCmdUI* pCmdUI);
	afx_msg void OnAxisX();
	afx_msg void OnUpdateAxisX(CCmdUI* pCmdUI);
	afx_msg void OnAxisY();
	afx_msg void OnUpdateAxisY(CCmdUI* pCmdUI);
	afx_msg void OnAxisZ();
	afx_msg void OnUpdateAxisZ(CCmdUI* pCmdUI);
	afx_msg void OnLightShadingFlat();
	afx_msg void OnUpdateLightShadingFlat(CCmdUI* pCmdUI);
	afx_msg void OnLightShadingGouraud();
	afx_msg void OnUpdateLightShadingGouraud(CCmdUI* pCmdUI);
	afx_msg void OnLightConstants();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnButtonBbox();
	afx_msg void OnUpdateButtonBbox(CCmdUI *pCmdUI);
	afx_msg void OnButtonVertNorm();
	afx_msg void OnUpdateButtonVertNorm(CCmdUI *pCmdUI);
	afx_msg void OnButtonPolyNorm();
	afx_msg void OnUpdateButtonPolyNorm(CCmdUI *pCmdUI);
	afx_msg void OnButtonColors();
	afx_msg void OnNormalCalculated();
	afx_msg void OnUpdateNormalCalculated(CCmdUI *pCmdUI); afx_msg void OnOptionsMousesensitivity();
	afx_msg void OnOptionsTessellationtolerance();
	afx_msg void OnNormalFromfile();
	afx_msg void OnButtonView();
	afx_msg void OnUpdateButtonView(CCmdUI *pCmdUI);
	afx_msg void OnButtonObject();
	afx_msg void OnUpdateButtonObject(CCmdUI *pCmdUI);
	afx_msg void OnOptionsPerspectivecontrol();
	afx_msg void OnActionSelect();
	afx_msg void OnUpdateActionSelect(CCmdUI *pCmdUI);
	afx_msg void OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags);
	afx_msg void OnRenderingWireframe();
	afx_msg void OnUpdateRenderingWireframe(CCmdUI *pCmdUI);
	afx_msg void OnRenderingSolidonscreen();
	afx_msg void OnUpdateRenderingSolidonscreen(CCmdUI *pCmdUI);
	afx_msg void OnRenderingSolidtofile();
	afx_msg void OnUpdateRenderingSolidtofile(CCmdUI *pCmdUI);
};

#ifndef _DEBUG  // debug version in CGWorkView.cpp
inline CCGWorkDoc* CCGWorkView::GetDocument()
   { return (CCGWorkDoc*)m_pDocument; }
#endif

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_CGWORKVIEW_H__5857316D_EA60_11D5_9FD5_00D0B718E2CD__INCLUDED_)
