#include "clistview_main.h"

//====================================================================================================
//������� ��������� ��������� ������
//====================================================================================================

BEGIN_MESSAGE_MAP(CListView_Main,CView)
 ON_WM_SIZE()
 ON_WM_PAINT()//��� ON_WM_PAINT ������ ������ �� ��������������!!! ���� ������ ��.
 ON_WM_RBUTTONDOWN()
 ON_COMMAND(IDC_MENU_LIST_ADD_USER,OnCommand_Menu_List_AddUser)
 ON_COMMAND(IDC_MENU_LIST_EDIT_USER,OnCommand_Menu_List_EditUser)
 ON_COMMAND(IDC_MENU_LIST_DELETE_USER,OnCommand_Menu_List_DeleteUser)
 ON_COMMAND(IDC_MENU_LIST_SET_LEADER,OnCommand_Menu_List_SetLeader)
 ON_COMMAND(IDC_MENU_LIST_RESET_LEADER,OnCommand_Menu_List_ResetLeader)
 ON_COMMAND(IDC_MENU_MAIN_SETTINGS,OnCommand_Menu_Main_Settings)
 ON_COMMAND(IDC_MENU_MAIN_EXPORT_TASK,OnCommand_Menu_Main_ExportTask)
 ON_COMMAND(IDC_MENU_MAIN_RESET_USER_LIST_BASE,OnCommand_Menu_Main_ResetUserListBase)
 ON_COMMAND(IDC_MENU_MAIN_RESET_TASK_LIST_BASE,OnCommand_Menu_Main_ResetTaskListBase)
 ON_COMMAND(IDC_MENU_MAIN_RESET_PROJECT_LIST_BASE,OnCommand_Menu_Main_ResetProjectListBase)
END_MESSAGE_MAP()

//====================================================================================================
//����������� ������
//====================================================================================================
CListView_Main::CListView_Main() 
{ 
}
//====================================================================================================
//���������� ������
//====================================================================================================
CListView_Main::~CListView_Main() 
{ 
}

//====================================================================================================
//������� ������
//====================================================================================================

//----------------------------------------------------------------------------------------------------
//������������ ����
//----------------------------------------------------------------------------------------------------
afx_msg BOOL CListView_Main::PreCreateWindow(CREATESTRUCT& cs)
{
 //cs.style&=~LVS_TYPEMASK;
 //cs.style|=LVS_REPORT;//|LVS_SHOWSELALWAYS|LVS_SINGLESEL|LVS_EX_ONECLICKACTIVATE|WS_BORDER|WS_VISIBLE;
 return(CListView::PreCreateWindow(cs));
}
//----------------------------------------------------------------------------------------------------
//�������� ����
//----------------------------------------------------------------------------------------------------
afx_msg void CListView_Main::OnInitialUpdate(void)
{ 
 CListView::OnInitialUpdate();
 //������� ��������
 RECT Rect;
 GetClientRect(&Rect);
 long width=Rect.right-Rect.left;
 CListCtrl &cListCtrl=GetListCtrl(); 
 cListCtrl.ModifyStyle(LVS_TYPEMASK,LVS_REPORT|WS_BORDER|WS_VISIBLE|LVS_SINGLESEL);
 cListCtrl.SetExtendedStyle(LVS_EX_GRIDLINES);
  
 cListCtrl.InsertColumn(0,"������������",LVCFMT_CENTER,width/5);
 cListCtrl.InsertColumn(1,"���������",LVCFMT_CENTER,width/5);
 cListCtrl.InsertColumn(2,"�����",LVCFMT_CENTER,width/5);
 cListCtrl.InsertColumn(3,"������",LVCFMT_CENTER,width/5);
 cListCtrl.InsertColumn(4,"�������",LVCFMT_CENTER,width/5);
 cListCtrl.DeleteAllItems();
 //�������� ����������� ����
 cMenu_List.LoadMenu(IDR_MENU_LIST);

 UpdateList();
}

//====================================================================================================
//������� ��������� ��������� ������
//====================================================================================================

//----------------------------------------------------------------------------------------------------
//��������� �������� ����
//----------------------------------------------------------------------------------------------------
afx_msg void CListView_Main::OnSize(UINT nType,int cx,int cy)
{ 
 CListView::OnSize(nType,cx,cy); 
 //���������� �������
 RECT Rect;
 GetClientRect(&Rect);
 long width=Rect.right-Rect.left;
 CListCtrl &cListCtrl=GetListCtrl();
 long n=0;
 while(1)
 {
  if (cListCtrl.SetColumnWidth(n,width/5)==FALSE) break;
  n++;
 }
}
//----------------------------------------------------------------------------------------------------
//������� ������ ������� �����
//----------------------------------------------------------------------------------------------------
afx_msg void CListView_Main::OnRButtonDown(UINT nFlags,CPoint point)
{
 //��������, ���� �� ��������� �������
 CListCtrl &cListCtrl=GetListCtrl();
 BOOL SelectedFunctionEnabled=FALSE;
 if (cListCtrl.GetSelectedCount()==0) SelectedFunctionEnabled=TRUE;//������ �� �������
 cMenu_List.EnableMenuItem(IDC_MENU_LIST_EDIT_USER,SelectedFunctionEnabled);
 cMenu_List.EnableMenuItem(IDC_MENU_LIST_DELETE_USER,SelectedFunctionEnabled);
 cMenu_List.EnableMenuItem(IDC_MENU_LIST_SET_LEADER,SelectedFunctionEnabled);
 cMenu_List.EnableMenuItem(IDC_MENU_LIST_RESET_LEADER,SelectedFunctionEnabled);
 //������ ���������� ����
 CPoint mpoint;
 GetCursorPos(&mpoint);
 cMenu_List.GetSubMenu(0)->TrackPopupMenu(TPM_LEFTALIGN|TPM_LEFTBUTTON,mpoint.x,mpoint.y,this); 
}
//----------------------------------------------------------------------------------------------------
//�������� ������������
//----------------------------------------------------------------------------------------------------
afx_msg void CListView_Main::OnCommand_Menu_List_AddUser(void)
{
 SUser sUser;
 sUser.Leader=false;
 sUser.Name="";
 sUser.Login="";
 sUser.Password="";
 sUser.JobTitle="";
 sUser.Telephone=""; 
 CDialog_UserSettings cDialog_UserSettings((LPCSTR)IDD_DIALOG_USER_SETTINGS,this);
 if (cDialog_UserSettings.Activate(sUser)==true)
 {
  CDocument_Main *cDocument_Main_Ptr=GetDocument();
  cDocument_Main_Ptr->AddUser(sUser); 
 }
 UpdateList();
}
//----------------------------------------------------------------------------------------------------
//������������� ������������
//----------------------------------------------------------------------------------------------------
afx_msg void CListView_Main::OnCommand_Menu_List_EditUser(void)
{
 //�������� ���������� ������������
 SUser sUser;
 if (GetSelectedUser(sUser)==false) return;//������
 CDialog_UserSettings cDialog_UserSettings((LPCSTR)IDD_DIALOG_USER_SETTINGS,this);
 if (cDialog_UserSettings.Activate(sUser)==true) SetSelectedUser(sUser);
 UpdateList();
}
//----------------------------------------------------------------------------------------------------
//������� ������������
//----------------------------------------------------------------------------------------------------
afx_msg void CListView_Main::OnCommand_Menu_List_DeleteUser(void)
{
 //�������� ���������� ������������
 SUser sUser;
 if (GetSelectedUser(sUser)==false) return;//������
 CSafeString name="������� ������������ ";
 name+=sUser.Name;
 name+=" ?";
 if (MessageBox(name,"�������������",MB_YESNO|MB_DEFBUTTON2)!=IDYES) return;
 CListCtrl &cListCtrl=GetListCtrl();
 if (cListCtrl.GetSelectedCount()==0) return;//������ �� �������
 long selected_item=cListCtrl.GetSelectionMark();
 if (vector_CSafeString_UserGUID.size()<=selected_item) return;
 CDocument_Main *cDocument_Main_Ptr=GetDocument();
 cDocument_Main_Ptr->DeleteUserByGUID(vector_CSafeString_UserGUID[selected_item]); 
 UpdateList();
}
//----------------------------------------------------------------------------------------------------
//��������� �������������
//----------------------------------------------------------------------------------------------------
afx_msg void CListView_Main::OnCommand_Menu_List_SetLeader(void)
{
 //�������� ���������� ������������
 SUser sUser;
 if (GetSelectedUser(sUser)==false) return;//������
 sUser.Leader=true;
 //���������� ���������� ������������
 SetSelectedUser(sUser);
}
//----------------------------------------------------------------------------------------------------
//��������� �����������
//----------------------------------------------------------------------------------------------------
afx_msg void CListView_Main::OnCommand_Menu_List_ResetLeader(void)
{
 //�������� ���������� ������������
 SUser sUser;
 if (GetSelectedUser(sUser)==false) return;//������
 sUser.Leader=false;
 //���������� ���������� ������������
 SetSelectedUser(sUser);
}
//----------------------------------------------------------------------------------------------------
//��������� �������
//----------------------------------------------------------------------------------------------------
afx_msg void CListView_Main::OnCommand_Menu_Main_Settings(void)
{
 CDocument_Main *cDocument_Main_Ptr=GetDocument();
 SServerSettings sServerSettings;
 cDocument_Main_Ptr->GetServerSettings(sServerSettings);
 CDialog_ServerSettings cDialog_ServerSettings((LPCSTR)IDD_DIALOG_SERVER_SETTINGS,this);
 if (cDialog_ServerSettings.Activate(sServerSettings)==true) cDocument_Main_Ptr->SetServerSettings(sServerSettings);
 cDocument_Main_Ptr->SaveState();
}
//----------------------------------------------------------------------------------------------------
//������� ���� �������
//----------------------------------------------------------------------------------------------------
afx_msg void CListView_Main::OnCommand_Menu_Main_ExportTask(void)
{
 CDocument_Main *cDocument_Main_Ptr=GetDocument();
 char Path[MAX_PATH];
 GetCurrentDirectory(MAX_PATH,Path); 
 CFileDialog cFileDialog(FALSE,"","",OFN_HIDEREADONLY|OFN_OVERWRITEPROMPT,"*.htm|*.htm||",this);
 cFileDialog.m_ofn.lpstrInitialDir=Path;
 char Title[256];
 strcpy(Title,"������� ���� �������");
 cFileDialog.m_ofn.lpstrTitle=Title;
 if (cFileDialog.DoModal()!=IDOK) return;
 cDocument_Main_Ptr->ExportTaskBase(cFileDialog.GetFileName());
}
//----------------------------------------------------------------------------------------------------
//�������� ���� �������������
//----------------------------------------------------------------------------------------------------
afx_msg void CListView_Main::OnCommand_Menu_Main_ResetUserListBase(void)
{
 if (MessageBox("�� �������, ��� ������ ������� ���� �������������?","�������������",MB_YESNO|MB_DEFBUTTON2)!=IDYES) return;
 CDocument_Main *cDocument_Main_Ptr=GetDocument();
 cDocument_Main_Ptr->ResetUserListBase();
}
//----------------------------------------------------------------------------------------------------
//�������� ���� �������
//----------------------------------------------------------------------------------------------------
afx_msg void CListView_Main::OnCommand_Menu_Main_ResetTaskListBase(void)
{
 if (MessageBox("�� �������, ��� ������ ������� ���� �������?","�������������",MB_YESNO|MB_DEFBUTTON2)!=IDYES) return;
 CDocument_Main *cDocument_Main_Ptr=GetDocument();
 cDocument_Main_Ptr->ResetTaskListBase();
}
//----------------------------------------------------------------------------------------------------
//�������� ���� ��������
//----------------------------------------------------------------------------------------------------
afx_msg void CListView_Main::OnCommand_Menu_Main_ResetProjectListBase(void)
{
 if (MessageBox("�� �������, ��� ������ ������� ���� ��������?","�������������",MB_YESNO|MB_DEFBUTTON2)!=IDYES) return;
 CDocument_Main *cDocument_Main_Ptr=GetDocument();
 cDocument_Main_Ptr->ResetProjectListBase();
}


//====================================================================================================
//������� ������
//====================================================================================================

//----------------------------------------------------------------------------------------------------
//�������� ������
//----------------------------------------------------------------------------------------------------
void CListView_Main::UpdateList(void)
{
 CDocument_Main *cDocument_Main_Ptr=GetDocument();
 list<SUser> list_SUser=cDocument_Main_Ptr->GetAllUser();
 list<SUser>::iterator iterator=list_SUser.begin();
 list<SUser>::iterator iterator_end=list_SUser.end(); 
 CListCtrl &cListCtrl=GetListCtrl(); 
 cListCtrl.DeleteAllItems();
 vector_CSafeString_UserGUID.clear();
 SUser sUser;
 long index=0;
 while(iterator!=iterator_end)
 {
  sUser=*iterator;
  
  LV_ITEM item;  
  item.mask=LVIF_TEXT;
  item.iItem=index;
  item.iSubItem=0;
  item.pszText=const_cast<LPSTR>((LPCSTR)sUser.Name);
  item.iItem=cListCtrl.InsertItem(&item);
  item.iSubItem=1;
  item.pszText=const_cast<LPSTR>((LPCSTR)sUser.JobTitle);
  cListCtrl.SetItem(&item);
  item.iSubItem=2;
  item.pszText=const_cast<LPSTR>((LPCSTR)sUser.Login);
  cListCtrl.SetItem(&item);
  item.iSubItem=3;
  item.pszText=const_cast<LPSTR>((LPCSTR)sUser.Password);
  cListCtrl.SetItem(&item);
  item.iSubItem=4;
  if (sUser.Leader==true) item.pszText="������������";
                     else item.pszText="���������";
  cListCtrl.SetItem(&item);
  index++;
  iterator++;
  vector_CSafeString_UserGUID.push_back(sUser.UserGUID);
 }
}
//----------------------------------------------------------------------------------------------------
//�������� ��������
//----------------------------------------------------------------------------------------------------
CDocument_Main* CListView_Main::GetDocument(void)
{
 ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CDocument)));// ��������� ���� �� ����� ���������
 return(reinterpret_cast<CDocument_Main*>(m_pDocument));//���������� ��������� �� ��������
}
//----------------------------------------------------------------------------------------------------
//�������� ���������� ������������ �� ������ ���������
//----------------------------------------------------------------------------------------------------
bool CListView_Main::GetSelectedUser(SUser &sUser)
{
 CListCtrl &cListCtrl=GetListCtrl();
 if (cListCtrl.GetSelectedCount()==0) return(false);//������ �� �������
 long selected_item=cListCtrl.GetSelectionMark();
 if (vector_CSafeString_UserGUID.size()<=selected_item) return(false);
 CDocument_Main *cDocument_Main_Ptr=GetDocument();
 return(cDocument_Main_Ptr->FindUserByGUID(vector_CSafeString_UserGUID[selected_item],sUser));
}
//----------------------------------------------------------------------------------------------------
//�������� ���������� ������������ � ����� ���������
//----------------------------------------------------------------------------------------------------
void CListView_Main::SetSelectedUser(const SUser &sUser)
{
 CListCtrl &cListCtrl=GetListCtrl();
 if (cListCtrl.GetSelectedCount()==0) return;//������ �� �������
 long selected_item=cListCtrl.GetSelectionMark();
 if (vector_CSafeString_UserGUID.size()<=selected_item) return;
 CDocument_Main *cDocument_Main_Ptr=GetDocument();
 cDocument_Main_Ptr->ChangeUserByGUID(vector_CSafeString_UserGUID[selected_item],sUser);
 //��������� ������ �������������
 UpdateList();
}

//====================================================================================================
//������
//====================================================================================================
IMPLEMENT_DYNCREATE(CListView_Main,CListView)
