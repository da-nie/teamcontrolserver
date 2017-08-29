#include "cuser.h"

//====================================================================================================
//����������� ������
//====================================================================================================
CUser::CUser(void)
{
 Name="";
 JobTitle="";
 Login="";
 Password="";
 Description="";
 Telephone="";
 UserGUID="";
 Leader=false;
}
//====================================================================================================
//���������� ������
//====================================================================================================
CUser::~CUser()
{		
}
//====================================================================================================
//������� ������
//====================================================================================================
//----------------------------------------------------------------------------------------------------
//�������� ���
//----------------------------------------------------------------------------------------------------
const CSafeString& CUser::GetName(void) const
{
 return(Name);
}
//----------------------------------------------------------------------------------------------------
//�������� �����
//----------------------------------------------------------------------------------------------------
const CSafeString& CUser::GetLogin(void) const
{
 return(Login);
}
//----------------------------------------------------------------------------------------------------
//�������� ������
//----------------------------------------------------------------------------------------------------
const CSafeString& CUser::GetPassword(void) const
{
 return(Password);
}
//----------------------------------------------------------------------------------------------------
//�������� ���������
//----------------------------------------------------------------------------------------------------
const CSafeString& CUser::GetJobTitle(void) const
{
 return(JobTitle);
}
//----------------------------------------------------------------------------------------------------
//�������� ��������
//----------------------------------------------------------------------------------------------------
const CSafeString& CUser::GetDescription(void) const
{
 return(Description);
}
//----------------------------------------------------------------------------------------------------
//�������� �������
//----------------------------------------------------------------------------------------------------
const CSafeString& CUser::GetTelephone(void) const
{
 return(Telephone);
}
//----------------------------------------------------------------------------------------------------
//�������� ���������� ������������� ������������
//----------------------------------------------------------------------------------------------------
const CSafeString& CUser::GetUserGUID(void) const
{
 return(UserGUID);
}
//----------------------------------------------------------------------------------------------------
//��������, �������� �� �������������
//----------------------------------------------------------------------------------------------------
const bool& CUser::GetLeader(void) const
{
 return(Leader);
}

//----------------------------------------------------------------------------------------------------
//������ ���
//----------------------------------------------------------------------------------------------------
void CUser::SetName(const char *name)
{
 Name=name;
}
//----------------------------------------------------------------------------------------------------
//������ �����
//----------------------------------------------------------------------------------------------------
void CUser::SetLogin(const char *login)
{
 Login=login;
}
//----------------------------------------------------------------------------------------------------
//������ ������
//----------------------------------------------------------------------------------------------------
void CUser::SetPassword(const char *password)
{
 Password=password;
}
//----------------------------------------------------------------------------------------------------
//������ ���������
//----------------------------------------------------------------------------------------------------
void CUser::SetJobTitle(const char *job_title)
{
 JobTitle=job_title;
}
//----------------------------------------------------------------------------------------------------
//������ ��������
//----------------------------------------------------------------------------------------------------
void CUser::SetDescription(const char *description)
{
 Description=description;
}
//----------------------------------------------------------------------------------------------------
//������ �������
//----------------------------------------------------------------------------------------------------
void CUser::SetTelephone(const char *telephone)
{
 Telephone=telephone;
}
//----------------------------------------------------------------------------------------------------
//������ ���������� ������������� ������������
//----------------------------------------------------------------------------------------------------
void CUser::SetUserGUID(const char *user_guid)
{
 UserGUID=user_guid;
}
//----------------------------------------------------------------------------------------------------
//������, �������� �� �������������
//----------------------------------------------------------------------------------------------------
void CUser::SetLeader(bool leader)
{
 Leader=leader;
}

//----------------------------------------------------------------------------------------------------
//������ �� ����� � ������
//----------------------------------------------------------------------------------------------------
bool CUser::IsLoginAndPassword(const char *login,const char *password) const
{
 if (Login.Compare(CSafeString(const_cast<char*>(login)))==0 && Password.Compare(CSafeString(const_cast<char*>(password)))==0) return(true);
 return(false);
}
//----------------------------------------------------------------------------------------------------
//������ �� UserGUID
//----------------------------------------------------------------------------------------------------
bool CUser::IsUserGUID(const char *guid)
{
 if (UserGUID.Compare(const_cast<char*>(guid))==0) return(true);    
 return(false);
}
