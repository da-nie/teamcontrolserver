#include "cprojectdatabaseengine_sql.h"

//====================================================================================================
//����������� ������
//====================================================================================================
CProjectDatabaseEngine_SQL::CProjectDatabaseEngine_SQL(const CString &project_list_table_name) 
{
 //����������� ����������� � ���� ������
 ProjectListTableName=project_list_table_name;
 ProjectListBaseInitString="ODBC;DRIVER=Microsoft Paradox Driver (*.db );FIL=Paradox 5.X;DBQ="+ProjectListTableName;

 cDatabaseEngine_SQL_Ptr=new CDatabaseEngine_SQL<CRAIICRecordset<CRecordset_ProjectList>,CProject>(ProjectListTableName);

 cRAIICDatabase_Ptr.reset(new CRAIICDatabase(&cDatabase_ProjectList,ProjectListBaseInitString));
}
//====================================================================================================
//���������� ������
//====================================================================================================
CProjectDatabaseEngine_SQL::~CProjectDatabaseEngine_SQL() 
{
 delete(cDatabaseEngine_SQL_Ptr);
}
//====================================================================================================
//������� ������
//====================================================================================================

//----------------------------------------------------------------------------------------------------
//����� ������ �� GUID
//----------------------------------------------------------------------------------------------------
bool CProjectDatabaseEngine_SQL::FindProjectByGUID(const CSafeString &guid,CProject &cProject)
{
 CString sql_condition="ProjectGUID='"+guid+"'"; 
 return(cDatabaseEngine_SQL_Ptr->FindOne(sql_condition,cProject));
}

//----------------------------------------------------------------------------------------------------
//�������� ��� �������
//----------------------------------------------------------------------------------------------------
std::list<CProject> CProjectDatabaseEngine_SQL::GetAllProject(void)
{
 std::list<CProject> list_CProject_Local; 
 {
  if (cRAIICDatabase_Ptr->IsOpen()==false) return(list_CProject_Local);
  CString sql_request="";
  sql_request+="SELECT * FROM ";
  sql_request+=ProjectListTableName;
  sql_request+=" ORDER BY ProjectName";
  CRAIICRecordset<CRecordset_ProjectList> cRAIICRecordset_ProjectList(&cDatabase_ProjectList,sql_request);
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
bool CProjectDatabaseEngine_SQL::AddProject(CProject &cProject)
{
 return(cDatabaseEngine_SQL_Ptr->Add(cProject));
}
//----------------------------------------------------------------------------------------------------
//������� ������
//----------------------------------------------------------------------------------------------------
bool CProjectDatabaseEngine_SQL::DeleteProject(const CProject &cProject)
{
 CString sql_condition="ProjectGUID='"+cProject.GetProjectGUID()+"'";
 return(cDatabaseEngine_SQL_Ptr->Delete(sql_condition));
}
//----------------------------------------------------------------------------------------------------
//�������� ������
//----------------------------------------------------------------------------------------------------
bool CProjectDatabaseEngine_SQL::ChangeProject(const CProject &cProject)
{
 CString sql_condition="ProjectGUID='"+cProject.GetProjectGUID()+"'";
 return(cDatabaseEngine_SQL_Ptr->Change(sql_condition,cProject));
}
//----------------------------------------------------------------------------------------------------
//�������� ����
//----------------------------------------------------------------------------------------------------
void CProjectDatabaseEngine_SQL::ResetBase(void)
{ 
 cDatabaseEngine_SQL_Ptr->ResetBase();
}


