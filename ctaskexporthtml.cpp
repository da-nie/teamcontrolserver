#include "ctaskexporthtml.h"

//====================================================================================================
//конструктор класса
//====================================================================================================
CTaskExportHTML::CTaskExportHTML(void)
{
 
}
//====================================================================================================
//деструктор класса
//====================================================================================================
CTaskExportHTML::~CTaskExportHTML() 
{
}
//====================================================================================================
//функции класса
//====================================================================================================

//----------------------------------------------------------------------------------------------------
//экспорт заданий
//----------------------------------------------------------------------------------------------------
bool CTaskExportHTML::Export(const CString &file_name,const list<CTask> &list_CTask,const list<CUser> &list_CUser,const list<CProject> &list_CProject)
{
 FILE *file=fopen(file_name,"wb");
 if (file==NULL) return(false);
 fprintf(file,"<html>\r\n");
 fprintf(file,"<head>\r\n");
 fprintf(file,"<style type=\"text/css\">\r\n");

 fprintf(file,"#head {\r\n");
 fprintf(file,"       font-size: 15pt;\r\n");
 fprintf(file,"       font-family: Comic Sans MS;\r\n");
 fprintf(file,"       font-weight: bold;\r\n");
 fprintf(file,"       text-align: center;\r\n");
 fprintf(file,"       color: #006633\r\n");
 fprintf(file,"      }\r\n");

 fprintf(file,"#table_col_head {\r\n");
 fprintf(file,"                 font-size: 10pt;\r\n");
 fprintf(file,"                 font-family: Comic Sans MS;\r\n");
 fprintf(file,"                 background-color: #AEADB4;\r\n");
 fprintf(file,"                 font-weight: bold;\r\n");
 fprintf(file,"                 vertical-align: middle;\r\n");
 fprintf(file,"                 text-align: center;\r\n");
 fprintf(file,"                 color: #5D0E0E;\r\n");
 fprintf(file,"                }\r\n");

 fprintf(file,"#table_row_1 {\r\n");
 fprintf(file,"              font-size: 12pt;\r\n");
 fprintf(file,"              font-family: Verdana, Arial, Helvetica, sans-serif;\r\n");
 fprintf(file,"              background-color: #D4D4D4;\r\n");
 fprintf(file,"              font-weight: bold;\r\n");
 fprintf(file,"              vertical-align: middle;\r\n");
 fprintf(file,"              text-align: left;\r\n");
 fprintf(file,"              color: #006633;\r\n");
 fprintf(file,"             }\r\n");

 fprintf(file,"#table_row_2 {\r\n");
 fprintf(file,"              font-size: 12pt;\r\n");
 fprintf(file,"              font-family: Verdana, Arial, Helvetica, sans-serif;\r\n");
 fprintf(file,"              font-weight: bold;\r\n");
 fprintf(file,"              background-color: #C4C4C4;\r\n");
 fprintf(file,"              vertical-align: middle;\r\n");
 fprintf(file,"              text-align: left;\r\n");
 fprintf(file,"              color: #006633;\r\n");
 fprintf(file,"             }\r\n");
 fprintf(file,"</style>\r\n");

 fprintf(file," <meta content=\"text/html; charset=windows-1251\" http-equiv=\"content-type\" id=table_col_head>\r\n");
 fprintf(file," <title>База заданий</title>\r\n");
 fprintf(file,"</head>\r\n");
 fprintf(file,"<body>\r\n");

 fprintf(file,"<table width=\"100%%\" border=\"0\">\r\n");
 fprintf(file,"<tr>\r\n");
 fprintf(file,"<td>\r\n");
 fprintf(file,"<p id=head>База заданий</p>\r\n");
 fprintf(file,"</td>\r\n");
 fprintf(file,"</tr>\r\n");
 fprintf(file,"<tr>\r\n");
 fprintf(file,"<td>\r\n");

 fprintf(file," <table width=\"100%%\" border=\"1\">\r\n");
 fprintf(file," <tr id=table_col_head>\r\n");
 fprintf(file,"  <td height=\"4%%\" width=\"15%%\">От</td>\r\n");
 fprintf(file,"  <td height=\"4%%\" width=\"15%%\">Для</td>\r\n");
 fprintf(file,"  <td height=\"4%%\" width=\"10%%\">Срок</td>\r\n");
 fprintf(file,"  <td height=\"4%%\" width=\"50%%\">Задание</td>\r\n");
 fprintf(file,"  <td height=\"4%%\" width=\"10%%\">Состояние</td>\r\n");
 fprintf(file," </tr>\r\n");

 list<CTask>::const_iterator iterator=list_CTask.begin();
 list<CTask>::const_iterator iterator_end=list_CTask.end();  
 long index=0;
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
   
  if (index%2==0) fprintf(file,"<tr id=table_row_1>\r\n");
             else fprintf(file,"<tr id=table_row_2>\r\n");

  fprintf(file,"<td height=\"4%%\" width=\"15%%\">%s</td>\r\n",from_name);
  fprintf(file,"<td height=\"4%%\" width=\"15%%\">%s</td>\r\n",for_name);
  const CDate &cDate=cTask.GetDate();
  fprintf(file,"<td height=\"4%%\" width=\"10%%\">%02i.%02i.%04i</td>\r\n",cDate.GetDay(),cDate.GetMonth(),cDate.GetYear());
  fprintf(file,"<td width=\"50%%\">");
  long size=strlen(task);
  for(long n=0;n<size;n++)
  {
   unsigned char s=task[n];
   if (s=='\n') 
   {
	fprintf(file,"<br>");
	continue;
   }
   if (s=='\t') s=' ';
   if (s<32) continue;
   fwrite(&s,sizeof(unsigned char),1,file);
  }
  fprintf(file,"</td>\r\n");
  if (cTask.GetState()==TASK_STATE_NO_READ) fprintf(file,"<td width=\"10%%\">Не прочитано</td>\r\n");
  if (cTask.GetState()==TASK_STATE_READED) fprintf(file,"<td width=\"10%%\">Прочитано</td>\r\n");
  if (cTask.GetState()==TASK_STATE_IS_RUNNING) fprintf(file,"<td width=\"10%%\">Выполняется</td>\r\n");
  if (cTask.GetState()==TASK_STATE_DONE) fprintf(file,"<td width=\"10%%\">Выполнено</td>\r\n");
  if (cTask.GetState()==TASK_STATE_CANCELED) fprintf(file,"<td width=\"10%%\">Отклонено</td>\r\n");
  if (cTask.GetState()==TASK_STATE_FINISHED) fprintf(file,"<td width=\"10%%\">Завершено</td>\r\n");
  fprintf(file,"</tr>\r\n");
  iterator++;
  index++;
 }

 fprintf(file," </table>\r\n");
 fprintf(file," </td>\r\n");
 fprintf(file," </tr>\r\n");
 fprintf(file," </tbody>\r\n");
 fprintf(file," </table>\r\n");
 fprintf(file," </body>\r\n");
 fprintf(file," </html>\r\n");

 fclose(file);
 return(true);
}
