#include <winsock2.h>    // ← обязательно первым!
#include <ws2tcpip.h>
#include <windows.h>
#include <shellapi.h>
#pragma comment(lib, "shell32.lib")

#include "TestServer.h"
#include <thread>

#if defined(_WIN32) || defined(_WIN64)
  #define TRAY_WINAPI 1
#endif

#if TRAY_WINAPI
  #define TRAY_ICON1 "assets/icon.ico"
  #define TRAY_ICON2 "assets/icon.ico"
#endif

#include "../thirdparty/tray.h"  

static jetfire27::Engine::Test::TestServer* g_server = nullptr;
static std::thread*                         g_thread = nullptr;

static void status_cb(struct tray_menu* item) {
    (void)item;
    MessageBoxA(nullptr, "Сервер запущен", "jetfire27 Engine", MB_OK);
}

static void restart_cb(struct tray_menu* item) {
    (void)item;
    MessageBoxA(nullptr, "Перезапуск сервера...", "jetfire27 Engine", MB_OK);

    g_server->Stop();
    if (g_thread->joinable()) g_thread->join();
    delete g_thread;
    delete g_server;

    g_server = new jetfire27::Engine::Test::TestServer(8080, "test.db");
    g_thread = new std::thread(&jetfire27::Engine::Test::TestServer::Run, g_server);

    MessageBoxA(nullptr, "Сервер перезапущен", "jetfire27 Engine", MB_OK);
}

static void quit_cb(struct tray_menu* item) {
    (void)item;
    g_server->Stop();
    if (g_thread->joinable()) g_thread->join();
    delete g_thread;
    delete g_server;

    tray_exit();
}

static struct tray_menu g_menu[] = {
    { (char*)"Посмотреть статус", 0, 0, status_cb, nullptr, nullptr },
    { (char*)"Перезапуск",         0, 0, restart_cb, nullptr, nullptr },
    { (char*)"-",                 0, 0, nullptr,     nullptr, nullptr }, 
    { (char*)"Выйти",             0, 0, quit_cb,    nullptr, nullptr },
    { nullptr,                    0, 0, nullptr,     nullptr, nullptr }  
};

static struct tray g_tray = {
    (char*)TRAY_ICON1,
    g_menu
};

int WINAPI wWinMain(HINSTANCE, HINSTANCE, PWSTR, int) {
    g_server = new jetfire27::Engine::Test::TestServer(8080, "test.db");
    g_thread = new std::thread(&jetfire27::Engine::Test::TestServer::Run, g_server);

    if (tray_init(&g_tray) < 0) {
        MessageBoxA(nullptr, "Не удалось инициализировать трей", 
                    "jetfire27 Engine", MB_ICONERROR);
        g_server->Stop();
        if (g_thread->joinable()) g_thread->join();
        delete g_thread;
        delete g_server;
        return 1;
    }

    while (tray_loop(1) == 0) {}
    g_server->Stop();
    if (g_thread->joinable()) g_thread->join();
    delete g_thread;
    delete g_server;

    return 0;
}
