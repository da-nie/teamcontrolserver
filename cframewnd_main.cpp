#include "cframewnd_main.h"

//-������� ��������� ��������� ������----------------------------------------
BEGIN_MESSAGE_MAP(CFrameWnd_Main,CFrameWnd)
 ON_WM_CREATE()
 ON_WM_TIMER()
 ON_WM_DESTROY()
 ON_WM_SYSCOMMAND()
 ON_MESSAGE(WM_SYSTEM_TRAY_ICON,OnSystemTrayIconMessage)
END_MESSAGE_MAP()
//-����������� ������--------------------------------------------------------
CFrameWnd_Main::CFrameWnd_Main(void)
{
 NotifyIconData.hIcon=NULL;
}
//-���������� ������---------------------------------------------------------
CFrameWnd_Main::~CFrameWnd_Main()
{
}
//-������� ������------------------------------------------------------------

//----------------------------------------------------------------------------------------------------
//�������� �����
//----------------------------------------------------------------------------------------------------
int CFrameWnd_Main::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
 //������ ������ � ��������� �����
 NotifyIconData.cbSize=sizeof(NOTIFYICONDATA);
 NotifyIconData.hWnd=m_hWnd;
 NotifyIconData.uID=IDI_ICON_MAIN;
 NotifyIconData.uFlags=NIF_ICON|NIF_MESSAGE|NIF_TIP;
 NotifyIconData.uCallbackMessage=WM_SYSTEM_TRAY_ICON;
 NotifyIconData.hIcon=(HICON)LoadImage(AfxGetInstanceHandle(),(char*)IDI_ICON_MAIN,IMAGE_ICON,32,32,LR_DEFAULTCOLOR);
 strcpy(NotifyIconData.szTip,"Team Control ������");
 Shell_NotifyIcon(NIM_ADD,&NotifyIconData);
 SetIcon(NotifyIconData.hIcon,TRUE);  
 return(CFrameWnd::OnCreate(lpCreateStruct));
}
//----------------------------------------------------------------------------------------------------
//����������� �����
//----------------------------------------------------------------------------------------------------
afx_msg void CFrameWnd_Main::OnDestroy(void)
{
 Shell_NotifyIcon(NIM_DELETE,&NotifyIconData);
 if (NotifyIconData.hIcon!=NULL) DeleteObject(NotifyIconData.hIcon);
 NotifyIconData.hIcon=NULL;
}
//----------------------------------------------------------------------------------------------------
//������� �������
//----------------------------------------------------------------------------------------------------
afx_msg void CFrameWnd_Main::OnTimer(UINT nIDEvent)
{
 if (nIDEvent==ID_TIMER_FRAMEWND_MAIN)
 { 
 }
 CFrameWnd::OnTimer(nIDEvent);
}
//----------------------------------------------------------------------------------------------------
//��������� ���������� ����
//----------------------------------------------------------------------------------------------------
afx_msg void CFrameWnd_Main::OnSysCommand(UINT nID,LPARAM lParam)
{
 if (nID==SC_MINIMIZE || nID==SC_CLOSE)//������ ���� � ��������� �����
 {
  ShowWindow(SW_HIDE);
  return;
 }
 CFrameWnd::OnSysCommand(nID,lParam);
}

//-������� ��������� ��������� ������----------------------------------------
//----------------------------------------------------------------------------------------------------
//��������� ��������� ����
//----------------------------------------------------------------------------------------------------
afx_msg void CFrameWnd_Main::OnSystemTrayIconMessage(WPARAM wParam,LPARAM lParam)
{
 if (wParam!=IDI_ICON_MAIN) return;//��� �� ���� ������ � ����� 
 if (lParam==WM_LBUTTONUP)//������ � ��������� ����� ������ ����
 {  
  SetForegroundWindow();
  ShowWindow(SW_SHOW);
 }
}
//-������--------------------------------------------------------------------
IMPLEMENT_DYNCREATE(CFrameWnd_Main,CFrameWnd)

