#include "cframewnd_main.h"

//-Функции обработки сообщений класса----------------------------------------
BEGIN_MESSAGE_MAP(CFrameWnd_Main,CFrameWnd)
 ON_WM_CREATE()
 ON_WM_TIMER()
 ON_WM_DESTROY()
 ON_WM_SYSCOMMAND()
 ON_MESSAGE(WM_SYSTEM_TRAY_ICON,OnSystemTrayIconMessage)
END_MESSAGE_MAP()
//-Конструктор класса--------------------------------------------------------
CFrameWnd_Main::CFrameWnd_Main(void)
{
 NotifyIconData.hIcon=NULL;
}
//-Деструктор класса---------------------------------------------------------
CFrameWnd_Main::~CFrameWnd_Main()
{
}
//-Функции класса------------------------------------------------------------

//----------------------------------------------------------------------------------------------------
//создание рамки
//----------------------------------------------------------------------------------------------------
int CFrameWnd_Main::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
 //создаём иконку в системном лотке
 NotifyIconData.cbSize=sizeof(NOTIFYICONDATA);
 NotifyIconData.hWnd=m_hWnd;
 NotifyIconData.uID=IDI_ICON_MAIN;
 NotifyIconData.uFlags=NIF_ICON|NIF_MESSAGE|NIF_TIP;
 NotifyIconData.uCallbackMessage=WM_SYSTEM_TRAY_ICON;
 NotifyIconData.hIcon=(HICON)LoadImage(AfxGetInstanceHandle(),(char*)IDI_ICON_MAIN,IMAGE_ICON,32,32,LR_DEFAULTCOLOR);
 strcpy(NotifyIconData.szTip,"Team Control сервер");
 Shell_NotifyIcon(NIM_ADD,&NotifyIconData);
 SetIcon(NotifyIconData.hIcon,TRUE);  
 return(CFrameWnd::OnCreate(lpCreateStruct));
}
//----------------------------------------------------------------------------------------------------
//уточтожение рамки
//----------------------------------------------------------------------------------------------------
afx_msg void CFrameWnd_Main::OnDestroy(void)
{
 Shell_NotifyIcon(NIM_DELETE,&NotifyIconData);
 if (NotifyIconData.hIcon!=NULL) DeleteObject(NotifyIconData.hIcon);
 NotifyIconData.hIcon=NULL;
}
//----------------------------------------------------------------------------------------------------
//событие таймера
//----------------------------------------------------------------------------------------------------
afx_msg void CFrameWnd_Main::OnTimer(UINT nIDEvent)
{
 if (nIDEvent==ID_TIMER_FRAMEWND_MAIN)
 { 
 }
 CFrameWnd::OnTimer(nIDEvent);
}
//----------------------------------------------------------------------------------------------------
//изменение параметров окна
//----------------------------------------------------------------------------------------------------
afx_msg void CFrameWnd_Main::OnSysCommand(UINT nID,LPARAM lParam)
{
 if (nID==SC_MINIMIZE || nID==SC_CLOSE)//свернём окно в системный лоток
 {
  ShowWindow(SW_HIDE);
  return;
 }
 CFrameWnd::OnSysCommand(nID,lParam);
}

//-Функции обработки сообщений класса----------------------------------------
//----------------------------------------------------------------------------------------------------
//обработка сообщений трея
//----------------------------------------------------------------------------------------------------
afx_msg void CFrameWnd_Main::OnSystemTrayIconMessage(WPARAM wParam,LPARAM lParam)
{
 if (wParam!=IDI_ICON_MAIN) return;//это не наша иконка в лотке 
 if (lParam==WM_LBUTTONUP)//нажали и отпустили левую кнопку мыши
 {  
  SetForegroundWindow();
  ShowWindow(SW_SHOW);
 }
}
//-Прочее--------------------------------------------------------------------
IMPLEMENT_DYNCREATE(CFrameWnd_Main,CFrameWnd)

