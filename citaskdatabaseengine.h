#ifndef CITASKDATABASE_ENGINE_H
#define CITASKDATABASE_ENGINE_H

//====================================================================================================
//��������
//====================================================================================================

//����������� ����� ������ � ����� ������ �������

//====================================================================================================
//������������ ����������
//====================================================================================================

#include <list>
#include "stdafx.h"
#include "craiicdatabase.h"
#include "craiicrecordset.h"
#include "crecordset_tasklist.h"

using namespace std;

//====================================================================================================
//���������
//====================================================================================================

//====================================================================================================
//����������� ����� ������ � ����� ������ �������
//====================================================================================================

class CITaskDatabaseEngine
{
 public:
  //-���������� ������-------------------------------------------------------
  virtual ~CITaskDatabaseEngine() {};
  //-������� ������----------------------------------------------------------
  virtual bool FindTaskByGUID(const CSafeString &guid,CTask &cTask)=0;//����� ������� �� GUID
  virtual list<CTask> GetAllTaskForUserGUID(const CSafeString &guid)=0;//�������� ��� ������� ��� � �� ������������ � �������� GUID
  virtual list<CTask> GetAllTask(void)=0;//�������� ��� �������
  virtual list<CTask> GetCommonTask(void)=0;//�������� ��� ����� �������
  virtual bool AddTask(CTask &cTask)=0;//�������� �������
  virtual bool DeleteTask(const CTask &cTask)=0;//������� �������
  virtual bool ChangeTask(const CTask &cTask,bool &for_user_change,bool &common_change,CTask &cTask_Deleted,CTask &cTask_Added)=0;//�������� �������
  virtual void ResetBase(void)=0;//�������� ����
};

#endif