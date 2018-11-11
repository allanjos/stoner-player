/**
 * Application main window.
 *
 * \file dlg_main.cpp
 * \author Allann Jones
 */

#include <iostream>
#include <iomanip>
#include <vector>

#include <sys/types.h>
#include <sys/stat.h>

#include "app.hpp"
#include "utils_include.hpp"
#include "utils_filesystem.hpp"
#include "ui/dlg_main.hpp"
#include "ui/panel_audio.hpp"
#include "pixmaps/play_16x16.xpm"

using namespace std;

#define IMG_INDEX_FOLDER       0
#define IMG_INDEX_FILE_AUDIO   1

TreeFileData::TreeFileData() {
    //
}

void TreeFileData::SetPath(wxString path) {
    this->path = path;
}

wxString TreeFileData::GetPath() {
    return this->path;
}

/**
 * Main frame window creation.
 */
MainFrame::MainFrame(const wxString& title)
  :wxFrame(NULL, wxID_ANY, title, wxDefaultPosition, wxDefaultSize, wxDEFAULT_FRAME_STYLE)
{
    int rc;

    App::outputDirectory = wxEmptyString;
    App::execDirectory = wxFileName::GetCwd();

    currentDirectory = wxGetCwd();

    // User's home directory
    wxString userHomeDirectory = wxStandardPaths::Get().GetUserConfigDir();

    SetAutoLayout(true);

    SetIcon(play_16x16_xpm);

    wxSize screenSize = wxGetDisplaySize();

    int windowWidth;
    int windowHeight;

    if (screenSize.GetWidth() >= 800)
        windowWidth = 800;
    else
        windowWidth = screenSize.GetWidth();

    if (screenSize.GetHeight() >= 400)
        windowHeight = 400;
    else
        windowHeight = screenSize.GetHeight();

    SetSize(windowWidth, windowHeight);

    // Top sizer

    wxBoxSizer *szTop = new wxBoxSizer(wxVERTICAL);

    SetSizer(szTop);

    // Controls

    wxPanel *panelAudio = new wxPanel(this, wxID_ANY, wxDefaultPosition);

    szTop->Add(panelAudio, 1, wxALL | wxEXPAND | wxGROW, 0);

    // Audio management panel

    App::panelAudioManager = new PanelAudio();

    rc = App::panelAudioManager->Create(panelAudio);

    if (rc != 0) {
        delete App::panelAudioManager;
    }

    wxGetApp().GetTopWindow()->Layout();

    Center(wxBOTH);
}

MainFrame::~MainFrame()
{
}

void MainFrame::OnKeyPress(wxKeyEvent& event)
{
    std::cerr << "MainFrame::OnKeyPress\n";
}

void MainFrame::ShowAbout()
{
    wxAboutDialogInfo aboutInfo;

    aboutInfo.SetName(_(APP_NAME));

    aboutInfo.SetVersion(_(APP_VERSION));

    aboutInfo.SetDescription(_(APP_DESCRIPTION));

    aboutInfo.SetCopyright("(C) 2018");

    aboutInfo.SetWebSite("http://www.olivum.com.br");

    aboutInfo.AddDeveloper("Allann Jones");

    wxAboutBox(aboutInfo);
}

void MainFrame::CloseRequest(wxCloseEvent& WXUNUSED(event))
{
    CloseApplication();
}

void MainFrame::OnQuit(wxCommandEvent& WXUNUSED(event))
{
    CloseApplication();
}

void MainFrame::CloseApplication() {
    Destroy();
}

wxString MainFrame::GetCurrentDirectory() {
    return currentDirectory;
}

void MainFrame::SetCurrentDirectory(wxString &path) {
    currentDirectory = path;
}

void MainFrame::DoUpdate()
{
}

// Event table

BEGIN_EVENT_TABLE(MainFrame, wxFrame)
    EVT_MENU(wxID_EXIT, MainFrame::OnQuit)

    EVT_BUTTON(wxID_EXIT, MainFrame::OnQuit)

    EVT_CLOSE(MainFrame::CloseRequest)

    EVT_CHAR(MainFrame::OnKeyPress)
END_EVENT_TABLE()
