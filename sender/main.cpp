#include "AudioSessionManager.h"
#include "AudioCapture.h"
#include <iostream>
#include <vector>

int main() {
    AudioSessionManager manager;
    std::vector<AudioSession> sessions = manager.enumerateAudioSessions();

    std::wcout << L"Available audio sessions:\n";
    for (size_t i = 0; i < sessions.size(); i++) {
        std::wcout << i << L": " << sessions[i].displayName
                   << L" (PID: " << sessions[i].processId << L")\n";
    }

    std::wcout << L"\nSelect a session to capture: ";
    int selectedIndex;
    std::wcin >> selectedIndex;

    if (selectedIndex >= 0 && selectedIndex < sessions.size()) {
        AudioCapture capture(sessions[selectedIndex].sessionControl);
        capture.start();

        std::wcout << L"Capturing audio... Press Enter to stop." << std::endl;
        std::cin.ignore();
        std::cin.get();

        capture.stop();
    } else {
        std::wcout << L"Invalid selection." << std::endl;
    }

    return 0;
}
