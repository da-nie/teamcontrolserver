#ifndef CTRANSCEIVER_TASK_H
#define CTRANSCEIVER_TASK_H

//====================================================================================================
//��������
//====================================================================================================

//����� �����-����������� ������ �������

//====================================================================================================
//������������ ����������
//====================================================================================================
#include "stdafx.h"
#include <list>
#include <vector>
#include <string>
#include "cdocument_main.h"
#include "protocol.h"
#include "ctransceiver.h"

using namespace std;

//====================================================================================================
//����� �����-����������� ������ �������
//====================================================================================================

class CTransceiver_Task:public CTransceiver
{
 protected:
  unsigned long Version;//����� ������ ���������
 public:  
  //�����������
  CTransceiver_Task(void);
  //����������
  ~CTransceiver_Task();
  //������� ������
  void SendTaskDataToClient(SClient &sClient,const CTask &cTask,CEvent &cEvent_Exit,bool &on_exit);//�������� ������� ������ �������
  void SendTaskDataToClientInPackage(SClient &sClient,const CTask &cTask,SERVER_ANSWER answer,SERVER_COMMAND command,CEvent &cEvent_Exit,bool &on_exit);//�������� ������� ������ ������� � ���� ������� ������
  bool ReadCTaskInArray(char *ptr,size_t &offset,size_t max_length,CTask &cTask);//��������� ������� �� ������� �������� ������
  void SendTaskBook(CDocument_Main *cDocument_Main_Ptr,SClient& sClient,SERVER_COMMAND command,CEvent &cEvent_Exit,bool &on_exit);//�������� ���� ������ �������
  void SendCommonTaskBook(CDocument_Main *cDocument_Main_Ptr,SClient& sClient,SERVER_COMMAND command,CEvent &cEvent_Exit,bool &on_exit);//�������� ���� ������ ����� �������
  void AddTask(CDocument_Main *cDocument_Main_Ptr,SClient& sClient,SERVER_COMMAND command,CEvent &cEvent_Exit,bool &on_exit);//���������� ������ �������
  void DeleteTask(CDocument_Main *cDocument_Main_Ptr,SClient& sClient,SERVER_COMMAND command,CEvent &cEvent_Exit,bool &on_exit);//�������� �������
  void ChangeTask(CDocument_Main *cDocument_Main_Ptr,SClient& sClient,SERVER_COMMAND command,CEvent &cEvent_Exit,bool &on_exit);//��������� �������
 protected:
  void SendTaskList(list<CTask> &list_CTask,SClient& sClient,SERVER_COMMAND command,SERVER_ANSWER answer,CEvent &cEvent_Exit,bool &on_exit);//�������� ������ �������
};

#endif
