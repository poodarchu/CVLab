
// ExperimentImgDlg.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "ExperimentImg.h"
#include "ExperimentImgDlg.h"
#include "afxdialogex.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif





// ����Ӧ�ó��򡰹��ڡ��˵���� CAboutDlg �Ի���

class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();

// �Ի�������
	enum { IDD = IDD_ABOUTBOX };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

// ʵ��
protected:
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialogEx(CAboutDlg::IDD)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialogEx)
END_MESSAGE_MAP()


// CExperimentImgDlg �Ի���




CExperimentImgDlg::CExperimentImgDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CExperimentImgDlg::IDD, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CExperimentImgDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_EDIT_INFO, mEditInfo);
}

BEGIN_MESSAGE_MAP(CExperimentImgDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_BUTTON_OPEN, &CExperimentImgDlg::OnBnClickedButtonOpen)
END_MESSAGE_MAP()


// CExperimentImgDlg ��Ϣ�������

BOOL CExperimentImgDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// ��������...���˵�����ӵ�ϵͳ�˵��С�

	// IDM_ABOUTBOX ������ϵͳ���Χ�ڡ�
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != NULL)
	{
		BOOL bNameValid;
		CString strAboutMenu;
		bNameValid = strAboutMenu.LoadString(IDS_ABOUTBOX);
		ASSERT(bNameValid);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}

	// ���ô˶Ի����ͼ�ꡣ��Ӧ�ó��������ڲ��ǶԻ���ʱ����ܽ��Զ�
	//  ִ�д˲���
	SetIcon(m_hIcon, TRUE);			// ���ô�ͼ��
	SetIcon(m_hIcon, FALSE);		// ����Сͼ��

	// TODO: �ڴ���Ӷ���ĳ�ʼ������

	return TRUE;  // ���ǽ��������õ��ؼ������򷵻� TRUE
}

void CExperimentImgDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialogEx::OnSysCommand(nID, lParam);
	}
}

// �����Ի��������С����ť������Ҫ����Ĵ���
//  �����Ƹ�ͼ�ꡣ����ʹ���ĵ�/��ͼģ�͵� MFC Ӧ�ó���
//  �⽫�ɿ���Զ���ɡ�

void CExperimentImgDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // ���ڻ��Ƶ��豸������

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// ʹͼ���ڹ����������о���
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// ����ͼ��
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialogEx::OnPaint();
	}
}

//���û��϶���С������ʱϵͳ���ô˺���ȡ�ù��
//��ʾ��
HCURSOR CExperimentImgDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}



void CExperimentImgDlg::OnBnClickedButtonOpen()
{
	//ѡ���ļ��Ի���
	TCHAR  szFilter[] = _T("JPEG(*.jpg)|*.jpg|BMP(*.jpg)|*.bmp|TIFF (*.tif)|*.tif|All Files (*.*)|*.*||");
	CString filePath("");
	CFileDialog fileOpenDlg(TRUE, NULL, NULL,OFN_HIDEREADONLY,szFilter);
	if (fileOpenDlg.DoModal() == IDOK) 
	{
		VERIFY(filePath = fileOpenDlg.GetPathName());
	}
	//CImage�����ͼ��
	CString strFilePath(filePath);
	CImage imSrc;
	imSrc.Load(strFilePath);
	//�ı��ؼ���ʾ�ļ���Ϣ
	mEditInfo.SetWindowTextA(strFilePath + "\n");
	

	CImage imThumb1;
	CDC* pDC = GetDC();
	imThumb1.Load(strFilePath);

	////ֱ�ӻ�������ͼ
	//if(imSrc.GetWidth()>imSrc.GetHeight())
	//	imThumb1.StretchBlt(pDC->m_hDC,10,10,150,imSrc.GetHeight()*150/imSrc.GetWidth());
	//else
	//	imThumb1.StretchBlt(pDC->m_hDC,10,10,imSrc.GetWidth()*150/imSrc.GetHeight(),150);
	//˫�������
	CRect   rect;              // �洢�ͻ�����С  
	CDC     MemDC;             // ���ڻ����ͼ���ڴ�DC  
	CBitmap MemBitmap;        // �ڴ�DC�г�����ʱͼ���λͼ 
	// ��ȡ��ǰ�ͻ�����С  
	GetClientRect( &rect );   
      
	// �������ݵ�ǰDC���ڴ�DC  
	MemDC.CreateCompatibleDC( pDC );  
  
	// ��������λͼ  
	//MemBitmap.CreateCompatibleBitmap( pDC, rect.Width(), rect.Height() );  
	MemBitmap.CreateCompatibleBitmap( pDC, 150, 150 );  
	// ������λͼѡ�뵽�ڴ滺��DC���൱�ڻ���������ѡ���ڴ�DC  
	MemDC.SelectObject( &MemBitmap );   
  
	 // ʹ��ԭ����ɫ����ڴ滺��DC  
	MemDC.FillSolidRect( rect, pDC->GetBkColor() );   

	// ����ͼ�����еġ�pDC->��ȫ����Ϊ��MemDC.�����ɡ�
	if(imSrc.GetWidth()>imSrc.GetHeight())
		imThumb1.StretchBlt(MemDC.m_hDC,0,(150-imSrc.GetHeight()*150/imSrc.GetWidth())/2,150,imSrc.GetHeight()*150/imSrc.GetWidth());
	else
		imThumb1.StretchBlt(MemDC.m_hDC,(150-imSrc.GetWidth()*150/imSrc.GetHeight())/2,0,imSrc.GetWidth()*150/imSrc.GetHeight(),150);
	//���ڴ滺��DC�е����ݸ��Ƶ���ǰDC
	//pDC->BitBlt( 0,0,rect.Width(),rect.Height(),&MemDC,0,0,SRCCOPY ); 
	pDC->BitBlt(10,10,150,150,&MemDC,0,0,SRCCOPY ); 

	DrawPara param;
	param.filepath = strFilePath;
	param.oriX = 10;
	param.oriY = 170;
	param.width = 150;
	param.height = 150;
	param.pDC = pDC;

	

}

//int ThreadDraw(&CExperimentImgDlg::ThreadDraw,DrawPara *param)
//{
//
//
//	 CImage imSrc;
//	 imSrc.Load(param->filepath);
//	 	//����Ŀ��ͼ�񣨻���������С
//	int dstWidth, dstHeigh;
//	if(imSrc.GetWidth() > imSrc.GetHeight())
//	{
//		dstWidth = param->width;
//		dstHeigh = imSrc.GetHeight()*param->width/imSrc.GetWidth();
//	}else
//	{
//		dstHeigh = param->height;
//		dstWidth = imSrc.GetWidth()*param->height/imSrc.GetHeight();
//	}
//	 	CDC     MemDC;             // ���ڻ����ͼ���ڴ�DC  
//	CBitmap MemBitmap;        // �ڴ�DC�г�����ʱͼ���λͼ 
//	MemDC.CreateCompatibleDC(param->pDC); 
//	MemBitmap.CreateCompatibleBitmap( param->pDC, param->width, param->height ); 
//	MemDC.SelectObject( &MemBitmap ); 
//
//	if(imSrc.GetWidth()>imSrc.GetHeight())
//		imSrc.StretchBlt(MemDC.m_hDC,0,(param->height-imSrc.GetHeight()*param->width/imSrc.GetWidth())/2,param->width,imSrc.GetHeight()*param->width/imSrc.GetWidth());
//	else
//		imSrc.StretchBlt(MemDC.m_hDC,(param->width-imSrc.GetWidth()*param->height/imSrc.GetHeight())/2,0,imSrc.GetWidth()*param->height/imSrc.GetHeight(),param->height);
//	
//	param->pDC->BitBlt(param->oriX,param->oriY,param->width,param->height,&MemDC,0,0,SRCCOPY ); 
//	return 1;
//
//}
