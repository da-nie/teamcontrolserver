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
  std::list<CThreadServerUnit*> list_CThreadServerUnitPtr;//������ ���������� �� ������ ��������� ��������
  CDocument_Main *cDocument_Main_Ptr;//��������� �� ��������
 public:  
  //�����������
  CThreadServer(void);
  //����������
  ~CThreadServer();
   void Start(void);//��������� �����
   void Stop(void);//���������� �����
   void SetDocument(CDocument_Main *cDocument_Main_Set_Ptr);//������ ����� ���������
   void OnUserAdded(const CUser &cUser);//��� �������� ������������
   void OnUserDeleted(const CUser &cUser);//��� ����� ������������
   void OnUserChanged(const CUser &cUser);//���� �������� ������ ������������

   void OnTaskAdded(const CTask &cTask);//���� ��������� �������
   void OnTaskDeleted(const CTask &cTask);//���� ������� �������
   void OnTaskChanged(const CTask &cTask);//���� �������� ������ �������

   void OnProjectAdded(const CProject &cProject);//��� �������� ������
   void OnProjectDeleted(const CProject &cProject);//��� ������ ������
   void OnProjectChanged(const CProject &cProject);//���� �������� ������ �������

   void SendPing(void);//�������� ������������� ��������� ��� �������� �����

 protected:
  SOCKET CreateServer(void);//�������� �������
  void DisconnectAllClientAndResetClientList(void);//���������� ���� �������� � ������� ������ ���� ��������
  void NewClient(SOCKET socket_server);//����������� ������ �������
  void Processing(void);//�������� ���� ���������
};

#endif



