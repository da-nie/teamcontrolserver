#ifndef CFRAMEWND_MAIN_H
#define CFRAMEWND_MAIN_H

#include "stdafx.h"
#include "cdocument_main.h"

class CFrameWnd_Main:public CFrameWnd
{
 protected:
  //-Переменные класса-------------------------------------------------------
  NOTIFYICONDATA NotifyIconData;//иконка
  //-Функции класса----------------------------------------------------------
  //-Прочее------------------------------------------------------------------
 public:
  //-Конструктор класса------------------------------------------------------
  CFrameWnd_Main(void);
  //-Деструктор класса-------------------------------------------------------
  ~CFrameWnd_Main();
  //-Переменные класса-------------------------------------------------------
  //-Функции класса----------------------------------------------------------
  afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
  afx_msg void OnTimer(UINT nIDEvent);
  afx_msg void OnDestroy(void);
  afx_msg void OnSysCommand(UINT nID,LPARAM lParam);//изменение параметров окна
  //-Функции обработки сообщений класса--------------------------------------
  DECLARE_MESSAGE_MAP()
  afx_msg void OnSystemTrayIconMessage(WPARAM wParam,LPARAM lParam);//обработка сообщений трея
  //-Прочее------------------------------------------------------------------
  DECLARE_DYNCREATE(CFrameWnd_Main) 
};
#endif