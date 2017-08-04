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
  bool FindProjectByGUID(const CString &guid,SProject &sProject);//����� ������ �� GUID
  list<SProject> GetAllProject(void);//�������� ��� �������
  bool AddProject(SProject &sProject);//�������� ������
  bool DeleteProject(const SProject &sProject);//������� ������
  bool ChangeProject(const SProject &sProject);//�������� ������
};

#endif