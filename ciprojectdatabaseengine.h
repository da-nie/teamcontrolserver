#ifndef CIPROJECTDATABASE_ENGINE_H
#define CIPROJECTDATABASE_ENGINE_H

//====================================================================================================
//��������
//====================================================================================================

//����������� ����� ������ � ����� ������ ��������

//====================================================================================================
//������������ ����������
//====================================================================================================

#include <list>
#include "stdafx.h"
#include "craiicdatabase.h"
#include "crecordset_projectlist.h"

using namespace std;

//====================================================================================================
//���������
//====================================================================================================

//====================================================================================================
//����������� ����� ������ � ����� ������ ��������
//====================================================================================================

class CIProjectDatabaseEngine
{
 public:
  //-���������� ������-------------------------------------------------------
  virtual ~CIProjectDatabaseEngine() {};
  //-������� ������----------------------------------------------------------
  virtual bool FindProjectByGUID(const CSafeString &guid,CProject &cProject)=0;//����� ������ �� GUID
  virtual list<CProject> GetAllProject(void)=0;//�������� ��� �������
  virtual bool AddProject(CProject &cProject)=0;//�������� ������
  virtual bool DeleteProject(const CProject &cProject)=0;//������� ������
  virtual bool ChangeProject(const CProject &cProject)=0;//�������� ������
  virtual void ResetBase(void)=0;//�������� ����
};

#endif