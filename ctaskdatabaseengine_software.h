#ifndef CTASKDATABASE_ENGINE_SOFTWARE_H
#define CTASKDATABASE_ENGINE_SOFTWARE_H

//====================================================================================================
//��������
//====================================================================================================

//����� ������ � ���� ������ ������� � �������������� ������������ ��������� ������ �������

//====================================================================================================
//������������ ����������
//====================================================================================================

#include "citaskdatabaseengine.h"

using namespace std;

//====================================================================================================
//���������
//====================================================================================================

//====================================================================================================
//����� ������ � ����� ������ ������� � �������������� ������������ ��������� ������ �������
//====================================================================================================

class CTaskDatabaseEngine_Software:public CITaskDatabaseEngine
{
 protected:
  //-���������� ������-------------------------------------------------------
  CString TaskListTableName;//��� ������� ���� ������ ������ �����  
  CString TaskListBaseInitString;//������ ������������� ���� ������ �������
  CDatabase cDatabase_TaskList;//���� ������ ������ �����
 public:
  //-����������� ������------------------------------------------------------
  CTaskDatabaseEngine_Software(const CString &task_list_table_name);
  //-���������� ������-------------------------------------------------------
  ~CTaskDatabaseEngine_Software();
  //-������� ������----------------------------------------------------------
  bool FindTaskByGUID(const CSafeString &guid,CTask &cTask) override;//����� ������� �� GUID
  list<CTask> GetAllTaskForUserGUID(const CSafeString &guid) override;//�������� ��� ������� ��� � �� ������������ � �������� GUID
  list<CTask> GetAllTask(void) override;//�������� ��� �������
  list<CTask> GetCommonTask(void) override;//�������� ����� �������
  bool AddTask(CTask &cTask) override;//�������� �������
  bool DeleteTask(const CTask &cTask) override;//������� �������
  bool ChangeTask(const CTask &cTask,bool &for_user_change,bool &common_change,CTask &cTask_Deleted,CTask &cTask_Added) override;//�������� �������
  void ResetBase(void) override;//�������� ����
};

#endif