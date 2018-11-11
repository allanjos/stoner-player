/**
 * Audio canvas.
 *
 * \author: Allann Jones
 */

#include <cmath>
#include "ui/gui_controls.hpp"
#include "ui/utils_audio_plot.hpp"

WaveFormCanvas::WaveFormCanvas(wxPanel *waveFormPanel, wxWindow *parent, wxString audioFilePath, wxWindowID id, const wxPoint &pos, const wxSize &size)
    : wxScrolledWindow( parent, CANVAS_AUDIO_PLOT_CANVAS_ID, pos, size, wxSIMPLE_BORDER|wxCLIP_CHILDREN)
{
    this->audioFilePath = audioFilePath;

    SetBackgroundColour(*wxBLACK);
    SetForegroundColour(*wxWHITE);

    //this->frameParent = frameParent;

    //this->waveFormPanel = waveFormPanel;

    /*
    wxBitmap bitmap( 100, 100 );

    wxMemoryDC dc;
    dc.SelectObject( bitmap );
    dc.SetBrush( wxBrush( wxT("orange"), wxSOLID ) );
    dc.SetPen( *wxBLACK_PEN );
    dc.DrawRectangle( 0, 0, 100, 100 );
    dc.SetBrush( *wxWHITE_BRUSH );
    dc.DrawRectangle( 20, 20, 60, 60 );
    dc.SelectObject( wxNullBitmap );
    */

    audioData.clear();

    SNDFILE *sndFile;
    SF_INFO sfInfo;
    short streamBuffer[4097];

    sndFile = sf_open(audioFilePath.mb_str(wxConvUTF8), SFM_READ, &sfInfo);

    if (sndFile == NULL) {
        wxMessageBox(wxT("Erro ao tentar abrir o descritor do arquivo de áudio SNDFILE."), wxT("Erro"), wxICON_ERROR);
        return;
    }

    int x = 0;

    while (1) {
        sf_count_t readFrames = sf_readf_short(sndFile, streamBuffer, 512);

        if (readFrames <= 0) {
            break;
        }

        for (int i = 0; i < readFrames; i++) {
            audioData.push_back(streamBuffer[i]);
        }
    }

    sf_close(sndFile);

    Connect((int) CANVAS_AUDIO_PLOT_CANVAS_ID, wxEVT_LEFT_DOWN, wxMouseEventHandler(WaveFormCanvas::OnLeftDown), NULL, this);
    Connect((int) CANVAS_AUDIO_PLOT_CANVAS_ID, wxEVT_MOTION, wxMouseEventHandler(WaveFormCanvas::OnMotion), NULL, this);
}

WaveFormCanvas::~WaveFormCanvas()
{
}

void WaveFormCanvas::OnLeftDown(wxMouseEvent &event)
{
    //wxMessageBox(wxT("OnLeftDown"));
}

void WaveFormCanvas::OnMotion(wxMouseEvent &event)
{
    //wxMessageBox(wxT("OnMotion"));
    wxPoint point = event.GetPosition();

    if (event.Dragging()) {
        wxMessageBox(wxString::Format(wxT("Dragging with mouse. Position=(%d, %d)"), point.x, point.y));
    }
}

void WaveFormCanvas::OnPaint(wxPaintEvent &WXUNUSED(event))
{
    wxPaintDC dc(this);
    PrepareDC(dc);

    //wxFont font = dc.GetFont();
    //std::cout << "DC default font point size: " << font.GetPointSize() << std::endl;
    //font.SetPointSize(waveFormPanel->graphTextFontSize);
    //dc.SetFont(font);

    //waveFormPanel->DrawStatisticalGraphMonth(dc);

    short streamBuffer[4097];
    wxString buffTmp;

    /*
    FILE *audioFd = fopen(audioFilePath.mb_str(wxConvUTF8), "rb");

    if (!audioFd) {
    wxMessageBox(wxString::Format(wxT("\nCannot open the audio file '%s'.\n\n"), audioFilePath.c_str()));
    }

    while (!feof(audioFd)) {
    memset(streamBuffer, 0, 512 * sizeof(short));

    int readBytes = fread(streamBuffer, sizeof(short), 512, audioFd);

    if (readBytes > 0) {
    for (int i = 0; i < readBytes; i++) {
    buffTmp.Printf(wxT("%s %d "), buffTmp.c_str(), streamBuffer[i]);
    }
    buffTmp.Printf(wxT("%s\n"), buffTmp.c_str());
    }
    }
    */

    SNDFILE *sndFile;
    SF_INFO sfInfo;
    int rc;

    sfInfo.format = 0;

    float pixelInchProportion = 14;

    // Using wxMM_TWIPS where Each logical unit is 1/20 of a point, or 1/1440 of an inch.
    // The pixel units must be converted to the wxMM_TWIPS measure.
    dc.SetMapMode(wxMM_TWIPS);

    int height = GetSize().GetHeight();
    int width = GetSize().GetWidth();

    int virtualWidth;
    int virtualHeight;

    GetVirtualSize(&virtualWidth, &virtualHeight);

    int marginTop = 17 * pixelInchProportion;
    int marginBottom = 17 * pixelInchProportion;

    virtualHeight -= 20;

    virtualWidth *= pixelInchProportion; //! \todo This is not exact, search for the relation between pixels and inchs.
    virtualHeight *= pixelInchProportion;
    width *= pixelInchProportion; //! \todo This is not exact, search for the relation between pixels and inchs.
    height *= pixelInchProportion;

    //wxMessageBox(wxString::Format(wxT("SIZE [ height: %d, width: %d ]  VIRTUAL SIZE: [ height: %d, width: %d ]"), height, width, virtualHeight, virtualWidth));

    //wxMessageBox(wxString::Format(wxT("Virtual size: height=%d, width=%d"), virtualHeight, virtualWidth));

    // Top Y
    int y0 = marginTop;
    // Maximum Y
    int yMax = virtualHeight - (2 * y0);
    // Middle Y
    //int yMiddle = (height * 13) / 2;
    int yMiddle = (virtualHeight - y0) / 2;
    // X's coordinate origin
    int x0 = pixelInchProportion;
    // Maximum audio data absolute value
    int maxAbsoluteAudioDataValue = yMiddle - marginTop;
    int currentX = 0;
    int currentY = 0;
    int previousX = 0;
    int previousY = 0;
    int previousValue = 0;

    /*
       dc.SetBrush(wxBrush(wxT("black"), wxSOLID));
       dc.SetPen(*wxBLACK_PEN);
       dc.DrawPoint(virtualWidth, yMiddle);
       */

    /*
       sndFile = sf_open(audioFilePath.mb_str(wxConvUTF8), SFM_READ, &sfInfo);

       if (sndFile == NULL) {
       wxMessageBox(wxT("Erro ao tentar abrir o descritor do arquivo de áudio SNDFILE."), wxT("Erro"), wxICON_ERROR);
       return;
       }
       */

    int x = x0;

    //dc.SetBrush(wxBrush(wxT("orange"), wxSOLID));
    //dc.SetPen(*wxBLACK_PEN );

    short majorAudioDataValue;
    short minorAudioDataValue;
    short majorAudioAbsoluteDataValue;
    short minorAudioAbsoluteDataValue;

    for (int audioDataIndex = 0; audioDataIndex < audioData.size(); audioDataIndex++) {
        if (audioDataIndex > 0) {
            if (audioData[audioDataIndex] > majorAudioDataValue) {
                majorAudioDataValue = audioData[audioDataIndex];
            }

            if (audioData[audioDataIndex] < minorAudioDataValue) {
                minorAudioDataValue = audioData[audioDataIndex];
            }

            if (fabs(audioData[audioDataIndex]) > majorAudioAbsoluteDataValue) {
                majorAudioAbsoluteDataValue = fabs(audioData[audioDataIndex]);
            }

            if (fabs(audioData[audioDataIndex]) < minorAudioAbsoluteDataValue) {
                minorAudioAbsoluteDataValue = fabs(audioData[audioDataIndex]);
            }
        }
        else {
            majorAudioDataValue = audioData[audioDataIndex];
            minorAudioDataValue = audioData[audioDataIndex];
            majorAudioAbsoluteDataValue = fabs(audioData[audioDataIndex]);
            minorAudioAbsoluteDataValue = fabs(audioData[audioDataIndex]);
        }
    }

    //wxMessageBox(wxString::Format(wxT("AUDIO DATA - Major value: %d, minor value: %d, maximum absolute value: %d"), majorAudioDataValue, minorAudioDataValue, yMiddle - y0));

    // Middle line, X coordinate
    //dc.SetBrush(wxBrush(wxT("blue"), wxSOLID));
    dc.SetPen(wxPen(*wxBLUE));
    //dc.DrawLine(0, yMiddle, currentX + pixelInchProportion, yMiddle);
    dc.DrawLine(0, yMiddle, virtualWidth * pixelInchProportion, yMiddle);


    // Audio data
    for (int audioDataIndex = 0; audioDataIndex < audioData.size(); audioDataIndex++) {
        int value;

        // 0xea, 0xf6, 0x28
        int colorRed;
        int colorGreen;
        int colorBlue;

        if (fabs(audioData[audioDataIndex]) > 8000 || fabs(previousValue) > 8000) {
            colorRed = 0xd4;
            colorGreen = 0x05;
            colorBlue = 0x05;
        }
        else if (fabs(audioData[audioDataIndex]) > 6000 || fabs(previousValue) > 6000) {
            colorRed = 0xf8;
            colorGreen = 0x6a;
            colorBlue = 0x6a;
        }
        else if (fabs(audioData[audioDataIndex]) > 5000 || fabs(previousValue) > 5000) {
            colorRed = 0xf6;
            colorGreen = 0x0a;
            colorBlue = 0x0a;
        }
        else if (fabs(audioData[audioDataIndex]) > 3000 || fabs(previousValue) > 3000) {
            colorRed = 0xf1;
            colorGreen = 0x7d;
            colorBlue = 0x0c;
        }
        else if (fabs(audioData[audioDataIndex]) > 2000 || fabs(previousValue) > 2000) {
            colorRed = 0xf1;
            colorGreen = 0x96;
            colorBlue = 0x3c;
        }
        else if (fabs(audioData[audioDataIndex]) > 1000 || fabs(previousValue) > 1000) {
            colorRed = 0xfd;
            colorGreen = 0xc6;
            colorBlue = 0x0a;
        }
        else if (fabs(audioData[audioDataIndex]) > 500 || fabs(previousValue) > 500) {
            colorRed = 0xf3;
            colorGreen = 0xca;
            colorBlue = 0x3c;
        }
        else {
            colorRed = 0xf1;
            colorGreen = 0xf3;
            colorBlue = 0x3c;
        }

        if (audioData[audioDataIndex] != 0) {
            value = maxAbsoluteAudioDataValue * audioData[audioDataIndex] / majorAudioAbsoluteDataValue;
        }
        else {
            value = 0;
        }

        currentX = ++x;
        currentY = yMiddle - value;

        //int limiarBottom =

        if (audioDataIndex > 0) {
            dc.SetPen(wxPen(wxColour(colorRed, colorGreen, colorBlue)));
            dc.DrawLine(previousX, previousY, currentX, currentY);
        }
        else {
            dc.SetPen(*wxGREEN_PEN);
            dc.DrawPoint(currentX, yMiddle - value);
        }

        previousX = currentX;
        previousY = currentY;
        previousValue = value;
    }

    dc.SetPen(*wxGREEN_PEN);

    // Beginning audio data line
    dc.DrawLine(0, y0, 0, yMax);

    // Final audio data line
    dc.DrawLine(currentX + pixelInchProportion, y0, currentX + pixelInchProportion, yMax);

    dc.SetPen(*wxWHITE_PEN);

    // Top line
    //dc.DrawLine(0, y0 - pixelInchProportion, currentX + pixelInchProportion, y0 - pixelInchProportion);
    // Top line
    dc.DrawLine(0, yMax + pixelInchProportion, currentX + pixelInchProportion, yMax + pixelInchProportion);

    //dc.DrawRectangle(0, 0, currentX + pixelInchProportion, y0 - pixelInchProportion);
    dc.DrawRectangle(0, 0, virtualWidth * pixelInchProportion, y0 - pixelInchProportion);

    wxFont font = dc.GetFont();

    font.SetPointSize(100);
    font.SetFamily(wxFONTFAMILY_MODERN);
    //font.SetStyle(wxFONTFLAG_ANTIALIASED);

    dc.SetFont(font);

    //wxMessageBox(wxString::Format(wxT("point size: %d"), dc.GetFont().GetPointSize()));

    dc.SetPen(*wxWHITE_PEN);
    dc.DrawText(wxString::Format(wxT("Max: %d, Min: %d"), majorAudioDataValue, minorAudioDataValue), 5 * pixelInchProportion, y0 - 15 * pixelInchProportion);

    /*
    dc.DrawText( _T("Loaded image"), 30, 10 );
    //if (my_square.Ok())
    //    dc.DrawBitmap( my_square, 30, 30 );

    dc.DrawText( _T("Drawn directly"), 150, 10 );
    dc.SetBrush( wxBrush( wxT("orange"), wxSOLID ) );
    dc.SetPen( *wxBLACK_PEN );
    dc.DrawRectangle( 150, 30, 100, 100 );
    dc.SetBrush( *wxWHITE_BRUSH );
    dc.DrawRectangle( 170, 50, 60, 60 );
    */
}

void WaveFormCanvas::SetAudioCurrentTime(int currentTime)
{
    this->audioCurrentTime = currentTime;
}

void WaveFormCanvas::UpdateGraph()
{
    //Refresh();
}

BEGIN_EVENT_TABLE(WaveFormCanvas, wxScrolledWindow)
EVT_PAINT(WaveFormCanvas::OnPaint)
END_EVENT_TABLE()