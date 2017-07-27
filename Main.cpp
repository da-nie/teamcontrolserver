#include "stdafx.h"
//------------------------------------------------------------------------------
#include "cdocument_main.h"
#include "clistview_main.h"
#include "cframewnd_main.h"

//------------------------------------------------------------------------------
class CWinApp_Main:public CWinApp
{
 protected:
  //-���������� ������-------------------------------------------------------
  //-������� ������----------------------------------------------------------
  //-������------------------------------------------------------------------
 public:
  //-����������� ������------------------------------------------------------
  CWinApp_Main(void);
  //-���������� ������-------------------------------------------------------
  ~CWinApp_Main();
  //-���������� ������-------------------------------------------------------
  //-������� ������----------------------------------------------------------
  BOOL InitInstance(void);
  int ExitInstance(void);
  //-������------------------------------------------------------------------
};
//-����������� ������--------------------------------------------------------
CWinApp_Main::CWinApp_Main(void)
{
}
//-���������� ������---------------------------------------------------------
CWinApp_Main::~CWinApp_Main()
{
}
//-������� ������------------------------------------------------------------
BOOL CWinApp_Main::InitInstance(void)
{ 
 char FileName[MAX_PATH];
 GetModuleFileName(NULL,FileName,MAX_PATH);
 //���������� �� �����
 long size=strlen(FileName);
 if (size>0) size--;
 while(size>0)
 {
  unsigned char s=FileName[size];
  if (s==0 || s=='\\')
  {
   FileName[size+1]=0;
   break;
  }
  size--;
 }
 SetCurrentDirectory(FileName);

 //�������������� ������
 WSADATA wsadata;
 if (WSAStartup(0x0202,&wsadata)!=0) 
 {
  MessageBox(NULL,"������ ������������� �������.","������",MB_OK);
  WSACleanup();
  return(FALSE);
 }
 if (wsadata.wVersion!=0x0202)
 {
  MessageBox(NULL,"�� ������ ������ ���������� �������.","������",MB_OK);
  WSACleanup();
  return(FALSE);
 }
 CSingleDocTemplate* pDocTemplate;//����� ������ 
 pDocTemplate=new CSingleDocTemplate(IDR_MENU_MAIN,RUNTIME_CLASS(CDocument_Main),RUNTIME_CLASS(CFrameWnd_Main),RUNTIME_CLASS(CListView_Main));
 AddDocTemplate(pDocTemplate);//�������� ������
 CCommandLineInfo cmdInfo;//����� ������
 ParseCommandLine(cmdInfo);//������ ��������� ������
 if (!ProcessShellCommand(cmdInfo)) return(FALSE);//������ ������ �� ����������
 m_pMainWnd->SetWindowText("������ Team Control");
 //m_pMainWnd->SetWindowPos(NULL,0,0,640,480,0);
 m_pMainWnd->ShowWindow(SW_HIDE);//������ ����
 m_pMainWnd->UpdateWindow();//��������� ���� ��������� ���������
 return(TRUE);
}
int CWinApp_Main::ExitInstance(void)
{
 //���������������� ������
 WSACleanup();
 return(CWinApp::ExitInstance());
}

CWinApp_Main cWinApp_Main;


