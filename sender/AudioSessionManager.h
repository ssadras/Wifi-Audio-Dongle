//
// Created by Sadra on 12/1/2024.
//

#ifndef WIFI_AUDIO_DONGLE_AUDIOSESSIONMANAGER_H
#define WIFI_AUDIO_DONGLE_AUDIOSESSIONMANAGER_H

#include <windows.h>
#include <audiopolicy.h>
#include <mmdeviceapi.h>
#include <string>
#include <vector>

struct AudioSession {
    DWORD processId;
    std::wstring displayName;
    IAudioSessionControl2* sessionControl;

    ~AudioSession();
};

class AudioSessionManager {
public:
    AudioSessionManager();
    ~AudioSessionManager();

    std::vector<AudioSession> enumerateAudioSessions();
    static std::wstring getProcessName(DWORD processId);

private:
    IMMDeviceEnumerator* deviceEnumerator;
    IMMDevice* device;
    IAudioSessionManager2* sessionManager;
};


#endif //WIFI_AUDIO_DONGLE_AUDIOSESSIONMANAGER_H
