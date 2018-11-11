/**
 * GUI main header. Main dialog.
 *
 * \file  main.hpp
 * \author Allann Jones
 */

/** \defgroup DlgMain Graphical User Interface - Dialogs - Main dialog */
/** @{ */

#ifndef DLG_MAIN_HPP
#define DLG_MAIN_HPP

#include <wx/xml/xml.h>

#include "ui/gui.hpp"
#include "ui/panel_audio.hpp"
#include "utils_filesystem.hpp"

typedef enum {
    NO_INTERFACE = -1,
    GUI_AUDIO_CONTROL = 0
} InterfaceID_t;

typedef struct {
    wxString path;
} FileInfo_t;

/**
 * Tree file data.
 */
class TreeFileData : public wxTreeItemData
{
    private:

        wxString path;

    public:

        TreeFileData();

        void SetPath(wxString path);

        wxString GetPath();
};

/**
 * Main frame. The main window of the application.
 */
class MainFrame: public wxFrame
{
    private:

        wxString currentDirectory;

        void OnKeyPress(wxKeyEvent& event);

        /**
         * Handles exit request in the main window.
         */
        void OnQuit(wxCommandEvent &event);

        /**
         * Close event handler.
         */
        void CloseRequest(wxCloseEvent &event);

        /**
         * Presents the dialog to close application.
         */
        void CloseApplication(void);

        /**
         * On key pressed.
         */
        void OnChar(wxKeyEvent& event);

    protected:

        /**
         * Event table declaration (Internal to wxWidgets).
         */
        DECLARE_EVENT_TABLE()

    public:

        /**
         * Object constructor.
         */
        MainFrame(const wxString& title);

        ~MainFrame();

        wxString GetCurrentDirectory(void);

        void SetCurrentDirectory(wxString &path);

        void DoUpdate();

        void ShowAbout();
};

#endif /* DLG_MAIN_HPP */

/** @} */