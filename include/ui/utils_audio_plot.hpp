#ifndef _UTILS_AUDIO_PLOT_HPP
#define _UTILS_AUDIO_PLOT_HPP

#include <vector>
#include <map>
#include "gui.hpp"
#include "audio/audio.hpp"
#include "portaudio.h"


class WaveFormCanvas;

class WaveFormPanel: wxWindow  {
    private:

        wxBoxSizer *szTop;
        wxFrame *frameParent;
        wxPanel *panel;

        wxFileName fileInfo;

        wxString filePath;

        wxString fileDir;

        wxBitmapButton *btnSave;
        wxBitmapButton *btnPrint;
        wxBitmapButton *btnStatistic;
        wxBitmapButton *btnClose;

        wxStatusBar *statusBar;

        bool audioControlIsOpened;

        //DECLARE_EVENT_TABLE()

    public:

        WaveFormPanel(wxFrame *frame);

        ~WaveFormPanel();

        void Close(void);

        int Create(wxPanel *panelParent);
};

class WaveFormCanvas: public wxScrolledWindow
{
    private:

        wxFrame *frameParent;
        WaveFormPanel *waveFormPanel;
        wxString audioFilePath;

        std::vector<short> audioData; /**< Audio data. */
        int audioCurrentTime;

        DECLARE_EVENT_TABLE()

        void OnPaint(wxPaintEvent &event);

        void OnLeftDown(wxMouseEvent &event);

        void OnMotion(wxMouseEvent &event);

    public:

        WaveFormCanvas(wxPanel *waveFormPanel, wxWindow *parent, wxString audioFilePath, wxWindowID, const wxPoint &pos, const wxSize &size);
        ~WaveFormCanvas();

        void UpdateGraph(void);
        void SetAudioCurrentTime(int currentTime);
};


#endif /* _UTILS_AUDIO_PLOT_HPP */
