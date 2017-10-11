#ifndef CTASK_H
#define CTASK_H

//====================================================================================================
//��������
//====================================================================================================

//����� �������

//====================================================================================================
//������������ ����������
//====================================================================================================
#include "stdafx.h"
#include "csafestring.h"
#include "cdate.h"
#include "protocol.h"

//====================================================================================================
//����� �������
//====================================================================================================
class CTask
{
 private:
  long Index;//������ �������
  CSafeString FromUserGUID;//�� ������������
  CSafeString ForUserGUID;//��� ������������
  CSafeString ProjectGUID;//�� �������
  CSafeString Task;//�������
  long State;//���������
  CDate cDate;//����
  CSafeString TaskGUID;//������������� �������
  CSafeString Answer;//����� �� �������
  bool AnswerNotRead;//�������� �� ����� �� �������
  bool PlannedPosition;//�������� �������
  bool AnswerReferenceExist;//���� ������ � ������ �����������
  bool TaskReferenceExist;//���� ������ � �������
  bool Common;//������� �������� �����
  CSafeString AnswerReference;//������ �� ���� � ������ �����������
  CSafeString TaskReference;//������ �� ���� � �������
  //������������ ����������
  bool ChangeCommonState;//��� ������ ������ �������� (����� ������, ����� �����, ����� ������ ���������� ���� ����� �������)

  //-���������� ������-------------------------------------------------------
 public:
  //-����������� ������------------------------------------------------------
  CTask(void);
  //-���������� ������-------------------------------------------------------
  ~CTask();
  //-������� ������----------------------------------------------------------
  const long& GetIndex(void) const;//������� ������
  const CSafeString& GetFromUserGUID(void) const;//�������� ���������� ������������� �� ������ ������������ �������
  const CSafeString& GetForUserGUID(void) const;//�������� ���������� ������������� ��� ������ ������������ �������
  const CSafeString& GetProjectGUID(void) const;//�������� ���������� ������������� �������
  const CSafeString& GetTask(void) const;//�������� �������
  const long& GetState(void) const;//������� ��������� �������
  const CDate& GetDate(void) const;//�������� ���� �������
  const CSafeString& GetTaskGUID(void) const;//�������� ���������� ������������� �������
  const CSafeString& GetAnswer(void) const;//�������� ����� �� �������
  const CSafeString& GetAnswerReference(void) const;//�������� ������ �� ���� � ������ �����������
  const CSafeString& GetTaskReference(void) const;//�������� ������ �� ���� � �������
  bool GetAnswerNotRead(void) const;//��������, �������� �� ����� �� �������  
  bool GetPlannedPosition(void) const;//��������, �������� �� ������� �������� ��������
  bool GetAnswerReferenceExist(void) const;//��������, ���� �� ������ � ������ �����������
  bool GetTaskReferenceExist(void) const;//��������, ���� �� ������ � �������
  bool GetCommon(void) const;//��������, �������� �� ������� �����

  void SetIndex(const long& index);//������ ������
  void SetFromUserGUID(const char *from_user_guid);//������ ���������� ������������� �� ������ ������������ �������
  void SetForUserGUID(const char *for_user_guid);//������ ���������� ������������� ��� ������ ������������ �������
  void SetProjectGUID(const char *project_guid);//������ ���������� ������������� �������
  void SetTask(const char *task);//������ �������
  void SetState(const long &state);//������� ��������� �������
  void SetDate(const CDate &cDate_Set);//������ ���� �������
  void SetTaskGUID(const char *task_guid);//������ ���������� ������������� �������
  void SetAnswer(const char *answer);//������ ����� �� �������
  void SetAnswerReference(const char *reference);//������ ������ �� ���� � ������ �����������
  void SetTaskReference(const char *reference);//������ ������ �� ���� � �������
  void SetAnswerNotRead(bool state);//������ �������� �� ����� �� �������
  void SetPlannedPosition(bool state);//������, �������� �� ������� �������� ��������
  void SetAnswerReferenceExist(bool state);//������, ���� �� ������ � ������ �����������
  void SetTaskReferenceExist(bool state);//������, ���� �� ������ � �������
  void SetCommon(bool state);//����������, �������� �� ������� �����
  void SetChangeCommonState(bool state);//����������, ��������� ������� �������� �������

  bool IsFromOrForUserGUID(const char *guid) const;//��� ������� �� ��� ��� ������������ � ���������� ���������������
  bool IsForUserGUID(const char *guid) const;//��� ������� ��� ������������ � ���������� ���������������
  bool IsTaskGUID(const char *guid) const;//������ �� TaskGUID
  bool IsAnswerNotRead(void) const;//�������� �� ����� �� �������
  bool IsPlannedPosition(void) const;//�������� �� ������� �������� ��������
  bool IsAnswerReferenceExist(void) const;//���� �� ������ � ������ �����������
  bool IsTaskReferenceExist(void) const;//���� �� ������ � �������
  bool IsCommon(void) const;//�������� �� ������� �����
  bool IsChangeCommonState(void) const;//��� �� ������ ������ �������� �������
};
#endif
