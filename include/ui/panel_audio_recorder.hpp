/**
 * Audio recorder panel.
 * \author Allann Jones
 */

#ifndef PANEL_AUDIO_RECORDER_HPP
#define PANEL_AUDIO_RECORDER_HPP

#include "ui/gui.hpp"
#include "ui/utils_audio_plot.hpp"
#include "audio/audio.hpp"
#include "portaudio.h"
//#include "sndlib.h"

class AudioRecorderControlPanel;

/**
 * Audio recorder thread.
 */
class ThreadAudioRecorder: public wxThread
{
    public:
        AudioRecorderControlPanel *audioControl;

        ThreadAudioRecorder(AudioRecorderControlPanel *audioControl);

        virtual void *Entry();
};

/**
 *  Audio recorder panel.
 */
class AudioRecorderControlPanel: wxWindow  {
    private:
        wxBoxSizer *szTop;
        wxWindow *frameParent;
        wxPanel *panel;

        wxString filePath;
        AudioInfo audioInfo;

        PaStreamParameters outputParameters;
        PaStreamParameters inputParameters;
        short *streamBuffer;

        wxFileName fileInfo;

        wxStaticText *lblFileSamples;

        wxChoice *choiceInputDevice;
        wxComboBox *comboSampleRate;
        wxChoice *choiceChannels;
        wxChoice *choiceSampleFormat;

        wxBitmapButton *btnSave;
        wxBitmapButton *btnRecordStart;
        wxBitmapButton *btnStop;
        wxBitmapButton *btnPause;
        wxBitmapButton *btnClose;

        wxStatusBar *statusBar;

        //SNDFILE *sndFile;
        SF_INFO sfInfo;
        sf_count_t frameCurrent;

        //FILE *audioFd;

        bool audioFileIsOpened;

        PaStream *stream;

        bool audioDeviceIsOpened;

        bool audioStreamEnd;

        WaveFormCanvas *audioGraphCanvas;

        static int RecordCallback(const void *inputBuffer,
                                  void *outputBuffer,
                                  unsigned long framesPerBuffer,
                                  const PaStreamCallbackTimeInfo* timeInfo,
                                  PaStreamCallbackFlags statusFlags,
                                  void *userData);

    public:

        ThreadAudioRecorder *threadAudioRecorder;

        bool threadAudioRecorderIsRunning;
        bool threadAudioRecorderStopRequested;
        bool threadAudioRecorderPauseRequested;

        AudioRecorderControlPanel(wxWindow *frame);

        ~AudioRecorderControlPanel();

        void SetFilePath(wxString &path);

        int Create(wxPanel *panelParent);

        void LoadAudioDevicesInfo();

        void RecordThreadStart(void);

        void RecordStart(void);

        void RecorderRequestStop(void);

        void RecorderRequestPause(void);

        bool RecorderIsRunning(void);

        void WaitRecorderStop(void);

        void SetSampleRateRequest(wxCommandEvent& WXUNUSED(event));

        void SetChannelsRequest(wxCommandEvent& WXUNUSED(event));

        void SetSamplesFormatRequest(wxCommandEvent& WXUNUSED(event));

        void RecordSaveRequest(wxCommandEvent& WXUNUSED(event));

        void RecordStartRequest(wxCommandEvent& WXUNUSED(event));

        void StopRequest(wxCommandEvent& WXUNUSED(event));

        /** Request stop recording and wait for stop it */
        int StopRecording(void);

        void PauseRequest(wxCommandEvent& WXUNUSED(event));

        void CloseRequest(wxCommandEvent& WXUNUSED(event));

        void Close(void);
};

#endif /* PANEL_AUDIO_RECORDER_HPP */
