#ifndef CFRAMEWND_MAIN_H
#define CFRAMEWND_MAIN_H

#include "stdafx.h"
#include "cdocument_main.h"

class CFrameWnd_Main:public CFrameWnd
{
 protected:
  //-���������� ������-------------------------------------------------------
  NOTIFYICONDATA NotifyIconData;//������
  //-������� ������----------------------------------------------------------
  //-������------------------------------------------------------------------
 public:
  //-����������� ������------------------------------------------------------
  CFrameWnd_Main(void);
  //-���������� ������-------------------------------------------------------
  ~CFrameWnd_Main();
  //-���������� ������-------------------------------------------------------
  //-������� ������----------------------------------------------------------
  afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
  afx_msg void OnTimer(UINT nIDEvent);
  afx_msg void OnDestroy(void);
  afx_msg void OnSysCommand(UINT nID,LPARAM lParam);//��������� ���������� ����
  //-������� ��������� ��������� ������--------------------------------------
  DECLARE_MESSAGE_MAP()
  afx_msg void OnSystemTrayIconMessage(WPARAM wParam,LPARAM lParam);//��������� ��������� ����
  //-������------------------------------------------------------------------
  DECLARE_DYNCREATE(CFrameWnd_Main) 
};
#endif