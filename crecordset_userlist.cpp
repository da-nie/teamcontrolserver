#include "crecordset_userlist.h"

//====================================================================================================
//конструктор класса
//====================================================================================================
CRecordset_UserList::CRecordset_UserList(CDatabase* pDatabase):CRecordset(pDatabase)
{
 Name="";
 JobTitle="";
 Login="";
 Password="";
 Description="";
 Telephone="";
 UserGUID="";
 Leader=0;
 m_nFields=8;
 m_nDefaultType=snapshot;
}
//====================================================================================================
//деструктор класса
//====================================================================================================
CRecordset_UserList::~CRecordset_UserList()
{		
}
//====================================================================================================
//функции класса
//====================================================================================================
CString CRecordset_UserList::GetDefaultConnect(void)
{
 return(_T("ODBC;DRIVER=Microsoft Paradox Driver (*.db );DBQ=UserBase"));
}
//----------------------------------------------------------------------------------------------------
//----------------------------------------------------------------------------------------------------
CString CRecordset_UserList::GetDefaultSQL(void)
{
 return(_T("[UserBase]"));
}
//----------------------------------------------------------------------------------------------------
//----------------------------------------------------------------------------------------------------
void CRecordset_UserList::DoFieldExchange(CFieldExchange* pFX)
{ 
 pFX->SetFieldType(CFieldExchange::outputColumn);
 RFX_Text(pFX,_T("[Name]"),Name); 
 RFX_Text(pFX,_T("[JobTitle]"),JobTitle);
 RFX_Text(pFX,_T("[Login]"),Login);
 RFX_Text(pFX,_T("[Password]"),Password);
 RFX_Text(pFX,_T("[Description]"),Description);
 RFX_Text(pFX,_T("[Telephone]"),Telephone);
 RFX_Text(pFX,_T("[GUID]"),UserGUID);
 RFX_Int(pFX,_T("[Leader]"),Leader);  
}
//----------------------------------------------------------------------------------------------------
//задать запись
//----------------------------------------------------------------------------------------------------
void CRecordset_UserList::SetRecord(const CUser& cUser)
{
 Name=cUser.GetName();
 JobTitle=cUser.GetJobTitle();
 Login=cUser.GetLogin();
 Password=cUser.GetPassword();
 Description=cUser.GetDescription();
 Telephone=cUser.GetTelephone();
 UserGUID=cUser.GetUserGUID();
 Leader=0;
 if (cUser.GetLeader()==true) Leader=1;
}
//----------------------------------------------------------------------------------------------------
//получить запись
//----------------------------------------------------------------------------------------------------
void CRecordset_UserList::GetRecord(CUser& cUser)
{
 cUser.SetName(Name);
 cUser.SetJobTitle(JobTitle);
 cUser.SetLogin(Login);
 cUser.SetPassword(Password);
 cUser.SetDescription(Description);
 cUser.SetTelephone(Telephone);
 cUser.SetUserGUID(UserGUID);
 cUser.SetLeader(false);
 if (Leader!=0) cUser.SetLeader(true);
}
