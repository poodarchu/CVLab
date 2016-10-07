
// MultimediaDlg.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "Multimedia.h"
#include "MultimediaDlg.h"
#include "afxdialogex.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

// CMultimediaDlg �Ի���

CMultimediaDlg::CMultimediaDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CMultimediaDlg::IDD, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
	m_pImgSrc = NULL;
	m_pImgSrc = NULL;
	m_nThreadNum = 1;
	m_pThreadParam = new ThreadParam[MAX_THREAD];
	srand(time(0));
}

void CMultimediaDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CMultimediaDlg, CDialogEx)
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_BUTTON_OPENFILE, &CMultimediaDlg::OnBnClickedButtonOpenfile)
	ON_BN_CLICKED(IDC_BUTTON_PROCESS, &CMultimediaDlg::OnBnClickedButtonProcess)
	ON_NOTIFY(NM_CUSTOMDRAW, IDC_SLIDER, &CMultimediaDlg::OnNMCustomdrawSlider)

	ON_MESSAGE(WM_NOISE, &CMultimediaDlg::OnNoiseThreadMsgReceived)
	ON_MESSAGE(WM_MEDIAN_FILTER, &CMultimediaDlg::OnMedianFilterThreadMsgReceived)
END_MESSAGE_MAP()


// CMultimediaDlg ��Ϣ�������

BOOL CMultimediaDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// ���ô˶Ի����ͼ�ꡣ��Ӧ�ó��������ڲ��ǶԻ���ʱ����ܽ��Զ�
	//  ִ�д˲���
	//SetIcon(m_hIcon, TRUE);			// ���ô�ͼ��
	//SetIcon(m_hIcon, FALSE);		// ����Сͼ��

	// TODO: �ڴ���Ӷ���ĳ�ʼ������
	CComboBox* cmb_thread = ((CComboBox*)GetDlgItem(IDC_COMBO_THREAD));
	
	cmb_thread->AddString("WIN���߳�");	
	cmb_thread->AddString("OpenMP");
	cmb_thread->AddString("CUDA");
	cmb_thread->SetCurSel(0);
	
	CComboBox* cmb_function = ((CComboBox*)GetDlgItem(IDC_COMBO_FUNCTION));
	cmb_function->AddString("��������");
	cmb_function->AddString("��ֵ�˲�");
	cmb_function->SetCurSel(0);

	CSliderCtrl *slider = (CSliderCtrl*)GetDlgItem(IDC_SLIDER);
	slider->SetRange(1, MAX_THREAD, TRUE);
	slider->SetPos(MAX_THREAD);
	
	AfxBeginThread((AFX_THREADPROC)&CMultimediaDlg::Update, this);
	return TRUE;  // ���ǽ��������õ��ؼ������򷵻� TRUE
}

// �����Ի��������С����ť������Ҫ����Ĵ���
//  �����Ƹ�ͼ�ꡣ����ʹ���ĵ�/��ͼģ�͵� MFC Ӧ�ó���
//  �⽫�ɿ���Զ���ɡ�

void CMultimediaDlg::OnPaint()
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
		if (m_pImgSrc !=NULL)
		{
			DrawPara p;
			p.height = m_pImgSrc->GetHeight();
			p.width =  m_pImgSrc->GetWidth();
			p.oriX = 30;
			p.oriY = 50;
			p.pDC = GetDC();
			p.pImgSrc = m_pImgSrc;
			ThreadDraw(&p);
		}
		
	}
}

UINT CMultimediaDlg::Update(void* p)
{
	while(1)
	{
		Sleep(200);
		CMultimediaDlg* dlg = (CMultimediaDlg*)p;
		if (dlg->m_pImgSrc !=NULL)
		{
			DrawPara p;
			p.height = dlg->m_pImgSrc->GetHeight();
			p.width =  dlg->m_pImgSrc->GetWidth();
			p.oriX = 30;
			p.oriY = 50;
			p.pDC = dlg->GetDC();
			p.pImgSrc = dlg->getImage();
			dlg->ThreadDraw(&p);
		}
	}
	return 0;
}

//���û��϶���С������ʱϵͳ���ô˺���ȡ�ù��
//��ʾ��
HCURSOR CMultimediaDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

void CMultimediaDlg::OnBnClickedButtonOpenfile()
{
	//ѡ���ļ��Ի���
	TCHAR  szFilter[] = _T("JPEG(*.jpg)|*.jpg|BMP(*.jpg)|*.bmp|TIFF (*.tif)|*.tif|All Files (*.*)|*.*||");
	CFileDialog fileOpenDlg(TRUE, NULL, NULL, OFN_HIDEREADONLY, szFilter);
	if (fileOpenDlg.DoModal() == IDOK)
	{
		CString imFilePath;
		VERIFY(imFilePath = fileOpenDlg.GetPathName());
		if (m_pImgSrc != NULL)
		{
			m_pImgSrc->Destroy();
			delete m_pImgSrc;
		}
		m_pImgSrc = new CImage();
		m_pImgSrc->Load(imFilePath);

		this->Invalidate();
	}
}

void CMultimediaDlg::ThreadDraw(DrawPara *p)
{
	CRect rect;
	GetClientRect(&rect);
	CDC     memDC;             // ���ڻ����ͼ���ڴ滭��  
	CBitmap memBitmap;         // ���ڻ����ͼ���ڴ滭��
	memDC.CreateCompatibleDC(p->pDC);  // ������ԭ���ʼ��ݵĻ���
	memBitmap.CreateCompatibleBitmap(p->pDC, p->width, p->height);  // ������ԭλͼ���ݵ��ڴ滭��
	memDC.SelectObject(&memBitmap);      // ���������뻭���Ĺ���
	memDC.FillSolidRect(rect,p->pDC->GetBkColor());

	// ��pImgSrc���������Ż����ڴ滭����
	p->pImgSrc->StretchBlt(memDC.m_hDC, 0, 0, p->width, p->height);
	
	// ���ѻ��õĻ������Ƶ������Ļ�������
	p->pDC->BitBlt(p->oriX, p->oriY, p->width, p->height, &memDC, 0, 0, SRCCOPY ); 
	memBitmap.DeleteObject();
	memDC.DeleteDC();
}

void CMultimediaDlg::ImageCopy(CImage* pImgSrc, CImage* pImgDrt)
{
    int MaxColors = pImgSrc->GetMaxColorTableEntries();
    RGBQUAD* ColorTab;
    ColorTab = new RGBQUAD[MaxColors];

    CDC *pDCsrc,*pDCdrc;
    if (!pImgDrt->IsNull())
    {
        pImgDrt->Destroy();
    }
    pImgDrt->Create(pImgSrc->GetWidth(),pImgSrc->GetHeight(),pImgSrc->GetBPP(),0);

    if (pImgSrc->IsIndexed())
    {
        pImgSrc->GetColorTable(0,MaxColors,ColorTab);
        pImgDrt->SetColorTable(0,MaxColors,ColorTab);
    }

    pDCsrc=CDC::FromHandle(pImgSrc->GetDC());
    pDCdrc=CDC::FromHandle(pImgDrt->GetDC());
    pDCdrc->BitBlt(0,0,pImgSrc->GetWidth(),pImgSrc->GetHeight(),pDCsrc,0,0,SRCCOPY);
    pImgSrc->ReleaseDC();
    pImgDrt->ReleaseDC();
    delete ColorTab;

}

BOOL CMultimediaDlg::OnEraseBkgnd(CDC* pDC)
{
	// TODO: �ڴ������Ϣ�����������/�����Ĭ��ֵ

	return TRUE;

}

void CMultimediaDlg::OnNMCustomdrawSlider(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMCUSTOMDRAW pNMCD = reinterpret_cast<LPNMCUSTOMDRAW>(pNMHDR);
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	
	CSliderCtrl *slider = (CSliderCtrl*)GetDlgItem(IDC_SLIDER);
	CString text("");
	m_nThreadNum = slider->GetPos();
	text.Format(_T("%d"), m_nThreadNum);
	GetDlgItem(IDC_STATIC_THREADNUM)->SetWindowText(text);
	*pResult = 0;
}

//ͼ����
void CMultimediaDlg::OnBnClickedButtonProcess()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	CComboBox* cmb_function = ((CComboBox*)GetDlgItem(IDC_COMBO_FUNCTION));
	int func = cmb_function->GetCurSel();
	switch (func)
	{
	case 0:  //��������
		AddNoise();
		break;
	case 1: //����Ӧ��ֵ�˲�
		MedianFilter();
		break;
	default: 
		break;
	}
}

void CMultimediaDlg::AddNoise()
{
	CComboBox* cmb_thread = ((CComboBox*)GetDlgItem(IDC_COMBO_THREAD));
	int thread = cmb_thread->GetCurSel();
	startTime = CTime::GetTickCount();
	switch (thread)
	{
	case 0://win���߳�
		{
			int subLength = m_pImgSrc->GetWidth() * m_pImgSrc->GetHeight() / m_nThreadNum;
			for (int i = 0; i < m_nThreadNum; ++i)
			{
				m_pThreadParam[i].startIndex = i * subLength;
				m_pThreadParam[i].endIndex = i != m_nThreadNum -1 ? 
					 (i + 1) * subLength - 1 :m_pImgSrc->GetWidth() * m_pImgSrc->GetHeight() - 1;
				m_pThreadParam[i].src = m_pImgSrc;
				AfxBeginThread((AFX_THREADPROC)&ImageProcess::addNoise, &m_pThreadParam[i]);
			}
		}

		break;

	case 1://openmp
		{
			int subLength = m_pImgSrc->GetWidth() * m_pImgSrc->GetHeight() / m_nThreadNum;

			#pragma omp parallel for num_threads(m_nThreadNum)
			for (int i = 0; i < m_nThreadNum; ++i)
			{
				m_pThreadParam[i].startIndex = i * subLength;
				m_pThreadParam[i].endIndex = i != m_nThreadNum -1 ? 
					 (i + 1) * subLength - 1 :m_pImgSrc->GetWidth() * m_pImgSrc->GetHeight() - 1;
				m_pThreadParam[i].src = m_pImgSrc;
				ImageProcess::addNoise(&m_pThreadParam[i]);
			}


			CTime endTime = CTime::GetTickCount();
			CString timeStr;
			timeStr.Format("��ʱ:%dms", endTime - startTime);
			AfxMessageBox(timeStr);
		}

		break;

	case 2://cuda
		break;
	}
}

void CMultimediaDlg::MedianFilter()
{
	CComboBox* cmb_thread = ((CComboBox*)GetDlgItem(IDC_COMBO_THREAD));
	int thread = cmb_thread->GetCurSel();
	startTime = CTime::GetTickCount();
	m_nThreadNum;
	switch (thread)
	{
	case 0://win���߳�
		{
			int subLength = m_pImgSrc->GetWidth() * m_pImgSrc->GetHeight() / m_nThreadNum;
			int h = m_pImgSrc->GetHeight() / m_nThreadNum;
			int w = m_pImgSrc->GetWidth()/ m_nThreadNum;
			for (int i = 0; i < m_nThreadNum; ++i)
			{
				m_pThreadParam[i].startIndex = i * subLength;
				m_pThreadParam[i].endIndex = i != m_nThreadNum -1 ? 
					 (i + 1) * subLength - 1 :m_pImgSrc->GetWidth() * m_pImgSrc->GetHeight() - 1;
				m_pThreadParam[i].maxSpan = MAX_SPAN;
				m_pThreadParam[i].src = m_pImgSrc;
				AfxBeginThread((AFX_THREADPROC)&ImageProcess::medianFilter, &m_pThreadParam[i]);
			}

		}

		break;

	case 1://openmp
		{
			int subLength = m_pImgSrc->GetWidth() * m_pImgSrc->GetHeight() / m_nThreadNum;

			#pragma omp parallel for num_threads(m_nThreadNum)
			for (int i = 0; i < m_nThreadNum; ++i)
			{
				m_pThreadParam[i].startIndex = i * subLength;
				m_pThreadParam[i].endIndex = i != m_nThreadNum -1 ? 
					 (i + 1) * subLength - 1 :m_pImgSrc->GetWidth() * m_pImgSrc->GetHeight() - 1;
				m_pThreadParam[i].maxSpan = MAX_SPAN;
				m_pThreadParam[i].src = m_pImgSrc;
				ImageProcess::medianFilter(&m_pThreadParam[i]);
			}

			CTime endTime = CTime::GetTickCount();
			CString timeStr;
			timeStr.Format("��ʱ:%dms", endTime - startTime);
			AfxMessageBox(timeStr);
		}

		break;

	case 2://cuda
		break;
	}
}


LRESULT CMultimediaDlg::OnMedianFilterThreadMsgReceived(WPARAM wParam, LPARAM lParam)
{
	static int tempCount = 0;
	if ((int)wParam == 1)
	{
		// �������̶߳�������ֵ1����ȫ������~��ʾʱ��
		if (m_nThreadNum == ++tempCount)
		{
			CTime endTime = CTime::GetTickCount();
			CString timeStr;
			timeStr.Format("��ʱ:%dms", endTime - startTime);
			tempCount = 0;

			// ��ʾ��Ϣ����
			AfxMessageBox(timeStr);
		}
	}

	return 0;
}

LRESULT CMultimediaDlg::OnNoiseThreadMsgReceived(WPARAM wParam, LPARAM lParam)
{
	static int tempCount = 0;
	if ((int)wParam == 1)
		tempCount++;
	if (m_nThreadNum == tempCount)
	{
		CTime endTime = CTime::GetTickCount();
		CString timeStr;
		timeStr.Format("��ʱ:%dms", endTime - startTime);
		tempCount = 0;

		AfxMessageBox(timeStr);
	}
	return 0;
}