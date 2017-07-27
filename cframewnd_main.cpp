#include "cframewnd_main.h"

//====================================================================================================
//����� ��������� ������
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
//����������� ������
//====================================================================================================
CFrameWnd_Main::CFrameWnd_Main(void)
{
 NotifyIconData.hIcon=NULL;
 hIcon_SysTray=NULL;
}
//====================================================================================================
//���������� ������
//====================================================================================================
CFrameWnd_Main::~CFrameWnd_Main()
{
}
//====================================================================================================
//������� ������
//====================================================================================================

//----------------------------------------------------------------------------------------------------
//�������� �����
//----------------------------------------------------------------------------------------------------
int CFrameWnd_Main::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
 hIcon_SysTray=(HICON)LoadImage(AfxGetInstanceHandle(),(char*)IDI_ICON_MAIN,IMAGE_ICON,32,32,LR_DEFAULTCOLOR);
 //������ ������ � ��������� �����
 NotifyIconData.cbSize=sizeof(NOTIFYICONDATA);
 NotifyIconData.hWnd=m_hWnd;
 NotifyIconData.uID=IDI_ICON_MAIN;
 NotifyIconData.uFlags=NIF_ICON|NIF_MESSAGE|NIF_TIP;
 NotifyIconData.uCallbackMessage=WM_SYSTEM_TRAY_ICON;
 NotifyIconData.hIcon=hIcon_SysTray; strcpy(NotifyIconData.szTip,"Team Control ������");
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
 if (hIcon_SysTray!=NULL) DeleteObject(hIcon_SysTray);
 hIcon_SysTray=NULL;
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

//====================================================================================================
//������� ��������� ��������� ������
//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

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
 if (lParam==WM_RBUTTONUP)//������ � ��������� ������ ������ ����
 {  
  SetForegroundWindow();
  //���������� ����
  CMenu *cMenu=new CMenu;
  cMenu->CreatePopupMenu();
  cMenu->AppendMenu(MF_STRING,IDC_MENU_SYSTRAY_EXIT,"��������� ������ ���������");
  CPoint cPoint;
  GetCursorPos(&cPoint);
  cMenu->TrackPopupMenu(TPM_LEFTBUTTON|TPM_RIGHTBUTTON,cPoint.x,cPoint.y,this);
  delete(cMenu);
 }
}
//----------------------------------------------------------------------------------------------------
//��������� ������� ������ �� ���������
//----------------------------------------------------------------------------------------------------
afx_msg void CFrameWnd_Main::OnCommand_Menu_SysTray_Exit(void)
{
 if (MessageBox("��������� ������ ���������?","�������������",MB_YESNO|MB_DEFBUTTON2)!=IDYES) return;
 DestroyWindow();
}

//-������--------------------------------------------------------------------
IMPLEMENT_DYNCREATE(CFrameWnd_Main,CFrameWnd)

