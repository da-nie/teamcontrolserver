#ifndef CSAFE_STRING_H
#define CSAFE_STRING_H

//====================================================================================================
//��������
//====================================================================================================

//����� ������, �� ������������ Copy On Write � ���������� ��� ������������� � ������������� �����������

//====================================================================================================
//������������ ����������
//====================================================================================================

#include <string.h>

//====================================================================================================
//����� ������, �� ������������ Copy On Write � ���������� ��� ������������� � ������������� �����������
//====================================================================================================

class CSafeString
{
 protected:
  char *String_Ptr;//��������� �� ������
  unsigned long Size;//������ ������
 public:
  //�����������
  CSafeString(char* str="");
  CSafeString(const CSafeString& cSafeString);//����������� �����������
  //����������
  ~CSafeString();
 public:
  //-������� ������----------------------------------------------------------
  unsigned long GetLength(void) const;//�������� ������ ������
  long Compare(const CSafeString& cSafeString) const;//�������� ������
  CSafeString& operator=(const CSafeString& cSafeString);//�������� "="
  CSafeString& operator=(const char *str);//�������� "="
  friend CSafeString operator+(const CSafeString& cSafeString_Left,const CSafeString& cSafeString_Right);//�������� "+"
  CSafeString& operator+=(const CSafeString& cSafeString);//�������� "+="
  CSafeString& operator+=(const char *str);//�������� "+="
  CSafeString& operator+=(const char symbol);//�������� "+="
  operator const char*(void) const;//�������� "(const char *)"
};

#endif
