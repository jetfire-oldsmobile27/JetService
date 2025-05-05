#include "demonizer.h"
#ifdef _WIN32
#  include <windows.h>
#else
#  include <fstream>
#  include <unistd.h>
#endif

using namespace jetfire27::Engine;

void Demonizer::Setup(const std::string& binPath, Mode mode) {
#ifdef _WIN32
    if (mode == Mode::AutoStart) {
        HKEY hKey;
        RegOpenKeyExA(HKEY_CURRENT_USER,
            "Software\\Microsoft\\Windows\\CurrentVersion\\Run",
            0, KEY_WRITE, &hKey);
        RegSetValueExA(hKey, "VisionAiDriver", 0, REG_SZ,
                       (const BYTE*)binPath.c_str(), binPath.size()+1);
        RegCloseKey(hKey);
    } else {
        // TODO: Windows Service через WinAPI
    }
#else
    if (mode == Mode::AutoStart) {
        std::ofstream f(std::string(getenv("HOME"))+"/.config/autostart/visionai-driver.desktop");
        f << "[Desktop Entry]\nType=Application\nExec="<<binPath
          <<"\nX-GNOME-Autostart-enabled=true\n";
    } else {
        std::ofstream f("/etc/systemd/system/visionai-driver.service");
        f << "[Unit]\nDescription=visionAi Driver Daemon\n\n"
          <<"[Service]\nExecStart="<<binPath<<"\nRestart=always\n\n"
          <<"[Install]\nWantedBy=multi-user.target\n";
    }
#endif
}
