#ifndef AUDIO_HPP
#define AUDIO_HPP

#include <vector>

#include "sndfile.h"
#include "ui/gui.hpp"

#define AUDIO_BUFFER_SIZE 512

/**
  * Class with audio utilities.
  */
class AudioInfo {
    private:
        wxString path;
        int type;
        wxString headerName;
        //wxString formatInfo;
        wxString formatName;
        wxString sampleFormat;
        int *loops;
        wxString comment;
        bool seekable;
        wxString durationStr;

        std::vector<short *>data;

        SNDFILE *file;

        SF_INFO sfinfo;
        SF_FORMAT_INFO formatInfo;
        SF_LOOP_INFO loopInfo;

    public:

        AudioInfo();

        ~AudioInfo();

        void Reset(void);

        int GetFileInformation(wxString path);

        int SeekFramesBegin(void);

        wxString GetFilePath(void);
        void SetFilePath(wxString filePath);

        int GetSampleRate(void);
        void SetSampleRate(int sampleRate);

        int GetSamples(void);

        int GetChannels(void);
        void SetChannels(int channels);

        float GetLength(void);

        int GetType(void);

        int GetFormat(void);
        void SetFormat(int format);

        wxString GetFormatInfo(void);
        wxString GetTimestamp(void);
        wxString GetComment(void);
        wxString GetDurationStr(void);

        int SetSFInfo(SF_INFO &sfInfo);
        SF_INFO GetSFInfo(void);

        int DataArrayAppend(short *data);
        int DataArrayClear(void);
        size_t DataArrayGetSize(void);
        short *DataArrayGetItem(size_t audioIndex);
};

#endif /* AUDIO_HPP */
