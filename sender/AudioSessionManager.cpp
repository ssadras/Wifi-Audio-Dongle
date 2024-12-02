//
// Created by Sadra on 12/1/2024.
//

#include "AudioSessionManager.h"
#include <windows.h>
#include <psapi.h>  // For GetModuleBaseName
#include <iostream>

AudioSession::~AudioSession() {
    if (sessionControl) {
        sessionControl->Release();
    }
}

AudioSessionManager::AudioSessionManager()
        : deviceEnumerator(nullptr), device(nullptr), sessionManager(nullptr) {
    CoInitialize(NULL);

    const CLSID CLSID_MMDeviceEnumerator = __uuidof(MMDeviceEnumerator);
    const IID IID_IMMDeviceEnumerator = __uuidof(IMMDeviceEnumerator);

    CoCreateInstance(
            CLSID_MMDeviceEnumerator, NULL,
            CLSCTX_ALL, IID_IMMDeviceEnumerator,
            (void **) &deviceEnumerator);

    deviceEnumerator->GetDefaultAudioEndpoint(eRender, eMultimedia, &device);
    device->Activate(__uuidof(IAudioSessionManager2), CLSCTX_ALL, NULL,
                     (void **) &sessionManager);
}

AudioSessionManager::~AudioSessionManager() {
    if (sessionManager) sessionManager->Release();
    if (device) device->Release();
    if (deviceEnumerator) deviceEnumerator->Release();
    CoUninitialize();
}

std::vector<AudioSession> AudioSessionManager::enumerateAudioSessions() {
    std::vector<AudioSession> sessions;
    IAudioSessionEnumerator *sessionEnumerator = nullptr;

    sessionManager->GetSessionEnumerator(&sessionEnumerator);

    int sessionCount = 0;
    sessionEnumerator->GetCount(&sessionCount);

    for (int i = 0; i < sessionCount; i++) {
        IAudioSessionControl *sessionControl = nullptr;
        sessionEnumerator->GetSession(i, &sessionControl);

        IAudioSessionControl2 *sessionControl2 = nullptr;
        if (SUCCEEDED(sessionControl->QueryInterface(__uuidof(IAudioSessionControl2), (void **) &sessionControl2))) {
            DWORD processId;
            sessionControl2->GetProcessId(&processId);

            std::wstring processName = getProcessName(processId);
            sessions.push_back({processId, processName, sessionControl2});
        }
        sessionControl->Release();
    }
    sessionEnumerator->Release();
    return sessions;
}

std::wstring AudioSessionManager::getProcessName(DWORD processId) {
    HANDLE process = OpenProcess(PROCESS_QUERY_INFORMATION | PROCESS_VM_READ, FALSE, processId);
    if (process) {
        WCHAR name[MAX_PATH];
        if (K32GetModuleBaseNameW(process, NULL, name, MAX_PATH)) {
            CloseHandle(process);
            return name;
        }
        CloseHandle(process);
    }
    return L"Unknown";
}