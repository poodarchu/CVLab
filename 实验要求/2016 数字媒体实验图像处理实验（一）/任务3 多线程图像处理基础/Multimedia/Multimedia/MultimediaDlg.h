
// MultimediaDlg.h : ͷ�ļ�
//

#pragma once
#include "ImageProcess.h"

//�����߳���
#define MAX_THREAD 8 
//ģ�����ĵ���Ե��������
#define MAX_SPAN 15 

struct DrawPara
{
	CImage* pImgSrc;
	CDC* pDC;
	int oriX;
	int oriY;
	int width;
	int height;
};

// CMultimediaDlg �Ի���
class CMultimediaDlg : public CDialogEx
{
// ����
public:
	CMultimediaDlg(CWnd* pParent = NULL);	// ��׼���캯��

// �Ի�������
	enum { IDD = IDD_MULTIMEDIA_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV ֧��
	void ThreadDraw(DrawPara *p);//����
	static UINT Update(void* p);
	void MedianFilter();//��ֵ�˲�
	void AddNoise();//��ӽ�������
	void ImageCopy(CImage* pImgSrc, CImage* pImgDrt);
	afx_msg LRESULT OnMedianFilterThreadMsgReceived(WPARAM wParam, LPARAM lParam);// ��ֵ�˲��߳���Ϣ���ܺ���
	afx_msg LRESULT OnNoiseThreadMsgReceived(WPARAM wParam, LPARAM lParam); //���������߳���Ϣ���ܺ���

	afx_msg void OnBnClickedButtonOpenfile();
	afx_msg void OnBnClickedButtonProcess(); //ͼ������ں���
	afx_msg void OnNMCustomdrawSlider(NMHDR *pNMHDR, LRESULT *pResult);
	BOOL OnEraseBkgnd(CDC* pDC);
	CImage* getImage(){return m_pImgSrc;}
	

// ʵ��
protected:
	HICON m_hIcon;
	CImage *m_pImgSrc;//ͼ��
	int m_nThreadNum;//�߳���
	ThreadParam* m_pThreadParam;
	CTime startTime;

	// ���ɵ���Ϣӳ�亯��
	virtual BOOL OnInitDialog();
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()

};
