/**
 * Audio main panel.
 *
 * \author Allann Jones
 */

#include "app.hpp"
#include "ui/panel_audio.hpp"
#include "ui/dlg_main.hpp"
#include "pixmaps/clear_16x16.xpm"
#include "pixmaps/folder_16x16.xpm"
#include "pixmaps/file_audio_16x16.xpm"
#include "pixmaps/audio_recorder_16x16.xpm"

PanelAudio::PanelAudio()
{
  frameParent = NULL;
  panel = NULL;
  audioControlPanel = NULL;
  audioRecorderControlPanel = NULL;

  currentInterface = GUI_AUDIO_EMPTY;
}

PanelAudio::~PanelAudio()
{
  switch (currentInterface) {
    case GUI_AUDIO_PLAYER:
      AudioPlayerDestroy();
      break;
    case GUI_AUDIO_RECORDER:
      AudioRecorderDestroy();
      break;
  }
}

int PanelAudio::Create(wxPanel *panel)
{
  this->panel = panel;

  /* Audio management panel content */
  wxBoxSizer *szPanelAudio = new wxBoxSizer(wxVERTICAL);
  panel->SetSizer(szPanelAudio);

  // Splitter

  wxSplitterWindow *splitter = new wxSplitterWindow(panel, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxSP_3D);
  szPanelAudio->Add(splitter, 1, wxALL | wxEXPAND | wxGROW, 5);

  // Panel left

  wxPanel *panelLeft = new wxPanel(splitter, wxID_ANY, wxDefaultPosition, wxSize(100, -1));

  panelLeft->SetBackgroundColour(*wxWHITE);

  // Panel right

  panelAudio = new wxScrolledWindow(splitter, wxID_ANY);

  panelAudio->SetScrollbars(10, 10, 10, 10);

  // Put panels on splitter

  splitter->SplitVertically(panelLeft, panelAudio, 200);

  // Left panel content

  wxBoxSizer *szPanelAudioListVertical = new wxBoxSizer(wxVERTICAL);
  panelLeft->SetSizer(szPanelAudioListVertical);

  // Audio toolbar
  toolbar = new wxAuiToolBar(panelLeft, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxAUI_TB_DEFAULT_STYLE | wxAUI_TB_PLAIN_BACKGROUND);
  szPanelAudioListVertical->Add(toolbar, 0, wxEXPAND|wxGROW, 0);

  toolbar->SetToolBitmapSize(wxSize(16, 16));

  toolbar->AddTool(PANEL_AUDIO_TOOLBAR_LOAD_FILE_ID, wxT(" Abrir"), wxBitmap(folder_16x16_xpm), wxT("Abrir arquivo de áudio"));
  toolbar->AddTool(PANEL_AUDIO_TOOLBAR_LIST_CLEAN_ID, wxT(" Limpar"), wxBitmap(clear_16x16_xpm), wxT("Limpar playlist"));
  toolbar->AddTool(PANEL_AUDIO_TOOLBAR_AUDIO_NEW_ID, wxT(" Novo"), wxBitmap(audio_recorder_16x16_xpm), wxT("Criar novo arquivo de áudio a partir de gravação via microfone."));

  toolbar->Realize();

  toolbar->EnableTool(PANEL_AUDIO_TOOLBAR_LIST_CLEAN_ID, false);

  // Audio list

  int audioListCtrlWidth = 200;

  listCtrlAudioList = new wxListCtrl(panelLeft, PANEL_AUDIO_LISTCTRL_AUDIO_ID, wxDefaultPosition, wxSize(audioListCtrlWidth, -1), wxLC_REPORT|wxLC_SINGLE_SEL|wxLC_HRULES|wxLC_VRULES);
  szPanelAudioListVertical->Add(listCtrlAudioList, 1, wxALL|wxEXPAND|wxGROW, 2);

  wxImageList* imageList = new wxImageList(16, 16);
  imageList->Add(wxIcon(file_audio_16x16_xpm));
  listCtrlAudioList->AssignImageList(imageList, wxIMAGE_LIST_SMALL);

  // Insert three columns
  wxListItem listItemColAudio;
  listItemColAudio.SetText(wxT("Playlist"));
  listCtrlAudioList->InsertColumn(0, listItemColAudio);
  listCtrlAudioList->SetColumnWidth(0, audioListCtrlWidth - 5);

  /*
  // Vertical sizer to acommodate the audio file information

  wxBoxSizer *szPanelAudioVerticalInfo = new wxBoxSizer(wxVERTICAL);
  szPanelAudioHorizontal->Add(szPanelAudioVerticalInfo, 1, wxALL|wxEXPAND|wxGROW, 0);

  // Audio player

  wxBoxSizer *szPanelAudioPlayer = new wxBoxSizer(wxVERTICAL);

  szPanelAudioVerticalInfo->Add(szPanelAudioPlayer, 1, wxALL | wxEXPAND | wxGROW, 5);

  panelAudio = new wxPanel(panelRight, wxID_ANY);

  //panelAudio->SetBackgroundColour(*wxWHITE);

  szPanelAudioPlayer->Add(panelAudio, 1, wxEXPAND|wxGROW, 0);
  */

  // Final configuration
  panel->Layout();
  szPanelAudio->Layout();

  wxGetApp().GetTopWindow()->Layout();

  toolbar->Connect((int) PANEL_AUDIO_TOOLBAR_LOAD_FILE_ID, wxEVT_COMMAND_TOOL_CLICKED, wxCommandEventHandler(PanelAudio::OnLoadFileRequest), NULL, this);
  toolbar->Connect((int) PANEL_AUDIO_TOOLBAR_LIST_CLEAN_ID, wxEVT_COMMAND_TOOL_CLICKED, wxCommandEventHandler(PanelAudio::OnListCleanRequest), NULL, this);
  toolbar->Connect((int) PANEL_AUDIO_TOOLBAR_AUDIO_NEW_ID, wxEVT_COMMAND_TOOL_CLICKED, wxCommandEventHandler(PanelAudio::OnAudioRecordDialogRequest), NULL, this);
  listCtrlAudioList->Connect((int) PANEL_AUDIO_LISTCTRL_AUDIO_ID, wxEVT_COMMAND_LIST_ITEM_ACTIVATED, wxListEventHandler(PanelAudio::OnAudioListItemActivated), NULL, this);
  listCtrlAudioList->Connect((int) PANEL_AUDIO_LISTCTRL_AUDIO_ID, wxEVT_COMMAND_LIST_KEY_DOWN, wxListEventHandler(PanelAudio::OnAudioListItemKeyDown), NULL, this);

  return 0;
}

int PanelAudio::AddAudioFile(wxString filePath)
{
  int imageIndex = 0;
  int listItemIndex = listCtrlAudioList->GetItemCount();

  listCtrlAudioList->Freeze();

  long itemIndex = listCtrlAudioList->InsertItem(listItemIndex, filePath, imageIndex);

  listCtrlAudioList->SetColumnWidth(0, wxLIST_AUTOSIZE);

  if (!wxFile::Exists(filePath)) {
    listCtrlAudioList->SetItemBackgroundColour(itemIndex, GUI_COLOR_LIST_ITEM_ERROR);
  }
  else {
    GUI::ListSetRowBgColor(listCtrlAudioList, itemIndex);
  }

  listCtrlAudioList->Thaw();

  // Refresh toolbar

  toolbar->EnableTool(PANEL_AUDIO_TOOLBAR_LIST_CLEAN_ID, true);

  toolbar->Refresh();
  toolbar->Update();

  return 0;
}

int PanelAudio::ListClear()
{
  if (listCtrlAudioList->GetItemCount() > 0) {
    listCtrlAudioList->DeleteAllItems();
  }

  return 0;
}

void PanelAudio::OnLoadFileRequest(wxCommandEvent &event)
{
  wxString caption = wxT("Selecione o arquivo de áudio");
  wxString wildcardOptions = wxT("Arquivos de áudio Wave (*.wav)|*.wav");

  wxFileDialog fileDialog(panel, caption, wxEmptyString, wxEmptyString, wildcardOptions, wxFD_OPEN | wxFD_FILE_MUST_EXIST | wxFD_CHANGE_DIR | wxFD_MULTIPLE);

  if (fileDialog.ShowModal() == wxID_OK) {
    wxArrayString filePathList;

    fileDialog.GetPaths(filePathList);

    for (unsigned int fileIndex = 0; fileIndex < filePathList.Count(); fileIndex++) {
      wxString filePath = filePathList[fileIndex].c_str();

      AddAudioFile(filePath);

      if (filePathList.Count() == 1) {
        AudioPlayerOpenFile(filePath);
      }
    }

    toolbar->EnableTool(PANEL_AUDIO_TOOLBAR_LIST_CLEAN_ID, true);

    toolbar->Refresh();
    toolbar->Update();
  }
}

void PanelAudio::OnListCleanRequest(wxCommandEvent &event)
{
  if (listCtrlAudioList->GetItemCount() < 1) {
    //wxMessageBox(wxT("A playlist está vazia."));
    return;
  }

  listCtrlAudioList->DeleteAllItems();

  if (currentInterface == GUI_AUDIO_PLAYER) {
    AudioPlayerDestroy();
  }

  currentInterface = GUI_AUDIO_EMPTY;

  toolbar->EnableTool(PANEL_AUDIO_TOOLBAR_LIST_CLEAN_ID, false);

  toolbar->Refresh();
  toolbar->Update();
}

void PanelAudio::OnAudioListItemActivated(wxListEvent &event)
{
  //wxMessageBox(wxString::Format(wxT("OnAudioListItemActivated: %s selected"), event.GetText().c_str()));

  wxString filePath = event.GetText();

  AudioPlayerOpenFile(filePath);
}

int PanelAudio::AudioPlayerOpenFile(wxString &filePath)
{
  switch (currentInterface) {
    case GUI_AUDIO_PLAYER:
      AudioPlayerDestroy();
      break;
    case GUI_AUDIO_RECORDER:
      AudioRecorderDestroy();
      break;
  }

  currentInterface = GUI_AUDIO_EMPTY;

  int rc = AudioPlayerCreate(filePath);

  if (rc != 0) {
    wxMessageBox(wxT("Erro ao tentar abrir a interface para o arquivo de áudio informado."));

    return -1;
  }

  return 0;
}


int PanelAudio::AudioPlayerCreate(wxString &filePath)
{
  int rc;

  audioControlPanel = new AudioControlPanel(this);

  audioControlPanel->SetFilePath(filePath);

  rc = audioControlPanel->Create(panelAudio);

  if (rc != 0) {
    delete audioControlPanel;
  }

  currentInterface = GUI_AUDIO_PLAYER;

  wxGetApp().GetTopWindow()->Layout();

  return rc;
}

void PanelAudio::AudioPlayerDestroy()
{
  delete audioControlPanel;

  currentInterface = GUI_AUDIO_EMPTY;

  wxGetApp().GetTopWindow()->Layout();
}


int PanelAudio::AudioRecorderCreate()
{
  int rc = 0;

  audioRecorderControlPanel = new AudioRecorderControlPanel(this);

  //audioRecorderControlPanel->SetFilePath(filePath);

  rc = audioRecorderControlPanel->Create(panelAudio);

  if (rc != 0) {
    delete audioRecorderControlPanel;
  }

  currentInterface = GUI_AUDIO_RECORDER;

  wxGetApp().GetTopWindow()->Layout();

  return rc;
}

void PanelAudio::AudioRecorderDestroy()
{
  delete audioRecorderControlPanel;

  currentInterface = GUI_AUDIO_EMPTY;

  wxGetApp().GetTopWindow()->Layout();
}

void PanelAudio::OnAudioListItemKeyDown(wxListEvent &event)
{
  if (event.GetIndex() < 0) {
    return;
  }

  if (event.GetKeyCode() == WXK_DELETE) {
    if (wxYES == wxMessageBox(wxT("Confirma a remoção do áudio da playlist?"),
                              wxT("Remoção de áudio da playlist"),
                              wxNO_DEFAULT|wxYES_NO|wxICON_QUESTION, NULL)) {
      listCtrlAudioList->DeleteItem(event.GetIndex());
    }
  }
}

void PanelAudio::OnAudioRecordDialogRequest(wxCommandEvent &event)
{
  switch (currentInterface) {
    case GUI_AUDIO_PLAYER:
      AudioPlayerDestroy();
      break;
    case GUI_AUDIO_RECORDER:
      AudioRecorderDestroy();
      break;
  }

  currentInterface = GUI_AUDIO_EMPTY;

  int rc = AudioRecorderCreate();

  if (rc != 0) {
    wxMessageBox(wxT("Erro ao tentar abrir a interface para gravação de áudio."));
  }
}
