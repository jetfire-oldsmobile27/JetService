#pragma once
#include <string>

namespace jetfire27::Engine {
    enum class Mode { AutoStart, Service };

    class Demonizer {
    public:
        static void Setup(const std::string& binPath, Mode mode);
    };
}
