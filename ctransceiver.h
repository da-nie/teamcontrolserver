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



//====================================================================================================
//���������
//====================================================================================================

//������
struct SClient
{
 SOCKET Socket;//����� �������
 CSafeString Login;//�����
 CSafeString Password;//������
 CSafeString UserGUID;//����������� �� ����������� ��������� �� ������ � ������ GUID 
 bool StuffingEnabled;//������� �� ������������
 std::vector<unsigned char> vector_Data;//�������� ������
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
  void SetString(CSafeString &text,char *buffer,long length);//������ ������ �� �������
  void SendBeginPackage(SOCKET socket_client,CEvent &cEvent_Exit,bool &on_exit);//��������� ��� ������ ������
  void SendEndPackage(SOCKET socket_client,CEvent &cEvent_Exit,bool &on_exit);//��������� ��� ����� ������
  void SendAnswer(SOCKET socket_client,SERVER_ANSWER answer,SERVER_COMMAND command,const char *data,unsigned long size,CEvent &cEvent_Exit,bool &on_exit);//��������� ����� �������
  void SendPart(SOCKET socket_client,const char *data,unsigned long size,CEvent &cEvent_Exit,bool &on_exit);//��������� ������ ������� � ����������� �������������
  void SendData(SOCKET socket_client,const char *package,long size,CEvent &cEvent_Exit,bool &on_exit);//��������� ������ �������
  void Pause(long time_ms);//�����

};

#endif



