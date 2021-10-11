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


//====================================================================================================
//����������� ����� �������� �������
//====================================================================================================

class CITaskExport
{
 public:
  //����������
  virtual ~CITaskExport() {};
  //-������� ������----------------------------------------------------------
  virtual bool Export(const CString &file_name,const std::list<CTask> &list_CTask,const std::list<CUser> &list_CUser,const std::list<CProject> &list_CProject)=0;//�������������� �������
};

#endif
