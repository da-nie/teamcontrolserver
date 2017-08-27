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

using namespace std;

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
  CString ProjectListTableName;//��� ������� ���� ������ ������ ��������  
  CString ProjectListBaseInitString;//������ ������������� ���� ������ ��������
  CDatabase cDatabase_ProjectList;//���� ������ ������ ��������
 public:
  //-����������� ������------------------------------------------------------
  CProjectDatabaseEngine_SQL(const CString &project_list_table_name);
  //-���������� ������-------------------------------------------------------
  ~CProjectDatabaseEngine_SQL();
  //-������� ������----------------------------------------------------------
  bool FindProjectByGUID(const CSafeString &guid,CProject &cProject);//����� ������ �� GUID
  list<CProject> GetAllProject(void);//�������� ��� �������
  bool AddProject(CProject &cProject);//�������� ������
  bool DeleteProject(const CProject &cProject);//������� ������
  bool ChangeProject(const CProject &cProject);//�������� ������
  void ResetBase(void);//�������� ����
};

#endif