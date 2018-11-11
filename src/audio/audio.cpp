/**
 * Audio utilities library.
 *
 * \author: Allann Jones
 */

#include "audio/audio.hpp"

static double get_signal_max(SNDFILE *file);
static double calc_decibels(SF_INFO * sfinfo, double max);
static const char *generate_duration_str(SF_INFO *sfinfo);

AudioInfo::AudioInfo()
{
    path = wxT("");

    file = NULL;

    type = 0;

    Reset();
}

AudioInfo::~AudioInfo()
{
    if (file) {
        sf_close(file);

        file = NULL;
    }

    this->data.clear();
}

void AudioInfo::Reset()
{
    comment = wxT("");
}

int AudioInfo::GetFileInformation(wxString path)
{
    printf("AudioInfo::GetFileInformation() entering.\n");

    if (path.IsEmpty() || path.Length() <= 0) {
        return -1;
    }

    this->path = path;

    static char strbuffer[1024];
    double signal_max, decibels;
    //double data[1024];

    memset (&sfinfo, 0, sizeof (sfinfo));

    char filename[path.Length() + 1];

    strcpy(filename, (const char *) path.ToAscii());

    fprintf(stderr, "Audio file path: %s\n", filename);

    if (!file) {
        if ((file = sf_open(filename, SFM_READ, &sfinfo)) == NULL) {
            printf ("Error : Not able to open input file %s.\n", filename);

            //memset (data, 0, sizeof (data));

            sf_command (file, SFC_GET_LOG_INFO, strbuffer, 1024);

            puts (strbuffer);
            puts (sf_strerror (NULL));

            return -1;
        }
    }

    formatInfo.format = sfinfo.format;

    //memset (&formatInfo, 0, sizeof(formatInfo));

    sf_command (file, SFC_GET_FORMAT_INFO, &formatInfo, sizeof(formatInfo));

    sf_command (file, SFC_GET_LOOP_INFO, &loopInfo, sizeof(loopInfo));

    printf("bpm: %f\n", loopInfo.bpm);


    printf ("========================================\n") ;
    sf_command (file, SFC_GET_LOG_INFO, strbuffer, 1024) ;
    puts(strbuffer) ;
    printf ("----------------------------------------\n") ;

    if (file == NULL) {
        printf ("Error : Not able to open input file %s.\n", filename) ;

        fflush (stdout) ;

        //memset (data, 0, sizeof (data)) ;

        puts (sf_strerror (NULL)) ;
    }
    else {
        printf ("Sample Rate: %d\n", sfinfo.samplerate) ;

        if (sfinfo.frames > 0x7FFFFFFF) {
            printf ("Frames     : unknown\n");
        }
        else {
            printf ("Frames     : %ld\n", (long) sfinfo.frames);
        }

        durationStr = wxString(generate_duration_str(&sfinfo), wxConvUTF8, strlen(generate_duration_str (&sfinfo)));

        //wxMessageBox(durationStr);

        printf ("Channels   : %d\n", sfinfo.channels);
        printf ("Format     : 0x%08X\n", sfinfo.format);
        printf ("Sections   : %d\n", sfinfo.sections);
        printf ("Seekable   : %s\n", (sfinfo.seekable ? "TRUE" : "FALSE"));
        printf ("Duration   : %s\n", generate_duration_str (&sfinfo));

        /* Do not use sf_signal_max because it doesn work for non-seekable files . */
        signal_max = get_signal_max(file);

        decibels = calc_decibels(&sfinfo, signal_max);

        printf ("Signal Max : %g (%4.2f dB)\n\n", signal_max, decibels);
    }

    fflush(stdout);

    sf_close(file);

    file = NULL;

    return 0;
}

int AudioInfo::SeekFramesBegin()
{
    sf_seek(file, 0, SEEK_SET);

    return 0;
}

wxString AudioInfo::GetFilePath()
{
    return path;
}

void AudioInfo::SetFilePath(wxString filePath)
{
    filePath = path;
}

int AudioInfo::GetSampleRate()
{
    return sfinfo.samplerate;
}

void AudioInfo::SetSampleRate(int sampleRate)
{
    sfinfo.samplerate = sampleRate;
}

int AudioInfo::GetSamples()
{
    return sfinfo.frames;
}

int AudioInfo::GetChannels()
{
    return sfinfo.channels;
}

void AudioInfo::SetChannels(int channels)
{
    sfinfo.channels = channels;
}

float AudioInfo::GetLength()
{
    //return length;
    return 0;
}

int AudioInfo::GetType()
{
    return type;
    //return 0;
}

int AudioInfo::GetFormat()
{
    return sfinfo.format;
}

void AudioInfo::SetFormat(int format)
{
    sfinfo.format = format;
}

wxString AudioInfo::GetFormatInfo()
{
    if (formatInfo.name != NULL) {
        return wxString(formatInfo.name, wxConvUTF8, strlen(formatInfo.name));
    }
    else {
        return wxT("");
    }
}

wxString AudioInfo::GetTimestamp()
{
    return wxT("");
}

wxString AudioInfo::GetComment()
{
    //return comment;
    return wxT("");
}

wxString AudioInfo::GetDurationStr()
{
    return durationStr;
}

int AudioInfo::SetSFInfo(SF_INFO &sfInfo)
{
    this->sfinfo = sfInfo;

    formatInfo.format = sfinfo.format;

    sf_command(file, SFC_GET_FORMAT_INFO, &formatInfo, sizeof(formatInfo));

    sf_command(file, SFC_GET_LOOP_INFO, &loopInfo, sizeof(loopInfo));

    return 0;
}

SF_INFO AudioInfo::GetSFInfo()
{
    return this->sfinfo;
}

int AudioInfo::DataArrayAppend(short *data)
{
    this->data.push_back(data);

    return 0;
}

int AudioInfo::DataArrayClear()
{
    this->data.clear();

    return 0;
}

size_t AudioInfo::DataArrayGetSize(void)
{
    return this->data.size();
}

short *AudioInfo::DataArrayGetItem(size_t audioIndex)
{
    return (short *) this->data[audioIndex];
}

static double get_signal_max(SNDFILE *file)
{
    double max, temp;
    int readcount, k, save_state;
    double audioData[1024];

    save_state = sf_command(file, SFC_GET_NORM_DOUBLE, NULL, 0);
    sf_command (file, SFC_SET_NORM_DOUBLE, NULL, SF_FALSE);

    max = 0.0;
    while ((readcount = sf_read_double(file, audioData, 1024))) {
        for (k = 0 ; k < readcount ; k++) {
            temp = fabs (audioData[k]);
            if (temp > max)
                max = temp ;
        }
    }

    sf_command(file, SFC_SET_NORM_DOUBLE, NULL, save_state);

    return max ;
}

static double calc_decibels(SF_INFO * sfinfo, double max)
{
    double decibels ;

    switch (sfinfo->format & SF_FORMAT_SUBMASK) {
        case SF_FORMAT_PCM_U8:
        case SF_FORMAT_PCM_S8:
            decibels = max / 0x80;
            break;

        case SF_FORMAT_PCM_16:
            decibels = max / 0x8000;
            break;

        case SF_FORMAT_PCM_24:
            decibels = max / 0x800000;
            break;

        case SF_FORMAT_PCM_32:
            decibels = max / 0x80000000;
            break;

        case SF_FORMAT_FLOAT:
        case SF_FORMAT_DOUBLE:
            decibels = max / 1.0;
            break;

        default:
            decibels = max / 0x8000;
            break;
    };

    return 20.0 * log10 (decibels);
}

static const char *generate_duration_str(SF_INFO *sfinfo)
{
    static char str [128];

    int seconds;

    memset (str, 0, sizeof (str));

    if (sfinfo->samplerate < 1)
        return NULL;

    if (sfinfo->frames / sfinfo->samplerate > 0x7FFFFFFF)
        return "unknown";

    seconds = sfinfo->frames / sfinfo->samplerate;

    snprintf (str, sizeof (str) - 1, "%02d:", seconds / 60 / 60);

    seconds = seconds % (60 * 60);
    snprintf (str + strlen (str), sizeof (str) - strlen (str) - 1, "%02d:", seconds / 60);

    seconds = seconds % 60;
    snprintf (str + strlen (str), sizeof (str) - strlen (str) - 1, "%02d.", seconds);

    seconds = ((1000 * sfinfo->frames) / sfinfo->samplerate) % 1000;
    snprintf (str + strlen (str), sizeof (str) - strlen (str) - 1, "%03d", seconds);

    return str;
}
