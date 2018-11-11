#ifndef APP_HPP
#define APP_HPP

#include "ui/gui.hpp"
#include "ui/dlg_main.hpp"
#include "ui/panel_audio.hpp"

class App {
    public:

        static PanelAudio *panelAudioManager;

        static MainFrame *mainFrame;

        static wxString execDirectory;

        static wxString outputDirectory;

        static wxString systemEncodingName;

        static wxCSConv systemEncoding;
};

/**
 * Main application
 */
class MainApplication: public wxApp
{
    private:

        MainFrame *mainFrame;

        int FilterEvent(wxEvent& event);

    public:

        /**
         * Called on dialog initialization (after instantiation).
         */
        virtual bool OnInit();
};

DECLARE_APP(MainApplication)

#endif /* APP_HPP */