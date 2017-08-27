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
bool CProjectDatabaseEngine_Software::FindProjectByGUID(const CSafeString &guid,CProject &cProject)
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
   cRAIICRecordset_ProjectList.GetMainObject().GetRecord(cProject);
   if (cProject.IsProjectGUIDCorrect(guid)==true) return(true);
   cRAIICRecordset_ProjectList.GetMainObject().MoveNext();
  }
 } 
 return(false);
}

//----------------------------------------------------------------------------------------------------
//�������� ��� �������
//----------------------------------------------------------------------------------------------------
list<CProject> CProjectDatabaseEngine_Software::GetAllProject(void)
{
 list<CProject> list_CProject_Local; 
 CRAIICDatabase cRAIICDatabase(&cDatabase_ProjectList,ProjectListBaseInitString);
 {
  if (cRAIICDatabase.IsOpen()==false) return(list_CProject_Local);
  CRAIICRecordset_ProjectList cRAIICRecordset_ProjectList(&cDatabase_ProjectList,ProjectListTableName);
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
//�������� ������
//----------------------------------------------------------------------------------------------------
bool CProjectDatabaseEngine_Software::AddProject(CProject &cProject)
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
 //���������������� ���� �� ������������, ��� ��� ��������� ������ �� ���������
 return(true);
}
//----------------------------------------------------------------------------------------------------
//������� ������
//----------------------------------------------------------------------------------------------------
bool CProjectDatabaseEngine_Software::DeleteProject(const CProject &cProject)
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
   CProject cProject_Local;
   cRAIICRecordset_ProjectList.GetMainObject().GetRecord(cProject_Local);
   if (cProject_Local.IsProjectGUIDCorrect(cProject.GetProjectGUID())==true)
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
bool CProjectDatabaseEngine_Software::ChangeProject(const CProject &cProject)
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
   CProject cProject_Local;
   cRAIICRecordset_ProjectList.GetMainObject().GetRecord(cProject_Local);
   if (cProject_Local.IsProjectGUIDCorrect(cProject.GetProjectGUID())==true)
   {
    cRAIICRecordset_ProjectList.GetMainObject().Edit();
    cRAIICRecordset_ProjectList.GetMainObject().SetRecord(cProject);   
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

