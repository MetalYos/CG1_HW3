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
#include "MaterialDlg.h"
#include "Vec4.h"
#include "Geometry.h"
#include "Material.h"
#include "CExportDialog.h"
#include "CDialogSilhouette.h"
#include "PngWrapper.h"

struct DVertex
{
	CPoint Pixel;
	double Z;
	Vec4 PosVS;
	Vec4 NormalVS;
	Vec4 Color;
};

struct Edge {
	//CPoint a, b;
	DVertex A, B;
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

	// Zbuffer array (screenWidth * screenHeight)
	double* zBuffer;

	double orthoHeight;		// Orthographic projection height
	bool isBBoxOn;
	bool isCColorDialogOpen;
	bool isBFCulling;

	// Silhouette options
	bool showSil;
	int silThickness;
	COLORREF silColor;
	
	// Mouse parameters
	bool mouseClicked;
	CPoint prevMousePos;
	CPoint mouseClickPos;
	
	// Fine tuning parameters
	double normalSizeFactor;
	double normalSign;
	bool showGeos;
	bool aroundEye;
	Vec4 m_sensitivity;
	bool isModelLoaded;

	//Background 
	bool isBGStretch;
	bool isBGFileOpen;
	CString BGFile;

	// Poly fill selection parameter
	enum PolyFillSelection
	{
		WIREFRAME,
		SOLID_SCREEN
	};
	PolyFillSelection currentPolySelection;

	// Image saving parameters
	bool saveToFile;
	int imgWidth;
	int imgHeight;
	PngWrapper imgToSave;

	// Quick hack
	std::vector< std::vector<Edge> > selectedPolys;

	// Drawing functions
	int GetOctant(CPoint a, CPoint b);
	void DrawPointOctant(CDC * pDC, int x, int y, COLORREF color, const CPoint& origA, int oct, int thickness = 0);
	CPoint TranslateBToFirstOctant(const CPoint& a, const CPoint& b, int oct);
	CPoint TranslatePointFrom8th(CPoint p, int oct);
	CPoint TranslatePointTo8th(CPoint p, int oct);
	void SetSelectedPoly(CPoint mousePos, Poly* p, std::vector<Vec4Line> poly);
	void DrawSelectedPolys(CDC* pDC);
	void DrawBoundingBox(CDC* pDC, const std::vector<Poly*>& polys, const Mat4& modelTransform,
		const Mat4& camTransform, const Mat4& projection, const Mat4& toView, COLORREF color);
	void DrawVertexNormal(CDC* pDC, const Vertex* v, const Vec4& normal,
		const Mat4& modelTransform, const Mat4& camTransform, const Mat4& projection,
		const Mat4& toView, COLORREF color);
	void DrawPolyNormal(CDC* pDC, const Poly* p, const Mat4& modelTransform,
		const Mat4 & normalTransform, const Mat4& camTransform, const Mat4& projection,
		const Mat4& toView, COLORREF color);
	void DrawBackground(CDC* pDC, CRect r);
	void DrawSilhouetteEdges(CDC * pDC, Geometry* geo, const Mat4 & modelTransform,
		const Mat4 & normalTransform, const Mat4 & camTransform, const Mat4 & projection,
		const Mat4 & toView, COLORREF color);

	//Dialogs
	CColorsDialog m_colorDialog; 
	CPerspectiveDialog m_perspDialog;
	CSensitivityDialog m_sensitivityDialog;
	CResolutionDialog m_resolutionDialog;
	CMaterialDlg m_materialDialog;
	CExportDialog m_exportDialog;
	CDialogSilhouette m_silDialog;

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CCGWorkView)
public:

	virtual void OnDraw(CDC* pDC);  // overridden to draw this view
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
	void DrawLine(CDC* pDC, COLORREF color, CPoint a, CPoint b, int thickness = 0);
	void DrawPoly(CDC* pDc, std::vector<Edge>);
	void ScanConvert(CDC* pDc, std::vector<Edge> poly, COLORREF color, Vec4 polyCenter = Vec4(0.0), Vec4 polyNormal = Vec4(0.0));
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
	Vec4 CalculateShading(LightParams* lights, Material* material, Vec4 pos, Vec4 normal, COLORREF color);
	bool IsBackFace(const Poly* p, const Mat4& modelTransform, const Mat4& normalTransform, const Mat4& camTransform);
	bool IsSilhouetteEdge(const PolyEdge* e, const Mat4& modelTransform, const Mat4& normalTransform, const Mat4& camTransform);
	Vec4 CalculateVertexNormal(const Vertex* v, const Mat4& modelTransform, const Mat4& normalTransform, const Mat4& camTransform);
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
	//afx_msg void OnUpdateNormalCalculated(CCmdUI *pCmdUI); 
	afx_msg void OnOptionsMousesensitivity();
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
//	afx_msg void OnUpdateRenderingSolidtofile(CCmdUI *pCmdUI);
	afx_msg void OnBackgroundStretch();
	afx_msg void OnUpdateBackgroundStretch(CCmdUI *pCmdUI);
	afx_msg void OnBackgroundRepeat();
	afx_msg void OnUpdateBackgroundRepeat(CCmdUI *pCmdUI);
	afx_msg void OnBackgroundOpen();
	afx_msg void OnLightSetmaterial();
	afx_msg void OnLightShadingPhong();
	afx_msg void OnUpdateLightShadingPhong(CCmdUI *pCmdUI);
	afx_msg void OnButtonBculling();
	afx_msg void OnButtonSil();
	afx_msg void OnButtonInverseN();
	afx_msg void OnOptionsSilhouetteoptions();
	afx_msg void OnBackgroundClear();
};

#ifndef _DEBUG  // debug version in CGWorkView.cpp
inline CCGWorkDoc* CCGWorkView::GetDocument()
   { return (CCGWorkDoc*)m_pDocument; }
#endif

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_CGWORKVIEW_H__5857316D_EA60_11D5_9FD5_00D0B718E2CD__INCLUDED_)
