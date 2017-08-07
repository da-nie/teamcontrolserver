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
//� CRecordset RFX ����� ������ ������ � ������� ���������� ����� � ����! ����� � SQL ����� ������ � ��������.
//������� ������ �������� �������� ��������������� ����������! ������ �� ������ � � ������ SetRecord.
//���������������� ���������� �� ������ ���� ���������� �������� ������ DoFieldExchange - �.�. ��� � ������ ��������!

//====================================================================================================
//������������ ����������
//====================================================================================================

#include <list>
#include <CString>
#include "stdafx.h"
#include "craiiccriticalsection.h"
#include "cuserdatabaseengine_software.h"
#include "ctaskdatabaseengine_software.h"
#include "cprojectdatabaseengine_software.h"

#include "cuserdatabaseengine_sql.h"
#include "ctaskdatabaseengine_sql.h"
#include "cprojectdatabaseengine_sql.h"


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
   CIUserDatabaseEngine *cIUserDatabaseEngine_Ptr;//��������� �� ����� ������ � ����� ������ �������������
   CIProjectDatabaseEngine *cIProjectDatabaseEngine_Ptr;//��������� �� ����� ������ � ����� ������ ��������
   CITaskDatabaseEngine *cITaskDatabaseEngine_Ptr;//��������� �� ����� ������ � ����� ������ �������

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

  void ExportTaskBase(CString file_name);//������� ���� �������

  void ResetUserListBase(void);//�������� ���� �������������
  void ResetTaskListBase(void);//�������� ���� �������
  void ResetProjectListBase(void);//�������� ���� ��������
 protected:
  //-������� ������----------------------------------------------------------  
  bool CreateGUID(CString &cString_GUID);//������� GUID
  //-������------------------------------------------------------------------
  DECLARE_DYNCREATE(CDocument_Main) 
};

#endif