#include "app.hpp"

PanelAudio *App::panelAudioManager = NULL;

MainFrame *App::mainFrame = NULL;

wxString App::outputDirectory;

wxString App::execDirectory;

wxString App::systemEncodingName = wxT("iso-8859-1");

wxCSConv App::systemEncoding(systemEncodingName);

IMPLEMENT_APP(MainApplication)

bool MainApplication::OnInit()
{
    wxImage::AddHandler(new wxGIFHandler);
    wxImage::AddHandler(new wxPNGHandler);

    wxFileSystem::AddHandler(new wxZipFSHandler);

    mainFrame = new MainFrame(wxT(APP_NAME " - " APP_DESCRIPTION " - " APP_VERSION));

    mainFrame->Show(TRUE);

    SetTopWindow(mainFrame);

    mainFrame->Raise();

    return TRUE;
}

int MainApplication::FilterEvent(wxEvent& event)
{
    if (event.GetEventType() == wxEVT_KEY_DOWN && ((wxKeyEvent&)event).GetKeyCode() == WXK_F1 && mainFrame) {
        std::cerr << "F1 was pressed\n";

        std::cerr << "Main frame has focus. Opening about dialog.\n";

        mainFrame->ShowAbout();

        return true;
    }

    return -1;
}