/**
 * Audio panel header.
 *
 * \author Allann Jones
 */

#ifndef PANEL_AUDIO_HPP
#define PANEL_AUDIO_HPP

#include "utils_include.hpp"
#include "ui/panel_audio_player.hpp"
#include "ui/panel_audio_recorder.hpp"

#define GUI_AUDIO_EMPTY    0x00
#define GUI_AUDIO_PLAYER   0x01
#define GUI_AUDIO_RECORDER 0x02

class PanelAudio: wxWindow {
    private:

        wxFrame *frameParent;
        wxPanel *panel;
        wxScrolledWindow *panelAudio;
        wxAuiToolBar *toolbar;
        AudioControlPanel *audioControlPanel;
        AudioRecorderControlPanel *audioRecorderControlPanel;

        int currentInterface;

        void OnLoadFileRequest(wxCommandEvent &event);
        void OnListCleanRequest(wxCommandEvent &event);
        void OnAudioListItemActivated(wxListEvent &event);
        void OnAudioListItemKeyDown(wxListEvent &event);

        // Audio player

        int  AudioPlayerOpenFile(wxString &filePath);

        int  AudioPlayerCreate(wxString &filePath);

        void AudioPlayerDestroy(void);


        // Audio recorder

        int AudioRecorderCreate(void);
        void AudioRecorderDestroy(void);

        void OnAudioRecordDialogRequest(wxCommandEvent &event);

    public:

        PanelAudio();
        ~PanelAudio();

        int Create(wxPanel *panel);

        wxListCtrl *listCtrlAudioList;

        int ListClear(void);

        int AddAudioFile(wxString filePath);
};

#endif /* PANEL_AUDIO_HPP */
