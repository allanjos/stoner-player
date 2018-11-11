/**
 * Audio recorder panel.
 * \author Allann Jones
 */

#include "app.hpp"
#include "ui/panel_audio_recorder.hpp"
#include "audio/audio.hpp"
#include "ui/gui.hpp"
#include "utils_include.hpp"
#include "ui/dlg_main.hpp"
#include "pixmaps/audio_recorder_16x16.xpm"
#include "pixmaps/record_16x16.xpm"
#include "pixmaps/save_hover_16x16.xpm"
#include "pixmaps/record_hover_16x16.xpm"
#include "pixmaps/pause_hover_16x16.xpm"
#include "pixmaps/stop_hover_16x16.xpm"
#include "pixmaps/save_16x16.xpm"
#include "pixmaps/stop_16x16.xpm"
#include "pixmaps/pause_16x16.xpm"
#include "utils_string.hpp"

wxString strListRecorderAudioEncoding[] = {wxT("LIN16")};
wxString strListRecorderAudioSampleFormat[] = {wxT("INT16")};

AudioRecorderControlPanel::AudioRecorderControlPanel(wxWindow *frame)
{
    frameParent = frame;

    panel = NULL;

    filePath = wxEmptyString;

    btnRecordStart = NULL;
    btnStop = NULL;
    btnPause = NULL;

    //audioFd = NULL;

    //sndFile = NULL;

    //audioFileIsOpened = false;

    audioDeviceIsOpened = false;

    // Audio recorder initial state
    threadAudioRecorderStopRequested = false;
    threadAudioRecorderPauseRequested = false;
    threadAudioRecorderIsRunning = false;

    // Audio information
    audioInfo.SetChannels(2);
    audioInfo.SetSampleRate(32000);

    sfInfo.channels = audioInfo.GetChannels();
    sfInfo.format = SF_FORMAT_WAV | SF_FORMAT_PCM_16;
    sfInfo.samplerate = audioInfo.GetSampleRate();
    sfInfo.sections = 1;
    sfInfo.seekable = 1;
    sfInfo.frames = 0;

    audioInfo.SetSFInfo(sfInfo);
}

AudioRecorderControlPanel::~AudioRecorderControlPanel()
{
    Close();
}

void AudioRecorderControlPanel::SetFilePath(wxString &path)
{
    this->filePath = path;
}

int AudioRecorderControlPanel::Create(wxPanel *panel)
{
    wxStaticText *label;

    /*
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
       */

    this->panel = panel;

    panel->SetAutoLayout(TRUE);

    /*
       szTop = new wxBoxSizer(wxVERTICAL);

       panel->SetSizer(szTop);
       */

    wxBoxSizer *szMargin = new wxBoxSizer(wxVERTICAL);

    panel->SetSizer(szMargin);

    szTop = new wxBoxSizer(wxVERTICAL);

    szMargin->Add(szTop, 1, wxALL|wxEXPAND|wxGROW, 1);

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

    wxBitmap bmpFileAudio(audio_recorder_16x16_xpm);
    wxStaticBitmap* stcBmpAudio = new wxStaticBitmap(panelTitle, wxID_STATIC, bmpFileAudio);
    szTitle->Add(stcBmpAudio, 0, wxALL | wxALIGN_CENTER_VERTICAL, 0);

    wxStaticText *lblTitle = new wxStaticText(panelTitle, wxID_STATIC, wxT(" Audio recorder"));

    wxFont titleFont = lblTitle->GetFont();

    titleFont.SetPointSize(titleFont.GetPointSize() + 2);

    lblTitle->SetFont(titleFont);

    szTitle->Add(lblTitle, 1, wxALL | wxALIGN_CENTER_VERTICAL | (wxALL & ~(wxTOP | wxBOTTOM)), 4);

    szTop->AddSpacer(1);

#if 0
    wxBoxSizer *szAudioGraphContainer = new wxBoxSizer(wxHORIZONTAL);
    szTop->Add(szAudioGraphContainer, 1, wxALL|wxEXPAND|wxGROW, 0);

    wxPanel *panelAudioGraph = new wxPanel(panel, wxID_ANY);
    //panelAudioGraph->SetBackgroundColour(*wxWHITE);
    szAudioGraphContainer->Add(panelAudioGraph, 1, wxALL|wxEXPAND|wxGROW, 0);


    wxBoxSizer *szAudioGraph = new wxBoxSizer(wxHORIZONTAL);
    panelAudioGraph->SetSizer(szAudioGraph);


    audioGraphCanvas = new WaveFormCanvas(panelAudioGraph, panelAudioGraph, filePath, wxID_ANY, wxPoint(0,0), wxSize(200, 200));
    szAudioGraph->Add(audioGraphCanvas, 1, wxALL|wxEXPAND|wxGROW, 0);

    audioGraphCanvas->SetScrollbars(50, 1, 100, 200);
#endif

    //
    // Controls
    //

    wxBoxSizer *szControlsAndInfo = new wxBoxSizer(wxHORIZONTAL);
    szTop->Add(szControlsAndInfo, 0, wxALL|wxEXPAND|wxGROW, 1);

    wxStaticBox *staticBoxAudioControls = new wxStaticBox(panel, wxID_ANY, wxT("Controls"));
    wxStaticBoxSizer *szBoxAudioControls = new wxStaticBoxSizer(staticBoxAudioControls, wxVERTICAL);

    szControlsAndInfo->Add(szBoxAudioControls, 0, wxRIGHT|wxALIGN_TOP, 1);

    szControlsAndInfo->Layout();

    szRow = new wxBoxSizer(wxHORIZONTAL);

    //szControlsAndInfo->Add(szRow, 0, wxALL, 5);
    szBoxAudioControls->Add(szRow, 0, wxALL|wxALIGN_TOP, 0);

    // Bitmaps for buttons
    wxBitmap bmpSave16x16(save_16x16_xpm);
    wxBitmap bmpSaveHover16x16(save_hover_16x16_xpm);
    wxBitmap bmpRecord16x16(record_16x16_xpm);
    wxBitmap bmpRecordHover16x16(record_hover_16x16_xpm);
    wxBitmap bmpPause16x16(pause_16x16_xpm);
    wxBitmap bmpPauseHover16x16(pause_hover_16x16_xpm);
    wxBitmap bmpStop16x16(stop_16x16_xpm);
    wxBitmap bmpStopHover16x16(stop_hover_16x16_xpm);

    // Record button

    btnRecordStart = new wxBitmapButton(panel, (int) DLG_AUDIO_CONTROL_BTN_RECORD_START_ID, bmpRecord16x16);

    btnRecordStart->SetBitmapHover(bmpRecordHover16x16);
    btnRecordStart->SetToolTip(wxT("Gravar áudio via microfone."));

    szRow->Add(btnRecordStart, 0, wxALL, 1);

    btnRecordStart->Connect((int) DLG_AUDIO_CONTROL_BTN_RECORD_START_ID, wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler(AudioRecorderControlPanel::RecordStartRequest), NULL, this);

    // Pause button

    btnPause = new wxBitmapButton(panel, DLG_AUDIO_CONTROL_BTN_RECORD_PAUSE_ID, bmpPause16x16);

    btnPause->SetBitmapHover(bmpPauseHover16x16);
    btnPause->SetToolTip(wxT("Realizar uma pausa na gravação. Você pode retomar a gravação clicando no botão de início de gravação de áudio."));

    btnPause->Disable();

    //btnPause->Disable();

    szRow->Add(btnPause, 0, wxALL, 1);

    btnPause->Connect((int) DLG_AUDIO_CONTROL_BTN_RECORD_PAUSE_ID, wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler(AudioRecorderControlPanel::PauseRequest), NULL, this);

    // Stop button

    btnStop = new wxBitmapButton(panel, DLG_AUDIO_CONTROL_BTN_RECORD_STOP_ID, bmpStop16x16);

    btnStop->SetBitmapHover(bmpStopHover16x16);
    btnStop->SetToolTip(wxT("Finalizar gravação de áudio via microfone."));

    btnStop->Disable();

    //btnStop->Disable();

    szRow->Add(btnStop, 0, wxALL, 1);

    btnStop->Connect((int) DLG_AUDIO_CONTROL_BTN_RECORD_STOP_ID, wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler(AudioRecorderControlPanel::StopRequest), NULL, this);

    // Save button

    btnSave = new wxBitmapButton(panel, (int) DLG_AUDIO_CONTROL_BTN_RECORD_SAVE_ID, bmpSave16x16);

    btnSave->SetBitmapHover(bmpSaveHover16x16);
    btnSave->SetToolTip(wxT("Salvar áudio gravado."));

    btnSave->Disable();

    szRow->Add(btnSave, 0, wxALL, 1);

    btnSave->Connect((int) DLG_AUDIO_CONTROL_BTN_RECORD_SAVE_ID, wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler(AudioRecorderControlPanel::RecordSaveRequest), NULL, this);

    szTop->Layout();

    //
    // Audio file information
    //

    wxStaticBox *staticBoxInformation = new wxStaticBox(panel, wxID_ANY, wxT("File information:"));
    wxStaticBoxSizer *szBoxInformation = new wxStaticBoxSizer(staticBoxInformation, wxVERTICAL);

    //wxBoxSizer *szBoxInformation = new wxBoxSizer(wxVERTICAL);

    szControlsAndInfo->Add(szBoxInformation, 1, wxEXPAND|wxGROW|wxALL, 0);

    // Path
    //szRow = new wxBoxSizer(wxHORIZONTAL);
    //szBoxInformation->Add(szRow, 1, wxALL|wxEXPAND, 0);

    //wxStaticText *label = new wxStaticText(panel, wxID_STATIC, wxT("Local: "));
    //szRow->Add(label, 0, wxALL|wxEXPAND, 1);

    //wxStaticText *lblPathValue = new wxStaticText(panel, wxID_STATIC, audioInfo.GetFilePath());
    //szRow->Add(lblPathValue, 0, wxALL|wxEXPAND, 1);

    // File name
    /*
    szRow = new wxBoxSizer(wxHORIZONTAL);
    szBoxInformation->Add(szRow, 0, wxALL|wxEXPAND, 0);

    wxStaticText *label = new wxStaticText(panel, wxID_STATIC, wxT("Arquivo: "));
    label->SetForegroundColour(wxColour(0x44, 0x44, 0x44));
    szRow->Add(label, 0, wxALL|wxEXPAND, 0);

    wxStaticText *lblFileName = new wxStaticText(panel, wxID_STATIC, fileInfo.GetFullName());
    lblFileName->SetForegroundColour(*wxBLUE);
    szRow->Add(lblFileName, 0, wxALL|wxEXPAND, 0);

    szBoxInformation->Layout();
    szRow->Layout();
    */

    // Sample rate
    szRow = new wxBoxSizer(wxHORIZONTAL);
    szBoxInformation->Add(szRow, 1, wxALL | wxEXPAND | wxGROW, 0);

    // Input device

    label = new wxStaticText(panel, wxID_STATIC, wxT("Recording device: "));
    label->SetForegroundColour(wxColour(0x44, 0x44, 0x44));
    szRow->Add(label, 0, wxALL | wxALIGN_CENTER_VERTICAL, 0);

    choiceInputDevice = new wxChoice(panel, wxID_ANY);
    szRow->Add(choiceInputDevice, 1, wxALL | wxALIGN_CENTER_VERTICAL, 3);

    /*
       choiceInputDevice->Connect(wxEVT_COMBOBOX,
       wxCommandEventHandler(AudioRecorderControlPanel::SetDevice),
       NULL,
       this);
       */

    wxFlexGridSizer *szgFields = new wxFlexGridSizer(0, 2, 0, 30);
    szBoxInformation->Add(szgFields, 0, wxALL|wxEXPAND|wxGROW, 0);

    // Samples
    szRow = new wxBoxSizer(wxHORIZONTAL);
    //szBoxInformation->Add(szRow, 0, wxALL|wxEXPAND, 0);
    szgFields->Add(szRow, 1, wxALL|wxEXPAND|wxGROW, 0);
    //szgFields->AddSpacer(20);

    label = new wxStaticText(panel, wxID_STATIC, wxT("Samples: "));
    label->SetForegroundColour(wxColour(0x44, 0x44, 0x44));
    szRow->Add(label, 0, wxALL | wxALIGN_CENTER_VERTICAL, 0);

    lblFileSamples = new wxStaticText(panel, wxID_STATIC, wxString::Format(wxT("%d"), audioInfo.GetSamples()));
    szRow->Add(lblFileSamples, 0, wxALL | wxALIGN_CENTER_VERTICAL, 0);

    //szBoxInformation->Layout();
    //szRow->Layout();

    // Sample rate
    szRow = new wxBoxSizer(wxHORIZONTAL);
    //szBoxInformation->Add(szRow, 0, wxALL|wxEXPAND, 0);
    szgFields->Add(szRow, 1, wxALL|wxEXPAND|wxGROW, 0);

    label = new wxStaticText(panel, wxID_STATIC, wxT("Sample rate: "));
    label->SetForegroundColour(wxColour(0x44, 0x44, 0x44));
    szRow->Add(label, 0, wxALL | wxALIGN_CENTER_VERTICAL, 0);

    comboSampleRate = new wxComboBox(panel, wxID_ANY, wxString::Format(wxT("%d"), audioInfo.GetSampleRate()));
    szRow->Add(comboSampleRate, 0, wxALL | wxALIGN_CENTER_VERTICAL, 0);

    comboSampleRate->Append(wxT("8000"));
    comboSampleRate->Append(wxT("11020"));
    comboSampleRate->Append(wxT("16000"));
    comboSampleRate->Append(wxT("22050"));
    comboSampleRate->Append(wxT("44100"));
    comboSampleRate->Append(wxT("48000"));
    comboSampleRate->Append(wxT("88200"));
    comboSampleRate->Append(wxT("96000"));
    comboSampleRate->Append(wxT("176400"));
    comboSampleRate->Append(wxT("192000"));
    comboSampleRate->Append(wxT("352800"));
    comboSampleRate->Append(wxT("384000"));

    comboSampleRate->Connect(wxEVT_COMBOBOX,
                             wxCommandEventHandler(AudioRecorderControlPanel::SetSampleRateRequest),
                             NULL,
                             this);

    //lblFileSampleRate = new wxStaticText(panel, wxID_STATIC, wxString::Format(wxT("%ld"), audioInfo.GetSampleRate()));
    //szRow->Add(lblFileSampleRate, 0, wxALL|wxEXPAND, 0);

    label = new wxStaticText(panel, wxID_STATIC, wxT(" HZ"));
    szRow->Add(label, 0, wxALL | wxALIGN_CENTER_VERTICAL, 0);

    //szRow->Layout();

    // Channels

    szRow = new wxBoxSizer(wxHORIZONTAL);
    szgFields->Add(szRow, 1, wxALL|wxEXPAND|wxGROW, 0);

    label = new wxStaticText(panel, wxID_STATIC, wxT("Channels: "));
    label->SetForegroundColour(wxColour(0x44, 0x44, 0x44));
    szRow->Add(label, 0, wxALL | wxALIGN_CENTER_VERTICAL, 0);

    choiceChannels = new wxChoice(panel, wxID_ANY);
    szRow->Add(choiceChannels, 0, wxALL | wxALIGN_CENTER_VERTICAL, 0);

    choiceChannels->Append(wxT("1"), (wxClientData *) new ContainerIntData(1));
    choiceChannels->Append(wxT("2"), (wxClientData *) new ContainerIntData(2));

    choiceChannels->Connect(wxEVT_CHOICE,
            wxCommandEventHandler(AudioRecorderControlPanel::SetChannelsRequest),
            NULL,
            this);

    choiceChannels->SetSelection(1);

    szRow->Layout();

    // Format

    szRow = new wxBoxSizer(wxHORIZONTAL);
    szgFields->Add(szRow, 1, wxALL|wxEXPAND|wxGROW, 0);

    label = new wxStaticText(panel, wxID_STATIC, wxT("Format: "));
    label->SetForegroundColour(wxColour(0x44, 0x44, 0x44));
    szRow->Add(label, 0, wxALL | wxALIGN_CENTER_VERTICAL, 0);

    wxStaticText *lblFileFormatInfo = new wxStaticText(panel, wxID_STATIC, audioInfo.GetFormatInfo());
    szRow->Add(lblFileFormatInfo, 0, wxALL | wxALIGN_CENTER_VERTICAL, 0);

    szRow->Layout();

    // Format

    szRow = new wxBoxSizer(wxHORIZONTAL);
    szgFields->Add(szRow, 1, wxALL|wxEXPAND|wxGROW, 0);

    label = new wxStaticText(panel, wxID_STATIC, wxT("Duration: "));
    label->SetForegroundColour(wxColour(0x44, 0x44, 0x44));
    szRow->Add(label, 0, wxALL | wxALIGN_CENTER_VERTICAL, 0);

    int sampleRate = audioInfo.GetSampleRate();
    int samplesCount = audioInfo.GetSamples();
    float audioDuration = (float) samplesCount / (float) sampleRate;
    wxString audioDurationStr;

    if (audioDuration > 1.0) {
        audioDurationStr = audioInfo.GetDurationStr() + wxT(" (") + wxString::Format(wxT("%.2f"), audioDuration) + wxT(" seconds") + wxT(")");
    }
    else {
        audioDurationStr = audioInfo.GetDurationStr() + wxT(" (") + wxString::Format(wxT("%.2f"), audioDuration) + wxT(" secondo") + wxT(")");
    }

    wxStaticText *lblAudioDuration = new wxStaticText(panel, wxID_STATIC, audioDurationStr);
    szRow->Add(lblAudioDuration, 0, wxALL | wxALIGN_CENTER_VERTICAL, 0);

    //label = new wxStaticText(panel, wxID_STATIC, audioDurationStr);
    //szRow->Add(label, 0, wxALL|wxEXPAND, 0);

    // Channels

    szRow = new wxBoxSizer(wxHORIZONTAL);
    szgFields->Add(szRow, 1, wxALL|wxEXPAND|wxGROW, 0);

    label = new wxStaticText(panel, wxID_STATIC, wxT("Sample format: "));
    label->SetForegroundColour(wxColour(0x44, 0x44, 0x44));
    szRow->Add(label, 0, wxALL | wxALIGN_CENTER_VERTICAL, 0);

    wxString sampleFormatList[] = {
        wxT("Float32"), wxT("Int16"),
        wxT("Int32"), wxT("Int24"),
        wxT("Int8"), wxT("UInt8")
    };

    choiceSampleFormat = new wxChoice(panel, wxID_ANY, wxDefaultPosition, wxDefaultSize, WXSIZEOF(sampleFormatList), sampleFormatList);
    szRow->Add(choiceSampleFormat, 0, wxALL | wxALIGN_CENTER_VERTICAL, 0);

    choiceSampleFormat->Connect(wxEVT_CHOICE,
            wxCommandEventHandler(AudioRecorderControlPanel::SetSamplesFormatRequest),
            NULL,
            this);

    choiceSampleFormat->SetSelection(1);


    szgFields->AddSpacer(1);

    panel->Layout();

    szBoxInformation->Layout();

    panel->Layout();

    szTop->Layout();

    statusBar = new wxStatusBar(panel, wxID_ANY);
    szMargin->Add(statusBar, 0, wxALL | wxEXPAND | wxGROW, 0);

    wxGetApp().GetTopWindow()->Layout();
    wxGetApp().GetTopWindow()->Refresh();
    wxGetApp().GetTopWindow()->Update();

    LoadAudioDevicesInfo();

    return 0;
}

void AudioRecorderControlPanel::LoadAudioDevicesInfo()
{
    PaError rcAudio = Pa_Initialize();

    if (rcAudio != paNoError) {
        std::cerr << "PortAudio initialization error: " << Pa_GetErrorText(rcAudio) << "\n";
        return;
    }

    std::cerr << "Portaudio version: " << Pa_GetVersion() << "\n";

    // Audio devices

    std::cerr << "Audio devices count: " << Pa_GetDeviceCount() << "\n";

    int numDevices = Pa_GetDeviceCount();

    if (numDevices < 0) {
        printf( "ERROR: Pa_CountDevices returned 0x%x\n", numDevices);
    }

    for (int i = 0; i < numDevices; i++) {
        const PaDeviceInfo *deviceInfo = Pa_GetDeviceInfo(i);

        std::cerr << "Audio device [" << i << "]\n";
        std::cerr << "\tName: " << deviceInfo->name << "\n";
        std::cerr << "\tMax. input channels: " << deviceInfo->maxInputChannels << "\n";
        std::cerr << "\tMax. output channels: " << deviceInfo->maxOutputChannels << "\n";
        std::cerr << "\tDefault sample rate: " << deviceInfo->defaultSampleRate << "\n";

        if (deviceInfo->maxInputChannels > 0) {
            wxString str(deviceInfo->name, wxConvUTF8);

            choiceInputDevice->Append(str, (wxClientData *) new ContainerIntData(i));
        }
    }

    Pa_Terminate();
}

void AudioRecorderControlPanel::SetSampleRateRequest(wxCommandEvent& WXUNUSED(event))
{
    //ContainerIntData *container = (ContainerIntData *) comboSampleRate->GetClientObject(comboSampleRate->GetSelection());

    if (comboSampleRate->GetStringSelection().Length() == 0) {
        std::cerr << "Invalid sample rate\n";

        return;
    }

    long sampleRate;

    if (comboSampleRate->GetStringSelection().ToLong(&sampleRate)) {
        audioInfo.SetSampleRate(sampleRate);

        sfInfo.samplerate = audioInfo.GetSampleRate();

        audioInfo.SetSFInfo(sfInfo);
    }
}

void AudioRecorderControlPanel::SetChannelsRequest(wxCommandEvent& WXUNUSED(event))
{
    ContainerIntData *container = (ContainerIntData *) choiceChannels->GetClientObject(choiceChannels->GetSelection());

    int channelsCount = container->GetData();

    audioInfo.SetChannels(channelsCount);

    sfInfo.channels = audioInfo.GetChannels();

    audioInfo.SetSFInfo(sfInfo);
}

void AudioRecorderControlPanel::SetSamplesFormatRequest(wxCommandEvent& WXUNUSED(event))
{
    // wxString sampleFormatList[] = {wxT("Float32"), wxT("Int16"),
    //                               wxT("Int32"), wxT("Int24"),
    //                               wxT("Int8"), wxT("UInt8")};

    wxString sampleFormatStr = choiceSampleFormat->GetString(choiceSampleFormat->GetSelection());

    //if (sampleFormatStr.Cmp(wxT("")))
}

void AudioRecorderControlPanel::RecordStartRequest(wxCommandEvent& WXUNUSED(event))
{
    std::cerr << "AudioRecorderControlPanel::RecordStartRequest()\n";

    if (choiceInputDevice->GetSelection() == wxNOT_FOUND) {
        std::cerr << "Input device not informed.\n";

        return;
    }

    RecordThreadStart();
}

void AudioRecorderControlPanel::RecordThreadStart()
{
    std::cerr << "AudioRecorderControlPanel::RecordThreadStart()\n";

    //RecordStart();

    threadAudioRecorder = new ThreadAudioRecorder(this);

    if (threadAudioRecorder->Create() != wxTHREAD_NO_ERROR) {
        //wxMessageBox(wxT("Não foi possível iniciar a thread para reprodução do arquivo de áudio."));

        return;
    }

    btnRecordStart->Disable();
    btnPause->Enable();
    btnStop->Enable();

    threadAudioRecorder->Run();

    btnSave->Enable();
}

void AudioRecorderControlPanel::RecordStart()
{
    std::cerr << "AudioRecorderControlPanel::RecordStart()\n";

    // Check if the recorder is already running
    if (threadAudioRecorderIsRunning) {
        wxMessageBox(wxT("O gravador de som já está sendo executado."));
        return;
    }

    if (!threadAudioRecorderPauseRequested) {
        audioInfo.DataArrayClear();
    }

    threadAudioRecorderStopRequested = false;
    threadAudioRecorderPauseRequested = false;

    PaError rcAudio;
    //int rc;

    //PaError err;

    /*
       char audioPath[CONST_STR_FILENAME_MAX_LEN];

       strcpy(audioPath, (const char *) audioInfo.GetFilePath().ToAscii());

       if (!audioFileIsOpened) {
       sfInfo.format = 0;

       sndFile = sf_open(audioPath, SFM_READ, &sfInfo);

       if (sndFile == NULL) {
       wxMessageBox(wxT("Erro ao tentar abrir o descritor do arquivo de áudio SNDFILE."), wxT("Erro"), wxICON_ERROR);
       goto TERM;
       }

       audioFileIsOpened = true;
       }

       if (!sf_format_check(&sfInfo)) {
       wxMessageBox(wxT("Dados do arquivo de áudio aberto não são válidos"), wxT("Erro"), wxICON_ERROR);
       goto TERM;
       }
       */

    // Initialyzing PortAudio

    if (!audioDeviceIsOpened) {
        rcAudio = Pa_Initialize();

        if (rcAudio != paNoError) {
            fprintf(stderr, "PortAudio intialyzing error: %s\n", Pa_GetErrorText(rcAudio));
            threadAudioRecorderIsRunning = false;
            return;
        }

        // PortAudio audio information
        //inputParameters.device = Pa_GetDefaultInputDevice(); /* default input device */
        inputParameters.device = 1; /* default input device */

        // No configured input device
        /* if (inputParameters.device == paNoDevice) {
           fprintf(stderr, "Input device is invalid.\n");

           return;
           }
           */

        std::cerr << "Channels: " << audioInfo.GetChannels() << "\n";
        std::cerr << "Sample rate: " << audioInfo.GetSampleRate() << "\n";

        inputParameters.channelCount = audioInfo.GetChannels();
        inputParameters.sampleFormat = paInt16;

        /*
           switch (audioInfo.GetFormat()) {
           case SF_FORMAT_WAV:
        //wxMessageBox(wxT("SF_FORMAT_WAV"));
        inputParameters.sampleFormat = paInt16;
        case SF_FORMAT_PCM_16:
        //wxMessageBox(wxT("SF_FORMAT_PCM_16"));
        inputParameters.sampleFormat = paInt16; break;
        default:
        //wxMessageBox(wxString::Format(wxT("FORMAT: %x"), audioInfo.GetFormat()));
        inputParameters.sampleFormat = paInt16;
        }
        */

        inputParameters.suggestedLatency = Pa_GetDeviceInfo(inputParameters.device)->defaultHighInputLatency;

        inputParameters.hostApiSpecificStreamInfo = NULL;

        threadAudioRecorderIsRunning = true;

        // Open audio device

        rcAudio = Pa_OpenStream(&stream,
                                &inputParameters,
                                NULL,
                                audioInfo.GetSampleRate(),
                                AUDIO_BUFFER_SIZE,
                                paClipOff,
                                AudioRecorderControlPanel::RecordCallback,
                                &audioInfo);

        if (rcAudio != paNoError) {
            std::cerr << "[PortAudio] Error on opening the I/O stream: " << Pa_GetErrorText(rcAudio) << "\n";
            goto TERM;
        }

        audioDeviceIsOpened = true;

        // Start recording

        rcAudio = Pa_StartStream(stream);

        if (rcAudio != paNoError) {
            std::cerr << "[PortAudio] Error on starting stream: " << Pa_GetErrorText(rcAudio) << "\n";
            goto TERM;
        }
    }

    //audioInfo.SetChannels(1);
    //audioInfo.SetSampleRate(8000);
    /*
    sfInfo.channels = 1;
    sfInfo.format = SF_FORMAT_WAV | SF_FORMAT_PCM_16;
    sfInfo.samplerate = 8000;
    sfInfo.sections = 1;
    sfInfo.seekable = 1;
    sfInfo.frames = 0;

    audioInfo.SetSFInfo(sfInfo);
    */

    while (!threadAudioRecorderStopRequested && !threadAudioRecorderPauseRequested) {
        Pa_Sleep(100);

        /*
        // Change 'short' to a generic audio data type
        streamBuffer = (short *) malloc(AUDIO_BUFFER_SIZE * sizeof(short));

        rc = Pa_ReadStream(stream, streamBuffer, AUDIO_BUFFER_SIZE);

        if (rc) {
        fprintf(stderr, "[PortAudio] Error on reading data to stream.\n");
        break;
        }

        //wroteBytes = sf_write_short(sndFile, streamBuffer, AUDIO_BUFFER_SIZE);

        audioInfo.DataArrayAppend((short *) streamBuffer);
        */
    }

    std::cerr << "Exiting from recording loop\n";

    if (threadAudioRecorderPauseRequested) {
        goto PAUSE_TERM;
    }

    threadAudioRecorderStopRequested = false;
    threadAudioRecorderPauseRequested = false;

    // Start stream
    rcAudio = Pa_StopStream(stream);

    if (rcAudio != paNoError) {
        fprintf(stderr, "[PortAudio] Error on starting stream: %s\n", Pa_GetErrorText(rcAudio));
        goto TERM;
    }

    // Terminating
PAUSE_TERM:

TERM:
    // Terminating PortAudio
    rcAudio = Pa_CloseStream(stream);

    if (rcAudio != paNoError ) {
        fprintf(stderr, "PortAudio error: %s\n", Pa_GetErrorText(rcAudio));
    }

    rcAudio = Pa_Terminate();

    if (rcAudio != paNoError ) {
        fprintf(stderr, "PortAudio error: %s\n", Pa_GetErrorText(rcAudio));
    }

    audioDeviceIsOpened = false;

    //sf_close(sndFile);

    //sndFile = NULL;

    //audioFileIsOpened = false;

    threadAudioRecorderIsRunning = false;
}

int AudioRecorderControlPanel::RecordCallback(const void *inputBuffer,
                                              void *outputBuffer,
                                              unsigned long framesCount,
                                              const PaStreamCallbackTimeInfo *timeInfo,
                                              PaStreamCallbackFlags statusFlags,
                                              void *userData) {
    //std::cerr << "RecordCallback() frames count: " << framesCount << "\n";

    (void) outputBuffer;
    (void) timeInfo;
    (void) statusFlags;

    if (inputBuffer == NULL) {
        std::cerr << "Input buffer is null\n";
        return paContinue;
    }

    const short *samples = (const short *) inputBuffer;
    AudioInfo *audioInfo = (AudioInfo *) userData;

    for (int channelIndex = 0; channelIndex < audioInfo->GetChannels(); channelIndex++) {
        short *streamBuffer = (short *) malloc(framesCount * sizeof(short));

        memcpy(streamBuffer, samples + AUDIO_BUFFER_SIZE * channelIndex, framesCount * sizeof(short));

        audioInfo->DataArrayAppend((short *) streamBuffer);
    }

    return paContinue;
}

void AudioRecorderControlPanel::RecorderRequestStop()
{
    if (RecorderIsRunning()) {
        std::cerr << "AudioRecorderControlPanel::RecorderRequestStop()";

        threadAudioRecorderStopRequested = true;
    }
}

void AudioRecorderControlPanel::RecorderRequestPause()
{
    if (RecorderIsRunning()) {
        threadAudioRecorderPauseRequested = true;
    }
}

bool AudioRecorderControlPanel::RecorderIsRunning()
{
    return threadAudioRecorderIsRunning;
}

void AudioRecorderControlPanel::WaitRecorderStop()
{
    /** \todo Reduce the CPU usage here. */
    while (RecorderIsRunning()) {
        std::cerr << "Waiting audio recorder stop\n";

        wxMilliSleep(200);
    }

    std::cerr << "Waiting audio recorder is over\n";
}

void AudioRecorderControlPanel::StopRequest(wxCommandEvent& WXUNUSED(event))
{
    StopRecording();
}

int AudioRecorderControlPanel::StopRecording()
{
    RecorderRequestStop();

    WaitRecorderStop();

    btnRecordStart->Enable();
    btnPause->Disable();
    btnStop->Disable();

    return 0;
}

void AudioRecorderControlPanel::PauseRequest(wxCommandEvent& WXUNUSED(event))
{
    btnPause->Disable();

    RecorderRequestPause();

    WaitRecorderStop();

    btnRecordStart->Enable();
}

void AudioRecorderControlPanel::RecordSaveRequest(wxCommandEvent& WXUNUSED(event))
{
    // Stop recording before save
    StopRecording();

    if (audioInfo.DataArrayGetSize() < 1) {
        wxMessageBox(wxT("Não há dados de áudio gravados. Para salvar um arquivo de áudio é necessário gravar áudio previamente."));

        return;
    }

    char audioFilePathANSI[CONST_STR_FILENAME_MAX_LEN];
    wxString wildcardOptions = wxT("Arquivos de áudio (*.wav)|*.wav");

    wxDateTime dateTimeNow = wxDateTime::Now();

    wxString defaultAudioFileName = wxT("audio_") + dateTimeNow.Format(wxT("%Y%m%d_%H%M%S")) + wxT(".wav");

    wxFileDialog fileDialog(wxGetApp().GetTopWindow(),
                            wxT("Informe o nome do arquivo de áudio a salvar"),
                            App::outputDirectory,
                            defaultAudioFileName,
                            wildcardOptions,
                            wxFD_OPEN);

    if (fileDialog.ShowModal() != wxID_OK) {
        return;
    }

    wxString audioFilePath = fileDialog.GetPath();

    wxCSConv systemEncoding(App::systemEncodingName);

    memset(audioFilePathANSI, 0, CONST_STR_MAX_LEN);
    strcpy(audioFilePathANSI, (const char *) audioFilePath.mb_str(systemEncoding));

    SNDFILE *sndFile;

    sndFile = sf_open(audioFilePathANSI, SFM_WRITE, &sfInfo);

    if (sndFile == NULL) {
        wxMessageBox(wxT("Erro ao tentar abrir o descritor do arquivo de áudio SNDFILE."), wxT("Erro"), wxICON_ERROR);

        return;
    }

    short *audioDataBuffer;
    sf_count_t wroteBytes;

    for (unsigned int audioDataIndex = 0; audioDataIndex < audioInfo.DataArrayGetSize(); audioDataIndex++) {
        //fprintf(stderr, "Audio data index %d\n", audioDataIndex);

        audioDataBuffer = audioInfo.DataArrayGetItem(audioDataIndex);

        //for (int i = 0; i < AUDIO_BUFFER_SIZE; i++) {
        //  fprintf(stderr, "  [%d]", audioDataBuffer[i]);
        //}

        wroteBytes = sf_write_short(sndFile, audioDataBuffer, AUDIO_BUFFER_SIZE);

        if (wroteBytes == 0) {
            //
        }
    }

    sf_close(sndFile);

    sndFile = NULL;

    App::panelAudioManager->AddAudioFile(audioFilePath);
}

void AudioRecorderControlPanel::CloseRequest(wxCommandEvent& WXUNUSED(event))
{
    Close();
}

void AudioRecorderControlPanel::Close()
{
    // Stop recording before close
    StopRecording();

    //if (audioFileIsOpened) {
    //sf_close(sndFile);

    //sndFile = NULL;

    /*
       fclose(audioFd);

       audioFd = NULL;
       */
    //}

    printf("Audio recorder is stopped.\n");

    // Checking the audio device
    if (audioDeviceIsOpened) {
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

        audioDeviceIsOpened = false;
    }

    printf("Audio recorder destroyed.\n");

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
 * Audio recorder thread.
 */

ThreadAudioRecorder::ThreadAudioRecorder(AudioRecorderControlPanel *audioControl)
{
    std::cerr << "ThreadAudioRecorder::ThreadAudioRecorder()\n";

    this->audioControl = audioControl;
}

void *ThreadAudioRecorder::Entry()
{
    std::cerr << "ThreadAudioRecorder::Entry()\n";

    audioControl->RecordStart();

    return NULL;
}

wxDEFINE_EVENT(RecordEvent, wxCommandEvent);
