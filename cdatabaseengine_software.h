#ifndef CIDATABASE_ENGINE_SOFTWARE_H
#define CIDATABASE_ENGINE_SOFTWARE_H

//====================================================================================================
//��������
//====================================================================================================

//����� ������ � ������ ������ � �������������� ������������ ��������� ������ �������

//====================================================================================================
//������������ ����������
//====================================================================================================

#include "cidatabaseengine.h"

using namespace std;

//====================================================================================================
//���������
//====================================================================================================

//====================================================================================================
//����� ������ � ������ ������ � �������������� ������������ ��������� ������ �������
//====================================================================================================

class CDatabaseEngine_Software:public CIDatabaseEngine
{
 protected:
  //-���������� ������-------------------------------------------------------
  CString UserListTableName;//��� ������� ���� ������ ������ �������������
  CString TaskListTableName;//��� ������� ���� ������ ������ �����  
  CString ProjectListTableName;//��� ������� ���� ������ ������ ��������  

  CString UserListBaseInitString;//������ ������������� ���� ������ �������������
  CString TaskListBaseInitString;//������ ������������� ���� ������ �������
  CString ProjectListBaseInitString;//������ ������������� ���� ������ ��������

  CDatabase cDatabase_UserList;//���� ������ ������ �������������
  CDatabase cDatabase_TaskList;//���� ������ ������ �����
  CDatabase cDatabase_ProjectList;//���� ������ ������ ��������
 public:
  //-����������� ������------------------------------------------------------
  CDatabaseEngine_Software(const CString &user_list_table_name,const CString &task_list_table_name,const CString &project_list_table_name);
  //-���������� ������-------------------------------------------------------
  ~CDatabaseEngine_Software();
  //-������� ������----------------------------------------------------------
  bool AddUser(SUser& sUser);//�������� ������������
  bool ChangeUser(long index,const SUser& sUser);//������ ������������
  bool GetUser(long index,SUser &sUser);//�������� ������������ �� �������
  bool DeleteUser(long index);//������� ������������ �� �������
  list<SUser> GetAllUser(void);//�������� ������ ���� �������������
  bool FindUserByLoginAndPassword(const CString& login,const CString& password,SUser& sUser);//����� ������������ �� ������ � ������
  bool FindUserByGUID(const CString& guid,SUser& sUser);//����� ������������ �� GUID
  bool FindTaskByGUID(const CString &guid,STask &sTask);//����� ������� �� GUID
  bool FindProjectByGUID(const CString &guid,SProject &sProject);//����� ������ �� GUID
  list<STask> GetAllTaskForUserGUID(const CString &guid);//�������� ��� ������� ��� � �� ������������ � �������� GUID
  list<STask> GetAllTask(void);//�������� ��� �������
  list<SProject> GetAllProject(void);//�������� ��� �������
  bool AddTask(STask &sTask);//�������� �������
  bool DeleteTask(const STask &sTask);//������� �������
  bool ChangeTask(const STask &sTask,bool &for_user_change,STask &sTask_Deleted,STask &sTask_Added);//�������� �������
  bool AddProject(SProject &sProject);//�������� ������
  bool DeleteProject(const SProject &sProject);//������� ������
  bool ChangeProject(const SProject &sProject);//�������� ������
};

#endif