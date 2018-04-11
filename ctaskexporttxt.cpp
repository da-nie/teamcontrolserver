#include "ctaskexporttxt.h"

//====================================================================================================
//конструктор класса
//====================================================================================================
CTaskExportTXT::CTaskExportTXT(void)
{
 
}
//====================================================================================================
//деструктор класса
//====================================================================================================
CTaskExportTXT::~CTaskExportTXT() 
{
}
//====================================================================================================
//функции класса
//====================================================================================================

//----------------------------------------------------------------------------------------------------
//экспорт заданий
//----------------------------------------------------------------------------------------------------
bool CTaskExportTXT::Export(const CString &file_name,const list<CTask> &list_CTask,const list<CUser> &list_CUser,const list<CProject> &list_CProject)
{
 FILE *file=fopen(file_name,"wb");
 if (file==NULL) return(false);
	
 list<CTask>::const_iterator iterator=list_CTask.begin();
 list<CTask>::const_iterator iterator_end=list_CTask.end();  
 while(iterator!=iterator_end)
 {
  const CTask &cTask=*iterator;  
  CUser cUser_From;
  CUser cUser_For; 
  
  list<CUser>::const_iterator iterator_user=list_CUser.begin();
  list<CUser>::const_iterator iterator_user_end=list_CUser.end();  
  while(iterator_user!=iterator_user_end)
  {
   const CUser &cUser=*iterator_user;
   if (cUser.GetUserGUID().Compare(cTask.GetForUserGUID())==0) cUser_For=cUser;
   if (cUser.GetUserGUID().Compare(cTask.GetFromUserGUID())==0) cUser_From=cUser;
   iterator_user++;
  }
  const char *from_name=cUser_From.GetName();
  const char *for_name=cUser_For.GetName();
  const char *task=cTask.GetTask();
  const CDate &cDate=cTask.GetDate();
  fprintf(file,"Срок до: %02i.%02i.%04i\r\n",cDate.GetDay(),cDate.GetMonth(),cDate.GetYear());
  fprintf(file,"От: %s\r\n",from_name);
  fprintf(file,"Для: %s\r\n",for_name);
  fprintf(file,"Задание:\r\n%s\r\n",task);
  fprintf(file,"--------------------------------------------------\r\n");
  iterator++;
 }
 fclose(file);
 return(true);
}
