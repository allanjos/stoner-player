/**
 * Audio player panel.
 * \author: Allann Jones
 */

#include "ui/panel_audio_player.hpp"
#include "audio/audio.hpp"
//#include "portaudio.h"
//#include "sndlib.h"
#include "ui/gui.hpp"
#include "app.hpp"
#include "utils_include.hpp"
#include "ui/dlg_main.hpp"
#include "pixmaps/play_hover_16x16.xpm"
#include "pixmaps/pause_hover_16x16.xpm"
#include "pixmaps/stop_hover_16x16.xpm"
#include "pixmaps/file_audio_16x16.xpm"
#include "pixmaps/play_16x16.xpm"
#include "pixmaps/stop_16x16.xpm"
#include "pixmaps/pause_16x16.xpm"

AudioControlPanel::AudioControlPanel(wxWindow *frame)
{
    frameParent = frame;

    panel = NULL;

    filePath = wxEmptyString;

    btnPlayStart = NULL;
    btnStop = NULL;
    btnPause = NULL;

    //audioFd = NULL;

    sndFile = NULL;

    audioFileIsOpened = false;

    audioStreamIsOpened = false;

    // Audio player initial state
    threadAudioPlayerStopRequested = false;
    threadAudioPlayerPauseRequested = false;
    threadAudioPlayerIsRunning = false;
}

AudioControlPanel::~AudioControlPanel()
{
    Close();
}

void AudioControlPanel::SetFilePath(wxString &path)
{
    this->filePath = path;
}

int AudioControlPanel::Create(wxPanel *panel)
{
    if (this->filePath.IsEmpty() || this->filePath.Length() <= 0) {
        return -1;
    }

    int rc;

    rc = audioInfo.GetFileInformation(this->filePath);

    if (rc != 0) {
        wxMessageBox(wxString::Format(wxT("Erro ao tentar obter informações sobre o arquivo de áudio '%s'"), this->filePath.c_str()));

        return -1;
    }

    fileInfo.Assign(this->filePath);

    this->panel = panel;

    panel->SetAutoLayout(TRUE);

    wxBoxSizer *szMargin = new wxBoxSizer(wxVERTICAL);

    panel->SetSizer(szMargin);

    szTop = new wxBoxSizer(wxVERTICAL);

    szMargin->Add(szTop, 1, wxALL | wxEXPAND | wxGROW, 1);

    wxBoxSizer *szRow;

    //
    // Title
    //
    szRow = new wxBoxSizer(wxHORIZONTAL);

    szTop->Add(szRow, 0, wxALL|wxEXPAND|wxGROW, 0);

    wxPanel *panelTitle = new wxPanel(panel, wxID_ANY);

    szRow->Add(panelTitle, 1, wxALL|wxEXPAND|wxGROW, 0);

    // File name
    wxBoxSizer *szTitle = new wxBoxSizer(wxHORIZONTAL);

    panelTitle->SetSizer(szTitle);

    wxBitmap bmpFileAudio(file_audio_16x16_xpm);
    wxStaticBitmap* stcBmpAudio = new wxStaticBitmap(panelTitle, wxID_STATIC, bmpFileAudio);
    szTitle->Add(stcBmpAudio, 0, wxALL | wxALIGN_CENTER_VERTICAL, 0);

    wxStaticText *lblTitle = new wxStaticText(panelTitle, wxID_STATIC, wxT(" Audio player"));

    wxFont titleFont = lblTitle->GetFont();

    titleFont.SetPointSize(titleFont.GetPointSize() + 2);

    lblTitle->SetFont(titleFont);

    szTitle->Add(lblTitle, 1, wxALL | wxALIGN_CENTER_VERTICAL | (wxALL & ~(wxTOP | wxBOTTOM)), 4);

    szTop->AddSpacer(1);

    wxBoxSizer *szAudioGraphContainer = new wxBoxSizer(wxHORIZONTAL);
    szTop->Add(szAudioGraphContainer, 1, wxALL | wxEXPAND | wxGROW, 0);

    wxPanel *panelAudioGraph = new wxPanel(panel, wxID_ANY);
    szAudioGraphContainer->Add(panelAudioGraph, 1, wxALL | wxEXPAND | wxGROW, 0);

    wxBoxSizer *szAudioGraph = new wxBoxSizer(wxHORIZONTAL);
    panelAudioGraph->SetSizer(szAudioGraph);

    audioGraphCanvas = new WaveFormCanvas(panelAudioGraph, panelAudioGraph, filePath, wxID_ANY, wxPoint(0,0), wxSize(200, 200));
    szAudioGraph->Add(audioGraphCanvas, 1, wxALL | wxEXPAND | wxGROW, 0);

    audioGraphCanvas->SetScrollbars(50, 1, 100, 200);

    // Controls

    wxBoxSizer *szControlsAndInfo = new wxBoxSizer(wxHORIZONTAL);
    szTop->Add(szControlsAndInfo, 0, wxALL | wxEXPAND | wxGROW, 1);

    wxStaticBox *staticBoxAudioControls = new wxStaticBox(panel, wxID_ANY, wxT("Controls"));
    wxStaticBoxSizer *szBoxAudioControls = new wxStaticBoxSizer(staticBoxAudioControls, wxVERTICAL);

    szControlsAndInfo->Add(szBoxAudioControls, 0, wxALL | wxALIGN_TOP, 1);

    szControlsAndInfo->Layout();

    szRow = new wxBoxSizer(wxHORIZONTAL);

    szBoxAudioControls->Add(szRow, 0, wxALL | wxALIGN_TOP, 0);

    wxBitmap bmpPlay16x16(play_16x16_xpm);
    wxBitmap bmpPlayHover16x16(play_hover_16x16_xpm);
    wxBitmap bmpPause16x16(pause_16x16_xpm);
    wxBitmap bmpPauseHover16x16(pause_hover_16x16_xpm);
    wxBitmap bmpStop16x16(stop_16x16_xpm);
    wxBitmap bmpStopHover16x16(stop_hover_16x16_xpm);

    // Play button
    btnPlayStart = new wxBitmapButton(panel, (int) DLG_AUDIO_CONTROL_BTN_PLAY_START_ID, bmpPlay16x16);

    btnPlayStart->SetBitmapHover(bmpPlayHover16x16);

    szRow->Add(btnPlayStart, 0, wxALL, 1);

    btnPlayStart->Connect((int) DLG_AUDIO_CONTROL_BTN_PLAY_START_ID, wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler(AudioControlPanel::PlayStartRequest), NULL, this);

    // Pause button

    btnPause = new wxBitmapButton(panel, DLG_AUDIO_CONTROL_BTN_PAUSE_ID, bmpPause16x16);

    btnPause->SetBitmapHover(bmpPauseHover16x16);

    //btnPause->Disable();

    szRow->Add(btnPause, 0, wxALL, 1);

    btnPause->Connect((int) DLG_AUDIO_CONTROL_BTN_PAUSE_ID, wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler(AudioControlPanel::PauseRequest), NULL, this);

    // Stop button

    btnStop = new wxBitmapButton(panel, DLG_AUDIO_CONTROL_BTN_STOP_ID, bmpStop16x16);

    btnStop->SetBitmapHover(bmpStopHover16x16);

    //btnStop->Disable();

    szRow->Add(btnStop, 0, wxALL, 1);

    btnStop->Connect((int) DLG_AUDIO_CONTROL_BTN_STOP_ID, wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler(AudioControlPanel::StopRequest), NULL, this);

    szTop->Layout();

    // Audio file information

    wxStaticBox *staticBoxInformation = new wxStaticBox(panel, wxID_ANY, wxT("File information"));
    wxStaticBoxSizer *szBoxInformation = new wxStaticBoxSizer(staticBoxInformation, wxVERTICAL);

    szControlsAndInfo->Add(szBoxInformation, 1, wxEXPAND|wxGROW|wxALL, 0);

    // File name
    szRow = new wxBoxSizer(wxHORIZONTAL);
    szBoxInformation->Add(szRow, 0, wxALL|wxEXPAND, 0);

    wxStaticText *label = new wxStaticText(panel, wxID_STATIC, wxT("File: "));
    label->SetForegroundColour(wxColour(0x44, 0x44, 0x44));
    szRow->Add(label, 0, wxALL|wxEXPAND, 0);

    wxStaticText *lblFileName = new wxStaticText(panel, wxID_STATIC, fileInfo.GetFullName());
    szRow->Add(lblFileName, 0, wxALL|wxEXPAND, 0);

    szBoxInformation->Layout();
    szRow->Layout();

    wxFlexGridSizer *szgFields = new wxFlexGridSizer(0, 2, 0, 30);
    szBoxInformation->Add(szgFields, 0, wxALL|wxEXPAND|wxGROW, 0);

    // Samples
    szRow = new wxBoxSizer(wxHORIZONTAL);
    szgFields->Add(szRow, 1, wxALL|wxEXPAND|wxGROW, 0);

    label = new wxStaticText(panel, wxID_STATIC, wxT("Samples: "));
    label->SetForegroundColour(wxColour(0x44, 0x44, 0x44));
    szRow->Add(label, 0, wxALL|wxEXPAND, 0);

    wxStaticText *lblFileSamples = new wxStaticText(panel, wxID_STATIC, wxString::Format(wxT("%ld"), audioInfo.GetSamples()));
    szRow->Add(lblFileSamples, 0, wxALL|wxEXPAND, 0);

    // Sample rate
    szRow = new wxBoxSizer(wxHORIZONTAL);
    szgFields->Add(szRow, 1, wxALL|wxEXPAND|wxGROW, 0);

    label = new wxStaticText(panel, wxID_STATIC, wxT("Sample rate: "));
    label->SetForegroundColour(wxColour(0x44, 0x44, 0x44));
    szRow->Add(label, 0, wxALL|wxEXPAND, 0);

    wxStaticText *lblFileSampleRate = new wxStaticText(panel, wxID_STATIC, wxString::Format(wxT("%ld"), audioInfo.GetSampleRate()));
    szRow->Add(lblFileSampleRate, 0, wxALL|wxEXPAND, 0);

    label = new wxStaticText(panel, wxID_STATIC, wxT(" HZ"));
    szRow->Add(label, 0, wxALL | wxEXPAND, 0);

    // Channels
    szRow = new wxBoxSizer(wxHORIZONTAL);
    szgFields->Add(szRow, 1, wxALL | wxEXPAND | wxGROW, 0);

    label = new wxStaticText(panel, wxID_STATIC, wxT("Channels: "));
    label->SetForegroundColour(wxColour(0x44, 0x44, 0x44));
    szRow->Add(label, 0, wxALL | wxEXPAND, 0);

    wxStaticText *lblFileChannels = new wxStaticText(panel, wxID_STATIC, wxString::Format(wxT("%ld"), audioInfo.GetChannels()));
    szRow->Add(lblFileChannels, 0, wxALL | wxEXPAND, 0);

    szRow->Layout();

    // Format
    szRow = new wxBoxSizer(wxHORIZONTAL);
    szgFields->Add(szRow, 1, wxALL | wxEXPAND | wxGROW, 0);

    label = new wxStaticText(panel, wxID_STATIC, wxT("Format: "));
    label->SetForegroundColour(wxColour(0x44, 0x44, 0x44));
    szRow->Add(label, 0, wxALL | wxEXPAND, 0);

    wxStaticText *lblFileFormatInfo = new wxStaticText(panel, wxID_STATIC, audioInfo.GetFormatInfo());
    szRow->Add(lblFileFormatInfo, 0, wxALL | wxEXPAND, 0);

    szRow->Layout();

    // Format
    szRow = new wxBoxSizer(wxHORIZONTAL);
    szgFields->Add(szRow, 1, wxALL | wxEXPAND | wxGROW, 0);

    label = new wxStaticText(panel, wxID_STATIC, wxT("Duration: "));
    label->SetForegroundColour(wxColour(0x44, 0x44, 0x44));
    szRow->Add(label, 0, wxALL | wxEXPAND, 0);

    int sampleRate = audioInfo.GetSampleRate();
    int samplesCount = audioInfo.GetSamples();
    float audioDuration = (float) samplesCount / (float) sampleRate;
    wxString audioDurationStr;

    if (audioDuration > 1.0) {
        audioDurationStr = audioInfo.GetDurationStr() + wxT(" (") + wxString::Format(wxT("%.2f"), audioDuration) + wxT(" seconds") + wxT(")");
    }
    else {
        audioDurationStr = audioInfo.GetDurationStr() + wxT(" (") + wxString::Format(wxT("%.2f"), audioDuration) + wxT(" second") + wxT(")");
    }

    wxStaticText *lblAudioDuration = new wxStaticText(panel, wxID_STATIC, audioDurationStr);
    szRow->Add(lblAudioDuration, 0, wxALL|wxEXPAND, 0);

    szRow->Layout();

    szgFields->AddSpacer(1);

    panel->Layout();

    std::cout << "Panel width: " << panel->GetSize().GetWidth() << std::endl;

    szBoxInformation->Layout();

    panel->Layout();
    szTop->Layout();

    wxGetApp().GetTopWindow()->Layout();
    wxGetApp().GetTopWindow()->Refresh();
    wxGetApp().GetTopWindow()->Update();

    return 0;
}

void AudioControlPanel::PlayStartRequest(wxCommandEvent& WXUNUSED(event))
{
    std::cerr << "AudioControlPanel::PlayStartRequest()\n";

    PlayThreadStart();
}

void AudioControlPanel::PlayThreadStart()
{
    std::cerr << "AudioControlPanel::PlayThreadStart()\n";

    PlayStart();

    /*
    threadAudioPlayer = new ThreadAudioPlayer(this);

    if (threadAudioPlayer->Create() != wxTHREAD_NO_ERROR) {
    //wxMessageBox(wxT("Não foi possível iniciar a thread para reprodução do arquivo de áudio."));

    return;
    }

    threadAudioPlayer->Run();
    */
}

void AudioControlPanel::PlayStart()
{
    if (threadAudioPlayerIsRunning) {
        fprintf(stderr, "Player is already running.\n");
        return;
    }

    threadAudioPlayerIsRunning = true;
    threadAudioPlayerStopRequested = false;
    threadAudioPlayerPauseRequested = false;

    //wxMutexGuiEnter();
    //btnPlayStart->Disable();
    //btnPause->Enable();
    //btnStop->Enable();
    //wxMutexGuiLeave();

    PaError rcAudio;
    int rc;
    //PaError err;
    PaStreamParameters outputParameters;
    PaStreamParameters inputParameters;
    short streamBuffer[4097];
    char audioPath[audioInfo.GetFilePath().Length() + 1];

    strcpy(audioPath, (const char *) audioInfo.GetFilePath().ToAscii());

    if (!audioFileIsOpened) {
        /*
        audioFd = fopen(audioPath, "rb");

        if (!audioFd) {
            wxMessageBox(wxString::Format(wxT("\nCannot open the audio file '%s'.\n\n"), audioPath));
            threadAudioPlayerIsRunning = false;
            return;
        }
        */

        sfInfo.format = 0;

        sndFile = sf_open(audioPath, SFM_READ, &sfInfo);

        if (sndFile == NULL) {
            wxMessageBox(wxT("Erro ao tentar abrir o descritor do arquivo de áudio SNDFILE."), wxT("Erro"), wxICON_ERROR);
        }

        audioFileIsOpened = true;
    }

    if (!sf_format_check(&sfInfo)) {
        wxMessageBox(wxT("Dados do arquivo de áudio aberto não são válidos"), wxT("Erro"), wxICON_ERROR);

        sf_close(sndFile);
    }
    else if (!audioStreamIsOpened) {
        rcAudio = Pa_Initialize();

        if (rcAudio != paNoError) {
            threadAudioPlayerIsRunning = false;

            sf_close(sndFile);

            fprintf(stderr, "PortAudio intialyzing error: %s\n", Pa_GetErrorText(rcAudio));

            return;
        }
        else {
            inputParameters.device = Pa_GetDefaultInputDevice();
            inputParameters.channelCount = audioInfo.GetChannels();

            switch (audioInfo.GetFormat()) {
                case SF_FORMAT_WAV:
                    inputParameters.sampleFormat = paInt16;
                case SF_FORMAT_PCM_16:
                    inputParameters.sampleFormat = paInt16; break;
                default:
                    inputParameters.sampleFormat = paInt16;
            }

            inputParameters.suggestedLatency = Pa_GetDeviceInfo(inputParameters.device)->defaultHighInputLatency ;
            inputParameters.hostApiSpecificStreamInfo = NULL;

            outputParameters.device = Pa_GetDefaultOutputDevice();
            outputParameters.channelCount = audioInfo.GetChannels();
            outputParameters.sampleFormat = inputParameters.sampleFormat;
            outputParameters.suggestedLatency = Pa_GetDeviceInfo(outputParameters.device)->defaultHighOutputLatency;
            outputParameters.hostApiSpecificStreamInfo = NULL;

            /* Open an audio I/O stream. */
            rcAudio = Pa_OpenStream(&stream,
                                    &inputParameters,
                                    &outputParameters,
                                    audioInfo.GetSampleRate(),
                                    paFramesPerBufferUnspecified,
                                    paClipOff,
                                    NULL,
                                    NULL);

            if (rcAudio != paNoError) {
                fprintf(stderr, "[PortAudio] Error on opening the I/O stream: %s\n", Pa_GetErrorText(rcAudio));
            }
            else {
                audioStreamIsOpened = true;

                /* Start stream */
                rcAudio = Pa_StartStream(stream);

                if (rcAudio != paNoError) {
                    fprintf(stderr, "[PortAudio] Error on starting stream: %s\n", Pa_GetErrorText(rcAudio));
                }
            }
        }
    }

    if (audioStreamIsOpened) {
        while (!threadAudioPlayerStopRequested && !threadAudioPlayerPauseRequested) {
            sf_count_t readFrames = sf_readf_short(sndFile, streamBuffer, 512);

            if (readFrames <= 0) {
                break;
            }

            audioGraphCanvas->UpdateGraph();

            rc = Pa_WriteStream(stream, streamBuffer, readFrames);

            if (rc) {
                fprintf(stderr, "[PortAudio] Error on writing data to stream.\n");
            }
        }

        // Start stream
        rcAudio = Pa_StopStream(stream);

        if (rcAudio != paNoError) {
            fprintf(stderr, "[PortAudio] Error on trying to stop stream: %s\n", Pa_GetErrorText(rcAudio));
        }

        // Terminating PortAudio
        rcAudio = Pa_CloseStream(stream);

        if (rcAudio != paNoError ) {
            fprintf(stderr, "PortAudio error: %s\n", Pa_GetErrorText(rcAudio));
        }
    }

    threadAudioPlayerStopRequested = false;
    threadAudioPlayerPauseRequested = false;

    rcAudio = Pa_Terminate();

    if (rcAudio != paNoError ) {
        fprintf(stderr, "PortAudio error: %s\n", Pa_GetErrorText(rcAudio));
    }

    audioStreamIsOpened = false;

    sf_close(sndFile);

    sndFile = NULL;

    /*
    fclose(audioFd);

    audioFd = NULL;
    */

    audioFileIsOpened = false;

    threadAudioPlayerIsRunning = false;

    /*
    wxMutexGuiEnter();
    btnPlayStart->Enable();
    btnStop->Disable();
    //btnPause->Disable();
    wxMutexGuiLeave();
    */

    printf("Audio player is finished.\n");
}

void AudioControlPanel::PlayerRequestStop()
{
    if (PlayerIsRunning()) {
        threadAudioPlayerStopRequested = true;
    }
}

void AudioControlPanel::PlayerRequestPause()
{
    if (PlayerIsRunning()) {
        threadAudioPlayerPauseRequested = true;
    }
}

bool AudioControlPanel::PlayerIsRunning()
{
    return threadAudioPlayerIsRunning;
}

void AudioControlPanel::WaitPlayerStop()
{
    while (PlayerIsRunning()) {
        printf("Waiting audio player stop.\n");
        wxMilliSleep(400);
    }
}

void AudioControlPanel::StopRequest(wxCommandEvent& WXUNUSED(event))
{
    PlayerRequestStop();

    WaitPlayerStop();
}

void AudioControlPanel::PauseRequest(wxCommandEvent& WXUNUSED(event))
{
    PlayerRequestPause();

    WaitPlayerStop();
}

void AudioControlPanel::CloseRequest(wxCommandEvent& WXUNUSED(event))
{
    Close();
}

void AudioControlPanel::Close()
{
    // Checking the audio player
    if (PlayerIsRunning()) {
        PlayerRequestStop();

        WaitPlayerStop();
    }

    if (audioFileIsOpened) {
        sf_close(sndFile);

        sndFile = NULL;

        /*
        fclose(audioFd);

        audioFd = NULL;
        */
    }

    printf("Audio player is stopped.\n");

    if (audioStreamIsOpened) {
        printf("Audio device is opened. Trying to close it.\n");

        PaError rcAudio;

        rcAudio = Pa_CloseStream(stream);

        if (rcAudio != paNoError ) {
            fprintf(stderr, "PortAudio error: %s\n", Pa_GetErrorText(rcAudio));
        }

        rcAudio = Pa_Terminate();

        if (rcAudio != paNoError) {
            fprintf(stderr, "PortAudio error: %s\n", Pa_GetErrorText(rcAudio));
        }

        audioStreamIsOpened = false;
    }

    printf("Audio player destroyed.\n");

    if (panel != NULL) {
        szTop->Clear();

        panel->SetSizer(NULL);

        panel->DestroyChildren();
    }

    wxGetApp().GetTopWindow()->Layout();

    /*
    if (panel != NULL) {
    szTop->Clear();

    panel->SetSizer(NULL);

    panel->DestroyChildren();
    }
    */
}

/*
 * Audio player thread.
 */

ThreadAudioPlayer::ThreadAudioPlayer(AudioControlPanel *audioControl)
{
    this->audioControl = audioControl;
}

void *ThreadAudioPlayer::Entry()
{
    printf("Audio player is running.\n");

    audioControl->PlayStart();

    return NULL;
}