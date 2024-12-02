//
// Created by Sadra on 12/1/2024.
//

#include "AudioCapture.h"
#include <iostream>

AudioCapture::AudioCapture(IAudioSessionControl2* sessionControl)
        : audioClient(nullptr), captureClient(nullptr), capturing(false) {

    if (!sessionControl) {
        std::cerr << "sessionControl is null" << std::endl;
        return;
    }

    IMMDeviceEnumerator* deviceEnumerator = nullptr;
    HRESULT hr = CoCreateInstance(__uuidof(MMDeviceEnumerator), NULL, CLSCTX_ALL, IID_PPV_ARGS(&deviceEnumerator));
    if (FAILED(hr)) {
        std::cerr << "Failed to create device enumerator: " << std::hex << hr << std::endl;
        return;
    }

    LPWSTR deviceId = nullptr;
    hr = sessionControl->GetSessionIdentifier(&deviceId);
    if (FAILED(hr) || !deviceId) {
        std::cerr << "Failed to get session identifier: " << std::hex << hr << std::endl;
        deviceEnumerator->Release();
        return;
    }

    IMMDevice* device = nullptr;
    hr = deviceEnumerator->GetDevice(deviceId, &device);
    CoTaskMemFree(deviceId);
    if (FAILED(hr) || !device) {
        std::cerr << "Failed to get audio device: " << std::hex << hr << std::endl;
        deviceEnumerator->Release();
        return;
    }

    hr = device->Activate(__uuidof(IAudioClient), CLSCTX_ALL, NULL, (void**)&audioClient);
    if (FAILED(hr) || !audioClient) {
        std::cerr << "Failed to activate audio client: " << std::hex << hr << std::endl;
        device->Release();
        deviceEnumerator->Release();
        return;
    }

    WAVEFORMATEX* waveFormat = nullptr;
    hr = audioClient->GetMixFormat(&waveFormat);
    if (FAILED(hr) || !waveFormat) {
        std::cerr << "Failed to get mix format: " << std::hex << hr << std::endl;
        device->Release();
        deviceEnumerator->Release();
        return;
    }

    hr = audioClient->Initialize(AUDCLNT_SHAREMODE_SHARED, 0, 0, 0, waveFormat, nullptr);
    if (FAILED(hr)) {
        std::cerr << "Failed to initialize audio client: " << std::hex << hr << std::endl;
        CoTaskMemFree(waveFormat);
        device->Release();
        deviceEnumerator->Release();
        return;
    }

    hr = audioClient->GetService(__uuidof(IAudioCaptureClient), (void**)&captureClient);
    if (FAILED(hr) || !captureClient) {
        std::cerr << "Failed to get audio capture client: " << std::hex << hr << std::endl;
    }

    CoTaskMemFree(waveFormat);
    device->Release();
    deviceEnumerator->Release();
}


AudioCapture::~AudioCapture() {
    stop();
    if (audioClient) audioClient->Release();
    if (captureClient) captureClient->Release();
}

void AudioCapture::start() {
    if (capturing) return;
    capturing = true;
    audioClient->Start();
    captureThread = std::thread(&AudioCapture::captureLoop, this);
}

void AudioCapture::stop() {
    capturing = false;
    if (captureThread.joinable()) {
        captureThread.join();
    }
    audioClient->Stop();
}

void AudioCapture::captureLoop() {
    while (capturing) {
        UINT32 packetLength = 0;
        captureClient->GetNextPacketSize(&packetLength);

        while (packetLength != 0) {
            BYTE* data;
            DWORD flags;
            UINT32 numFramesAvailable;

            captureClient->GetBuffer(&data, &numFramesAvailable, &flags, nullptr, nullptr);

            // For now, just print the size of the captured data
            std::cout << "Captured " << numFramesAvailable << " frames" << std::endl;

            captureClient->ReleaseBuffer(numFramesAvailable);
            captureClient->GetNextPacketSize(&packetLength);
        }

        Sleep(10);  // Sleep to avoid excessive CPU usage
    }
}
