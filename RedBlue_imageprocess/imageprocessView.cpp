// imageprocessView.cpp : implementation of the CImageprocessView class
//

#include "stdafx.h"
#include "imageprocess.h"

#include "imageprocessDoc.h"
#include "imageprocessView.h"

#include<iostream>
#include<fstream>
using namespace std;

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CImageprocessView

IMPLEMENT_DYNCREATE(CImageprocessView, CView)

BEGIN_MESSAGE_MAP(CImageprocessView, CView)
	//{{AFX_MSG_MAP(CImageprocessView)
	ON_COMMAND(ID_OPEN, OnOpen)
	ON_COMMAND(ID_SAVE_BMP, OnSaveBmp)
	ON_COMMAND(ID_MENUITEM32776, OnForZoom)
	ON_COMMAND(ID_MENUITEM32777, OnNearestInterZoom)
	ON_COMMAND(ID_MENUITEM32779, OnNearestInterZoom3)
	ON_COMMAND(ID_MENUITEM32778, OnBilinearInterZoom)
	//}}AFX_MSG_MAP
	// Standard printing commands
	ON_COMMAND(ID_FILE_PRINT, CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_DIRECT, CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_PREVIEW, CView::OnFilePrintPreview)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CImageprocessView construction/destruction

CImageprocessView::CImageprocessView()
{
	// TODO: add construction code here

}

CImageprocessView::~CImageprocessView()
{
}

BOOL CImageprocessView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: Modify the Window class or styles here by modifying
	//  the CREATESTRUCT cs

	return CView::PreCreateWindow(cs);
}

/////////////////////////////////////////////////////////////////////////////
// CImageprocessView drawing

void CImageprocessView::OnDraw(CDC* pDC)
{
	CImageprocessDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);			
	
	mybmp.Draw(pDC,CPoint(0,0),sizeDibDisplay);

//	CSize sizeNew(sizeDibDisplay.cx/2,sizeDibDisplay.cy/2);

	zoombmp.Draw(pDC,CPoint(sizeDibDisplay.cx,0),sizeDibDisplay2);



}

/////////////////////////////////////////////////////////////////////////////
// CImageprocessView printing

BOOL CImageprocessView::OnPreparePrinting(CPrintInfo* pInfo)
{
	// default preparation
	return DoPreparePrinting(pInfo);
}

void CImageprocessView::OnBeginPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: add extra initialization before printing
}

void CImageprocessView::OnEndPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: add cleanup after printing
}

/////////////////////////////////////////////////////////////////////////////
// CImageprocessView diagnostics

#ifdef _DEBUG
void CImageprocessView::AssertValid() const
{
	CView::AssertValid();
}

void CImageprocessView::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}

CImageprocessDoc* CImageprocessView::GetDocument() // non-debug version is inline
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CImageprocessDoc)));
	return (CImageprocessDoc*)m_pDocument;
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CImageprocessView message handlers

void CImageprocessView::OnOpen() 
{

	CFileDialog FileDlg(TRUE, _T("*.bmp"), "",OFN_FILEMUSTEXIST|OFN_PATHMUSTEXIST|OFN_HIDEREADONLY,"image files (*.bmp; *.jpg) |*.bmp;*.jpg|AVI files (*.avi) |*.avi|All Files (*.*)|*.*||",NULL);
	char title[]= {"Open Image"};
	FileDlg.m_ofn.lpstrTitle= title;

	CFile file ;
	if( FileDlg.DoModal() == IDOK ) 
	{
		if(!file.Open(FileDlg.GetPathName(), CFile::modeRead))
		{
			AfxMessageBox("cannot open the file");
			return;
		}
		if(!mybmp.Read(&file))
		{
			AfxMessageBox("cannot read the file");
			return;
		}
	}
		
	if (mybmp.m_lpBMIH->biCompression != BI_RGB)
	{
		AfxMessageBox("Can not read compressed file.");
		return ;
	}
	sizeDibDisplay=mybmp.GetDimensions();
	Invalidate();
}




void CImageprocessView::OnSaveBmp() 
{
	CFileDialog FileDlg(FALSE, _T("*.bmp"), "",OFN_FILEMUSTEXIST|OFN_PATHMUSTEXIST|OFN_HIDEREADONLY,"image files (*.bmp) |*.bmp|AVI files (*.avi) |*.avi|All Files (*.*)|*.*||",NULL);
	char title[]= {"Open Image"};
	FileDlg.m_ofn.lpstrTitle= title;
	CFile file ;
	if( FileDlg.DoModal() == IDOK ) 
	{
		if(!file.Open(FileDlg.GetPathName(), CFile::modeWrite | CFile::modeCreate | CFile::typeBinary))
		{
			AfxMessageBox("cannot open the file");
			return;
		}
		else
	      	mybmp.Write(&file);
	}

   	
}


void CImageprocessView::OnForZoom()
{
	sizeDibDisplay = mybmp.GetDimensions();//原图像尺度
	long lWidth = sizeDibDisplay.cx;
	long lHeight = sizeDibDisplay.cy;

	float fXZoomRatio = 3.0;//放大3倍
	float fYZoomRatio = 3.0;

	long lNewWidth = sizeDibDisplay.cx*fXZoomRatio;//新图像尺度
	long lNewHeight = sizeDibDisplay.cy*fYZoomRatio;
	sizeDibDisplay2.cx = lNewWidth;
	sizeDibDisplay2.cy = lNewHeight;
	zoombmp.CreateCDib(sizeDibDisplay2,mybmp.m_lpBMIH->biBitCount);

	bool * flag = new bool[lNewWidth*lNewHeight];
	for(int i =1; i< lNewWidth*lNewHeight; i++)
		flag[i] = false;

	for(int y = 0; y < lHeight; y++)
	for(int x = 0; x < lWidth; x++)
	{
		RGBQUAD color = mybmp.GetPixel(x,y);

		//计算点在新图像上的位置
		int x0 = x * fXZoomRatio + 0.5;
		int y0 = y * fYZoomRatio + 0.5;

		for(int m1 = -1; m1 <= 1; m1++)//x方向
		for(int m2 = -1; m2 <= 1; m2++)//y方向
		{
			if((x0 + m1 >= 0) && (x0 +m1 < lNewWidth) && (y0 + m2 >= 0) && (y0+m2 < lNewHeight))
			{
				zoombmp.WritePixel(x0 + m1, y0 + m2,color);
				flag[y0*lNewWidth + x0] = true;
			}
		}
	}
	Invalidate();
}

void CImageprocessView::OnNearestInterZoom() 
{
	sizeDibDisplay = mybmp.GetDimensions();
	long lWidth = sizeDibDisplay.cx;
	long lHeight = sizeDibDisplay.cy;

	float fXZoomRatio = 4.0;//放大4倍
	float fYZoomRatio = 4.0;

	long lNewWidth = sizeDibDisplay.cx*fXZoomRatio;//新图像尺度
	long lNewHeight = sizeDibDisplay.cy*fYZoomRatio;
	sizeDibDisplay2.cx = lNewWidth;
	sizeDibDisplay2.cy = lNewHeight;
	zoombmp.CreateCDib(sizeDibDisplay2,mybmp.m_lpBMIH->biBitCount);

	for(int y = 0; y < lNewHeight; y++)
	for(int x = 0; x < lNewWidth; x++)
	{
		RGBQUAD color;

		//计算新点在原图像上的位置(最近邻插值法)
		int x0 = (long) (x / fXZoomRatio + 0.5);
		int y0 = (long) (y / fYZoomRatio + 0.5);

		if((x0 >= 0) && (x0 < sizeDibDisplay.cx) && (y0 >= 0) && (y0 < sizeDibDisplay.cy))
		{
			color = mybmp.GetPixel(x0,y0);
		}
		else
		{
			color.rgbGreen = 255;
			color.rgbRed   = 255;
			color.rgbBlue  = 255;
		}
		zoombmp.WritePixel(x, y,color);
	}
	Invalidate();
}

void CImageprocessView::OnNearestInterZoom3() 
{
	sizeDibDisplay = mybmp.GetDimensions();
	long lWidth = sizeDibDisplay.cx;
	long lHeight = sizeDibDisplay.cy;

	float fXZoomRatio = 3.0;//放大3倍
	float fYZoomRatio = 3.0;

	long lNewWidth = sizeDibDisplay.cx*fXZoomRatio;//新图像尺度
	long lNewHeight = sizeDibDisplay.cy*fYZoomRatio;
	sizeDibDisplay2.cx = lNewWidth;
	sizeDibDisplay2.cy = lNewHeight;
	zoombmp.CreateCDib(sizeDibDisplay2,mybmp.m_lpBMIH->biBitCount);

	for(int y = 0; y < lNewHeight; y++)
	for(int x = 0; x < lNewWidth; x++)
	{
		RGBQUAD color;

		//计算新点在原图像上的位置(最近邻插值法)
		int x0 = (long) (x / fXZoomRatio + 0.5);
		int y0 = (long) (y / fYZoomRatio + 0.5);

		if((x0 >= 0) && (x0 < sizeDibDisplay.cx) && (y0 >= 0) && (y0 < sizeDibDisplay.cy))
		{
			color = mybmp.GetPixel(x0,y0);
		}
		else
		{
			color.rgbGreen = 255;
			color.rgbRed   = 255;
			color.rgbBlue  = 255;
		}
		zoombmp.WritePixel(x, y,color);
	}
	Invalidate();	
}

void CImageprocessView::OnBilinearInterZoom() 
{
	sizeDibDisplay = mybmp.GetDimensions();
	long lWidth = sizeDibDisplay.cx;
	long lHeight = sizeDibDisplay.cy;

	float fXZoomRatio = 3.0;//放大3倍
	float fYZoomRatio = 3.0;

	long lNewWidth = sizeDibDisplay.cx*fXZoomRatio;//新图像尺度
	long lNewHeight = sizeDibDisplay.cy*fYZoomRatio;
	sizeDibDisplay2.cx = lNewWidth;
	sizeDibDisplay2.cy = lNewHeight;
	zoombmp.CreateCDib(sizeDibDisplay2,mybmp.m_lpBMIH->biBitCount);

	for(int y = 0; y < lNewHeight; y++)
	for(int x = 0; x < lNewWidth; x++)
	{
		RGBQUAD color;

		//计算新点在原图像上的位置(双线性插值法)
		float cx = x / fXZoomRatio;
		float cy = y / fYZoomRatio;

		if( ((int)(cx)-1) >= 0 && ((int)(cx)+1) < sizeDibDisplay.cx && ((int)(cy)-1) >= 0 && ((int)(cy)+1) < sizeDibDisplay.cy)
		{
			//色彩公式:f(i+u,j+v) = (1-u)(1-v)f(i,j) + (1-u)vf(i,j+1) + u(1-v)f(i+1,j) + uvf(i+1,j+1)
			float u = cx - (int)cx;
			float v = cy - (int)cy;
			int i = (int)cx;
			int j = (int)cy;
			
			int red,green,blue;
			red = (1-u)*(1-v)*mybmp.GetPixel(i,j).rgbRed + (1-u)*v*mybmp.GetPixel(i,j+1).rgbRed
					+ u*(1-v)*mybmp.GetPixel(i+1,j).rgbRed + u*v*mybmp.GetPixel(i+1,j+1).rgbRed;
			green = (1-u)*(1-v)*mybmp.GetPixel(i,j).rgbGreen + (1-u)*v*mybmp.GetPixel(i,j+1).rgbGreen
					+ u*(1-v)*mybmp.GetPixel(i+1,j).rgbGreen + u*v*mybmp.GetPixel(i+1,j+1).rgbGreen;
			blue = (1-u)*(1-v)*mybmp.GetPixel(i,j).rgbBlue + (1-u)*v*mybmp.GetPixel(i,j+1).rgbBlue
					+ u*(1-v)*mybmp.GetPixel(i+1,j).rgbBlue + u*v*mybmp.GetPixel(i+1,j+1).rgbBlue;

			color.rgbRed = red;
			color.rgbGreen = green;
			color.rgbBlue = blue;

		}
		else
		{
			color.rgbGreen = 255;
			color.rgbRed   = 255;
			color.rgbBlue  = 255;
		}
		zoombmp.WritePixel(x, y,color);
	}
	Invalidate();		
}
