#include "cprojectdatabaseengine_software.h"

//====================================================================================================
//����������� ������
//====================================================================================================
CProjectDatabaseEngine_Software::CProjectDatabaseEngine_Software(const CString &project_list_table_name) 
{
 //����������� ����������� � ���� ������
 ProjectListTableName=project_list_table_name;
 ProjectListBaseInitString="ODBC;DRIVER=Microsoft Paradox Driver (*.db );FIL=Paradox 5.X;DBQ="+ProjectListTableName;
}
//====================================================================================================
//���������� ������
//====================================================================================================
CProjectDatabaseEngine_Software::~CProjectDatabaseEngine_Software() 
{
}
//====================================================================================================
//������� ������
//====================================================================================================

//----------------------------------------------------------------------------------------------------
//����� ������ �� GUID
//----------------------------------------------------------------------------------------------------
bool CProjectDatabaseEngine_Software::FindProjectByGUID(const CSafeString &guid,SProject &sProject)
{
 CRAIICDatabase cRAIICDatabase(&cDatabase_ProjectList,ProjectListBaseInitString);
 {
  if (cRAIICDatabase.IsOpen()==false) return(false);
  CRAIICRecordset_ProjectList cRAIICRecordset_ProjectList(&cDatabase_ProjectList,ProjectListTableName);
  if (cRAIICRecordset_ProjectList.IsOk()==false) return(false);
  if (cRAIICRecordset_ProjectList.GetMainObject().GetRecordCount()==0) return(false);
  cRAIICRecordset_ProjectList.GetMainObject().MoveFirst();
  while(cRAIICRecordset_ProjectList.GetMainObject().IsEOF()==FALSE)
  {
   cRAIICRecordset_ProjectList.GetMainObject().GetRecord(sProject);
   if (sProject.ProjectGUID.Compare(guid)==0) return(true);    
   cRAIICRecordset_ProjectList.GetMainObject().MoveNext();
  }
 } 
 return(false);
}

//----------------------------------------------------------------------------------------------------
//�������� ��� �������
//----------------------------------------------------------------------------------------------------
list<SProject> CProjectDatabaseEngine_Software::GetAllProject(void)
{
 list<SProject> list_SProject_Local; 
 CRAIICDatabase cRAIICDatabase(&cDatabase_ProjectList,ProjectListBaseInitString);
 {
  if (cRAIICDatabase.IsOpen()==false) return(list_SProject_Local);
  CRAIICRecordset_ProjectList cRAIICRecordset_ProjectList(&cDatabase_ProjectList,ProjectListTableName);
  if (cRAIICRecordset_ProjectList.IsOk()==false) return(list_SProject_Local);
  if (cRAIICRecordset_ProjectList.GetMainObject().GetRecordCount()==0) return(list_SProject_Local);
  cRAIICRecordset_ProjectList.GetMainObject().MoveFirst();
  while(cRAIICRecordset_ProjectList.GetMainObject().IsEOF()==FALSE)
  {
   SProject sProject;
   cRAIICRecordset_ProjectList.GetMainObject().GetRecord(sProject);
   cRAIICRecordset_ProjectList.GetMainObject().MoveNext();
   list_SProject_Local.push_back(sProject);
  }
 } 
 return(list_SProject_Local);
}

//----------------------------------------------------------------------------------------------------
//�������� ������
//----------------------------------------------------------------------------------------------------
bool CProjectDatabaseEngine_Software::AddProject(SProject &sProject)
{
 CRAIICDatabase cRAIICDatabase(&cDatabase_ProjectList,ProjectListBaseInitString);
 {
  if (cRAIICDatabase.IsOpen()==false) return(false);
  CRAIICRecordset_ProjectList cRAIICRecordset_ProjectList(&cDatabase_ProjectList,ProjectListTableName);
  if (cRAIICRecordset_ProjectList.IsOk()==false) return(false);
  if (cRAIICRecordset_ProjectList.GetMainObject().CanAppend()==TRUE)
  {
   if (cRAIICRecordset_ProjectList.GetMainObject().GetRecordCount()!=0) cRAIICRecordset_ProjectList.GetMainObject().MoveLast();
   cRAIICRecordset_ProjectList.GetMainObject().AddNew();
   cRAIICRecordset_ProjectList.GetMainObject().SetRecord(sProject);
   cRAIICRecordset_ProjectList.GetMainObject().Update();
  }
 }
 //���������������� ���� �� ������������, ��� ��� ��������� ������ �� ���������
 return(true);
}
//----------------------------------------------------------------------------------------------------
//������� ������
//----------------------------------------------------------------------------------------------------
bool CProjectDatabaseEngine_Software::DeleteProject(const SProject &sProject)
{
 CRAIICDatabase cRAIICDatabase(&cDatabase_ProjectList,ProjectListBaseInitString);
 {
  if (cRAIICDatabase.IsOpen()==false) return(false);
  CRAIICRecordset_ProjectList cRAIICRecordset_ProjectList(&cDatabase_ProjectList,ProjectListTableName);
  if (cRAIICRecordset_ProjectList.IsOk()==false) return(false);
  if (cRAIICRecordset_ProjectList.GetMainObject().GetRecordCount()==0) return(false);
  cRAIICRecordset_ProjectList.GetMainObject().MoveFirst();   
  while(cRAIICRecordset_ProjectList.GetMainObject().IsEOF()==FALSE)
  {
   SProject sProject_Local;
   cRAIICRecordset_ProjectList.GetMainObject().GetRecord(sProject_Local);
   if (sProject_Local.ProjectGUID.Compare(sProject.ProjectGUID)==0)
   {
    cRAIICRecordset_ProjectList.GetMainObject().Delete();
    return(true);
   }
   cRAIICRecordset_ProjectList.GetMainObject().MoveNext();
  }
 } 
 return(false);
}
//----------------------------------------------------------------------------------------------------
//�������� ������
//----------------------------------------------------------------------------------------------------
bool CProjectDatabaseEngine_Software::ChangeProject(const SProject &sProject)
{
 CRAIICDatabase cRAIICDatabase(&cDatabase_ProjectList,ProjectListBaseInitString);
 {
  if (cRAIICDatabase.IsOpen()==false) return(false);
  CRAIICRecordset_ProjectList cRAIICRecordset_ProjectList(&cDatabase_ProjectList,ProjectListTableName);
  if (cRAIICRecordset_ProjectList.IsOk()==false) return(false);
  if (cRAIICRecordset_ProjectList.GetMainObject().GetRecordCount()==0) return(false);
  cRAIICRecordset_ProjectList.GetMainObject().MoveFirst();
  while(cRAIICRecordset_ProjectList.GetMainObject().IsEOF()==FALSE)
  {
   SProject sProject_Local;
   cRAIICRecordset_ProjectList.GetMainObject().GetRecord(sProject_Local);
   if (sProject_Local.ProjectGUID.Compare(sProject.ProjectGUID)==0)
   {
    cRAIICRecordset_ProjectList.GetMainObject().Edit();
    cRAIICRecordset_ProjectList.GetMainObject().SetRecord(sProject);   
    cRAIICRecordset_ProjectList.GetMainObject().Update();
	return(true);
   }
   cRAIICRecordset_ProjectList.GetMainObject().MoveNext();
  }
 }
 return(false);
}
//----------------------------------------------------------------------------------------------------
//�������� ����
//----------------------------------------------------------------------------------------------------
void CProjectDatabaseEngine_Software::ResetBase(void)
{ 
 CRAIICDatabase cRAIICDatabase(&cDatabase_ProjectList,ProjectListBaseInitString);
 {
  CSafeString sql_request="";
  sql_request+="DELETE * FROM ";
  sql_request+=ProjectListTableName;
  cDatabase_ProjectList.ExecuteSQL(sql_request);
 }
}

