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

 ((CEdit *)GetDlgItem(IDC_EDIT_DIALOG_USER_SETTINGS_NAME))->SetWindowText(cUser_Local.GetName());
 ((CEdit *)GetDlgItem(IDC_EDIT_DIALOG_USER_SETTINGS_LOGIN))->SetWindowText(cUser_Local.GetLogin());
 ((CEdit *)GetDlgItem(IDC_EDIT_DIALOG_USER_SETTINGS_PASSWORD))->SetWindowText(cUser_Local.GetPassword());
 ((CEdit *)GetDlgItem(IDC_EDIT_DIALOG_USER_SETTINGS_JOB_TITLE))->SetWindowText(cUser_Local.GetJobTitle());
 ((CEdit *)GetDlgItem(IDC_EDIT_DIALOG_USER_SETTINGS_DESCRIPTION))->SetWindowText(cUser_Local.GetDescription());
 ((CEdit *)GetDlgItem(IDC_EDIT_DIALOG_USER_SETTINGS_TELEPHONE))->SetWindowText(cUser_Local.GetTelephone());

 return(CDialog::OnInitDialog());
}
//----------------------------------------------------------------------------------------------------
//запуск диалога
//----------------------------------------------------------------------------------------------------
bool CDialog_UserSettings::Activate(CUser &cUser)
{
 cUser_Local=cUser;
 long ret=DoModal();
 if (ret==0)
 {
  cUser=cUser_Local;
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
 cUser_Local.SetName(name);
 cUser_Local.SetLogin(login);
 cUser_Local.SetPassword(password);
 cUser_Local.SetJobTitle(job_title);
 cUser_Local.SetDescription(description);
 cUser_Local.SetTelephone(telephone);
 EndDialog(0);
}
//----------------------------------------------------------------------------------------------------
//отменить настройки
//----------------------------------------------------------------------------------------------------
afx_msg void CDialog_UserSettings::OnCommand_Button_Cancel(void)
{
 EndDialog(-1);
}


