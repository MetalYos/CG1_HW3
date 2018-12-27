// CGWorkView.cpp : implementation of the CCGWorkView class
//
#include "ALMath.h"
#include "stdafx.h"
#include "CGWork.h"

#include "CGWorkDoc.h"
#include "CGWorkView.h"

#include <iostream>
using std::cout;
using std::endl;
#include "MaterialDlg.h"
#include "LightDialog.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#include "PngWrapper.h"
#include "iritSkel.h"

#include <assert.h>
#include <algorithm>
#include "Scene.h"

#include "PngWrapper.h"
// For Status Bar access
#include "MainFrm.h"

// Use this macro to display text messages in the status bar.
#define STATUS_BAR_TEXT(str) (((CMainFrame*)GetParentFrame())->getStatusBar().SetWindowText(str))

#define D_PERSP // activates Perspective with d param


/////////////////////////////////////////////////////////////////////////////
// CCGWorkView

IMPLEMENT_DYNCREATE(CCGWorkView, CView)

BEGIN_MESSAGE_MAP(CCGWorkView, CView)
	//{{AFX_MSG_MAP(CCGWorkView)
	ON_WM_ERASEBKGND()
	ON_WM_CREATE()
	ON_WM_DESTROY()
	ON_WM_SIZE()
	ON_COMMAND(ID_FILE_LOAD, OnFileLoad)
	ON_COMMAND(ID_VIEW_ORTHOGRAPHIC, OnViewOrthographic)
	ON_UPDATE_COMMAND_UI(ID_VIEW_ORTHOGRAPHIC, OnUpdateViewOrthographic)
	ON_COMMAND(ID_VIEW_PERSPECTIVE, OnViewPerspective)
	ON_UPDATE_COMMAND_UI(ID_VIEW_PERSPECTIVE, OnUpdateViewPerspective)
	ON_COMMAND(ID_ACTION_ROTATE, OnActionRotate)
	ON_UPDATE_COMMAND_UI(ID_ACTION_ROTATE, OnUpdateActionRotate)
	ON_COMMAND(ID_ACTION_SCALE, OnActionScale)
	ON_UPDATE_COMMAND_UI(ID_ACTION_SCALE, OnUpdateActionScale)
	ON_COMMAND(ID_ACTION_TRANSLATE, OnActionTranslate)
	ON_UPDATE_COMMAND_UI(ID_ACTION_TRANSLATE, OnUpdateActionTranslate)
	ON_COMMAND(ID_AXIS_X, OnAxisX)
	ON_UPDATE_COMMAND_UI(ID_AXIS_X, OnUpdateAxisX)
	ON_COMMAND(ID_AXIS_Y, OnAxisY)
	ON_UPDATE_COMMAND_UI(ID_AXIS_Y, OnUpdateAxisY)
	ON_COMMAND(ID_AXIS_Z, OnAxisZ)
	ON_UPDATE_COMMAND_UI(ID_AXIS_Z, OnUpdateAxisZ)
	ON_COMMAND(ID_LIGHT_SHADING_FLAT, OnLightShadingFlat)
	ON_UPDATE_COMMAND_UI(ID_LIGHT_SHADING_FLAT, OnUpdateLightShadingFlat)
	ON_COMMAND(ID_LIGHT_SHADING_GOURAUD, OnLightShadingGouraud)
	ON_UPDATE_COMMAND_UI(ID_LIGHT_SHADING_GOURAUD, OnUpdateLightShadingGouraud)
	ON_COMMAND(ID_LIGHT_CONSTANTS, OnLightConstants)
	//}}AFX_MSG_MAP
	ON_WM_TIMER()
	ON_WM_MOUSEMOVE()
	ON_WM_LBUTTONDOWN()
	ON_COMMAND(ID_BUTTON_BBOX, &CCGWorkView::OnButtonBbox)
	ON_UPDATE_COMMAND_UI(ID_BUTTON_BBOX, &CCGWorkView::OnUpdateButtonBbox)
	ON_COMMAND(ID_BUTTON_VERT_NORM, &CCGWorkView::OnButtonVertNorm)
	ON_UPDATE_COMMAND_UI(ID_BUTTON_VERT_NORM, &CCGWorkView::OnUpdateButtonVertNorm)
	ON_COMMAND(ID_BUTTON_POLY_NORM, &CCGWorkView::OnButtonPolyNorm)
	ON_UPDATE_COMMAND_UI(ID_BUTTON_POLY_NORM, &CCGWorkView::OnUpdateButtonPolyNorm)
	ON_COMMAND(ID_NORMAL_CALCULATED, &CCGWorkView::OnNormalCalculated)
	ON_COMMAND(ID_BUTTON_COLORS, &CCGWorkView::OnButtonColors)
	ON_COMMAND(ID_OPTIONS_MOUSESENSITIVITY, &CCGWorkView::OnOptionsMousesensitivity)
	ON_COMMAND(ID_OPTIONS_TESSELLATIONTOLERANCE, &CCGWorkView::OnOptionsTessellationtolerance)
	ON_COMMAND(ID_NORMAL_FROMFILE, &CCGWorkView::OnNormalFromfile)
	ON_COMMAND(ID_BUTTON_VIEW, &CCGWorkView::OnButtonView)
	ON_UPDATE_COMMAND_UI(ID_BUTTON_VIEW, &CCGWorkView::OnUpdateButtonView)
	ON_COMMAND(ID_BUTTON_OBJECT, &CCGWorkView::OnButtonObject)
	ON_UPDATE_COMMAND_UI(ID_BUTTON_OBJECT, &CCGWorkView::OnUpdateButtonObject)
	ON_COMMAND(ID_OPTIONS_PERSPECTIVECONTROL, &CCGWorkView::OnOptionsPerspectivecontrol)
	ON_COMMAND(ID_ACTION_SELECT, &CCGWorkView::OnActionSelect)
	ON_UPDATE_COMMAND_UI(ID_ACTION_SELECT, &CCGWorkView::OnUpdateActionSelect)
	ON_WM_KEYDOWN()
	ON_COMMAND(ID_RENDERING_WIREFRAME, &CCGWorkView::OnRenderingWireframe)
	ON_UPDATE_COMMAND_UI(ID_RENDERING_WIREFRAME, &CCGWorkView::OnUpdateRenderingWireframe)
	ON_COMMAND(ID_RENDERING_SOLIDONSCREEN, &CCGWorkView::OnRenderingSolidonscreen)
	ON_UPDATE_COMMAND_UI(ID_RENDERING_SOLIDONSCREEN, &CCGWorkView::OnUpdateRenderingSolidonscreen)
	ON_COMMAND(ID_RENDERING_SOLIDTOFILE, &CCGWorkView::OnRenderingSolidtofile)
	ON_COMMAND(ID_BACKGROUND_STRETCH, &CCGWorkView::OnBackgroundStretch)
	ON_UPDATE_COMMAND_UI(ID_BACKGROUND_STRETCH, &CCGWorkView::OnUpdateBackgroundStretch)
	ON_COMMAND(ID_BACKGROUND_REPEAT, &CCGWorkView::OnBackgroundRepeat)
	ON_UPDATE_COMMAND_UI(ID_BACKGROUND_REPEAT, &CCGWorkView::OnUpdateBackgroundRepeat)
	ON_COMMAND(ID_BACKGROUND_OPEN, &CCGWorkView::OnBackgroundOpen)
	ON_COMMAND(ID_LIGHT_SETMATERIAL, &CCGWorkView::OnLightSetmaterial)
	ON_COMMAND(ID_LIGHT_SHADING_PHONG, &CCGWorkView::OnLightShadingPhong)
	ON_UPDATE_COMMAND_UI(ID_LIGHT_SHADING_PHONG, &CCGWorkView::OnUpdateLightShadingPhong)
	ON_COMMAND(ID_BUTTON_BCULLING, &CCGWorkView::OnButtonBculling)
	ON_COMMAND(ID_BUTTON_SIL, &CCGWorkView::OnButtonSil)
	ON_COMMAND(ID_BUTTON_INVERSE_N, &CCGWorkView::OnButtonInverseN)
	ON_COMMAND(ID_OPTIONS_SILHOUETTEOPTIONS, &CCGWorkView::OnOptionsSilhouetteoptions)
	ON_COMMAND(ID_BACKGROUND_CLEAR, &CCGWorkView::OnBackgroundClear)
END_MESSAGE_MAP()

// A patch to fix GLaux disappearance from VS2005 to VS2008
void auxSolidCone(GLdouble radius, GLdouble height) {
        GLUquadric *quad = gluNewQuadric();
        gluQuadricDrawStyle(quad, GLU_FILL);
        gluCylinder(quad, radius, 0.0, height, 20, 20);
        gluDeleteQuadric(quad);
}

/////////////////////////////////////////////////////////////////////////////
// CCGWorkView construction/destruction

CCGWorkView::CCGWorkView()
{
	// Set default values
	m_isAxis_X = true;
	m_isAxis_Y = false;
	m_isAxis_Z = false;
	m_nAction = ID_ACTION_ROTATE;
	m_nView = ID_VIEW_ORTHOGRAPHIC;	
	m_bIsPerspective = false;

	m_nLightShading = ID_LIGHT_SHADING_FLAT;

	m_lMaterialAmbient = 0.2;
	m_lMaterialDiffuse = 0.8;
	m_lMaterialSpecular = 1.0;
	m_nMaterialCosineFactor = 32;

	//init the first light to be enabled
	m_lights[LIGHT_ID_1].Enabled = true;
	m_lights[LIGHT_ID_1].DirX = 1.0;
	m_lights[LIGHT_ID_1].DirY = 1.0;
	m_lights[LIGHT_ID_1].DirZ = 1.0;
	m_pDbBitMap = NULL;
	m_pDbDC = NULL;

	// Init zBuffer
	zBuffer = NULL;

	isFirstDraw = true;
	isBBoxOn = false;
	isCColorDialogOpen = false;
	mouseClicked = false;
	m_sensitivity = Vec4(100, 100, 300);
	orthoHeight = 5.0;
	m_nCoordSpace = ID_BUTTON_VIEW;
	normalSizeFactor = 0.5;
	showGeos = true;
	aroundEye = true;
	currentPolySelection = WIREFRAME;
	isBGStretch = false;
	isBGFileOpen = false;
	isModelLoaded = false;
	isBFCulling = false;
	showSil = false;
	silThickness = 3;
	silColor = AL_YELLO_CREF;
	normalSign = 1.0;

	saveToFile = false;
}

CCGWorkView::~CCGWorkView()
{
}


/////////////////////////////////////////////////////////////////////////////
// CCGWorkView diagnostics

#ifdef _DEBUG
void CCGWorkView::AssertValid() const
{
	CView::AssertValid();
}

void CCGWorkView::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}

CCGWorkDoc* CCGWorkView::GetDocument() // non-debug version is inline
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CCGWorkDoc)));
	return (CCGWorkDoc*)m_pDocument;
}
#endif //_DEBUG


/////////////////////////////////////////////////////////////////////////////
// CCGWorkView Window Creation - Linkage of windows to CGWork

BOOL CCGWorkView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: Modify the Window class or styles here by modifying
	//  the CREATESTRUCT cs

	// An CGWork window must be created with the following
	// flags and must NOT include CS_PARENTDC for the
	// class style.

	cs.style |= WS_CLIPSIBLINGS | WS_CLIPCHILDREN;

	return CView::PreCreateWindow(cs);
}

void CCGWorkView::DrawLine(CDC * pDC, COLORREF color, CPoint a, CPoint b, int thickness)
{
	int oct = GetOctant(a, b);
	b = TranslateBToFirstOctant(a, b, oct);

	int x, y, dx, dy, d, De, Dne;
	x = a.x;
	y = a.y;
	dx = b.x - a.x;
	dy = b.y - a.y;
	d = 2 * dy - dx;
	De = 2 * dy;
	Dne = 2 * (dy - dx);
	DrawPointOctant(pDC, x, y, color, a, oct, thickness);
	while (x < b.x) {
		if (d < 0) {
			d += De;
			x++;
		}
		else {
			d += Dne;
			x++;
			y++;
		}
		DrawPointOctant(pDC, x, y, color, a, oct, thickness);
	}
}

void CCGWorkView::DrawPoly(CDC * pDc, std::vector<Edge> edges)
{
	for (Edge e : edges) {
		DrawLine(pDc, e.color, e.A.Pixel, e.B.Pixel);
	}
}

class EdgeSorterY
{
public:
	bool operator()(const Edge& e1, const Edge& e2)
	{
		return (((e1.A.Pixel.y < e2.A.Pixel.y) && (e1.A.Pixel.y < e2.B.Pixel.y)) ||
			((e1.B.Pixel.y < e2.A.Pixel.y) && (e1.B.Pixel.y < e2.B.Pixel.y)));
	}
};

class EdgeComparer
{
public:
	bool operator()(const Edge& e1, const Edge& e2)
	{
		return ((e1.A.Pixel == e2.A.Pixel) && (e1.B.Pixel == e2.B.Pixel));
	}
};

struct Intersection {
	int x;
	double z;
	Vec4 color;
	Vec4 normal;
	Vec4 pos;
};

class IntersectionsSorter
{
public:
	bool operator()(const Intersection& i1, const Intersection& i2)
	{
		auto less = std::less<int>();
		return less(i1.x, i2.x);
	}
};

class IntersectionsComparer
{
public:
	bool operator()(const Intersection& i1, const Intersection& i2)
	{
		return (i1.x == i2.x);
	}
};

void CCGWorkView::ScanConvert(CDC* pDc, std::vector<Edge> poly, COLORREF color, Vec4 polyCenter, Vec4 polyNormal)
{
	assert(poly.size() > 2);
	COLORREF objectColor = color;
	Material* m = Scene::GetInstance().GetModels().back()->GetMaterial();
	if (m_nLightShading == ID_LIGHT_SHADING_FLAT)
	{
		// Calculate color
		Vec4 c = CalculateShading(m_lights, m, polyCenter, polyNormal, objectColor);
		color = RGB((int)(c[0] * 255.0), (int)(c[1] * 255.0), (int)(c[2] * 255.0));
	}

	// Sort edges according to the ymin value
	EdgeSorterY sorter;
	std::sort(poly.begin(), poly.end(), sorter);
	
	std::vector<Edge> activeList;

	// find ymax of edges in poly
	int ymax = poly[0].A.Pixel.y;
	for (unsigned int i = 0; i < poly.size(); i++)
	{
		if (ymax <= poly[i].A.Pixel.y)
			ymax = poly[i].A.Pixel.y;
		if (ymax <= poly[i].B.Pixel.y)
			ymax = poly[i].B.Pixel.y;
	}
	
	int ymin = max(min(poly[0].A.Pixel.y, poly[0].B.Pixel.y), 0);
	// Iterate over scan lines from ymin to ymax
	for (int y = ymin; y <= ymax; y++)
	{
		// Iterate over the edges in Poly
		//for (unsigned int i = 0; i < poly.size(); i++)
		for (auto it = poly.begin(); it != poly.end(); ++it)
		{
			Edge polyEdge = *it;
			int edgeYMin = (polyEdge.A.Pixel.y < polyEdge.B.Pixel.y) ? polyEdge.A.Pixel.y : polyEdge.B.Pixel.y;
			int edgeYMax = (polyEdge.A.Pixel.y < polyEdge.B.Pixel.y) ? polyEdge.B.Pixel.y : polyEdge.A.Pixel.y;

			if (activeList.size() == 0 && edgeYMin <= y)
			{
				activeList.push_back(polyEdge);
				continue;
			}

			// Iterate over the edges in active list
			int size = activeList.size();
			for (unsigned int i = 0; i < activeList.size(); i++)
			{
				// If the poly edge is not in the active list and it's ymin is smaller
				// than the scanline, add it to the active list
				if ((polyEdge.A.Pixel != activeList[i].A.Pixel) || (polyEdge.B.Pixel != activeList[i].B.Pixel))
				{
					if (edgeYMin <= y)
					{
						activeList.push_back(polyEdge);
						break;
					}
				}
				else
					break;
			}
			for (unsigned int i = 0; i < activeList.size(); i++)
			{
				// If the poly edge is in the active list and it's ymax is smaller
				// than the scanline, remove it from the active list
				if (((polyEdge.A.Pixel == activeList[i].A.Pixel) && (polyEdge.B.Pixel == activeList[i].B.Pixel)) &&
					(edgeYMax < y))
				{
					activeList.erase(activeList.begin() + i);
				}
			}
		}

		EdgeComparer eComp;
		auto it = std::unique(activeList.begin(), activeList.end(), eComp);
		activeList.resize(std::distance(activeList.begin(), it));

		// Calculate points of intersections of A members with line Y = y
		std::vector<Intersection> intersections;
		for (Edge e : activeList)
		{
			int dy = e.B.Pixel.y - e.A.Pixel.y;
			if (dy == 0)
				continue;
			int dx = e.B.Pixel.x - e.A.Pixel.x;

			Intersection i;
			// Calculate X axis intersections
			int x = (int)floor((y * dx + (e.A.Pixel.x * e.B.Pixel.y - e.B.Pixel.x * e.A.Pixel.y)) / dy);
			i.x = x;

			// Calculate Z pos at intersections
			double z = e.A.Z - (e.A.Z - e.B.Z) * ((double)(e.A.Pixel.y - y) / (double)(e.A.Pixel.y - e.B.Pixel.y));
			i.z = z;

			if (m_nLightShading == ID_LIGHT_SHADING_GOURAUD)
			{
				// Calculate Color at intersections
				for (int j = 0; j < 3; j++)
					i.color[j] = e.A.Color[j] - (e.A.Color[j] - e.B.Color[j]) * ((double)(e.A.Pixel.y - y) / (double)(e.A.Pixel.y - e.B.Pixel.y));
			}
			if (m_nLightShading == ID_LIGHT_SHADING_PHONG)
			{
				// Calculate Normal at intersections
				for (int j = 0; j < 3; j++)
				{
					i.normal[j] = e.A.NormalVS[j] - (e.A.NormalVS[j] - e.B.NormalVS[j]) * ((double)(e.A.Pixel.y - y) / (double)(e.A.Pixel.y - e.B.Pixel.y));
					i.pos[j] = e.A.PosVS[j] - (e.A.PosVS[j] - e.B.PosVS[j]) * ((double)(e.A.Pixel.y - y) / (double)(e.A.Pixel.y - e.B.Pixel.y));
				}
			}

			intersections.push_back(i);
		}

		// Sort intersections by decreasing x values
		IntersectionsSorter sorter;
		std::sort(intersections.begin(), intersections.end(), sorter);
		
		IntersectionsComparer comp;
		auto it2 = std::unique(intersections.begin(), intersections.end(), comp);
		intersections.resize(std::distance(intersections.begin(), it2));
		
		if (intersections.size() == 1)
		{
			// Draw pixel
		}
		for (unsigned int i = 0; i < intersections.size(); i += 2)
		{
			// Draw line according to shading and zbuffer
			int x0 = intersections[i].x;
			if ((i + 1) >= intersections.size())
				break;
			int x1 = intersections[i + 1].x;

			// Get intersections z pos
			double z0 = intersections[i].z;
			if ((i + 1) >= intersections.size())
				break;
			double z1 = intersections[i + 1].z;

			// Get intersections RGB
			Vec4 I0 = intersections[i].color;
			if ((i + 1) >= intersections.size())
				break;
			Vec4 I1 = intersections[i + 1].color;

			// Get intersections Normal
			Vec4 n0 = intersections[i].normal;
			if ((i + 1) >= intersections.size())
				break;
			Vec4 n1 = intersections[i + 1].normal;

			// Get intersections Pos
			Vec4 v0 = intersections[i].pos;
			if ((i + 1) >= intersections.size())
				break;
			Vec4 v1 = intersections[i + 1].pos;

			int x = x0;
			while (x <= x1)
			{
				// Caluclate z Pos at (x, y)
				double zp = z1 - (z1 - z0) * ((double)(x1 - x) / (double)(x1 - x0));

				// Calcualte RGB at (x, y) according to shading
				if (m_nLightShading == ID_LIGHT_SHADING_GOURAUD)
				{
					Vec4 colorVec = I1 - (I1 - I0) * ((double)(x1 - x) / (double)(x1 - x0));
					color = RGB((int)colorVec[0], (int)colorVec[1], (int)colorVec[2]);
				}
				if (m_nLightShading == ID_LIGHT_SHADING_PHONG)
				{
					Vec4 normal = n1 - (n1 - n0) * ((double)(x1 - x) / (double)(x1 - x0));
					Vec4 pos = v1 - (v1 - v0) * ((double)(x1 - x) / (double)(x1 - x0));
					// Calculate color
					Vec4 c = CalculateShading(m_lights, m, pos, normal, objectColor);
					color = RGB((int)(c[0] * 255.0), (int)(c[1] * 255.0), (int)(c[2] * 255.0));
				}
				// Compare z Pos to zBuffer, if z Pos > zBuffer,
				// Draw and update z buffer
				int width = saveToFile ? imgWidth : m_WindowWidth;
				int height = saveToFile ? imgHeight : m_WindowHeight;
				int index = min(x + width * y, width * height - 1);
				if (zp > zBuffer[index])
				{
					if (!saveToFile)
						pDc->SetPixel(x, y, color);
					else
						imgToSave.SetValue(x, y, SET_RGB(GET_A(color), GET_B(color), GET_G(color)));
					zBuffer[index] = zp;
				}
				x++;
			}
		}
	}
}


int CCGWorkView::GetOctant(CPoint a, CPoint b)
{
	//normalize at b as if a is in (0,0)
	b.x -= a.x;
	b.y -= a.y;

	//problematic areas
	if (b == CPoint(0, 0)) return 1;
	if (b.x == 0) return b.y > 0 ? 7 : 3;
	if (b.y == 0) return b.x > 0 ? 1 : 5;

	//Right half
	if (b.x > 0) {
		//I
		if (b.y < 0) {
			return b.x > abs(b.y) ? 1 : 2;
		}
		//IV
		else {
			return b.y > b.x ? 7 : 8;
		}
	}

	//Left half
	else {
		//II
		if (b.y < 0) {
			return b.x > b.y ? 3 : 4;
		}
		//III
		else
		{
			return abs(b.x) > b.y ? 5 : 6;
		}
	}
}

void CCGWorkView::DrawPointOctant(CDC * pDC, int x, int y, COLORREF color, const CPoint& origA, int oct, int thickness)
{
	CPoint normalized = CPoint(x - origA.x, y - origA.y);
	CPoint transformed = TranslatePointFrom8th(normalized, oct);
	CPoint actualPoint = origA + transformed;

	if (thickness == 0)
		if(!saveToFile)
			pDC->SetPixel(actualPoint, color);
		else
			imgToSave.SetValue(actualPoint.x, actualPoint.y, SET_RGB(GET_A(color), GET_B(color), GET_G(color)));
	else
	{
		// Draw thickness
		int startX = actualPoint.x;
		int endX = actualPoint.x;
		int startY = actualPoint.y;
		int endY = actualPoint.y;
		for (int i = thickness; i > 0; i--)
		{
			if ((startX == actualPoint.x) && (actualPoint.x - i >= 0))
				startX = actualPoint.x - i;
			if ((endX == actualPoint.x) && (actualPoint.x + i < m_WindowWidth))
				endX = actualPoint.x + i;
			if ((startY == actualPoint.y) && (actualPoint.y - i >= 0))
				startY = actualPoint.y - i;
			if ((endY == actualPoint.y) && (actualPoint.y + i < m_WindowHeight))
				endY = actualPoint.y + i;
		}

		CPoint pix;
		for (int x = startX; x <= endX; x++)
		{
			for (int y = startY; y <= endY; y++)
			{
				pix.x = x;
				pix.y = y;
				if (!saveToFile)
					pDC->SetPixel(pix, color);
				else
					imgToSave.SetValue(x, y, SET_RGB(GET_A(color), GET_B(color), GET_G(color)));
			}
		}
	}
}

CPoint CCGWorkView::TranslateBToFirstOctant(const CPoint & a, const CPoint & b, int oct)
{
	assert(0 < oct && oct <= 8);
	CPoint normalized = b - a;
	CPoint translated = TranslatePointTo8th(normalized, oct);
	return a + translated;
}

//Translates 8th octant point to given octant
CPoint CCGWorkView::TranslatePointFrom8th(CPoint p, int oct)
{
	assert(0 < oct && oct <= 8);

	int x, y;
	switch (oct)
	{
	case 1: x = p.x;	y = -p.y;	break;
	case 2: x = p.y;	y = -p.x;	break;
	case 3: x = -p.y;	y = -p.x;	break;
	case 4: x = -p.x;	y = -p.y;	break;
	case 5: x = -p.x;	y = p.y;	break;
	case 6: x = -p.y;	y = p.x;	break;
	case 7: x = p.y;	y = p.x;	break;
	case 8: x = p.x;	y = p.y;	break;
	default:
		break;
	}
	return CPoint(x, y);
}

CPoint CCGWorkView::TranslatePointTo8th(CPoint p, int oct)
{
	assert(0 < oct && oct <= 8);

	//Translation is identical except 2nd and 6th octants
	if (oct == 2 || oct == 6) {
		p.x *= -1;
		p.y *= -1;
	}

	return TranslatePointFrom8th(p, oct);
}

void CCGWorkView::SetSelectedPoly(CPoint mousePos, Poly* p, std::vector<Vec4Line> poly)
{
	std::vector<double> point;
	point.push_back(mousePos.x);
	point.push_back(mousePos.y);

	if (PointPolyIntersection(point, poly))
		p->IsSelected = true;
	else
		p->IsSelected = false;
}

void CCGWorkView::DrawSelectedPolys(CDC* pDC)
{
	for (std::vector<Edge>& poly : selectedPolys)
	{
		for (Edge& edge : poly)
		{
			edge.color = AL_YELLO_CREF;
		}
		DrawPoly(pDC, poly);
	}
	selectedPolys.clear();
}

void CCGWorkView::DrawBoundingBox(CDC * pDC, const std::vector<Poly*>& polys, const Mat4 & modelTransform, const Mat4 & camTransform, const Mat4 & projection, const Mat4 & toView, COLORREF color)
{
	for (Poly* p : polys)
	{
		std::vector<Edge> poly;
		for (unsigned int i = 0; i < p->Vertices.size(); i++)
		{
			Vec4 pos1 = p->Vertices[i]->Pos;
			Vec4 pos2 = p->Vertices[(i + 1) % p->Vertices.size()]->Pos;

			Vec4 clipped1 = pos1 * modelTransform * camTransform * projection;
			Vec4 clipped2 = pos2 * modelTransform * camTransform * projection;

			clipped1 /= clipped1[3];
			clipped2 /= clipped2[3];

			Vec4 pix1Vec(clipped1 * toView);
			Vec4 pix2Vec(clipped2 * toView);

			CPoint pix1((int)pix1Vec[0], (int)pix1Vec[1]);
			CPoint pix2((int)pix2Vec[0], (int)pix2Vec[1]);

			DVertex dv1, dv2;
			dv1.Pixel = pix1;
			dv2.Pixel = pix2;

			poly.push_back({ dv1, dv2 , color });
		}

		DrawPoly(pDC, poly);
	}
}

void CCGWorkView::DrawVertexNormal(CDC * pDC, const Vertex * v, const Mat4 & modelTransform,
	const Mat4 & normalTransform, const Mat4 & camTransform, const Mat4 & projection, 
	const Mat4 & toView, COLORREF color)
{
	// Transform vertex position to view space
	Vec4 startPos = v->Pos * modelTransform * camTransform;
	
	// Transform normal to view space
	Vec4 normal = Scene::GetInstance().GetCalcNormalState() ?
		v->CalcNormal : v->Normal;
	normal *= normalSign;
	normal = normal * normalTransform * camTransform;
	normal = Vec4::Normalize3(normal);

	// Calculate endPos
	Vec4 endPos = startPos + normal * normalSizeFactor;

	// Transform Start and End Pos to screen space
	startPos = startPos * projection;
	startPos /= startPos[3];
	startPos = startPos * toView;

	endPos = endPos * projection;
	endPos /= endPos[3];
	endPos = endPos * toView;

	// Draw normal
	CPoint startPosPix((int)startPos[0], (int)startPos[1]);
	CPoint endPosPix((int)endPos[0], (int)endPos[1]);
	DrawLine(pDC, color, startPosPix, endPosPix);
}

void CCGWorkView::DrawPolyNormal(CDC * pDC, const Poly * p, const Mat4 & modelTransform, 
	const Mat4 & normalTransform, const Mat4 & camTransform, const Mat4 & projection, 
	const Mat4 & toView, COLORREF color)
{
	// Transform poly center position to view space
	Vec4 polyCenter = p->Center * modelTransform * camTransform;

	// Transform normal to view space
	Vec4 normal = Scene::GetInstance().GetCalcNormalState() ?
		p->CalcNormal : p->Normal;
	normal *= normalSign;
	normal = normal * normalTransform * camTransform;
	normal = Vec4::Normalize3(normal);

	// Calculate endPos
	Vec4 endPos = polyCenter + normal * normalSizeFactor;

	// Transform PolyCenter and End Pos to screen space
	polyCenter = polyCenter * projection;
	polyCenter /= polyCenter[3];
	polyCenter = polyCenter * toView;

	endPos = endPos * projection;
	endPos /= endPos[3];
	endPos = endPos * toView;

	// Draw normal
	CPoint polyCenterPix((int)polyCenter[0], (int)polyCenter[1]);
	CPoint endPosPix((int)endPos[0], (int)endPos[1]);
	DrawLine(pDC, color, polyCenterPix, endPosPix);
}

void CCGWorkView::DrawBackground(CDC* pDC, CRect r)
{
	if (((currentPolySelection != WIREFRAME) || saveToFile) && isBGFileOpen) {

		CT2A BG(BGFile);
		PngWrapper pngReadFile(BG);
		pngReadFile.ReadPng();

		int width = saveToFile ? imgWidth : m_WindowWidth;
		int height = saveToFile ? imgHeight : m_WindowHeight;

		if (isBGStretch) {
			// Scale factors
			double cx = (double)width / (double)pngReadFile.GetWidth();
			double cy = (double)height / (double)pngReadFile.GetHeight();

			for (int x = 0; x < width; x++) {
				for (int y = 0; y < height; y++) {
					// Calculate position in image
					double v = x / cx;
					double w = y / cy;

					// Pick the nearest neighbor to (v,w)
					int c = pngReadFile.GetValue((int)round(v), (int)round(w));
					int r = GET_R(c);
					int g = GET_G(c);
					int b = GET_B(c);
					if (!saveToFile)
						pDC->SetPixel(x, y, RGB(r, g, b));
					else
						imgToSave.SetValue(x, y, SET_RGB(GET_A(RGB(r, g, b)), GET_B(RGB(r, g, b)), GET_G(RGB(r, g, b))));
				}
			}
		}
		else {
			for (int x = 0; x < width; x++) {
				for (int y = 0; y < height; y++) {
					int c = pngReadFile.GetValue(x % pngReadFile.GetWidth(), y % pngReadFile.GetHeight());
					int r = GET_R(c);
					int g = GET_G(c);
					int b = GET_B(c);

					if (!saveToFile)
						pDC->SetPixel(x, y, RGB(r, g, b));
					else
						imgToSave.SetValue(x, y, SET_RGB(GET_A(RGB(r, g, b)), GET_B(RGB(r, g, b)), GET_G(RGB(r, g, b))));
				}
			}
		}
	}
	else {
		COLORREF bGColorRef = m_colorDialog.BackgroundColor;
		if (!saveToFile)
			pDC->FillSolidRect(&r, bGColorRef);
		else
		{
			for (int x = 0; x < imgWidth; x++)
				for (int y = 0; y < imgHeight; y++)
					imgToSave.SetValue(x, y, SET_RGB(GET_A(bGColorRef), GET_B(bGColorRef), GET_G(bGColorRef)));
		}
	}
}

void CCGWorkView::DrawSilhouetteEdges(CDC * pDC, Geometry* geo, const Mat4 & modelTransform,
	const Mat4 & normalTransform, const Mat4 & camTransform, const Mat4 & projection,
	const Mat4 & toView, COLORREF color)
{
	for (PolyEdge* e : geo->Edges)
	{
		if (IsSilhouetteEdge(e, modelTransform, normalTransform, camTransform))
		{
			// Transform from object space to projected space
			Vec4 p1 = e->A->Pos * modelTransform * camTransform * projection;
			Vec4 p2 = e->B->Pos * modelTransform * camTransform * projection;
			p1 /= p1[3];
			p2 /= p2[3];

			// Transform to screen space
			p1 = p1 * toView;
			p2 = p2 * toView;

			// Draw line
			CPoint p1Pix((int)p1[0], (int)p1[1]);
			CPoint p2Pix((int)p2[0], (int)p2[1]);
			DrawLine(pDC, color, p1Pix, p2Pix, silThickness - 1); // converting thickness to be 0-2 from 1-3
		}
	}
}

int CCGWorkView::OnCreate(LPCREATESTRUCT lpCreateStruct) 
{
	if (CView::OnCreate(lpCreateStruct) == -1)
		return -1;

	InitializeCGWork();

	return 0;
}

// This method initialized the CGWork system.
BOOL CCGWorkView::InitializeCGWork()
{
	m_pDC = new CClientDC(this);
	
	if ( NULL == m_pDC ) { // failure to get DC
		::AfxMessageBox(CString("Couldn't get a valid DC."));
		return FALSE;
	}

	CRect r;
	GetClientRect(&r);
	m_pDbDC = new CDC();
	m_pDbDC->CreateCompatibleDC(m_pDC);
	SetTimer(1, 1, NULL);
	m_pDbBitMap = CreateCompatibleBitmap(m_pDC->m_hDC, r.right, r.bottom);	
	m_pDbDC->SelectObject(m_pDbBitMap);

	return TRUE;
}


/////////////////////////////////////////////////////////////////////////////
// CCGWorkView message handlers


void CCGWorkView::OnSize(UINT nType, int cx, int cy) 
{
	CView::OnSize(nType, cx, cy);

	if ( 0 >= cx || 0 >= cy ) {
		return;
	}

	// save the width and height of the current window
	m_WindowWidth = cx;
	m_WindowHeight = cy;

	// compute the aspect ratio
	// this will keep all dimension scales equal
	m_AspectRatio = (GLdouble)m_WindowWidth/(GLdouble)m_WindowHeight;

	CRect r;
	GetClientRect(&r);
	DeleteObject(m_pDbBitMap);
	m_pDbBitMap = CreateCompatibleBitmap(m_pDC->m_hDC, r.right, r.bottom);	
	m_pDbDC->SelectObject(m_pDbBitMap);

	// Update camera projections
	Camera* camera = Scene::GetInstance().GetCamera();
	bool isPerspective = camera->IsPerspective();
	OrthographicParams oParams = camera->GetOrthographicParameters();
	PerspectiveParams pParams = camera->GetPerspectiveParameters();
#ifdef D_PERSP
	camera->SetPerspective2(pParams.FOV, m_AspectRatio, pParams.Near, pParams.Far);
#else
	camera->SetPerspective(pParams.FOV, m_AspectRatio, pParams.Near, pParams.Far);
#endif
	double width = m_AspectRatio * orthoHeight;
	Scene::GetInstance().GetCamera()->SetOrthographic(-width / 2.0,
		width / 2.0, orthoHeight / 2.0, -orthoHeight / 2.0, oParams.Near, oParams.Far);
	camera->SwitchProjection(isPerspective);
}


BOOL CCGWorkView::SetupViewingFrustum(void)
{
    return TRUE;
}


// This viewing projection gives us a constant aspect ration. This is done by
// increasing the corresponding size of the ortho cube.
BOOL CCGWorkView::SetupViewingOrthoConstAspect(void)
{
	return TRUE;
}

Vec4 CCGWorkView::CalculateShading(LightParams* lights, Material* material, Vec4 pos, Vec4 normal, COLORREF color)
{
	Vec4 ambient = (Vec4(m_ambientLight.ColorR, m_ambientLight.ColorG, m_ambientLight.ColorB) / 255.0) * material->Ka;
	Vec4 diffuse;
	Vec4 specular;
	int numEnabledLights = 0;
	for (int i = 0; i < MAX_LIGHT; i++)
	{
		if (lights[i].Enabled)
		{
			Vec4 lightPos(lights[i].PosX, lights[i].PosY, lights[i].PosZ);
			Vec4 direction(lights[i].DirX, lights[i].DirY, lights[i].DirZ);
			if (lights[i].Space == LIGHT_SPACE_LOCAL)
			{
				Mat4 camTransform = Scene::GetInstance().GetCamera()->GetTranform();

				lightPos = lightPos * camTransform;

				camTransform[3][0] = 0.0;
				camTransform[3][1] = 0.0;
				camTransform[3][2] = 0.0;
				direction = direction * camTransform;
			}
			if (lights[i].Type == LIGHT_TYPE_POINT)
				direction = pos - lightPos;

			Vec4 intensity(lights[i].ColorR, lights[i].ColorG, lights[i].ColorB);
			intensity /= 255.0;	// So intensity will be between 0.0 - 1.0

			// normalize everything
			direction = Vec4::Normalize3(direction);
			normal = Vec4::Normalize3(normal);

			Vec4 camPos(0.0);
			if (!m_bIsPerspective)
			{
				camPos[2] = 1.0;
			}

			// Diffuse calculation
			Vec4 diffuseCoeff = material->Kd * max(Vec4::Dot3(-normal, -direction), 0.0);
			diffuse += intensity * diffuseCoeff;

			// Specular calculation
			Vec4 camDir = Vec4::Normalize3(camPos - pos);
			Vec4 R = normal * 2;
			R = R * max(Vec4::Dot3(-normal, -direction), 0.0);
			R = R - direction;
			R = Vec4::Normalize3(R);
			Vec4 specularCoeff = material->Ks * pow(max(Vec4::Dot3(R, camDir), 0.0), material->Specular);
			specular += intensity * specularCoeff;

			if (lights[i].Type != LIGHT_TYPE_DIRECTIONAL)
			{
				double distance = Vec4::Distance3(pos, lightPos);
				if (lights[i].Attenuation == LIGHT_ATTENUATION_NONE)
					distance = 1.0;
				if (lights[i].Attenuation == LIGHT_ATTENUATION_POWEROF2)
					distance *= distance;

				diffuse /= distance;
				specular /= distance;
			}

			numEnabledLights++;
		}
	}

	// Normalize diffuse and specular by number of lights
	/*
	if (numEnabledLights > 0)
	{
		diffuse /= numEnabledLights;
		specular /= numEnabledLights;
	}
	*/

	Vec4 objectColor =  Vec4(GetRValue(color) / 255.0, GetGValue(color) / 255.0, GetBValue(color) / 255.0);
	Vec4 sum = (ambient + diffuse) * objectColor + specular;
	return Vec4(min(sum[0], 1.0), min(sum[1], 1.0), min(sum[2], 1.0));
}

bool CCGWorkView::IsBackFace(const Poly* p, const Mat4& modelTransform, const Mat4& normalTransform, const Mat4& camTransform)
{
	Mat4 projection = Scene::GetInstance().GetCamera()->GetProjection();
	Vec4 normal = Scene::GetInstance().GetCalcNormalState() ?
		p->CalcNormal : p->Normal;
	normal *= normalSign;

	// Transform normal and poly center to View Space
	normal = Vec4::Normalize3(normal * normalTransform * camTransform);
	Vec4 center = p->Center * modelTransform * camTransform;

	if (m_bIsPerspective)
	{
		if (Vec4::Dot3(center, normal) > 0)
			return true;
		return false;
	}

	normal = Vec4::Normalize3(normal * projection);
	return normal[2] < 0;
}

bool CCGWorkView::IsSilhouetteEdge(const PolyEdge* e, const Mat4 & modelTransform, const Mat4 & normalTransform, const Mat4 & camTransform)
{
	if (e->Polys.size() != 2)
		return false;

	bool isPoly1BF = IsBackFace(e->Polys[0], modelTransform, normalTransform, camTransform);
	bool isPoly2BF = IsBackFace(e->Polys[1], modelTransform, normalTransform, camTransform);
	return (isPoly1BF ^ isPoly2BF);
}

BOOL CCGWorkView::OnEraseBkgnd(CDC* pDC) 
{
	// Windows will clear the window with the background color every time your window 
	// is redrawn, and then CGWork will clear the viewport with its own background color.

	// Clear zBuffer
	if (zBuffer != NULL)
	{
		delete zBuffer;
		zBuffer = NULL;
	}
	
	return true;
}



/////////////////////////////////////////////////////////////////////////////
// CCGWorkView drawing
/////////////////////////////////////////////////////////////////////////////

void CCGWorkView::OnDraw(CDC* pDC)
{
	static float theta = 0.0f;
	CCGWorkDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if (!pDoc)
	    return;
	CRect r;

	GetClientRect(&r);
	CDC *pDCToUse = /*m_pDC*/m_pDbDC;

	if (isFirstDraw)
	{
		// Get Viewport parameters
		m_WindowWidth = r.Width();
		m_WindowHeight = r.Height();
		m_AspectRatio = (double)m_WindowWidth / (double)m_WindowHeight;
		
		// Set initial Projection matricies
		double width = orthoHeight * m_AspectRatio;
		Scene::GetInstance().GetCamera()->SetPerspective(45.0, m_AspectRatio, 1.0, 1000.0);
		Scene::GetInstance().GetCamera()->SetOrthographic(orthoHeight, m_AspectRatio, 1, 1000.0);

		// Set Perspective dialog default values
		m_perspDialog.ProjPlane = Scene::GetInstance().GetCamera()->GetPerspectiveParameters().Near;
		m_perspDialog.FOV = Scene::GetInstance().GetCamera()->GetPerspectiveParameters().FOV;

		isFirstDraw = false;
	}

	// Initialize zBuffer
	int bufferSize = (saveToFile) ? (imgWidth * imgHeight) : (m_WindowWidth * m_WindowHeight);
	zBuffer = new double[bufferSize];
	for (int i = 0; i < bufferSize; i++)
		zBuffer[i] = -DBL_MAX;

	// Background Drawing
	DrawBackground(pDCToUse, r);
	
	// Don't draw if model is not finished loading
	if (isModelLoaded)
	{
		std::vector<Model*> models = Scene::GetInstance().GetModels();
		Camera* camera = Scene::GetInstance().GetCamera();
		Mat4 camTransform = camera->GetTranform();
		Mat4 projection = camera->GetProjection();
		Mat4 toView(Vec4(m_WindowWidth / 2.0, 0.0, 0.0, 0.0),
			Vec4(0.0, m_WindowHeight / 2.0, 0.0, 0.0),
			Vec4(0.0, 0.0, 1.0, 0.0),
			Vec4((m_WindowWidth - 1) / 2.0, (m_WindowHeight - 1) / 2.0, 0.0, 1.0));

		PerspectiveParams pParams = camera->GetPerspectiveParameters();
		OrthographicParams oParams = camera->GetOrthographicParameters();
		if (saveToFile)
		{
			double imgAspectRatio = (double)imgWidth / (double)imgHeight;
			if (m_bIsPerspective)
				camera->SetPerspective2(pParams.FOV, imgAspectRatio, pParams.Near, pParams.Far);
			else
			{
				double width = orthoHeight * imgAspectRatio;
				camera->SetOrthographic(orthoHeight, imgAspectRatio, oParams.Near, oParams.Far);
			}

			projection = camera->GetProjection();
			toView = Mat4(Vec4(imgWidth / 2.0, 0.0, 0.0, 0.0),
				Vec4(0.0, imgHeight / 2.0, 0.0, 0.0),
				Vec4(0.0, 0.0, 1.0, 0.0),
				Vec4((imgWidth - 1) / 2.0, (imgHeight - 1) / 2.0, 0.0, 1.0));
		}

		for (Model* model : models)
		{
			Mat4 transform = model->GetTransform();
			Mat4 normalTransform = model->GetNormalTransform();
			COLORREF color = isCColorDialogOpen ? m_colorDialog.WireframeColor :
				RGB(model->GetColor()[0], model->GetColor()[1], model->GetColor()[2]);
			COLORREF normalColor = isCColorDialogOpen ? m_colorDialog.NormalColor :
				RGB(model->GetNormalColor()[0], model->GetNormalColor()[1], model->GetNormalColor()[2]);
			for (Geometry* geo : model->GetGeometries())
			{
				color = (!showGeos) ? color :
					RGB((BYTE)geo->Color[0], (BYTE)geo->Color[1], (BYTE)geo->Color[2]);
				COLORREF selectedColor = RGB((BYTE)(255.0 - model->GetColor()[0]),
					(BYTE)(255.0 - model->GetColor()[1]), (BYTE)(255.0 - model->GetColor()[2]));
				// Draw Polys
				std::vector<Poly*> polygons = geo->Polygons;
				for (Poly* p : polygons)
				{
					std::vector<Edge> poly;
					std::vector<Vec4Line> polyEdges;

					// Check Backface culling, and draw only if it is front facing
					if (isBFCulling && IsBackFace(p, transform, normalTransform, camTransform))
						continue;

					for (unsigned int i = 0; i < p->Vertices.size(); i++)
					{
						// Save vertices in object space
						Vec4 pos1 = p->Vertices[i]->Pos;
						Vec4 pos2 = p->Vertices[(i + 1) % p->Vertices.size()]->Pos;

						// Transform vertices from object space to normalized device coords
						Vec4 clipped1 = pos1 * transform * camTransform * projection;
						Vec4 clipped2 = pos2 * transform * camTransform * projection;

						// Divide by W
						clipped1 /= clipped1[3];
						clipped2 /= clipped2[3];

						// Transform from NDC to screen space
						Vec4 pix1Vec(clipped1 * toView);
						Vec4 pix2Vec(clipped2 * toView);

						CPoint pix1((int)pix1Vec[0], (int)pix1Vec[1]);
						CPoint pix2((int)pix2Vec[0], (int)pix2Vec[1]);

						// Contruct poly for mouse selection test
						polyEdges.push_back({ pix1Vec, pix2Vec });

						// Initialize vertex for drawing
						DVertex dVertex1, dVertex2;

						Vec4 normal1 = Scene::GetInstance().GetCalcNormalState() ?
							p->Vertices[i]->CalcNormal : p->Vertices[i]->Normal;
						normal1 *= normalSign;
						Vec4 normal2 = Scene::GetInstance().GetCalcNormalState() ?
							p->Vertices[(i + 1) % p->Vertices.size()]->CalcNormal :
							p->Vertices[(i + 1) % p->Vertices.size()]->Normal;
						normal2 *= normalSign;

						// Save parameters in DVertex
						dVertex1.Pixel = pix1;
						dVertex1.PosVS = pos1 * transform * camTransform;
						dVertex1.Z = clipped1[2];
						dVertex1.NormalVS = normal1 * transform * camTransform;
						// Calculate color
						dVertex1.Color = CalculateShading(m_lights, model->GetMaterial(), dVertex1.PosVS, dVertex1.NormalVS, color) * 255.0;

						dVertex2.Pixel = pix2;
						dVertex2.PosVS = pos2 * transform * camTransform;
						dVertex2.Z = clipped2[2];
						dVertex2.NormalVS = normal2 * transform * camTransform;
						// Calculate color
						dVertex2.Color = CalculateShading(m_lights, model->GetMaterial(), dVertex2.PosVS, dVertex2.NormalVS, color) * 255.0;

						// Construct poly for drawing
						if (m_colorDialog.IsDiscoMode)
							color = AL_RAINBOW_CREF;
						poly.push_back({ dVertex1, dVertex2 , color });

						// Draw vertex normal if needed
						if (Scene::GetInstance().AreVertexNormalsOn())
						{
							DrawVertexNormal(pDCToUse, p->Vertices[i], transform, normalTransform,
								camTransform, projection, toView, normalColor);
						}
					}

					// If mouse button was clicked in Select mode perform intersection caculation
					if (mouseClicked)
						SetSelectedPoly(mouseClickPos, p, polyEdges);

					// If the poly was selected, save it in the selectedPolys list
					if (p->IsSelected)
						selectedPolys.push_back(poly);

					Vec4 polyCenter = p->Center * transform * camTransform;
					Vec4 normal = Scene::GetInstance().GetCalcNormalState() ?
						p->CalcNormal : p->Normal;
					normal = normal * normalTransform * camTransform;
					normal *= normalSign;

					// Draw poly in wireframe mode or fill it, according to user selection
					if (currentPolySelection == WIREFRAME && !saveToFile)
						DrawPoly(pDCToUse, poly);
					else if (currentPolySelection == SOLID_SCREEN || saveToFile)
						ScanConvert(pDCToUse, poly, color, polyCenter, normal);

					// Draw poly normal if needed
					if (Scene::GetInstance().ArePolyNormalsOn())
					{
						DrawPolyNormal(pDCToUse, p, transform, normalTransform,
							camTransform, projection, toView, normalColor);
					}
				}

				// Draw Geo Bounding Box if needed
				if (Scene::GetInstance().GetBBoxState() && showGeos)
				{
					DrawBoundingBox(pDCToUse, geo->BBox, transform, camTransform, projection, toView, color);
				}

				// Draw Silhouette Edges if needed
				if (showSil)
				{
					DrawSilhouetteEdges(pDCToUse, geo, transform, normalTransform, camTransform, 
						projection, toView, silColor);
				}
			}

			// Reset mouseclicked flag
			mouseClicked = false;

			// Draw the selected polys in the end to make sure they are on top
			DrawSelectedPolys(pDCToUse);

			// Draw Model Bounding Box if needed
			if (Scene::GetInstance().GetBBoxState() && !showGeos)
			{
				DrawBoundingBox(pDCToUse, model->GetBBox(), transform, camTransform, projection, toView, color);
			}
		}

		if (saveToFile)
		{
			imgToSave.WritePng();
			AfxMessageBox(L"Rendered to file successfully", MB_OK | MB_ICONINFORMATION);
			saveToFile = false;

			// Restore original projection
			if (m_bIsPerspective)
				camera->SetPerspective2(pParams.FOV, m_AspectRatio, pParams.Near, pParams.Far);
			else
			{
				double width = orthoHeight * m_AspectRatio;
				camera->SetOrthographic(orthoHeight, m_AspectRatio, oParams.Near, oParams.Far);
			}
		}
	}

	if (pDCToUse != m_pDC) 
	{
		m_pDC->BitBlt(r.left, r.top, r.Width(), r.Height(), pDCToUse, r.left, r.top, SRCCOPY);
	}
}


/////////////////////////////////////////////////////////////////////////////
// CCGWorkView CGWork Finishing and clearing...

void CCGWorkView::OnDestroy() 
{
	CView::OnDestroy();

	// delete the DC
	if ( m_pDC ) {
		delete m_pDC;
	}

	if (m_pDbDC) {
		delete m_pDbDC;
	}
}



/////////////////////////////////////////////////////////////////////////////
// User Defined Functions

void CCGWorkView::RenderScene() {
	// do nothing. This is supposed to be overriden...

	return;
}


void CCGWorkView::OnFileLoad() 
{
	TCHAR szFilters[] = _T ("IRIT Data Files (*.itd)|*.itd|All Files (*.*)|*.*||");

	CFileDialog dlg(TRUE, _T("itd"), _T("*.itd"), OFN_FILEMUSTEXIST | OFN_HIDEREADONLY ,szFilters);

	if (dlg.DoModal () == IDOK) {
		isModelLoaded = false;

		// Delete previous models
		Scene::GetInstance().DeleteModels();

		// Create Model
		Scene::GetInstance().CreateModel();

		// Load model from file
		m_strItdFileName = dlg.GetPathName();		// Full path and filename
		PngWrapper p;
		CGSkelProcessIritDataFiles(m_strItdFileName, 1);

		// Get New Model and Camera
		Camera* camera = Scene::GetInstance().GetCamera();
		Model* model = Scene::GetInstance().GetModels().back();
		
		// Build Building Box
		model->BuildBoundingBox();
		// Get Bounding Box location and dimensions
		Vec4 bboxCenter = model->GetBBoxCenter();
		Vec4 bboxDim = model->GetBBoxDimensions();
		double maxDim = max(max(bboxDim[0], bboxDim[1]), bboxDim[2]);

		// Set Camera position
		double radius = maxDim / 2.0;
		double f = sin(ToRadians(camera->GetPerspectiveParameters().FOV / 2.0));
		double offset = 3.0;
		if (bboxDim[0] > bboxDim[1])
		{
			f = sin(ToRadians(m_AspectRatio * camera->GetPerspectiveParameters().FOV / 2.0));
			offset = 3.5;
		}
		double zPos = abs(radius / f) * offset;
		camera->LookAt(bboxCenter - Vec4(0.0, 0.0, zPos), bboxCenter, Vec4(0.0, 1.0, 0.0));

		// Set Perspective projection
#ifdef D_PERSP
		camera->SetPerspective2(45.0, m_AspectRatio, 0.1, 2.0);
#else
		camera->SetPerspective(45.0, m_AspectRatio, 1.0, 1000.0);
#endif

		// Set orthographic projection dimensions
		double orthoOffset = 1.0;
		orthoHeight = abs(maxDim) + orthoOffset;
		camera->SetOrthographic(orthoHeight, m_AspectRatio, 1.0, 1000.0);

		// Switch camera to the correct projection (the one that is selected)
		camera->SwitchProjection(m_bIsPerspective);
		
		// Set ColorDialog model color
		m_colorDialog.WireframeColor = RGB(model->GetColor()[0], model->GetColor()[1], model->GetColor()[2]);

		// Model material using the Material dialog
		Material* material = model->GetMaterial();
		material->Ka = m_materialDialog.GetAmbientCoeffs();
		material->Kd = m_materialDialog.GetDiffuseCoeffs();
		material->Ks = m_materialDialog.GetShininessCoeffs();
		material->Specular = m_materialDialog.GetSpecularCoeff();

		isModelLoaded = true;
		Invalidate();	// force a WM_PAINT for drawing.
	} 
}

// VIEW HANDLERS ///////////////////////////////////////////

// Note: that all the following Message Handlers act in a similar way.
// Each control or command has two functions associated with it.

void CCGWorkView::OnViewOrthographic() 
{
	m_nView = ID_VIEW_ORTHOGRAPHIC;
	m_bIsPerspective = false;

	Scene::GetInstance().GetCamera()->SwitchProjection(m_bIsPerspective);

	Invalidate();		// redraw using the new view.
}

void CCGWorkView::OnUpdateViewOrthographic(CCmdUI* pCmdUI) 
{
	pCmdUI->SetCheck(m_nView == ID_VIEW_ORTHOGRAPHIC);
}

void CCGWorkView::OnViewPerspective() 
{
	m_nView = ID_VIEW_PERSPECTIVE;
	m_bIsPerspective = true;

	Scene::GetInstance().GetCamera()->SwitchProjection(m_bIsPerspective);

	Invalidate();
}

void CCGWorkView::OnUpdateViewPerspective(CCmdUI* pCmdUI) 
{
	pCmdUI->SetCheck(m_nView == ID_VIEW_PERSPECTIVE);
}




// ACTION HANDLERS ///////////////////////////////////////////

void CCGWorkView::OnActionRotate() 
{
	m_nAction = ID_ACTION_ROTATE;
}

void CCGWorkView::OnUpdateActionRotate(CCmdUI* pCmdUI) 
{
	pCmdUI->SetCheck(m_nAction == ID_ACTION_ROTATE);
}

void CCGWorkView::OnActionTranslate() 
{
	m_nAction = ID_ACTION_TRANSLATE;
}

void CCGWorkView::OnUpdateActionTranslate(CCmdUI* pCmdUI) 
{
	pCmdUI->SetCheck(m_nAction == ID_ACTION_TRANSLATE);
}

void CCGWorkView::OnActionScale() 
{
	m_nAction = ID_ACTION_SCALE;
}

void CCGWorkView::OnUpdateActionScale(CCmdUI* pCmdUI) 
{
	pCmdUI->SetCheck(m_nAction == ID_ACTION_SCALE);
}

void CCGWorkView::OnActionSelect()
{
	m_nAction = ID_ACTION_SELECT;
}


void CCGWorkView::OnUpdateActionSelect(CCmdUI *pCmdUI)
{
	pCmdUI->SetCheck(m_nAction == ID_ACTION_SELECT);
}




// AXIS HANDLERS ///////////////////////////////////////////


// Gets calles when the X button is pressed or when the Axis->X menu is selected.
// The only thing we do here is set the ChildView member variable m_nAxis to the 
// selected axis.
void CCGWorkView::OnAxisX() 
{
	m_isAxis_X = !m_isAxis_X;
}

// Gets called when windows has to repaint either the X button or the Axis pop up menu.
// The control is responsible for its redrawing.
// It sets itself disabled when the action is a Scale action.
// It sets itself Checked if the current axis is the X axis.
void CCGWorkView::OnUpdateAxisX(CCmdUI* pCmdUI) 
{
	pCmdUI->SetCheck(m_isAxis_X);
}


void CCGWorkView::OnAxisY() 
{
	m_isAxis_Y = !m_isAxis_Y;
}

void CCGWorkView::OnUpdateAxisY(CCmdUI* pCmdUI) 
{
	pCmdUI->SetCheck(m_isAxis_Y);
}


void CCGWorkView::OnAxisZ() 
{
	m_isAxis_Z = !m_isAxis_Z;
}

void CCGWorkView::OnUpdateAxisZ(CCmdUI* pCmdUI) 
{
	pCmdUI->SetCheck(m_isAxis_Z);
}





// OPTIONS HANDLERS ///////////////////////////////////////////




// LIGHT SHADING HANDLERS ///////////////////////////////////////////

void CCGWorkView::OnLightShadingFlat() 
{
	m_nLightShading = ID_LIGHT_SHADING_FLAT;
	Invalidate();
}

void CCGWorkView::OnUpdateLightShadingFlat(CCmdUI* pCmdUI) 
{
	pCmdUI->SetCheck(m_nLightShading == ID_LIGHT_SHADING_FLAT);
}


void CCGWorkView::OnLightShadingGouraud() 
{
	m_nLightShading = ID_LIGHT_SHADING_GOURAUD;
	Invalidate();
}

void CCGWorkView::OnUpdateLightShadingGouraud(CCmdUI* pCmdUI) 
{
	pCmdUI->SetCheck(m_nLightShading == ID_LIGHT_SHADING_GOURAUD);
}

void CCGWorkView::OnLightShadingPhong()
{
	m_nLightShading = ID_LIGHT_SHADING_PHONG;
	Invalidate();
}


void CCGWorkView::OnUpdateLightShadingPhong(CCmdUI *pCmdUI)
{
	pCmdUI->SetCheck(m_nLightShading == ID_LIGHT_SHADING_PHONG);
}

// LIGHT SETUP HANDLER ///////////////////////////////////////////

void CCGWorkView::OnLightConstants() 
{
	CLightDialog dlg;

	for (int id=LIGHT_ID_1;id<MAX_LIGHT;id++)
	{	    
	    dlg.SetDialogData((LightID)id,m_lights[id]);
	}
	dlg.SetDialogData(LIGHT_ID_AMBIENT,m_ambientLight);

	if (dlg.DoModal() == IDOK) 
	{
	    for (int id=LIGHT_ID_1;id<MAX_LIGHT;id++)
	    {
		m_lights[id] = dlg.GetDialogData((LightID)id);
	    }
	    m_ambientLight = dlg.GetDialogData(LIGHT_ID_AMBIENT);
	}	
	Invalidate();
}

void CCGWorkView::OnTimer(UINT_PTR nIDEvent)
{
	CView::OnTimer(nIDEvent);
	if (nIDEvent == 1 && m_colorDialog.IsDiscoMode)
		Invalidate();
}

void CCGWorkView::OnLButtonDown(UINT nFlags, CPoint point)
{
	prevMousePos = point;

	if (m_nAction == ID_ACTION_SELECT && Scene::GetInstance().GetModels().size() > 0)
	{
		mouseClicked = true;
		mouseClickPos = point;
		selectedPolys.clear();
	}

	CView::OnLButtonDown(nFlags, point);

	Invalidate();
}

void CCGWorkView::OnMouseMove(UINT nFlags, CPoint point)
{
	double dx = point.x - prevMousePos.x;
	double dy = point.y - prevMousePos.y;

	if ((nFlags & MK_LBUTTON) == MK_LBUTTON)
	{
		if (Scene::GetInstance().GetModels().size() == 0)
			return;
		
		Model* model = Scene::GetInstance().GetModels().back();
		Camera* camera = Scene::GetInstance().GetCamera();

		if (m_nAction == ID_ACTION_TRANSLATE)
		{
			double x_trans = m_isAxis_X ? dx / m_sensitivity[0] : 0.0;
			double y_trans = m_isAxis_Y ? dx / m_sensitivity[0] : 0.0;
			double z_trans = m_isAxis_Z ? dx / m_sensitivity[0] : 0.0;

			if (m_nCoordSpace == ID_BUTTON_OBJECT) 
				model->Translate(Mat4::Translate(x_trans, y_trans, z_trans));
			else
				camera->Translate(Mat4::Translate(x_trans, y_trans, z_trans));
		}
		else if (m_nAction == ID_ACTION_ROTATE)
		{

			if (m_isAxis_X)
			{
				if (m_nCoordSpace == ID_BUTTON_OBJECT)
					model->Rotate(Mat4::RotateX(dx / m_sensitivity[1]));
				else
					camera->Rotate(Mat4::RotateX(-dx / m_sensitivity[1]), aroundEye);
			}
			if (m_isAxis_Y)
			{
				if (m_nCoordSpace == ID_BUTTON_OBJECT)
					model->Rotate(Mat4::RotateY(dx / m_sensitivity[1]));
				else
					camera->Rotate(Mat4::RotateY(-dx / m_sensitivity[1]), aroundEye);
			}
			if (m_isAxis_Z)
			{
				if (m_nCoordSpace == ID_BUTTON_OBJECT)
					model->Rotate(Mat4::RotateZ(dx / m_sensitivity[1]));
				else
					camera->Rotate(Mat4::RotateZ(-dx / m_sensitivity[1]), aroundEye);
			}
		}
		else if (m_nAction == ID_ACTION_SCALE)
		{
			double x_trans = 1.0 + (m_isAxis_X ? (dx / m_sensitivity[2]) : 0.0);
			double y_trans = 1.0 + (m_isAxis_Y ? (dx / m_sensitivity[2]) : 0.0);
			double z_trans = 1.0 + (m_isAxis_Z ? (dx / m_sensitivity[2]) : 0.0);

			x_trans = (x_trans < 0.01) ? 0.01 : x_trans;
			y_trans = (y_trans < 0.01) ? 0.01 : y_trans;
			z_trans = (z_trans < 0.01) ? 0.01 : z_trans;
			
			if (m_nCoordSpace == ID_BUTTON_OBJECT)
				model->Scale(Mat4::Scale(x_trans, y_trans, z_trans));
			else
				camera->Scale(Mat4::Scale(x_trans, y_trans, z_trans), aroundEye);
		}
		Invalidate();
	}

	prevMousePos = point;

	CView::OnMouseMove(nFlags, point);
}


void CCGWorkView::OnButtonBbox()
{
	isBBoxOn = !isBBoxOn;
	if (Scene::GetInstance().GetModels().size() == 0)
		return;

	Scene::GetInstance().SetBBoxState(isBBoxOn);

	Invalidate();
}


void CCGWorkView::OnUpdateButtonBbox(CCmdUI *pCmdUI)
{
	pCmdUI->SetCheck(isBBoxOn);
}


void CCGWorkView::OnButtonVertNorm()
{
	if (Scene::GetInstance().GetModels().size() == 0)
		return;

	Scene::GetInstance().SetNormals(!(Scene::GetInstance().AreVertexNormalsOn()), 
		Scene::GetInstance().ArePolyNormalsOn());
	Invalidate();
}


void CCGWorkView::OnUpdateButtonVertNorm(CCmdUI *pCmdUI)
{
	if (Scene::GetInstance().GetModels().size() == 0)
		return;
	
	pCmdUI->SetCheck(Scene::GetInstance().AreVertexNormalsOn());
}


void CCGWorkView::OnButtonPolyNorm()
{
	if (Scene::GetInstance().GetModels().size() == 0)
		return;

	Scene::GetInstance().SetNormals(Scene::GetInstance().AreVertexNormalsOn(),
		!(Scene::GetInstance().ArePolyNormalsOn()));
	Invalidate();
}


void CCGWorkView::OnUpdateButtonPolyNorm(CCmdUI *pCmdUI)
{
	if (Scene::GetInstance().GetModels().size() == 0)
		return;

	pCmdUI->SetCheck(Scene::GetInstance().ArePolyNormalsOn());
}



void CCGWorkView::OnNormalCalculated()
{
	Scene::GetInstance().SetCalcNormalState(true);

	CWnd* pParent = GetParent();
	CMenu* pMenu = pParent->GetMenu();
	if (pMenu != NULL)
	{
		pMenu->CheckMenuItem(ID_NORMAL_CALCULATED, MF_CHECKED | MF_BYCOMMAND);
		pMenu->CheckMenuItem(ID_NORMAL_FROMFILE, MF_UNCHECKED | MF_BYCOMMAND);
	}
	Invalidate();
}

void CCGWorkView::OnNormalFromfile()
{
	Scene::GetInstance().SetCalcNormalState(false);

	CWnd* pParent = GetParent();
	CMenu* pMenu = pParent->GetMenu();
	if (pMenu != NULL)
	{
		pMenu->CheckMenuItem(ID_NORMAL_CALCULATED, MF_UNCHECKED | MF_BYCOMMAND);
		pMenu->CheckMenuItem(ID_NORMAL_FROMFILE, MF_CHECKED | MF_BYCOMMAND);
	}
	Invalidate();
}

void CCGWorkView::OnButtonColors()
{
	isCColorDialogOpen = true;
	if (m_colorDialog.DoModal() == IDOK)
	{
		if (Scene::GetInstance().GetModels().size() > 0)
		{
			Model* model = Scene::GetInstance().GetModels().back();
			COLORREF oldModelColor = RGB((BYTE)model->GetColor()[0], (BYTE)model->GetColor()[1],
				(BYTE)model->GetColor()[2]);
			Vec4 normColor(GetRValue(m_colorDialog.NormalColor), GetGValue(m_colorDialog.NormalColor),
				GetBValue(m_colorDialog.NormalColor));

			if (oldModelColor != m_colorDialog.WireframeColor)
			{
				Vec4 modelColor(GetRValue(m_colorDialog.WireframeColor), GetGValue(m_colorDialog.WireframeColor),
					GetBValue(m_colorDialog.WireframeColor));
				model->SetColor(modelColor);
				showGeos = false;
			}
			model->SetNormalColor(normColor);

			isCColorDialogOpen = false;
		}

		Invalidate();
	}
}

void CCGWorkView::OnOptionsMousesensitivity()
{
	if (m_sensitivityDialog.DoModal() == IDOK)
	{
		m_sensitivity = m_sensitivityDialog.GetSensitivity();
		Invalidate();
	}
}


void CCGWorkView::OnOptionsTessellationtolerance()
{
	
	if (m_resolutionDialog.DoModal() == IDOK)
	{
		// Save fineness in Scene
		Scene::GetInstance().SetFineNess(m_resolutionDialog.Resolution);

		if (Scene::GetInstance().GetModels().size() > 0)
		{
			// Delete previous models
			Scene::GetInstance().GetInstance().GetModels().back()->DeleteGeometries();
			// Load model from file
			PngWrapper p;
			CGSkelProcessIritDataFiles(m_strItdFileName, 1);
			// Build Building Box
			Scene::GetInstance().GetModels().back()->BuildBoundingBox();

			Invalidate();
		}
	}
}


void CCGWorkView::OnButtonView()
{
	m_nCoordSpace = ID_BUTTON_VIEW;
}


void CCGWorkView::OnUpdateButtonView(CCmdUI *pCmdUI)
{
	pCmdUI->SetCheck(m_nCoordSpace == ID_BUTTON_VIEW);
}


void CCGWorkView::OnButtonObject()
{
	m_nCoordSpace = ID_BUTTON_OBJECT;
}


void CCGWorkView::OnUpdateButtonObject(CCmdUI *pCmdUI)
{
	pCmdUI->SetCheck(m_nCoordSpace == ID_BUTTON_OBJECT);
}


void CCGWorkView::OnOptionsPerspectivecontrol()
{
	Camera* camera = Scene::GetInstance().GetCamera();
	PerspectiveParams pParams = camera->GetPerspectiveParameters();
#ifdef D_PERSP
	m_perspDialog.ProjPlane = pParams.Far;
#else
	m_perspDialog.ProjPlane = pParams.Near;
#endif
	m_perspDialog.FOV = pParams.FOV;

	if (m_perspDialog.DoModal() == IDOK)
	{
#ifdef D_PERSP
		camera->SetPerspective2(m_perspDialog.FOV, pParams.AspectRatio, pParams.Near, m_perspDialog.ProjPlane);
#else
		camera->SetPerspective(pParams.Left, pParams.Right, pParams.Top, pParams.Bottom,
			m_perspDialog.ProjPlane, pParams.Far);
#endif
		camera->SwitchProjection(m_bIsPerspective);

		Invalidate();
	}
}


void CCGWorkView::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags)
{
	if (Scene::GetInstance().GetModels().size() > 0)
	{
		if (Scene::GetInstance().ArePolyNormalsOn() ||
			Scene::GetInstance().AreVertexNormalsOn())
		{
			if (nChar == VK_DOWN)
			{
				normalSizeFactor -= 0.01;
				if (normalSizeFactor < 0.01)
					normalSizeFactor = 0.01;
			}
			if (nChar == VK_UP)
			{
				normalSizeFactor += 0.01;
			}
		}
	}

	if (nChar == 0x47) // G key
		showGeos = !showGeos;
	if (nChar == 0x45) // E key
		aroundEye = !aroundEye;
	if (nChar == 0x44) // D key
		m_colorDialog.IsDiscoMode = !m_colorDialog.IsDiscoMode;
	if (nChar == 0x57) // W key
		currentPolySelection = WIREFRAME;
	if (nChar == 0x53) // S key
		currentPolySelection = SOLID_SCREEN;
	if (nChar == 0x31) // 1 key
		m_nLightShading = ID_LIGHT_SHADING_FLAT;
	if (nChar == 0x32) // 2 key
		m_nLightShading = ID_LIGHT_SHADING_GOURAUD;
	if (nChar == 0x33) // 3 key
		m_nLightShading = ID_LIGHT_SHADING_PHONG;

	CView::OnKeyDown(nChar, nRepCnt, nFlags);
	Invalidate();
}


void CCGWorkView::OnRenderingWireframe()
{
	currentPolySelection = WIREFRAME;
	Invalidate();
}


void CCGWorkView::OnUpdateRenderingWireframe(CCmdUI *pCmdUI)
{
	pCmdUI->SetCheck(currentPolySelection == WIREFRAME);
}


void CCGWorkView::OnRenderingSolidonscreen()
{
	currentPolySelection = SOLID_SCREEN;
	Invalidate();
}


void CCGWorkView::OnUpdateRenderingSolidonscreen(CCmdUI *pCmdUI)
{
	pCmdUI->SetCheck(currentPolySelection == SOLID_SCREEN);
}


void CCGWorkView::OnRenderingSolidtofile()
{
	// Set dialog size variables to be the screen size
	if ((m_exportDialog.GetWidth() == 0) || (m_exportDialog.GetHeight() == 0))
	{
		m_exportDialog.SetWidth(m_WindowWidth);
		m_exportDialog.SetHeight(m_WindowHeight);
	}

	if (m_exportDialog.DoModal() == IDOK) {
		imgWidth = m_exportDialog.GetWidth();
		imgHeight = m_exportDialog.GetHeight();
		CT2A filename(m_exportDialog.GetFileName());

		imgToSave.SetWidth(imgWidth);
		imgToSave.SetHeight(imgHeight);
		imgToSave.SetFileName(filename);
		if (!imgToSave.InitWritePng()) return;

		saveToFile = true;
		Invalidate();
 	}
}


void CCGWorkView::OnBackgroundStretch()
{
	isBGStretch = true;
	Invalidate();
}


void CCGWorkView::OnUpdateBackgroundStretch(CCmdUI *pCmdUI)
{
	pCmdUI->SetCheck(isBGStretch);
}


void CCGWorkView::OnBackgroundRepeat()
{
	isBGStretch = false;
	Invalidate();
}


void CCGWorkView::OnUpdateBackgroundRepeat(CCmdUI *pCmdUI)
{
	pCmdUI->SetCheck(!isBGStretch);
}


void CCGWorkView::OnBackgroundOpen()
{
	TCHAR szFilters[] = _T("PNG Image Files (*.png)");

	CFileDialog dlg(TRUE, _T("png"), _T("*.png"), OFN_FILEMUSTEXIST | OFN_HIDEREADONLY, szFilters);

	if (dlg.DoModal() == IDOK) {
		BGFile = dlg.GetPathName();
		isBGFileOpen = true; 

		Invalidate();
	}
}


void CCGWorkView::OnLightSetmaterial()
{
	if (m_materialDialog.DoModal() == IDOK)
	{
		if (Scene::GetInstance().GetModels().size() > 0)
		{
			Material* material = Scene::GetInstance().GetModels().back()->GetMaterial();
			material->Ka = m_materialDialog.GetAmbientCoeffs();
			material->Kd = m_materialDialog.GetDiffuseCoeffs();
			material->Ks = m_materialDialog.GetShininessCoeffs();
			material->Specular = m_materialDialog.GetSpecularCoeff();
		}

		Invalidate();
	}
}


void CCGWorkView::OnButtonBculling()
{
	isBFCulling = !isBFCulling;
	Invalidate();
}


void CCGWorkView::OnButtonSil()
{
	showSil = !showSil;
	Invalidate();
}


void CCGWorkView::OnButtonInverseN()
{
	normalSign = -1.0 * normalSign;
	Invalidate();
}


void CCGWorkView::OnOptionsSilhouetteoptions()
{
	m_silDialog.Thickness = silThickness;
	m_silDialog.Color = silColor;
	INT_PTR result = m_silDialog.DoModal();

	if (result == IDOK)
	{
		silThickness = m_silDialog.Thickness;
		silColor = m_silDialog.Color;
		Invalidate();
	}
}


void CCGWorkView::OnBackgroundClear()
{
	isBGFileOpen = false;
	Invalidate();
}
