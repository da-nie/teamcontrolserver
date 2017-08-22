#ifndef CITASK_EXPORT_H
#define CITASK_EXPORT_H

//====================================================================================================
//��������
//====================================================================================================

//����������� ����� �������� �������

//====================================================================================================
//������������ ����������
//====================================================================================================
#include "stdafx.h"
#include "csafestring.h"
#include "crecordset_userlist.h"
#include "crecordset_tasklist.h"
#include "crecordset_projectlist.h"

#include <list>
using namespace std;

//====================================================================================================
//����������� ����� �������� �������
//====================================================================================================

class CITaskExport
{
 public:
  //����������
  virtual ~CITaskExport() {};
  //-������� ������----------------------------------------------------------
  virtual bool Export(const CString &file_name,list<STask> &list_STask,list<SUser> &list_SUser)=0;//�������������� �������
};

#endif
