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
bool CTaskExportTXT::Export(const CString &file_name,list<STask> &list_STask,list<SUser> &list_SUser)
{
 FILE *file=fopen(file_name,"wb");
 if (file==NULL) return(false);
	
 list<STask>::iterator iterator=list_STask.begin();
 list<STask>::iterator iterator_end=list_STask.end();  
 while(iterator!=iterator_end)
 {
  STask &sTask=*iterator;  
  SUser sUser_From;
  SUser sUser_For; 
  
  list<SUser>::iterator iterator_user=list_SUser.begin();
  list<SUser>::iterator iterator_user_end=list_SUser.end();  
  while(iterator_user!=iterator_user_end)
  {
   SUser &sUser=*iterator_user;
   if (sUser.UserGUID.Compare(sTask.ForUserGUID)==0) sUser_For=sUser;
   if (sUser.UserGUID.Compare(sTask.FromUserGUID)==0) sUser_From=sUser;
   iterator_user++;
  }
  const char *from_name=sUser_From.Name;
  const char *for_name=sUser_For.Name;
  const char *task=sTask.Task;
  fprintf(file,"Срок до: %02i.%02i.%04i\r\n",sTask.Day,sTask.Month,sTask.Year);
  fprintf(file,"От: %s\r\n",from_name);
  fprintf(file,"Для: %s\r\n",for_name);
  fprintf(file,"Задание:\r\n%s\r\n",task);
  fprintf(file,"--------------------------------------------------\r\n");
  iterator++;
 }
 fclose(file);
 return(true);
}
