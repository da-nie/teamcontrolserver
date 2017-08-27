#ifndef CTRANSCEIVER_PROJECT_H
#define CTRANSCEIVER_PROJECT_H

//====================================================================================================
//��������
//====================================================================================================

//����� �����-����������� ������ ��������

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
//����� �����-����������� ������ ��������
//====================================================================================================

class CTransceiver_Project:public CTransceiver
{
 protected:
 public:  
  //�����������
  CTransceiver_Project(void);
  //����������
  ~CTransceiver_Project();
  //������� ������
  void SendProjectDataToClient(SClient &sClient,const CProject &cProject,CEvent &cEvent_Exit,bool &on_exit);//�������� ������� ������ �������
  void SendProjectDataToClientInPackage(SClient &sClient,const CProject &cProject,SERVER_ANSWER answer,SERVER_COMMAND command,CEvent &cEvent_Exit,bool &on_exit);//�������� ������� ������ ������� � ���� ������� ������
  bool ReadCProjectInArray(char *ptr,size_t &offset,size_t max_length,CProject &cProject);//��������� ������� �� ������� �������� ������
  void SendProjectBook(CDocument_Main *cDocument_Main_Ptr,SClient& sClient,SERVER_COMMAND command,CEvent &cEvent_Exit,bool &on_exit);//�������� ���� ������ ��������
  void AddProject(CDocument_Main *cDocument_Main_Ptr,SClient& sClient,SERVER_COMMAND command,CEvent &cEvent_Exit,bool &on_exit);//���������� ������ �������
  void DeleteProject(CDocument_Main *cDocument_Main_Ptr,SClient& sClient,SERVER_COMMAND command,CEvent &cEvent_Exit,bool &on_exit);//�������� �������
  void ChangeProject(CDocument_Main *cDocument_Main_Ptr,SClient& sClient,SERVER_COMMAND command,CEvent &cEvent_Exit,bool &on_exit);//��������� �������
};

#endif
