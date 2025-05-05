// src/main.cpp

#include <cstdio>
#include <thread>
#include "testserver.h"    // имя файла в lowercase

// Выбор backend для tray.h
#if defined(_WIN32) || defined(_WIN64)
#  define TRAY_WINAPI 1
#elif defined(__linux__)
#  define TRAY_APPINDICATOR 1
#elif defined(__APPLE__)
#  define TRAY_APPKIT 1
#endif

#if TRAY_APPINDICATOR
#  define TRAY_ICON1 "indicator-messages"
#  define TRAY_ICON2 "indicator-messages-new"
#elif TRAY_APPKIT
#  define TRAY_ICON1 "icon.png"
#  define TRAY_ICON2 "icon.png"
#elif TRAY_WINAPI
#  define TRAY_ICON1 "assets/icon.ico"
#  define TRAY_ICON2 "assets/icon.ico"
#endif

#include "../thirdparty/tray.h"  

static jetfire27::Engine::Test::TestServer* g_server = nullptr;
static std::thread*                         g_thread = nullptr;

static void status_cb(struct tray_menu* item) {
    (void)item;
    std::printf("[Tray] Service launch\n");
}

static void restart_cb(struct tray_menu* item) {
    (void)item;
    std::printf("[Tray] Restart service...\n");
    g_server->Stop();
    if (g_thread->joinable()) g_thread->join();
    delete g_thread;
    delete g_server;
    g_server = new jetfire27::Engine::Test::TestServer(8080, "test.db");
    g_thread  = new std::thread(&jetfire27::Engine::Test::TestServer::Run, g_server);
    std::printf("[Tray] Service restarted\n");
}

static void quit_cb(struct tray_menu* item) {
    (void)item;
    tray_exit();
}

static struct tray_menu g_menu[] = {
    { (char*)"Посмотреть статус", 0, 0, status_cb,  NULL, NULL },
    { (char*)"Перезапуск",        0, 0, restart_cb, NULL, NULL },
    { (char*)"-",                 0, 0, NULL,       NULL, NULL },  
    { (char*)"Выйти",             0, 0, quit_cb,    NULL, NULL },
    { NULL,                       0, 0, NULL,       NULL, NULL }   
};

static struct tray g_tray = {
    (char*)TRAY_ICON1,
    g_menu
};

int main(int argc, char** argv) {
    (void)argc; (void)argv;

    g_server = new jetfire27::Engine::Test::TestServer(8080, "test.db");
    g_thread = new std::thread(&jetfire27::Engine::Test::TestServer::Run, g_server);

    if (tray_init(&g_tray) < 0) {
        std::fprintf(stderr, "Ошибка: не удалось инициализировать трей-иконку\n");
        g_server->Stop();
        if (g_thread->joinable()) {g_thread->join();};
        delete g_thread;
        delete g_server;
        return 1;
    }

    while (tray_loop(1) == 0) {
    }
    g_server->Stop();
    if (g_thread->joinable()) {g_thread->join();};
    delete g_thread;
    delete g_server;
    return 0;
}
