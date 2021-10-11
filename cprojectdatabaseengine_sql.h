#ifndef CPROJECTDATABASE_ENGINE_SQL_H
#define CPROJECTDATABASE_ENGINE_SQL_H

//====================================================================================================
//��������
//====================================================================================================

//����� ������ � ����� ������ �������� � �������������� SQL

//====================================================================================================
//������������ ����������
//====================================================================================================

#include "ciprojectdatabaseengine.h"
#include "cdatabaseengine_sql.h"
#include "craiicrecordset.h"

#include <memory>



//====================================================================================================
//���������
//====================================================================================================

//====================================================================================================
//����� ������ � ����� ������ �������� � �������������� SQL
//====================================================================================================

class CProjectDatabaseEngine_SQL:public CIProjectDatabaseEngine
{
 protected:
  //-���������� ������-------------------------------------------------------
  CDatabaseEngine_SQL<CRAIICRecordset<CRecordset_ProjectList>,CProject> *cDatabaseEngine_SQL_Ptr;//����� ��� ������ � ����� ������

  CString ProjectListTableName;//��� ������� ���� ������ ������ ��������  
  CString ProjectListBaseInitString;//������ ������������� ���� ������ ��������
  CDatabase cDatabase_ProjectList;//���� ������ ������ ��������


  std::shared_ptr<CRAIICDatabase> cRAIICDatabase_Ptr;//��������� �� ����� ������� ���� ������
 public:
  //-����������� ������------------------------------------------------------
  CProjectDatabaseEngine_SQL(const CString &project_list_table_name);
  //-���������� ������-------------------------------------------------------
  ~CProjectDatabaseEngine_SQL();
  //-������� ������----------------------------------------------------------
  bool FindProjectByGUID(const CSafeString &guid,CProject &cProject) override;//����� ������ �� GUID
  std::list<CProject> GetAllProject(void) override;//�������� ��� �������
  bool AddProject(CProject &cProject) override;//�������� ������
  bool DeleteProject(const CProject &cProject) override;//������� ������
  bool ChangeProject(const CProject &cProject) override;//�������� ������
  void ResetBase(void) override;//�������� ����
};

#endif