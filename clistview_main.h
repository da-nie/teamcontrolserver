#ifndef CLISTVIEW_MAIN_H
#define CLISTVIEW_MAIN_H

//====================================================================================================
//��������
//====================================================================================================

//����� ������������� ������ ��� ������ �������������.

//====================================================================================================
//������������ ����������
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
//����� �������������
//====================================================================================================

class CListView_Main:public CListView
{
 protected: 
  //-���������� ������-------------------------------------------------------
  CMenu cMenu_List;//���������� ���� � ������ �������������
  vector<CSafeString> vector_CSafeString_UserGUID;//������ GUID ������������� � ������
  //-������------------------------------------------------------------------
 public:
  //-����������� ������------------------------------------------------------
  CListView_Main();
  //-���������� ������-------------------------------------------------------
  ~CListView_Main();
  //-���������� ������-------------------------------------------------------
  //-������� ������----------------------------------------------------------
  afx_msg BOOL PreCreateWindow(CREATESTRUCT& cs);//������������ ����
  afx_msg void OnInitialUpdate(void);//�������� ����
  //-������� ��������� ��������� ������--------------------------------------
  afx_msg void OnSize(UINT nType,int cx,int cy);//��������� �������� ����
  afx_msg void OnUpdate(CView *pSender,LPARAM lHint,CObject *pHint);//�������� ���
  afx_msg void OnRButtonDown(UINT nFlags,CPoint point);//������� ������ ������� �����
  afx_msg void OnCommand_Menu_List_AddUser(void);//�������� ������������
  afx_msg void OnCommand_Menu_List_EditUser(void);//������������� ������������
  afx_msg void OnCommand_Menu_List_DeleteUser(void);//������� ������������
  afx_msg void OnCommand_Menu_List_SetLeader(void);//��������� �������������
  afx_msg void OnCommand_Menu_List_ResetLeader(void);//��������� �����������
  afx_msg void OnCommand_Menu_Main_Settings(void);//��������� �������
  afx_msg void OnCommand_Menu_Main_ExportTask(void);//������� ���� �������
  afx_msg void OnCommand_Menu_Main_ResetUserListBase(void);//�������� ���� �������������
  afx_msg void OnCommand_Menu_Main_ResetTaskListBase(void);//�������� ���� �������
  afx_msg void OnCommand_Menu_Main_ResetProjectListBase(void);//�������� ���� ��������
  afx_msg void OnCommand_Menu_Main_SaveCRC(void);//��������� ����������� ����� ��������� � ����������

  DECLARE_MESSAGE_MAP()
 public:
  //-������� ������----------------------------------------------------------
  void UpdateList(void);//�������� ������
 protected:
  //-������� ������----------------------------------------------------------
  CDocument_Main* GetDocument(void);//�������� ��������
  bool GetSelectedUser(CUser &cUser);//�������� �������� ���������� ������������ �� ������ ���������
  void SetSelectedUser(const CUser &cUser);//�������� ���������� ������������ � ����� ���������
  //-������------------------------------------------------------------------
  DECLARE_DYNCREATE(CListView_Main)
  
};
#endif