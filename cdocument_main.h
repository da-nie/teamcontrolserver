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
#include "stdafx.h"
#include "craiiccriticalsection.h"
#include "cuserdatabaseengine_software.h"
#include "ctaskdatabaseengine_software.h"
#include "cprojectdatabaseengine_software.h"

#include "cuserdatabaseengine_sql.h"
#include "ctaskdatabaseengine_sql.h"
#include "cprojectdatabaseengine_sql.h"

#include "ctaskexporthtml.h"
#include "ctaskexporttxt.h"
#include "ctaskexportcsv.h"

#include "crc.h"



//====================================================================================================
//���������
//====================================================================================================

//��������� �������
struct SServerSettings
{
 unsigned short Port;//���� �������
};

//��������� CRC
struct SCRC
{
 unsigned short CRC16_Programm;//CRC ���������
 unsigned short CRC16_Loader;//CRC ����������
 bool EnabledCRCProgramm;//���� CRC ���������
 bool EnabledCRCLoader;//���� CRC ����������
};

//������������ ������������
struct SConnected
{
 CSafeString GUID;//������������� ������������� ������������
 bool Connected;//���������
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
   SCRC sCRC;//����������� ����� ��������� � ����������

   std::list<SConnected> list_SConnected;//������������ ������������
   bool ChangeConnectedList;//��������� �� ������ ����������� �������������

   CCriticalSection cCriticalSection;//����������� ������ ��� ������� � ������
  } sProtectedVariables;

  CITaskExport *cITaskExport_Ptr;//��������� �� ����� �������� �������
 public:
  //-����������� ������------------------------------------------------------
  CDocument_Main(void);
  //-���������� ������-------------------------------------------------------
  ~CDocument_Main();
  //-������� ������----------------------------------------------------------
  std::list<SConnected> GetConnectedList(void);//�������� ������ ������������ �������������
  bool GetChangeConnectedListAndResetState(void);//��������, ��������� �� ������ ������������ ������������� � �������� ��������
  void SaveState(void);//���������� ���������
  bool AddUser(CUser& cUser);//�������� ������������
  bool ChangeUser(long index,const CUser& cUser);//������ ������������
  bool GetUser(long index,CUser &cUser);//�������� ������������ �� �������
  void DeleteUser(long index);//������� ������������ �� �������
  std::list<CUser> GetAllUser(void);//�������� ������ ���� �������������
  bool FindUserByLoginAndPassword(const CSafeString& login,const CSafeString& password,CUser& cUser);//����� ������������ �� ������ � ������
  bool FindUserByGUID(const CSafeString& guid,CUser& cUser);//����� ������������ �� GUID
  bool ChangeUserByGUID(const CSafeString& guid,const CUser& cUser);//�������� ������������ �� GUID
  bool DeleteUserByGUID(const CSafeString& guid);//������� ������������ �� GUID
  bool FindTaskByGUID(const CSafeString &guid,CTask &cTask);//����� ������� �� GUID
  bool FindProjectByGUID(const CSafeString &guid,CProject &cProject);//����� ������ �� GUID
  void GetCRC(SCRC &sCRC);//�������� CRC ��������� � ����������
  void GetServerSettings(SServerSettings &sServerSettings);//�������� ��������� �������
  void SetServerSettings(const SServerSettings &sServerSettings);//���������� ��������� ������� (������ ����� �����������)
  std::list<CTask> GetAllTaskForUserGUID(const CSafeString &guid);//�������� ��� ������� ��� � �� ������������ � �������� GUID
  std::list<CTask> GetCommonTask();//�������� ��� ����� �������
  std::list<CTask> GetAllTask(void);//�������� ��� �������
  std::list<CProject> GetAllProject(void);//�������� ��� �������

  bool AddTask(CTask &cTask);//�������� �������
  bool DeleteTask(const CTask &cTask);//������� �������
  bool ChangeTask(CTask &cTask);//�������� �������

  bool AddProject(CProject &cProject);//�������� ������
  bool DeleteProject(const CProject &cProject);//������� ������
  bool ChangeProject(const CProject &cProject);//�������� ������

  void ExportTaskBase(const CString &file_name);//������� ���� �������

  void ResetUserListBase(void);//�������� ���� �������������
  void ResetTaskListBase(void);//�������� ���� �������
  void ResetProjectListBase(void);//�������� ���� ��������

  void SetUserConnected(const CSafeString& guid,bool connected);//������, � ���� �� ������������

  void SendPing(void);//��������� ������������� ������ �������� �����
  void BackUpAllDatabase(void);//��������� ��������� ����������� ��� ������
  void SaveCRC(void);//��������� CRC ��������� � ����������
 protected:
  //-������� ������----------------------------------------------------------  
  bool CreateGUID(CSafeString &cSafeString_GUID);//������� GUID
  void Execute(const char *Name,const char *Param,const char *Directory);//��������� �� ���������� ����
  //-������------------------------------------------------------------------
  DECLARE_DYNCREATE(CDocument_Main) 
};

#endif