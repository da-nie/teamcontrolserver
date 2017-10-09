#ifndef CTRANSCEIVER_USER_H
#define CTRANSCEIVER_USER_H

//====================================================================================================
//��������
//====================================================================================================

//����� �����-����������� ������ �������������

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
//����� �����-����������� ������ �������������
//====================================================================================================

class CTransceiver_User:public CTransceiver
{
 protected:
  unsigned long Version;//����� ������ ���������
 public:  
  //�����������
  CTransceiver_User(void);
  //����������
  ~CTransceiver_User();
  //������� ������
  void SendUserDataToClient(const SClient &sClient,const CUser &cUser,CEvent &cEvent_Exit,bool &on_exit);//�������� ������� ������ ������������
  void SendUserDataToClientInPackage(const SClient &sClient,const CUser &cUser,SERVER_ANSWER answer,SERVER_COMMAND command,CEvent &cEvent_Exit,bool &on_exit);//�������� ������� ������ ������������ � ���� ������� ������
  void SendUserBook(CDocument_Main *cDocument_Main_Ptr,SClient& sClient,SERVER_COMMAND command,CEvent &cEvent_Exit,bool &on_exit);//�������� ���� ������ �������������

};

#endif
