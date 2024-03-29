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

#include <memory>



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

  std::shared_ptr<CRAIICDatabase> cRAIICDatabase_Ptr;//��������� �� ����� ������� ���� ������
 public:
  //-����������� ������------------------------------------------------------
  CTaskDatabaseEngine_SQL(const CString &task_list_table_name);
  //-���������� ������-------------------------------------------------------
  ~CTaskDatabaseEngine_SQL();
  //-������� ������----------------------------------------------------------
  bool FindTaskByGUID(const CSafeString &guid,CTask &cTask) override;//����� ������� �� GUID
  std::list<CTask> GetAllTaskForUserGUID(const CSafeString &guid) override;//�������� ��� ������� ��� � �� ������������ � �������� GUID
  std::list<CTask> GetAllTask(void) override;//�������� ��� �������
  std::list<CTask> GetCommonTask(void) override;//�������� ��� ����� �������
  bool AddTask(CTask &cTask) override;//�������� �������
  bool DeleteTask(const CTask &cTask) override;//������� �������
  bool ChangeTask(const CTask &cTask,bool &for_user_change,bool &common_change,CTask &cTask_Deleted,CTask &cTask_Added) override;//�������� �������
  void ResetBase(void) override;//�������� ����
};

#endif