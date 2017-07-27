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
void CRecordset_UserList::SetRecord(const SUser& sUser)
{
 Name=sUser.Name;
 JobTitle=sUser.JobTitle;
 Login=sUser.Login;
 Password=sUser.Password;
 Description=sUser.Description;
 Telephone=sUser.Telephone;
 UserGUID=sUser.UserGUID;
 Leader=0;
 if (sUser.Leader==true) Leader=1;
}
//----------------------------------------------------------------------------------------------------
//получить запись
//----------------------------------------------------------------------------------------------------
void CRecordset_UserList::GetRecord(SUser& sUser)
{
 sUser.Name=Name;
 sUser.JobTitle=JobTitle;
 sUser.Login=Login;
 sUser.Password=Password;
 sUser.Description=Description;
 sUser.Telephone=Telephone;
 sUser.UserGUID=UserGUID;
 sUser.Leader=false;
 if (Leader!=0) sUser.Leader=true;
}


//====================================================================================================
//класс RAII (Resource Acquisition is Initialization) для работы с CRecordset_UserList
//====================================================================================================

//----------------------------------------------------------------------------------------------------
//конструктор
//----------------------------------------------------------------------------------------------------
CRAIICRecordset_UserList::CRAIICRecordset_UserList(CDatabase *cDatabase_Ptr,const CString &table_name)
{
 Ok=false;
 if (cDatabase_Ptr==NULL) return;
 if (cDatabase_Ptr->IsOpen()==FALSE) return;
 cRecordset_UserList.m_pDatabase=cDatabase_Ptr;
 if (cRecordset_UserList.Open(CRecordset::snapshot,table_name,CRecordset::none)==FALSE) return;
 if (cRecordset_UserList.IsOpen()==FALSE) return;
 Ok=true;
}
//----------------------------------------------------------------------------------------------------
//деструктор
//----------------------------------------------------------------------------------------------------
CRAIICRecordset_UserList::~CRAIICRecordset_UserList()
{
 if (Ok==false) return;
 if (cRecordset_UserList.IsOpen()==TRUE) cRecordset_UserList.Close();
}
//----------------------------------------------------------------------------------------------------
//проверка, произошёл ли захват объекта
//----------------------------------------------------------------------------------------------------
bool CRAIICRecordset_UserList::IsOk(void)
{
 return(Ok);
}
//----------------------------------------------------------------------------------------------------
//получение оригинального объекта
//----------------------------------------------------------------------------------------------------
CRecordset_UserList& CRAIICRecordset_UserList::GetMainObject(void)
{
 return(cRecordset_UserList);
}
