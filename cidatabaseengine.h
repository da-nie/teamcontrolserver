#ifndef CIDATABASE_ENGINE_H
#define CIDATABASE_ENGINE_H

//====================================================================================================
//��������
//====================================================================================================

//����������� ����� ������ � ������ ������

//====================================================================================================
//������������ ����������
//====================================================================================================

#include <list>
#include <CString>
#include "stdafx.h"
#include "craiicdatabase.h"
#include "crecordset_userlist.h"
#include "crecordset_tasklist.h"
#include "crecordset_projectlist.h"

using namespace std;

//====================================================================================================
//���������
//====================================================================================================

//====================================================================================================
//����������� ����� ������ � ������ ������
//====================================================================================================

class CIDatabaseEngine
{
 public:
  //-���������� ������-------------------------------------------------------
  virtual ~CIDatabaseEngine() {};
  //-������� ������----------------------------------------------------------
  virtual bool AddUser(SUser& sUser)=0;//�������� ������������
  virtual bool ChangeUser(long index,const SUser& sUser)=0;//������ ������������
  virtual bool GetUser(long index,SUser &sUser)=0;//�������� ������������ �� �������
  virtual bool DeleteUser(long index)=0;//������� ������������ �� �������
  virtual list<SUser> GetAllUser(void)=0;//�������� ������ ���� �������������
  virtual bool FindUserByLoginAndPassword(const CString& login,const CString& password,SUser& sUser)=0;//����� ������������ �� ������ � ������
  virtual bool FindUserByGUID(const CString& guid,SUser& sUser)=0;//����� ������������ �� GUID
  virtual bool FindTaskByGUID(const CString &guid,STask &sTask)=0;//����� ������� �� GUID
  virtual bool FindProjectByGUID(const CString &guid,SProject &sProject)=0;//����� ������ �� GUID
  virtual list<STask> GetAllTaskForUserGUID(const CString &guid)=0;//�������� ��� ������� ��� � �� ������������ � �������� GUID
  virtual list<STask> GetAllTask(void)=0;//�������� ��� �������
  virtual list<SProject> GetAllProject(void)=0;//�������� ��� �������
  virtual bool AddTask(STask &sTask)=0;//�������� �������
  virtual bool DeleteTask(const STask &sTask)=0;//������� �������
  virtual bool ChangeTask(const STask &sTask,bool &for_user_change,STask &sTask_Deleted,STask &sTask_Added)=0;//�������� �������
  virtual bool AddProject(SProject &sProject)=0;//�������� ������
  virtual bool DeleteProject(const SProject &sProject)=0;//������� ������
  virtual bool ChangeProject(const SProject &sProject)=0;//�������� ������
};

#endif