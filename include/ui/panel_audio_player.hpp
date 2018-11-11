/**
 * Audio player painel header.
 * \author: Allann Jones
 */

#ifndef PANEL_AUDIO_PLAYER_HPP
#define PANEL_AUDIO_PLAYER_HPP


#include "ui/gui.hpp"
#include "ui/utils_audio_plot.hpp"
#include "audio/audio.hpp"
#include "portaudio.h"

class AudioControlPanel;

/**
 * Audio player thread.
 */
class ThreadAudioPlayer: public wxThread
{
    public:
        AudioControlPanel *audioControl;

        ThreadAudioPlayer(AudioControlPanel *audioControl);

        virtual void *Entry();
};

class AudioControlPanel: wxWindow  {
    private:

        wxBoxSizer *szTop;
        wxWindow *frameParent;
        wxPanel *panel;

        wxString filePath;
        AudioInfo audioInfo;

        wxFileName fileInfo;

        wxBitmapButton *btnPlayStart;
        wxBitmapButton *btnStop;
        wxBitmapButton *btnPause;
        wxBitmapButton *btnClose;

        SNDFILE *sndFile;
        SF_INFO sfInfo;
        sf_count_t frameCurrent;

        bool audioFileIsOpened;
        bool audioStreamIsOpened;
        bool audioStreamEnd;
        PaStream *stream;

        WaveFormCanvas *audioGraphCanvas;

    public:

        ThreadAudioPlayer *threadAudioPlayer;
        bool threadAudioPlayerIsRunning;
        bool threadAudioPlayerStopRequested;
        bool threadAudioPlayerPauseRequested;

        AudioControlPanel(wxWindow *frame);

        ~AudioControlPanel();

        void SetFilePath(wxString &path);

        int Create(wxPanel *panelParent);

        void PlayThreadStart(void);

        void PlayStart(void);

        void PlayerRequestStop(void);

        void PlayerRequestPause(void);

        bool PlayerIsRunning(void);

        void WaitPlayerStop(void);

        void PlayStartRequest(wxCommandEvent& WXUNUSED(event));

        void StopRequest(wxCommandEvent& WXUNUSED(event));

        void PauseRequest(wxCommandEvent& WXUNUSED(event));

        void CloseRequest(wxCommandEvent& WXUNUSED(event));

        void Close(void);
};

#endif /* PANEL_AUDIO_PLAYER_HPP */