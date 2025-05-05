from conan import ConanFile
from conan.tools.cmake import CMake, CMakeToolchain, CMakeDeps
from conan.tools.system.package_manager import Apt, Brew, Chocolatey

class jetfire27EngineConan(ConanFile):
    name = "jetfire27_engine"
    version = "0.1"
    license = "LGPL"
    settings = "os", "compiler", "build_type", "arch"
    requires = ["boost/1.87.0", "sqlite3/3.49.1"]
    generators = "CMakeToolchain", "CMakeDeps"
    exports_sources = "include/*", "src/*", "CMakeLists.txt", "main.cpp"

    def layout(self):
        pass

    def system_requirements(self): # unusuable maybe in future
        if self.settings.os == "Linux":
            Apt(self).install(["libappindicator3-dev", "libgtk-3-dev"], update=True)
        elif self.settings.os == "Macos":
            Brew(self).install(["gtk+3", "libappindicator"])
        elif self.settings.os == "Windows":
            pass

    def build(self):
        cmake = CMake(self)
        cmake.configure()
        cmake.build()
