#include "cframewnd_main.h"

//====================================================================================================
//карта сообщений класса
//====================================================================================================
BEGIN_MESSAGE_MAP(CFrameWnd_Main,CFrameWnd)
 ON_WM_CREATE()
 ON_WM_TIMER()
 ON_WM_DESTROY()
 ON_WM_SYSCOMMAND()
 ON_MESSAGE(WM_SYSTEM_TRAY_ICON,OnSystemTrayIconMessage)
 ON_COMMAND(IDC_MENU_SYSTRAY_EXIT,OnCommand_Menu_SysTray_Exit)
END_MESSAGE_MAP()
//====================================================================================================
//конструктор класса
//====================================================================================================
CFrameWnd_Main::CFrameWnd_Main(void)
{
 NotifyIconData.hIcon=NULL;
 hIcon_SysTray=NULL;
}
//====================================================================================================
//деструктор класса
//====================================================================================================
CFrameWnd_Main::~CFrameWnd_Main()
{
}
//====================================================================================================
//функции класса
//====================================================================================================

//----------------------------------------------------------------------------------------------------
//создание рамки
//----------------------------------------------------------------------------------------------------
int CFrameWnd_Main::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
 hIcon_SysTray=(HICON)LoadImage(AfxGetInstanceHandle(),(char*)IDI_ICON_MAIN,IMAGE_ICON,32,32,LR_DEFAULTCOLOR);
 //создаём иконку в системном лотке
 NotifyIconData.cbSize=sizeof(NOTIFYICONDATA);
 NotifyIconData.hWnd=m_hWnd;
 NotifyIconData.uID=IDI_ICON_MAIN;
 NotifyIconData.uFlags=NIF_ICON|NIF_MESSAGE|NIF_TIP;
 NotifyIconData.uCallbackMessage=WM_SYSTEM_TRAY_ICON;
 NotifyIconData.hIcon=hIcon_SysTray; strcpy(NotifyIconData.szTip,"Team Control сервер");
 Shell_NotifyIcon(NIM_ADD,&NotifyIconData);
 SetIcon(NotifyIconData.hIcon,TRUE);  
 return(CFrameWnd::OnCreate(lpCreateStruct));
}
//----------------------------------------------------------------------------------------------------
//уничтожение рамки
//----------------------------------------------------------------------------------------------------
afx_msg void CFrameWnd_Main::OnDestroy(void)
{
 Shell_NotifyIcon(NIM_DELETE,&NotifyIconData);
 if (hIcon_SysTray!=NULL) DeleteObject(hIcon_SysTray);
 hIcon_SysTray=NULL;
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

//====================================================================================================
//функции обработки сообщений класса
//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

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
 if (lParam==WM_RBUTTONUP)//нажали и отпустили правую кнопку мыши
 {  
  SetForegroundWindow();
  //отображаем меню
  CMenu *cMenu=new CMenu;
  cMenu->CreatePopupMenu();
  cMenu->AppendMenu(MF_STRING,IDC_MENU_SYSTRAY_EXIT,"Завершить работу программы");
  CPoint cPoint;
  GetCursorPos(&cPoint);
  cMenu->TrackPopupMenu(TPM_LEFTBUTTON|TPM_RIGHTBUTTON,cPoint.x,cPoint.y,this);
  delete(cMenu);
 }
}
//----------------------------------------------------------------------------------------------------
//обработка команды выхода из программы
//----------------------------------------------------------------------------------------------------
afx_msg void CFrameWnd_Main::OnCommand_Menu_SysTray_Exit(void)
{
 if (MessageBox("Завершить работу программы?","Подтверждение",MB_YESNO|MB_DEFBUTTON2)!=IDYES) return;
 DestroyWindow();
}

//-Прочее--------------------------------------------------------------------
IMPLEMENT_DYNCREATE(CFrameWnd_Main,CFrameWnd)

