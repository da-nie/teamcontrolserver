#include "cdialog_serversettings.h"

//====================================================================================================
//������� ��������� ��������� ������
//====================================================================================================

BEGIN_MESSAGE_MAP(CDialog_ServerSettings,CDialog)
 ON_WM_DESTROY()
 ON_COMMAND(IDC_BUTTON_DIALOG_SERVER_SETTINGS_OK,OnCommand_Button_Ok)
 ON_COMMAND(IDC_BUTTON_DIALOG_SERVER_SETTINGS_CANCEL,OnCommand_Button_Cancel)
END_MESSAGE_MAP()

//====================================================================================================
//����������� ������
//====================================================================================================
CDialog_ServerSettings::CDialog_ServerSettings(LPCTSTR lpszTemplateName,CWnd* pParentWnd):CDialog(lpszTemplateName,pParentWnd)
{
}
//====================================================================================================
//���������� ������
//====================================================================================================
CDialog_ServerSettings::~CDialog_ServerSettings()
{
}
//====================================================================================================
//������� ������
//====================================================================================================

//---------------------------------------------------------------------------
//������ Enter
//---------------------------------------------------------------------------
afx_msg void CDialog_ServerSettings::OnOK(void)
{
}
//----------------------------------------------------------------------------------------------------
//������ ESC
//----------------------------------------------------------------------------------------------------
afx_msg void CDialog_ServerSettings::OnCancel(void)
{
}
//----------------------------------------------------------------------------------------------------
//������������� �������
//----------------------------------------------------------------------------------------------------
afx_msg BOOL CDialog_ServerSettings::OnInitDialog(void)
{
 ((CEdit *)GetDlgItem(IDC_EDIT_DIALOG_SERVER_SETTINGS_PORT))->SetLimitText(5);

 char port_name[255];
 sprintf(port_name,"%i",sServerSettings_Local.Port);
 ((CEdit *)GetDlgItem(IDC_EDIT_DIALOG_SERVER_SETTINGS_PORT))->SetWindowText(port_name);

 //���������, �������� �� ��������� � ������������
 bool AutoLoad=false;
 char KeyName[]="Software\\Microsoft\\Windows\\CurrentVersion\\Run";
 HKEY hKey;
 if (RegOpenKey(HKEY_LOCAL_MACHINE,KeyName,&hKey)==ERROR_SUCCESS)
 {
  unsigned char buffer[MAX_PATH];
  DWORD dwType=REG_SZ;
  DWORD dwLength=MAX_PATH; 
  if (RegQueryValueEx(hKey,"Team Control Server",NULL,&dwType,buffer,&dwLength)==ERROR_SUCCESS) AutoLoad=true;
  RegCloseKey(hKey);
 }
 if (AutoLoad==true) ((CButton *)GetDlgItem(IDC_CHECK_DIALOG_SERVER_SETTINGS_RUN_ON_WINDOWS))->SetCheck(1);
                else ((CButton *)GetDlgItem(IDC_CHECK_DIALOG_SERVER_SETTINGS_RUN_ON_WINDOWS))->SetCheck(0);
 return(CDialog::OnInitDialog());
}
//----------------------------------------------------------------------------------------------------
//������ �������
//----------------------------------------------------------------------------------------------------
bool CDialog_ServerSettings::Activate(SServerSettings &sServerSettings)
{
 sServerSettings_Local=sServerSettings;
 long ret=DoModal();
 if (ret==0)
 {
  sServerSettings=sServerSettings_Local;
  return(true);
 }
 return(false);
}

//====================================================================================================
//������� ��������� ��������� ������
//====================================================================================================

//----------------------------------------------------------------------------------------------------
//����������� ����
//----------------------------------------------------------------------------------------------------
afx_msg void CDialog_ServerSettings::OnDestroy(void)
{
 CDialog::OnDestroy();
}
//----------------------------------------------------------------------------------------------------
//��������� ���������
//----------------------------------------------------------------------------------------------------
afx_msg void CDialog_ServerSettings::OnCommand_Button_Ok(void)
{ 
 char port[255];
 ((CEdit *)GetDlgItem(IDC_EDIT_DIALOG_SERVER_SETTINGS_PORT))->GetWindowText(port,255);
 sServerSettings_Local.Port=atoi(port);

 if (((CButton *)GetDlgItem(IDC_CHECK_DIALOG_SERVER_SETTINGS_RUN_ON_WINDOWS))->GetCheck())//����� ���������� � ������������
 {
  //������������ � ������
  char *Patch=new char[MAX_PATH];
  char KeyName[]="Software\\Microsoft\\Windows\\CurrentVersion\\Run";
  GetModuleFileName(NULL,Patch,MAX_PATH);
  HKEY hKey;
  DWORD lRetVal;
  if (RegCreateKeyEx(HKEY_LOCAL_MACHINE,KeyName,0,NULL,REG_OPTION_NON_VOLATILE,KEY_ALL_ACCESS,0,&hKey,&lRetVal)!=ERROR_SUCCESS) return;
  RegSetValueEx(hKey,"Team Control Server",0,REG_SZ,(unsigned char*)Patch,strlen(Patch));
  RegCloseKey(hKey);
  delete[](Patch);
 }
 else//����� ������ �� ������������
 {
  //������� �������� ����� �� �������
  char KeyName[]="Software\\Microsoft\\Windows\\CurrentVersion\\Run";
  HKEY hKey;
  if (RegOpenKey(HKEY_LOCAL_MACHINE,KeyName,&hKey)!=ERROR_SUCCESS) return;
  RegDeleteValue(hKey,"Team Control Server");
  RegCloseKey(hKey);
 }

 EndDialog(0);
}
//----------------------------------------------------------------------------------------------------
//�������� ���������
//----------------------------------------------------------------------------------------------------
afx_msg void CDialog_ServerSettings::OnCommand_Button_Cancel(void)
{
 EndDialog(-1);
}


