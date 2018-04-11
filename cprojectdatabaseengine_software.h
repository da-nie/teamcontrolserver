#ifndef CPROJECTDATABASE_ENGINE_SOFTWARE_H
#define CPROJECTDATABASE_ENGINE_SOFTWARE_H

//====================================================================================================
//��������
//====================================================================================================

//����� ������ � ����� ������ �������� � �������������� ������������ ��������� ������ �������

//====================================================================================================
//������������ ����������
//====================================================================================================

#include "ciprojectdatabaseengine.h"

using namespace std;

//====================================================================================================
//���������
//====================================================================================================

//====================================================================================================
//����� ������ � ����� ������ �������� � �������������� ������������ ��������� ������ �������
//====================================================================================================

class CProjectDatabaseEngine_Software:public CIProjectDatabaseEngine
{
 protected:
  //-���������� ������-------------------------------------------------------
  CString ProjectListTableName;//��� ������� ���� ������ ������ ��������  
  CString ProjectListBaseInitString;//������ ������������� ���� ������ ��������
  CDatabase cDatabase_ProjectList;//���� ������ ������ ��������
 public:
  //-����������� ������------------------------------------------------------
  CProjectDatabaseEngine_Software(const CString &project_list_table_name);
  //-���������� ������-------------------------------------------------------
  ~CProjectDatabaseEngine_Software();
  //-������� ������----------------------------------------------------------
  bool FindProjectByGUID(const CSafeString &guid,CProject &cProject) override;//����� ������ �� GUID
  list<CProject> GetAllProject(void) override;//�������� ��� �������
  bool AddProject(CProject &cProject) override;//�������� ������
  bool DeleteProject(const CProject &cProject) override;//������� ������
  bool ChangeProject(const CProject &cProject) override;//�������� ������
  void ResetBase(void) override;//�������� ����
};

#endif