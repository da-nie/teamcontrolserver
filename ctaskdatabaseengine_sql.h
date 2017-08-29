#ifndef CTASKDATABASE_ENGINE_SQL_H
#define CTASKDATABASE_ENGINE_SQL_H

//====================================================================================================
//��������
//====================================================================================================

//����� ������ � ���� ������ ������� � �������������� SQL
//====================================================================================================
//������������ ����������
//====================================================================================================

#include "citaskdatabaseengine.h"
#include "cdatabaseengine_sql.h"
#include "craiicrecordset.h"

using namespace std;

//====================================================================================================
//���������
//====================================================================================================

//====================================================================================================
//����� ������ � ����� ������ ������� � �������������� SQL
//====================================================================================================

class CTaskDatabaseEngine_SQL:public CITaskDatabaseEngine
{
 protected:
  //-���������� ������-------------------------------------------------------
  CDatabaseEngine_SQL<CRAIICRecordset<CRecordset_TaskList>,CTask> *cDatabaseEngine_SQL_Ptr;//����� ��� ������ � ����� ������
  CString TaskListTableName;//��� ������� ���� ������ ������ �����  
  CString TaskListBaseInitString;//������ ������������� ���� ������ �������
  CDatabase cDatabase_TaskList;//���� ������ ������ �����
 public:
  //-����������� ������------------------------------------------------------
  CTaskDatabaseEngine_SQL(const CString &task_list_table_name);
  //-���������� ������-------------------------------------------------------
  ~CTaskDatabaseEngine_SQL();
  //-������� ������----------------------------------------------------------
  bool FindTaskByGUID(const CSafeString &guid,CTask &cTask);//����� ������� �� GUID
  list<CTask> GetAllTaskForUserGUID(const CSafeString &guid);//�������� ��� ������� ��� � �� ������������ � �������� GUID
  list<CTask> GetAllTask(void);//�������� ��� �������
  bool AddTask(CTask &cTask);//�������� �������
  bool DeleteTask(const CTask &cTask);//������� �������
  bool ChangeTask(const CTask &cTask,bool &for_user_change,CTask &cTask_Deleted,CTask &cTask_Added);//�������� �������
  void ResetBase(void);//�������� ����
};

#endif