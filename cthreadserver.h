#ifndef THREAD_SERVER_H
#define THREAD_SERVER_H

//====================================================================================================
//��������
//====================================================================================================

//����� ���������� ������

//====================================================================================================
//������������ ����������
//====================================================================================================
#include "stdafx.h"
#include <list>
#include <vector>
#include <string>
#include "cdocument_main.h"
#include "protocol.h"
#include "cthreadserverunit.h"
#include "craiiccriticalsection.h"

using namespace std;

//====================================================================================================
//����� ���������� ������
//====================================================================================================

class CThreadServer
{
 //������������� �������
 friend UINT ThreadServer(LPVOID pParam);//������� ������
 protected:
  CEvent cEvent_Exit;//���������� �� ����� �� ������
  CWinThread *cWinThread_Thread;//����� ���������
  list<CThreadServerUnit*> list_CThreadServerUnitPtr;//������ ���������� �� ������ ��������� ��������
  CDocument_Main *cDocument_Main_Ptr;//��������� �� ��������
 public:  
  //�����������
  CThreadServer(void);
  //����������
  ~CThreadServer();
   void Start(void);//��������� �����
   void Stop(void);//���������� �����
   void SetDocument(CDocument_Main *cDocument_Main_Set_Ptr);//������ ����� ���������
   void OnUserAdded(const SUser &sUser);//��� �������� ������������
   void OnUserDeleted(const SUser &sUser);//��� ����� ������������
   void OnUserChanged(const SUser &sUser);//���� �������� ������ ������������

   void OnTaskAdded(const STask &sTask);//���� ��������� �������
   void OnTaskDeleted(const STask &sTask);//���� ������� �������
   void OnTaskChanged(const STask &sTask);//���� �������� ������ �������

   void OnProjectAdded(const SProject &sProject);//��� �������� ������
   void OnProjectDeleted(const SProject &sProject);//��� ������ ������
   void OnProjectChanged(const SProject &sProject);//���� �������� ������ �������

 protected:
  SOCKET CreateServer(void);//�������� �������
  void DisconnectAllClientAndResetClientList(void);//���������� ���� �������� � ������� ������ ���� ��������
  void NewClient(SOCKET socket_server);//����������� ������ �������
  void Processing(void);//�������� ���� ���������
};

#endif



