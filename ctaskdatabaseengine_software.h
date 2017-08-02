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
  bool FindTaskByGUID(const CString &guid,STask &sTask);//����� ������� �� GUID
  list<STask> GetAllTaskForUserGUID(const CString &guid);//�������� ��� ������� ��� � �� ������������ � �������� GUID
  list<STask> GetAllTask(void);//�������� ��� �������
  bool AddTask(STask &sTask);//�������� �������
  bool DeleteTask(const STask &sTask);//������� �������
  bool ChangeTask(const STask &sTask,bool &for_user_change,STask &sTask_Deleted,STask &sTask_Added);//�������� �������
};

#endif