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

**JetService** is a cross-platform C++20 application that provides:

* REST-client based on Boost.Beast (GET/POST).
* SQLite wrapper (open/create database, execute queries).
* Template JSON-parser (Boost.JSON) with `Marshall`/`UnMarshall` methods.
* Binary conversion to daemon/service (Windows Service or systemd/autostart).
* HTTP test server for CRUD operations with the database.
* System tray icon with “View Status”, “Restart” and “Exit” menus, implemented via a lightweight cross-platform library `tray.h` from zserge.

---

## Project structure

```
project-root/
├─── CMakeLists.txt
├──── conanfile.txt
├──── README.md
├─── include/
│ ├──── RestClient.h
│ ├─── SQLiteDB.h
│ ├─── Parser.h
│ ├──── Daemonizer.h
│ ├─── TestRecord.h
│ └─── TestServer.h
├─── src/
│ ├──── RestClient.cpp
│ ├──── SQLiteDB.cpp
│ ├─── Parser.cpp
│ ├──── Daemonizer.cpp
│ ├─── TestServer.cpp
│ └─── main.cpp
└─── thirdparty/
    └─── tray.h
```

---

### Components

### 1. REST-client (`jetfire27::Engine::REST::RestClient`)

* Based on Boost.Asio + Boost.Beast.
* The `Get(host, target, port)` method for HTTP-GET requests.
* Easily extendable with POST/PUT support if needed.

### 2. SQLite wrapper (`jetfire27::Engine::DB::SQLiteDB`)

* Opens or creates a database file.
* Methods `Execute(sql)` and `Execute(sql, callback, data)` to execute queries and selections.
* Generates `std::runtime_error` exceptions on errors.

### 3. JSON parser (`jetfire27::Engine::JsonParser::Parser<T>`)

* Template class with methods:

  * ``std::string Marshall(const T&)``
  * `T UnMarshall(const std::string&)`
* An example of specialization for `TestRecord` is shown in `TestServer.cpp`.

### 4. Daemonizer (`jetfire27::Engine::Daemonizer`)

* `Setup(binPath, Mode)` method:

  * Windows AutoStart (registry) or Service (via WinAPI/sc.exe).
  * Linux autostart (`~/.config/autostart`) or systemd unit (`/etc/systemd/system`).

### 5. Test server (`jetfire27::Engine::Test::TestServer`)

* Listens on the HTTP port (default is 8080).
* GET `/items` - returns all records from the `test` table.
* POST `/items` - accepts JSON (`{“id”:0, “name”:“...”)`), adds a record.
* Uses Boost.Asio for TCP reception and Boost.JSON + SQLite.

### 6. System tray (library `tray.h`)

* Lightweight cross-platform C99 header library by zserge.
* `tray_init(&tray)`, `tray_loop(blocking)`, `tray_exit()`.
* Context menu is specified via the `struct tray_menu[]` array.
* Supports Windows (WinAPI), Linux (AppIndicator/Gtk), macOS (AppKit).

---

## Startup and usage

1. Run the binary:

   ```bash
   ./JetService.exe # Windows
   ./JetService     # Linux/macOS
   ```

2. The “JetService” icon will appear in the system tray.

3. Right click on the icon → menu:

   * **View Status** - displays the server status message.
   * **Restart** - stops and restarts the server.
   * **Exit** - closes the tray and terminates the server correctly.

4. HTTP API (Postman or curl):

   * `GET http://localhost:8080/items`
   * `POST http://localhost:8080/items` with JSON body `{“id”:0, “name”: “Name”}`

---

## API Structure

| Endpoint | Method | Description |
| -------- | ----- | -------------------------------------------- |
| `/items` | GET | Returns JSON array of records |
| `/items` | POST | Adds a record, returns `{status: “ok”}` | |

Example GET response:

```json
[
  {“id”:1,“name”:“Jetfire”},
  {“id”:27,“name”:“Oldmobile”}
]
```

---

## How to remove force JetService?

| OS | Service Search | Termination |
| ----------- | ----------------------------- | -------------------------------------------- |
| **Linux** | `systemctl status jetservice` | `systemctl stop jetservice` |
| **macOS** | `launchctl list` | `launchctl remove jetservice` |
| **Windows** | `sc query jetservice` | `sc stop jetservice && sc delete jetservice` | |

## License

The project is distributed under the LGPL license. See the `LICENSE` file.