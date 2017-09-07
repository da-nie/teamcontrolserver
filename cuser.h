#ifndef CUSER_H
#define CUSER_H

//====================================================================================================
//��������
//====================================================================================================

//����� ������������

//====================================================================================================
//������������ ����������
//====================================================================================================
#include "stdafx.h"
#include "csafestring.h"

//====================================================================================================
//����� ������������
//====================================================================================================
class CUser
{
 private:
  CSafeString Name;//���
  CSafeString Login;//�����
  CSafeString Password;//������
  CSafeString JobTitle;//���������
  CSafeString Description;//��������
  CSafeString Telephone;//�������
  CSafeString UserGUID;//���������� ������������� ������������
  bool Leader;//�������� �� �������������  
  //-���������� ������-------------------------------------------------------
 public:
  //-����������� ������------------------------------------------------------
  CUser(void);
  //-���������� ������-------------------------------------------------------
  ~CUser();
  //-������� ������----------------------------------------------------------
  const CSafeString& GetName(void) const;//�������� ���
  const CSafeString& GetLogin(void) const;//�������� �����
  const CSafeString& GetPassword(void) const;//�������� ������
  const CSafeString& GetJobTitle(void) const;//�������� ���������
  const CSafeString& GetDescription(void) const;//�������� ��������
  const CSafeString& GetTelephone(void) const;//�������� �������
  const CSafeString& GetUserGUID(void) const;//�������� ���������� ������������� ������������
  const bool& GetLeader(void) const;//��������, �������� �� �������������

  void SetName(const char *name);//������ ���
  void SetLogin(const char *login);//������ �����
  void SetPassword(const char *password);//������ ������
  void SetJobTitle(const char *job_title);//������ ���������
  void SetDescription(const char *description);//������ ��������
  void SetTelephone(const char *telephone);//������ �������
  void SetUserGUID(const char *user_guid);//������ ���������� ������������� ������������
  void SetLeader(bool leader);//������, �������� �� �������������

  bool IsLoginAndPassword(const char *login,const char *password) const;//������ �� ����� � ������
  bool IsUserGUID(const char *guid);//������ �� UserGUID
};
#endif
