#ifndef CTRANSCEIVER_H
#define CTRANSCEIVER_H

//====================================================================================================
//��������
//====================================================================================================

//����� �����-�����������

//====================================================================================================
//������������ ����������
//====================================================================================================
#include "stdafx.h"
#include <list>
#include <vector>
#include <string>
#include "cdocument_main.h"
#include "protocol.h"

using namespace std;

//====================================================================================================
//���������
//====================================================================================================

//������
struct SClient
{
 SOCKET Socket;//����� �������
 CString Login;//�����
 CString Password;//������
 CString UserGUID;//����������� �� ����������� ��������� �� ������ � ������ GUID
 bool StuffingEnabled;//������� �� ������������
 vector<unsigned char> vector_Data;//�������� ������
};

//====================================================================================================
//����� �����-�����������
//====================================================================================================

class CTransceiver
{
 protected:
 public:  
  //�����������
  CTransceiver(void);
  //����������
  ~CTransceiver();
 protected:
  void SetString(CString &text,char *buffer,long length);//������ ������ �� �������
  void SendBeginPackage(SOCKET socket_client,CEvent &cEvent_Exit,bool &on_exit);//��������� ��� ������ ������
  void SendEndPackage(SOCKET socket_client,CEvent &cEvent_Exit,bool &on_exit);//��������� ��� ����� ������
  void SendAnswer(SOCKET socket_client,SERVER_ANSWER answer,SERVER_COMMAND command,const char *data,unsigned long size,CEvent &cEvent_Exit,bool &on_exit);//��������� ����� �������
  void SendPart(SOCKET socket_client,const char *data,unsigned long size,CEvent &cEvent_Exit,bool &on_exit);//��������� ������ ������� � ����������� �������������
  void SendData(SOCKET socket_client,const char *package,long size,CEvent &cEvent_Exit,bool &on_exit);//��������� ������ �������
  void Pause(long time_ms);//�����

};

#endif



