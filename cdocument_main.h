#ifndef CDOCUMENT_MAIN_H
#define CDOCUMENT_MAIN_H

//====================================================================================================
//��������
//====================================================================================================

//����� ���������� ��� ������ �������������.

//��������!!!
//���� ������ ������ ���� ������� ������ ��� Microsoft Paradox Driver (*.db ) � �� ��� �����!
//������ ���� � ����� Paradox �������� �� ����� - ����� ������ �������!!!
//����������� ������� ����� - ��� ���� �� ������� ������ �������� � �����.
//��� ����� � ������� ������ ���� �� ���� 8 �������� (���� � ������� �� ����������� BDE).
//����� �������� ������� Microsoft �� ����� ��� ���� �������. GetShortPathName ������ �� ������ ���
//������� ���, �� ���� �� ������� � � ������� ������������ � ����.

//====================================================================================================
//������������ ����������
//====================================================================================================

#include <list>
#include <CString>
#include "stdafx.h"
#include "craiiccriticalsection.h"
#include "craiicdatabase.h"
#include "crecordset_userlist.h"
#include "crecordset_tasklist.h"
#include "crecordset_projectlist.h"

using namespace std;

//====================================================================================================
//���������
//====================================================================================================

//��������� �������
struct SServerSettings
{
 unsigned short Port;//���� �������
};

//====================================================================================================
//����� ����������
//====================================================================================================

class CDocument_Main:public CDocument 
{
 protected:
  //-���������� ������-------------------------------------------------------

  //���������� ���������� ������
  struct SProtectedVariables
  {
   CString UserListTableName;//��� ������� ���� ������ ������ �������������
   CString TaskListTableName;//��� ������� ���� ������ ������ �����  
   CString ProjectListTableName;//��� ������� ���� ������ ������ ��������  

   CString UserListBaseInitString;//������ ������������� ���� ������ �������������
   CString TaskListBaseInitString;//������ ������������� ���� ������ �������
   CString ProjectListBaseInitString;//������ ������������� ���� ������ ��������

   CDatabase cDatabase_UserList;//���� ������ ������ �������������
   CDatabase cDatabase_TaskList;//���� ������ ������ �����
   CDatabase cDatabase_ProjectList;//���� ������ ������ ��������

   SServerSettings sServerSettings;//��������� �������
   CCriticalSection cCriticalSection;//����������� ������ ��� ������� � ������
  } sProtectedVariables;
 public:
  //-����������� ������------------------------------------------------------
  CDocument_Main(void);
  //-���������� ������-------------------------------------------------------
  ~CDocument_Main();
  //-������� ������----------------------------------------------------------
  void SaveState(void);//���������� ���������
  bool AddUser(SUser& sUser);//�������� ������������
  bool ChangeUser(long index,const SUser& sUser);//������ ������������
  bool GetUser(long index,SUser &sUser);//�������� ������������ �� �������
  void DeleteUser(long index);//������� ������������ �� �������
  list<SUser> GetAllUser(void);//�������� ������ ���� �������������
  bool FindUserByLoginAndPassword(const CString& login,const CString& password,SUser& sUser);//����� ������������ �� ������ � ������
  bool FindUserByGUID(const CString& guid,SUser& sUser);//����� ������������ �� GUID
  bool FindTaskByGUID(const CString &guid,STask &sTask);//����� ������� �� GUID
  bool FindProjectByGUID(const CString &guid,SProject &sProject);//����� ������ �� GUID
  void GetServerSettings(SServerSettings &sServerSettings);//�������� ��������� �������
  void SetServerSettings(const SServerSettings &sServerSettings);//���������� ��������� ������� (������ ����� �����������)
  list<STask> GetAllTaskForUserGUID(const CString &guid);//�������� ��� ������� ��� � �� ������������ � �������� GUID
  list<STask> GetAllTask(void);//�������� ��� �������
  list<SProject> GetAllProject(void);//�������� ��� �������

  bool AddTask(STask &sTask);//�������� �������
  bool DeleteTask(const STask &sTask);//������� �������
  bool ChangeTask(const STask &sTask);//�������� �������

  bool AddProject(SProject &sProject);//�������� ������
  bool DeleteProject(const SProject &sProject);//������� ������
  bool ChangeProject(const SProject &sProject);//�������� ������
 protected:
  //-������� ������----------------------------------------------------------  
  bool CreateGUID(CString &cString_GUID);//������� GUID
  //-������------------------------------------------------------------------
  DECLARE_DYNCREATE(CDocument_Main) 
};

#endif