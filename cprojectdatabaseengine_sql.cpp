#include "cprojectdatabaseengine_sql.h"

//====================================================================================================
//����������� ������
//====================================================================================================
CProjectDatabaseEngine_SQL::CProjectDatabaseEngine_SQL(const CString &project_list_table_name) 
{
 //����������� ����������� � ���� ������
 ProjectListTableName=project_list_table_name;
 ProjectListBaseInitString="ODBC;DRIVER=Microsoft Paradox Driver (*.db );FIL=Paradox 5.X;DBQ="+ProjectListTableName;
}
//====================================================================================================
//���������� ������
//====================================================================================================
CProjectDatabaseEngine_SQL::~CProjectDatabaseEngine_SQL() 
{
}
//====================================================================================================
//������� ������
//====================================================================================================

//----------------------------------------------------------------------------------------------------
//����� ������ �� GUID
//----------------------------------------------------------------------------------------------------
bool CProjectDatabaseEngine_SQL::FindProjectByGUID(const CSafeString &guid,SProject &sProject)
{
 CRAIICDatabase cRAIICDatabase(&cDatabase_ProjectList,ProjectListBaseInitString);
 {
  if (cRAIICDatabase.IsOpen()==false) return(false);
  CString sql_request="";
  sql_request+="SELECT * FROM ";
  sql_request+=ProjectListTableName;
  sql_request+=" WHERE (ProjectGUID='"+guid+"')";
  CRAIICRecordset_ProjectList cRAIICRecordset_ProjectList(&cDatabase_ProjectList,sql_request);
  if (cRAIICRecordset_ProjectList.IsOk()==false) return(false);
  if (cRAIICRecordset_ProjectList.GetMainObject().GetRecordCount()==0) return(false);
  cRAIICRecordset_ProjectList.GetMainObject().MoveFirst();
  if (cRAIICRecordset_ProjectList.GetMainObject().IsEOF()==TRUE) return(false);
  cRAIICRecordset_ProjectList.GetMainObject().GetRecord(sProject);
  return(true);
 } 
 return(false);
}

//----------------------------------------------------------------------------------------------------
//�������� ��� �������
//----------------------------------------------------------------------------------------------------
list<SProject> CProjectDatabaseEngine_SQL::GetAllProject(void)
{
 list<SProject> list_SProject_Local; 
 CRAIICDatabase cRAIICDatabase(&cDatabase_ProjectList,ProjectListBaseInitString);
 {
  CString sql_request="";
  sql_request+="SELECT * FROM ";
  sql_request+=ProjectListTableName;
  sql_request+=" ORDER BY ProjectName";
  CRAIICRecordset_ProjectList cRAIICRecordset_ProjectList(&cDatabase_ProjectList,sql_request);
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
bool CProjectDatabaseEngine_SQL::AddProject(SProject &sProject)
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
bool CProjectDatabaseEngine_SQL::DeleteProject(const SProject &sProject)
{
 CRAIICDatabase cRAIICDatabase(&cDatabase_ProjectList,ProjectListBaseInitString);
 {
  if (cRAIICDatabase.IsOpen()==false) return(false);
  CString sql_request="";
  sql_request+="SELECT * FROM ";
  sql_request+=ProjectListTableName;
  sql_request+=" WHERE (ProjectGUID='"+sProject.ProjectGUID+"')";
  CRAIICRecordset_ProjectList cRAIICRecordset_ProjectList(&cDatabase_ProjectList,sql_request);
  if (cRAIICRecordset_ProjectList.IsOk()==false) return(false);
  if (cRAIICRecordset_ProjectList.GetMainObject().GetRecordCount()==0) return(false);
  cRAIICRecordset_ProjectList.GetMainObject().MoveFirst();   
  if (cRAIICRecordset_ProjectList.GetMainObject().IsEOF()==TRUE) return(false);
  SProject sProject_Local;
  cRAIICRecordset_ProjectList.GetMainObject().GetRecord(sProject_Local);
  cRAIICRecordset_ProjectList.GetMainObject().Delete();
  return(true);
 } 
 return(false);
}
//----------------------------------------------------------------------------------------------------
//�������� ������
//----------------------------------------------------------------------------------------------------
bool CProjectDatabaseEngine_SQL::ChangeProject(const SProject &sProject)
{
 CRAIICDatabase cRAIICDatabase(&cDatabase_ProjectList,ProjectListBaseInitString);
 {
  if (cRAIICDatabase.IsOpen()==false) return(false);
  CString sql_request="";
  sql_request+="SELECT * FROM ";
  sql_request+=ProjectListTableName;
  sql_request+=" WHERE (ProjectGUID='"+sProject.ProjectGUID+"')";
  CRAIICRecordset_ProjectList cRAIICRecordset_ProjectList(&cDatabase_ProjectList,sql_request);
  if (cRAIICRecordset_ProjectList.IsOk()==false) return(false);
  if (cRAIICRecordset_ProjectList.GetMainObject().GetRecordCount()==0) return(false);
  cRAIICRecordset_ProjectList.GetMainObject().MoveFirst();
  if (cRAIICRecordset_ProjectList.GetMainObject().IsEOF()==TRUE) return(false);
  SProject sProject_Local;
  cRAIICRecordset_ProjectList.GetMainObject().GetRecord(sProject_Local);
  cRAIICRecordset_ProjectList.GetMainObject().Edit();
  cRAIICRecordset_ProjectList.GetMainObject().SetRecord(sProject);   
  cRAIICRecordset_ProjectList.GetMainObject().Update();
  return(true);
 }
 return(false);
}
//----------------------------------------------------------------------------------------------------
//�������� ����
//----------------------------------------------------------------------------------------------------
void CProjectDatabaseEngine_SQL::ResetBase(void)
{ 
 CRAIICDatabase cRAIICDatabase(&cDatabase_ProjectList,ProjectListBaseInitString);
 {
  CString sql_request="";
  sql_request+="DELETE * FROM ";
  sql_request+=ProjectListTableName;
  cDatabase_ProjectList.ExecuteSQL(sql_request);
 }
}


