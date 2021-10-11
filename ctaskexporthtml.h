#ifndef CTASK_EXPORT_HTML_H
#define CTASK_EXPORT_HTML_H

//====================================================================================================
//��������
//====================================================================================================

//����� �������� ������� � html

//====================================================================================================
//������������ ����������
//====================================================================================================

#include "stdafx.h"
#include "citaskexport.h"

//====================================================================================================
//����� �������� ������� � html
//====================================================================================================

class CTaskExportHTML:public CITaskExport
{
 public:
  //�����������
  CTaskExportHTML(void);
  //����������
  ~CTaskExportHTML();
  //-������� ������----------------------------------------------------------
  bool Export(const CString &file_name,const std::list<CTask> &list_CTask,const std::list<CUser> &list_CUser,const std::list<CProject> &list_CProject) override;//�������������� �������
};

#endif
