
// Multimedia.h : PROJECT_NAME Ӧ�ó������ͷ�ļ�
//

#pragma once

#ifndef __AFXWIN_H__
	#error "�ڰ������ļ�֮ǰ������stdafx.h�������� PCH �ļ�"
#endif

#include "resource.h"		// ������


// CMultimediaApp:
// �йش����ʵ�֣������ Multimedia.cpp
//

class CMultimediaApp : public CWinApp
{
public:
	CMultimediaApp();

// ��д
public:
	virtual BOOL InitInstance();

// ʵ��

	DECLARE_MESSAGE_MAP()
};

extern CMultimediaApp theApp;