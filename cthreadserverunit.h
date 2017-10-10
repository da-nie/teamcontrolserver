#ifndef THREAD_SERVER_UNIT_H
#define THREAD_SERVER_UNIT_H

//====================================================================================================
//��������
//====================================================================================================

//����� ���������� ������ ��� ������ �������

//====================================================================================================
//������������ ����������
//====================================================================================================
#include "stdafx.h"
#include <list>
#include <vector>
#include <string>
#include "cdocument_main.h"
#include "protocol.h"
#include "craiiccriticalsection.h"
#include "ctransceiver_user.h"
#include "ctransceiver_task.h"
#include "ctransceiver_project.h"
#include "ctransceiver_autorization.h"
#include "ctransceiver_file.h"
#include "ctransceiver_ping.h"

using namespace std;

//====================================================================================================
//����� ���������� ������ ��� ������ �������
//====================================================================================================

class CThreadServerUnit
{
 //������������� �������
 friend UINT ThreadServerUnit(LPVOID pParam);//������� ������
 protected:
  CEvent cEvent_Exit;//���������� �� ����� �� ������
  CWinThread *cWinThread_Thread;//����� ���������
  char *Buffer;//����� �����/��������
  SClient sClient_My;//������
  CDocument_Main *cDocument_Main_Ptr;//��������� �� ��������
  
  //���������� ���������� ������
  struct SProtectedVariables
  {
   //������ ��� ������������� � ���������
   vector<CUser> vector_CUser_Deleted;//�������� ������������
   vector<CUser> vector_CUser_Changed;//��������� ������������
   vector<CUser> vector_CUser_Added;//����������� ������������

   vector<CTask> vector_CTask_Deleted;//�������� �������
   vector<CTask> vector_CTask_Changed;//��������� ������� 
   vector<CTask> vector_CTask_Added;//����������� �������

   vector<CProject> vector_CProject_Deleted;//�������� �������
   vector<CProject> vector_CProject_Changed;//��������� �������
   vector<CProject> vector_CProject_Added;//����������� �������

   bool ClientOnLine;//��������� �� ������ (�������� �� �����)

   bool SendPing;//��������� �� ������� ��������� ��� �������� �����

   CCriticalSection cCriticalSection;//����������� ������ ��� �������
  } sProtectedVariables;

  CTransceiver_Ping cTransceiver_Ping;//����� �����-����������� ������ ��������� ��� �������� �����
  CTransceiver_User cTransceiver_User;//����� �����-����������� ������ ������������
  CTransceiver_Task cTransceiver_Task;//����� �����-����������� ������ �������
  CTransceiver_Project cTransceiver_Project;//����� �����-����������� ������ ��������
  CTransceiver_File cTransceiver_File;//����� �����-����������� ������ ���������
  CTransceiver_Autorization cTransceiver_Autorization;//����� �����-����������� ������ �����������
 public:  
  //�����������
  CThreadServerUnit(void);
  //����������
  ~CThreadServerUnit();
   void Start(SOCKET socket_client);//��������� �����
   void Stop(void);//���������� �����
   void SetDocument(CDocument_Main *cDocument_Main_Set_Ptr);//������ ����� ���������
   void InitClient(SOCKET socket_client);//���������������� �������
   void OnUserAdded(const CUser &cUser);//��� �������� ������������
   void OnUserDeleted(const CUser &cUser);//��� ����� ������������
   void OnUserChanged(const CUser &cUser);//���� �������� ������ ������������

   void OnTaskAdded(const CTask &cTask);//���� ��������� �������
   void OnTaskDeleted(const CTask &cTask);//���� ������� �������
   void OnTaskChanged(const CTask &cTask);//���� �������� ������ �������

   void OnProjectAdded(const CProject &cProject);//��� �������� ������
   void OnProjectDeleted(const CProject &cProject);//��� ������ ������
   void OnProjectChanged(const CProject &cProject);//���� �������� ������ �������

   void SendPing(void);//������� ������� ��������� ��� �������� �����

   bool GetClientOnLine(void);//��������, � ���� �� ������   
 protected:
  void ClientDisconnect(const SClient& sClient);//���������� �� �������
  void SetClientOnLine(bool state);//������, � ���� �� ������

  void Processing(void);//�������� ���� ���������
  void LinkProcessing(SClient &sClient,bool &on_exit);//��������� �����

  void NewDataFromClient(SClient& sClient,char *data,unsigned long length,bool &on_exit);//������� ������ �� �������
  void ExecuteCommand_GetClientProgrammCRC(SClient& sClient,SERVER_COMMAND command,bool &on_exit);//��������� ������� ��������� CRC ���������� ���������
  void ExecuteCommand_GetClientProgrammAndLoader(SClient& sClient,SERVER_COMMAND command,bool &on_exit);//��������� ������� ��������� ������ ���������� ��������� � ����������
  void ExecuteCommand_Autorization(SClient& sClient,SERVER_COMMAND command,bool &on_exit);//��������� ������� �����������
  void ExecuteCommand_GetUserBook(SClient& sClient,SERVER_COMMAND command,bool &on_exit);//��������� ������� ��������� ������ �������������
  void ExecuteCommand_GetTaskBook(SClient& sClient,SERVER_COMMAND command,bool &on_exit);//��������� ������� ��������� ������� ������������
  void ExecuteCommand_GetProjectBook(SClient& sClient,SERVER_COMMAND command,bool &on_exit);//��������� ������� ��������� ��������
  void ExecuteCommand_GetCommonTaskBook(SClient& sClient,SERVER_COMMAND command,bool &on_exit);//��������� ������� ��������� ����� �������
  void ExecuteCommand_AddedTask(SClient& sClient,SERVER_COMMAND command,bool &on_exit);//��������� ������� ���������� ������ �������
  void ExecuteCommand_DeletedTask(SClient& sClient,SERVER_COMMAND command,bool &on_exit);//��������� ������� �������� �������
  void ExecuteCommand_ChangedTask(SClient& sClient,SERVER_COMMAND command,bool &on_exit);//��������� ������� ��������� �������
  void ExecuteCommand_AddedProject(SClient& sClient,SERVER_COMMAND command,bool &on_exit);//��������� ������� ���������� ������ �������
  void ExecuteCommand_DeletedProject(SClient& sClient,SERVER_COMMAND command,bool &on_exit);//��������� ������� �������� �������
  void ExecuteCommand_ChangedProject(SClient& sClient,SERVER_COMMAND command,bool &on_exit);//��������� ������� ��������� �������
};

#endif



