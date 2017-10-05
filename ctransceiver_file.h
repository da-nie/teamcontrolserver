#ifndef CTRANSCEIVER_FILE_H
#define CTRANSCEIVER_FILE_H

//====================================================================================================
//��������
//====================================================================================================

//����� �����-����������� ������

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
#include "crc.h"

using namespace std;

//====================================================================================================
//����� �����-����������� ������
//====================================================================================================

class CTransceiver_File:public CTransceiver
{
 protected:
 public:  
  //�����������
  CTransceiver_File(void);
  //����������
  ~CTransceiver_File();
  //������� ������
  void SendCRCClientProgrammFileToClient(SClient &sClient,const SCRC &sCRC,SERVER_COMMAND command,const CSafeString &programm_file_name,const CSafeString &loader_file_name,CEvent &cEvent_Exit,bool &on_exit);//�������� ������� ����������� ����� ����� ���������� ���������
  void SendClientProgrammAndLoaderFileToClient(SClient &sClient,const SCRC &sCRC,SERVER_COMMAND command,const CSafeString &programm_file_name,const CSafeString &loader_file_name,CEvent &cEvent_Exit,bool &on_exit);//�������� ������� ����
 protected:
};

#endif
