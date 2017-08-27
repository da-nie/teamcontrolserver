#include "cprojectdatabaseengine_sql.h"

//====================================================================================================
//конструктор класса
//====================================================================================================
CProjectDatabaseEngine_SQL::CProjectDatabaseEngine_SQL(const CString &project_list_table_name) 
{
 //настраиваем подключение к базе данных
 ProjectListTableName=project_list_table_name;
 ProjectListBaseInitString="ODBC;DRIVER=Microsoft Paradox Driver (*.db );FIL=Paradox 5.X;DBQ="+ProjectListTableName;
}
//====================================================================================================
//деструктор класса
//====================================================================================================
CProjectDatabaseEngine_SQL::~CProjectDatabaseEngine_SQL() 
{
}
//====================================================================================================
//функции класса
//====================================================================================================

//----------------------------------------------------------------------------------------------------
//найти проект по GUID
//----------------------------------------------------------------------------------------------------
bool CProjectDatabaseEngine_SQL::FindProjectByGUID(const CSafeString &guid,CProject &cProject)
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
  cRAIICRecordset_ProjectList.GetMainObject().GetRecord(cProject);
  return(true);
 } 
 return(false);
}

//----------------------------------------------------------------------------------------------------
//получить все проекты
//----------------------------------------------------------------------------------------------------
list<CProject> CProjectDatabaseEngine_SQL::GetAllProject(void)
{
 list<CProject> list_CProject_Local; 
 CRAIICDatabase cRAIICDatabase(&cDatabase_ProjectList,ProjectListBaseInitString);
 {
  CString sql_request="";
  sql_request+="SELECT * FROM ";
  sql_request+=ProjectListTableName;
  sql_request+=" ORDER BY ProjectName";
  CRAIICRecordset_ProjectList cRAIICRecordset_ProjectList(&cDatabase_ProjectList,sql_request);
  if (cRAIICRecordset_ProjectList.IsOk()==false) return(list_CProject_Local);
  if (cRAIICRecordset_ProjectList.GetMainObject().GetRecordCount()==0) return(list_CProject_Local);
  cRAIICRecordset_ProjectList.GetMainObject().MoveFirst();
  while(cRAIICRecordset_ProjectList.GetMainObject().IsEOF()==FALSE)
  {
   CProject cProject;
   cRAIICRecordset_ProjectList.GetMainObject().GetRecord(cProject);
   cRAIICRecordset_ProjectList.GetMainObject().MoveNext();
   list_CProject_Local.push_back(cProject);
  }
 }
 return(list_CProject_Local);
}

//----------------------------------------------------------------------------------------------------
//добавить проект
//----------------------------------------------------------------------------------------------------
bool CProjectDatabaseEngine_SQL::AddProject(CProject &cProject)
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
   cRAIICRecordset_ProjectList.GetMainObject().SetRecord(cProject);
   cRAIICRecordset_ProjectList.GetMainObject().Update();
  }
 }
 //автоинкрементное поле не используется, так что считывать заново не требуется
 return(true);
}
//----------------------------------------------------------------------------------------------------
//удалить проект
//----------------------------------------------------------------------------------------------------
bool CProjectDatabaseEngine_SQL::DeleteProject(const CProject &cProject)
{
 CRAIICDatabase cRAIICDatabase(&cDatabase_ProjectList,ProjectListBaseInitString);
 {
  if (cRAIICDatabase.IsOpen()==false) return(false);
  CString sql_request="";
  sql_request+="SELECT * FROM ";
  sql_request+=ProjectListTableName;
  sql_request+=" WHERE (ProjectGUID='"+cProject.GetProjectGUID()+"')";
  CRAIICRecordset_ProjectList cRAIICRecordset_ProjectList(&cDatabase_ProjectList,sql_request);
  if (cRAIICRecordset_ProjectList.IsOk()==false) return(false);
  if (cRAIICRecordset_ProjectList.GetMainObject().GetRecordCount()==0) return(false);
  cRAIICRecordset_ProjectList.GetMainObject().MoveFirst();   
  if (cRAIICRecordset_ProjectList.GetMainObject().IsEOF()==TRUE) return(false);
  CProject cProject_Local;
  cRAIICRecordset_ProjectList.GetMainObject().GetRecord(cProject_Local);
  cRAIICRecordset_ProjectList.GetMainObject().Delete();
  return(true);
 } 
 return(false);
}
//----------------------------------------------------------------------------------------------------
//изменить проект
//----------------------------------------------------------------------------------------------------
bool CProjectDatabaseEngine_SQL::ChangeProject(const CProject &cProject)
{
 CRAIICDatabase cRAIICDatabase(&cDatabase_ProjectList,ProjectListBaseInitString);
 {
  if (cRAIICDatabase.IsOpen()==false) return(false);
  CString sql_request="";
  sql_request+="SELECT * FROM ";
  sql_request+=ProjectListTableName;
  sql_request+=" WHERE (ProjectGUID='"+cProject.GetProjectGUID()+"')";
  CRAIICRecordset_ProjectList cRAIICRecordset_ProjectList(&cDatabase_ProjectList,sql_request);
  if (cRAIICRecordset_ProjectList.IsOk()==false) return(false);
  if (cRAIICRecordset_ProjectList.GetMainObject().GetRecordCount()==0) return(false);
  cRAIICRecordset_ProjectList.GetMainObject().MoveFirst();
  if (cRAIICRecordset_ProjectList.GetMainObject().IsEOF()==TRUE) return(false);
  CProject cProject_Local;
  cRAIICRecordset_ProjectList.GetMainObject().GetRecord(cProject_Local);
  cRAIICRecordset_ProjectList.GetMainObject().Edit();
  cRAIICRecordset_ProjectList.GetMainObject().SetRecord(cProject);   
  cRAIICRecordset_ProjectList.GetMainObject().Update();
  return(true);
 }
 return(false);
}
//----------------------------------------------------------------------------------------------------
//очистить базу
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


