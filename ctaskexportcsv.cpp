#include "ctaskexportcsv.h"

//====================================================================================================
//конструктор класса
//====================================================================================================
CTaskExportCSV::CTaskExportCSV(void)
{
 
}
//====================================================================================================
//деструктор класса
//====================================================================================================
CTaskExportCSV::~CTaskExportCSV() 
{
}
//====================================================================================================
//функции класса
//====================================================================================================

//----------------------------------------------------------------------------------------------------
//экспорт заданий
//----------------------------------------------------------------------------------------------------
bool CTaskExportCSV::Export(const CString &file_name,const std::list<CTask> &list_CTask,const std::list<CUser> &list_CUser,const std::list<CProject> &list_CProject)
{
 FILE *file=fopen(file_name,"wb");
 if (file==NULL) return(false);

 fprintf(file,"Номер задания;");
 fprintf(file,"От;");
 fprintf(file,"Для;");
 fprintf(file,"Срок;");
 fprintf(file,"Задание;");
 fprintf(file,"Состояние;");
 fprintf(file,"Плановая позиция;");
 fprintf(file,"Видно всем;");
 fprintf(file,"По проекту;");
 fprintf(file,"\r\n");

 std::list<CTask>::const_iterator iterator=list_CTask.begin();
 std::list<CTask>::const_iterator iterator_end=list_CTask.end();  
 long index=0;
 while(iterator!=iterator_end)
 {
  const CTask &cTask=*iterator;  
  CUser cUser_From;
  CUser cUser_For; 
  CProject cProject; 
  
  std::list<CUser>::const_iterator iterator_user=list_CUser.begin();
  std::list<CUser>::const_iterator iterator_user_end=list_CUser.end();  
  while(iterator_user!=iterator_user_end)
  {
   const CUser &cUser=*iterator_user;
   if (cUser.GetUserGUID().Compare(cTask.GetForUserGUID())==0) cUser_For=cUser;
   if (cUser.GetUserGUID().Compare(cTask.GetFromUserGUID())==0) cUser_From=cUser;
   iterator_user++;
  }

  cProject.SetProjectName("Без проекта");
  std::list<CProject>::const_iterator iterator_project=list_CProject.begin();
  std::list<CProject>::const_iterator iterator_project_end=list_CProject.end();    
  while(iterator_project!=iterator_project_end)
  {
   const CProject &cProject_local=*iterator_project;
   if (cProject.GetProjectGUID().Compare(cTask.GetProjectGUID())==0) cProject=cProject_local;
   iterator_project++;
  }

  const char *from_name=cUser_From.GetName();
  const char *for_name=cUser_For.GetName();
  const char *task=cTask.GetTask();
  const char *project=cProject.GetProjectName();

  fprintf(file,"%ld;",cTask.GetIndex());
  fprintf(file,"%s;",from_name);
  fprintf(file,"%s;",for_name);
  const CDate &cDate=cTask.GetDate();
  fprintf(file,"%02i.%02i.%04i;",cDate.GetDay(),cDate.GetMonth(),cDate.GetYear());
  long size=strlen(task);
  for(long n=0;n<size;n++)
  {
   unsigned char s=task[n];
   if (s==';') 
   {
	fprintf(file," ");
	continue;
   }
   if (s=='\n') fprintf(file," ");
   if (s=='\t') s=' ';
   if (s<32) continue;
   fwrite(&s,sizeof(unsigned char),1,file);
  }
  fprintf(file,";");
  if (cTask.GetState()==TASK_STATE_NO_READ) fprintf(file,"Не прочитано");
  if (cTask.GetState()==TASK_STATE_READED) fprintf(file,"Прочитано");
  if (cTask.GetState()==TASK_STATE_IS_RUNNING) fprintf(file,"Выполняется");
  if (cTask.GetState()==TASK_STATE_DONE) fprintf(file,"Выполнено");
  if (cTask.GetState()==TASK_STATE_CANCELED) fprintf(file,"Отклонено");
  if (cTask.GetState()==TASK_STATE_FINISHED) fprintf(file,"Завершено");
  fprintf(file,";");
  if (cTask.IsPlannedPosition()) fprintf(file,"Да;");
                            else fprintf(file,"Нет;");
  if (cTask.IsCommon()) fprintf(file,"Да;");
                   else fprintf(file,"Нет;");
  fprintf(file,"%s;",project);
  fprintf(file,"\r\n");
  iterator++;
  index++;
 }
 fclose(file);
 return(true);
}
