#ifndef CLISTVIEW_MAIN_H
#define CLISTVIEW_MAIN_H

//====================================================================================================
//описание
//====================================================================================================

// ласс отображаемого списка дл€ списка пользователей.

//====================================================================================================
//подключаемые библиотеки
//====================================================================================================

#include <list>
#include <string>
#include <vector>
#include "stdafx.h"
#include "cdocument_main.h"
#include "cdialog_usersettings.h"
#include "cdialog_serversettings.h"

using namespace std;

//====================================================================================================
//класс представлени€
//====================================================================================================

class CListView_Main:public CListView
{
 protected: 
  //-ѕеременные класса-------------------------------------------------------
  CMenu cMenu_List;//выпадающее меню в списке пользователей
  vector<CSafeString> vector_CSafeString_UserGUID;//список GUID пользователей в списке
  //-ѕрочее------------------------------------------------------------------
 public:
  //- онструктор класса------------------------------------------------------
  CListView_Main();
  //-ƒеструктор класса-------------------------------------------------------
  ~CListView_Main();
  //-ѕеременные класса-------------------------------------------------------
  //-‘ункции класса----------------------------------------------------------
  afx_msg BOOL PreCreateWindow(CREATESTRUCT& cs);//предсоздание окна
  afx_msg void OnInitialUpdate(void);//создание окна
  //-‘ункции обработки сообщений класса--------------------------------------
  afx_msg void OnSize(UINT nType,int cx,int cy);//изменение размеров окна
  afx_msg void OnUpdate(CView *pSender,LPARAM lHint,CObject *pHint);//обновить вид
  afx_msg void OnRButtonDown(UINT nFlags,CPoint point);//нажатие правой кнопкой мышки
  afx_msg void OnCommand_Menu_List_AddUser(void);//добавить пользовател€
  afx_msg void OnCommand_Menu_List_EditUser(void);//редактировать пользовател€
  afx_msg void OnCommand_Menu_List_DeleteUser(void);//удалить пользовател€
  afx_msg void OnCommand_Menu_List_SetLeader(void);//назначить руководителем
  afx_msg void OnCommand_Menu_List_ResetLeader(void);//назначить сотрудником
  afx_msg void OnCommand_Menu_Main_Settings(void);//настройка сервера
  afx_msg void OnCommand_Menu_Main_ExportTask(void);//экспорт базы заданий
  afx_msg void OnCommand_Menu_Main_ResetUserListBase(void);//очистить базу пользователей
  afx_msg void OnCommand_Menu_Main_ResetTaskListBase(void);//очистить базу заданий
  afx_msg void OnCommand_Menu_Main_ResetProjectListBase(void);//очистить базу проектов
  afx_msg void OnCommand_Menu_Main_SaveCRC(void);//запомнить контрольную сумму программы и загрузчика

  DECLARE_MESSAGE_MAP()
 public:
  //-‘ункции класса----------------------------------------------------------
  void UpdateList(void);//обновить список
 protected:
  //-‘ункции класса----------------------------------------------------------
  CDocument_Main* GetDocument(void);//получить документ
  bool GetSelectedUser(CUser &cUser);//получить итератор выбранного пользовател€ из класса документа
  void SetSelectedUser(const CUser &cUser);//записать выбранного пользовател€ в класс документа
  //-ѕрочее------------------------------------------------------------------
  DECLARE_DYNCREATE(CListView_Main)
  
};
#endif