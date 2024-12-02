//
// Created by Sadra on 12/1/2024.
//

#ifndef WIFI_AUDIO_DONGLE_AUDIOCAPTURE_H
#define WIFI_AUDIO_DONGLE_AUDIOCAPTURE_H

#include <windows.h>
#include <audioclient.h>
#include <audiopolicy.h>
#include <mmdeviceapi.h>
#include <vector>
#include <thread>
#include <atomic>

class AudioCapture {
public:
    AudioCapture(IAudioSessionControl2* sessionControl);
    ~AudioCapture();

    void start();
    void stop();

private:
    IAudioClient* audioClient;
    IAudioCaptureClient* captureClient;
    std::thread captureThread;
    std::atomic<bool> capturing;

    void captureLoop();
};

#endif //WIFI_AUDIO_DONGLE_AUDIOCAPTURE_H
