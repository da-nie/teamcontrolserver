#include "cdialog_usersettings.h"

//====================================================================================================
//функции обработки сообщений класса
//====================================================================================================

BEGIN_MESSAGE_MAP(CDialog_UserSettings,CDialog)
 ON_WM_DESTROY()
 ON_COMMAND(IDC_BUTTON_DIALOG_USER_SETTINGS_OK,OnCommand_Button_Ok)
 ON_COMMAND(IDC_BUTTON_DIALOG_USER_SETTINGS_CANCEL,OnCommand_Button_Cancel)
END_MESSAGE_MAP()

//====================================================================================================
//конструктор класса
//====================================================================================================
CDialog_UserSettings::CDialog_UserSettings(LPCTSTR lpszTemplateName,CWnd* pParentWnd):CDialog(lpszTemplateName,pParentWnd)
{
}
//====================================================================================================
//деструктор класса
//====================================================================================================
CDialog_UserSettings::~CDialog_UserSettings()
{
}
//====================================================================================================
//функции класса
//====================================================================================================

//---------------------------------------------------------------------------
//нажали Enter
//---------------------------------------------------------------------------
afx_msg void CDialog_UserSettings::OnOK(void)
{
}
//----------------------------------------------------------------------------------------------------
//нажали ESC
//----------------------------------------------------------------------------------------------------
afx_msg void CDialog_UserSettings::OnCancel(void)
{
}
//----------------------------------------------------------------------------------------------------
//инициализация диалога
//----------------------------------------------------------------------------------------------------
afx_msg BOOL CDialog_UserSettings::OnInitDialog(void)
{
 ((CEdit *)GetDlgItem(IDC_EDIT_DIALOG_USER_SETTINGS_NAME))->SetLimitText(254);
 ((CEdit *)GetDlgItem(IDC_EDIT_DIALOG_USER_SETTINGS_LOGIN))->SetLimitText(254);
 ((CEdit *)GetDlgItem(IDC_EDIT_DIALOG_USER_SETTINGS_PASSWORD))->SetLimitText(254);
 ((CEdit *)GetDlgItem(IDC_EDIT_DIALOG_USER_SETTINGS_JOB_TITLE))->SetLimitText(254);
 ((CEdit *)GetDlgItem(IDC_EDIT_DIALOG_USER_SETTINGS_DESCRIPTION))->SetLimitText(254);
 ((CEdit *)GetDlgItem(IDC_EDIT_DIALOG_USER_SETTINGS_TELEPHONE))->SetLimitText(254);

 ((CEdit *)GetDlgItem(IDC_EDIT_DIALOG_USER_SETTINGS_NAME))->SetWindowText(sUser_Local.Name);
 ((CEdit *)GetDlgItem(IDC_EDIT_DIALOG_USER_SETTINGS_LOGIN))->SetWindowText(sUser_Local.Login);
 ((CEdit *)GetDlgItem(IDC_EDIT_DIALOG_USER_SETTINGS_PASSWORD))->SetWindowText(sUser_Local.Password);
 ((CEdit *)GetDlgItem(IDC_EDIT_DIALOG_USER_SETTINGS_JOB_TITLE))->SetWindowText(sUser_Local.JobTitle);
 ((CEdit *)GetDlgItem(IDC_EDIT_DIALOG_USER_SETTINGS_DESCRIPTION))->SetWindowText(sUser_Local.Description);
 ((CEdit *)GetDlgItem(IDC_EDIT_DIALOG_USER_SETTINGS_TELEPHONE))->SetWindowText(sUser_Local.Telephone);

 return(CDialog::OnInitDialog());
}
//----------------------------------------------------------------------------------------------------
//запуск диалога
//----------------------------------------------------------------------------------------------------
bool CDialog_UserSettings::Activate(SUser &sUser)
{
 sUser_Local=sUser;
 long ret=DoModal();
 if (ret==0)
 {
  sUser=sUser_Local;
  return(true);
 }
 return(false);
}

//====================================================================================================
//функции обработки сообщений класса
//====================================================================================================

//----------------------------------------------------------------------------------------------------
//уничтожение окна
//----------------------------------------------------------------------------------------------------
afx_msg void CDialog_UserSettings::OnDestroy(void)
{
 CDialog::OnDestroy();
}
//----------------------------------------------------------------------------------------------------
//применить настройки
//----------------------------------------------------------------------------------------------------
afx_msg void CDialog_UserSettings::OnCommand_Button_Ok(void)
{ 
 char name[255];
 char login[255];
 char password[255];
 char job_title[255];
 char description[255];
 char telephone[255];
 ((CEdit *)GetDlgItem(IDC_EDIT_DIALOG_USER_SETTINGS_NAME))->GetWindowText(name,255);
 ((CEdit *)GetDlgItem(IDC_EDIT_DIALOG_USER_SETTINGS_LOGIN))->GetWindowText(login,255);
 ((CEdit *)GetDlgItem(IDC_EDIT_DIALOG_USER_SETTINGS_PASSWORD))->GetWindowText(password,255);
 ((CEdit *)GetDlgItem(IDC_EDIT_DIALOG_USER_SETTINGS_JOB_TITLE))->GetWindowText(job_title,255);
 ((CEdit *)GetDlgItem(IDC_EDIT_DIALOG_USER_SETTINGS_DESCRIPTION))->GetWindowText(description,255);
 ((CEdit *)GetDlgItem(IDC_EDIT_DIALOG_USER_SETTINGS_TELEPHONE))->GetWindowText(telephone,255);
 sUser_Local.Name=name;
 sUser_Local.Login=login;
 sUser_Local.Password=password;
 sUser_Local.JobTitle=job_title;
 sUser_Local.Description=description;
 sUser_Local.Telephone=telephone;
 EndDialog(0);
}
//----------------------------------------------------------------------------------------------------
//отменить настройки
//----------------------------------------------------------------------------------------------------
afx_msg void CDialog_UserSettings::OnCommand_Button_Cancel(void)
{
 EndDialog(-1);
}


