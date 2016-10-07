
// ExperimentImgDlg.h : ͷ�ļ�
//

#pragma once
#include "afxwin.h"

struct DrawPara
{
	CString filepath;
	CDC* pDC;
	int oriX;
	int oriY;
	int width;
	int height;
};

// CExperimentImgDlg �Ի���
class CExperimentImgDlg : public CDialogEx
{
// ����
public:
	CExperimentImgDlg(CWnd* pParent = NULL);	// ��׼���캯��
	int ThreadDraw(DrawPara *p);

// �Ի�������
	enum { IDD = IDD_EXPERIMENTIMG_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV ֧��


// ʵ��
protected:
	HICON m_hIcon;

	// ���ɵ���Ϣӳ�亯��
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedButtonOpen();
	CEdit mEditInfo;
};
