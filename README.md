# JetService
Minimalistic system service with SQLite, REST API, generic json parser, and [zserge tray lib](https://github.com/zserge/tray)

1) For install dependencies just run in root:
```powershell
conan install . --output-folder=build --build=missing
```
2) Choose conan-default profile
3) And then just run as service:
```powershell
.\build\Release\
```
