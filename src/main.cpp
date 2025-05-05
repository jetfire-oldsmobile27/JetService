#include <cstdio>
#include <thread>
#include <memory>
#include <optional>

#include "testserver.h"    

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

using jetfire27::Engine::Test::TestServer;

static std::unique_ptr<TestServer>  g_server;
static std::optional<std::thread>   g_thread;

static void start_server() {
    if (g_thread && g_thread->joinable()) {return;};

    g_server = std::make_unique<TestServer>(8080, "test.db");
    g_thread.emplace(&TestServer::Run, g_server.get());
}

static void stop_server() {
    if (g_server) {
        g_server->Stop();
    }
    if (g_thread && g_thread->joinable()) {
        g_thread->join();
    }
    g_thread.reset();        
    g_server.reset();        
}

static void status_cb(struct tray_menu* /*item*/) {
    std::printf("[Tray] service is running\n");
}

static void restart_cb(struct tray_menu* /*item*/) {
    std::printf("[Tray] restarting service...\n");
    stop_server();
    start_server();
    std::printf("[Tray] service restarted\n");
}

static void quit_cb(struct tray_menu* /*item*/) {
    tray_exit();
}

static struct tray_menu g_menu[] = {
    { (char*)"Посмотреть статус", 0, 0, status_cb,  nullptr, nullptr },
    { (char*)"Перезапуск",        0, 0, restart_cb, nullptr, nullptr },
    { (char*)"-",                 0, 0, nullptr,     nullptr, nullptr },
    { (char*)"Выйти",             0, 0, quit_cb,    nullptr, nullptr },
    { nullptr,                    0, 0, nullptr,     nullptr, nullptr }
};

static struct tray g_tray = {
    (char*)TRAY_ICON1,
    g_menu
};

int main(int /*argc*/, char** /*argv*/) {
    start_server();

    if (tray_init(&g_tray) < 0) {
        std::fprintf(stderr, "Error: cannot initialize tray icon\n");
        stop_server();
        return 1;
    }

    while (tray_loop(1) == 0) {
    }

    std::printf("[Main] Exiting, stopping service...\n");
    stop_server();
    std::printf("[Main] Shutdown complete\n");
    return 0;
}
