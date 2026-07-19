![Screenshot](public/assets/images/logo.jpg)

# F.E.A.R. Online — Reverse Engineering Notes
## Status: Not working yet.

Notes on analyzing authentication and client startup for F.E.A.R. Online
(Steam release, published by Aeria Games) for the purpose of standing up
a private server.

- **Steam App ID:** 223650 (only available via direct link)
- Install: `Win+R` → `steam://install/223650/` (includes DLC)

---

## Tools

| Tool | Purpose |
|---|---|
| [**DebugView**](https://learn.microsoft.com/ru-ru/sysinternals/downloads/debugview) | Launcher debug messages, used to locate spots inside `Launcher.exe` via strings |
| [**TCPView**](https://learn.microsoft.com/ru-ru/sysinternals/downloads/tcpview) | Outgoing requests from a specific process (`Engine.exe`, `Launcher.exe`) |
| [**Process Monitor**](https://learn.microsoft.com/ru-ru/sysinternals/downloads/procmon) | General process/file/registry activity monitor |
| [**Wireshark**](https://www.wireshark.org/) | Deeper inspection of outgoing requests |
| [**Ghidra**](https://github.com/NationalSecurityAgency/ghidra/releases) | String search, disassembly, and decompilation of functions |
| [**x32dbg**](https://x64dbg.com/) | Step-by-step debugging, breakpoints, call stack, memory values, DLL load log and function call tracing |
| **Python custom server** | Intercepting launcher requests and generating responses |

---

## Hosts file (for local development)

```
127.0.0.1 fearonline.patch.aeriagames.com
127.0.0.1 www.fearonline.patch.aeriagames.com
127.0.0.1 fearonline.aeriagames.com
127.0.0.1 www.fearonline.aeriagames.com
127.0.0.1 aeriagames.com
127.0.0.1 www.aeriagames.com
127.0.0.1 oclient.com
127.0.0.1 www.oclient.com
127.0.0.1 test.aeriagames.com
127.0.0.1 www.test.aeriagames.com
```

## Python custom server
The server listens on ports **80/443/30003** (http/https/tcp respectively).

Firstly, copy files from C:\Program Files (x86)\Steam\steamapps\common\FEAR Online\LauncherData\Temp to ./public/LivePatch/ClientFear 
and C:\Program Files (x86)\Steam\steamapps\common\FEAR Online\LauncherData\Temp\Config.xml to ./public/LivePatch/Launcher.

- Windows
```
server.bat
```
- Linux
```
server.sh
```
- or for all with Python 3
```
python server.py
```

## Server endpoints
| URL | Purpose |
|---|---|
| `http://fearonline.patch.aeriagames.com/LivePatch/Launcher/` | Launcher files, used to fetch launcher updates |
| `http://fearonline.patch.aeriagames.com/LivePatch/ClientFear/` | Files required for the launcher to operate |
| `http://fearonline.aeriagames.com/fogame/notice` | Launcher home/notice page |
| `http://www.aeriagames.com/dialog/oauth?response_type=code&state=xyz&scope=scope_general,scope_billing&lang=en&client_id=652064e22d0ba471c6e7befe6fc91c650519e6f1c&redirect_uri=https://www.aeriagames.com/content_only_launcher&` | Login window (OAuth) |
| `https://www.aeriagames.com/content_only_launcher` | Redirect target where `code=[temp_code]` is returned |
| `https://www.aeriagames.com/social_connect/steam/connect/callback/redirect` | Likely Steam-linked authentication |
| `http://fearonline.aeriagames.com/fogame/steam_notice` | Likely the home page for the Steam version |
| `https://www.aeriagames.com/dialog/oauth/authorize?response_type=code&client_id=652064e22d0ba471c6e7befe6fc91c650519e6f1c&state=xyz&redirect_url=https://www.oclient.com/code2token.php&` | OAuth2 endpoint with redirect |
| `https://www.oclient.com/code2token.php` | Exchanges `code` → `access_token` |
| `[ip]:30003` | Port the game connects to after launch (server browser / shop, presumably) |

> `aeriagames.com` and `oclient.com` are completely different domains, yet
> `oclient` can exchange a code issued by `aeriagames.com` for an
> `access_token` — implying they share a compatible backend.

---

## Launcher.exe

![Screenshot](examples/image_1.png)

- Loads **ZLauncher.dll** — contains the callbacks the `.exe` needs to
  handle authentication events.
- **ZAeria.dll** — present alongside `Launcher.exe`, but it's unknown
  whether it's ever actually loaded (no load event for it has been
  observed either way). It internally references an auth window with
  the same title as the one shown during login, but this could easily
  be coincidental — no direct call to that window has been traced from
  the `.exe` or `ZLauncher.dll` yet, and no confirmation that this DLL
  is loaded at all.
- Contacts the server for client version information.
- **Config.xml** — important file, contains auth hosts and startup settings.
- Compares versions, downloads some configuration files.
- Opens the OAuth window (embedded IE 11) — not visible in the F11Chooser debugger.
- User enters login/password → a temporary `code=` is received.
- The `code` must be exchanged for an `access_token` (see open questions).
- On successful authentication, launches `Engine.exe` with the required parameters.

### aeria_launcher.exe

A separate, much smaller exe. Likely intended to load `ZAeria.dll`, but
no evidence of it actually being used in the current launch chain has
been found.

### Registry

The engine reads/writes a registry key:

```
HKEY_CURRENT_USER\SOFTWARE\FOO Productions\F.E.A.R. Online\1.00.0000
Value: USERID
```

This is used to build a local path like
`%LOCALAPPDATA%\InPlay Interactive\<USERID>\` for **local save/user
data** — not a network authentication mechanism. This value has not
appeared locally yet, presumably because the OAuth flow is currently
breaking before `Launcher.exe` gets a chance to write it.

---

## Engine.exe

- Loads **GameClient.dll** — this is where command-line arguments are
  validated, and where traces of Steam authentication live.
- **Argument syntax is `+key value`** (not `-key=value`).

### Confirmed working command line

The following full parameter set was reconstructed from the command-line
builder inside `Launcher.exe` (`FUN_0048f700`) and confirmed to work by
direct test: the game launches and reaches an actual TCP connection
attempt to `LoginServerIP:LoginServerPort` (i.e. it passes every
command-line validation check inside `GameClient.dll`).

```
"C:\Program Files (x86)\Steam\steamapps\common\FEAR Online\FEAR_Online\Engine.exe" +PB AE +UIPB none +BANNER 1 +UID test123 +LauncherID 1 +windowed 1 +Lan en +LoginServerIP 127.0.0.1 +LoginServerPort 30003
```

| Parameter | Meaning / notes |
|---|---|
| `PB` | Likely a region/publisher code. Confirmed working value: `AE`. Other known values: `IN`, `NO`, `WI`. |
| `UIPB` | Unclear purpose. Tested with `none`; corresponds to a config field that was empty (`UIPB=""`) in the original launcher config. |
| `BANNER` | Unclear purpose. Hardcoded to `1` inside `Launcher.exe`. |
| `UID` | Likely the user's unique account ID. Passed into the command-line builder from an external parameter (`param_2`) — presumably comes from the server response after successful authentication. Tested with an arbitrary placeholder value. |
| `LauncherID` | Unclear purpose. Built via `wsprintfW` from an internal numeric field in `Launcher.exe`. |
| `windowed` | Windowed mode, `1`/`0`. |
| `Lan` | Localization/language code. Set to `en`. |
| `LoginServerIP` | Login server IP address. |
| `LoginServerPort` | Login server port. Default/tested value: `30003`. |

Not included in the tested command line (added only conditionally by
`Launcher.exe`, see below):
- `nVersionType 1` — added only if a certain internal config flag is set.
- `OnSteam 1` / `SteamID <value>` — added only if the Steam gate check
  passes (see "Open question: user identification" below).

### Parameter validation order at startup

1. `+PB` is checked first.
   - If the key is **missing entirely** → dialog **"Please Excute 'Launch FEAROnline.exe'"** → `PostQuitMessage`.
   - If the key is **present but its value fails a later check** → dialog **"Wrong Argument!"**.
2. The value `AE` **passes** the initial string comparison, but a
   **region-specific validator function** is then called, which can
   still reject it → "Wrong Argument!" again.

### Findings from decompiled functions (GameClient.dll, via Ghidra)

- **Dialog dispatch function** — the routine that ultimately shows both
  "Please Excute..." and "Wrong Argument!" reads the `PB` value and
  compares it against four known codes: `"IN"`, `"AE"`, `"NO"`, `"WI"`.
  Each code has its own dedicated validator function; only if that
  validator succeeds does startup continue past this stage.
- **Region "AE" validator** — after matching `"AE"`, this function checks
  an `"OnSteam"` flag and, if true, reads a `"SteamID"` parameter (via
  the exact same command-line getter used for `PB`), then compares it
  against something via a separate helper. If this comparison fails, the
  user sees "Please Execute Steam client and steam login" and the
  process quits. This strongly suggests `SteamID` is also expected to be
  supplied as a command-line parameter by the launcher (not yet
  confirmed experimentally).
- **LoginServerIP / LoginServerPort reader** — reads both parameters
  through the same getter mechanism, with explicit debug logging calls
  (`"Program Run Command Argu - Not Exist LoginServerIP"`,
  `"...Invalid Info LoginServerPort"`, etc.). It's unclear yet whether
  this logging goes to a file (`enginemsg.txt`, seen in the strings
  dump) or only to `OutputDebugString`; no such log file has been found
  locally yet.
- **CSV server-list parser** — looks for either a built-in archive
  resource named `c3.Arch01` or, as a fallback, a file at
  `./CSV/EtcInfo.csv` with columns: `Index, GameServerIP,
  GameServerPort, Language, EventKey, AcceptChannel`. This file is
  currently missing from the install and will be needed once the
  authentication/login-server stage is resolved. Exact delimiter/row
  format not yet confirmed.
- **Local user-data path builder** — combines `%LOCALAPPDATA%\InPlay
  Interactive\` with the `USERID` registry value (see above) to build a
  save-data folder path. Confirmed this is unrelated to network
  authentication.

---

## ZNetwork.dll

A separate DLL of interest — its exports contain every in-game player
action. These are likely hooked to in-game events elsewhere, in the
`.exe` or another DLL.

---

## Engine.exe command-line syntax — trial results

Tested experimentally via x32dbg (breakpoint on the `call eax` inside
`GameClient.dll` that performs the parameter lookup, then inspecting the
`EAX` register after the call):

| Syntax tried | Result |
|---|---|
| `-PB=AE` | Not found (EAX = 0) |
| `-PB AE` | Not found (EAX = 0) |
| `-PB PB_NoOverride` | Not found (EAX = 0) |
| `PB AE` (no prefix) | Not found (EAX = 0) |
| **`+PB AE`** | **Found** (EAX ≠ 0), value `"AE"` matches the string in code |

**Conclusion: the correct syntax is `+key value`.**

With `+PB AE` alone, the "Please Excute..." dialog changed to **"Wrong
Argument!"**, confirming the key is found and the value `AE` is
recognized, but that in isolation the additional region-specific
validator was not satisfied.

**Update:** once the full parameter set reconstructed from
`Launcher.exe`'s command-line builder was used (see "Confirmed working
command line" above), the game launched successfully and reached a real
TCP connection attempt on `LoginServerIP:LoginServerPort`. The earlier
"Wrong Argument!" was therefore caused by missing parameters
(`UID`, `LauncherID`, `Lan`, etc.), not by an invalid `PB` value.

---

## Open question: user identification

1. `Launcher.exe` performs a full OAuth flow (login → `code` → exchange
   for `access_token` via `code2token.php`) — this **should not be
   bypassable via Steam authentication alone**: no publisher with this
   business model (monetization through its own cash shop and account
   system) would skip its own account registration, even for a
   Steam-purchased copy.
2. Steam's ACF manifest (`LauncherPath: steam.exe`) contains no
   game-specific launch parameters — Steam itself doesn't launch
   `Engine.exe` directly; the real entry point is `Launcher.exe`
   (confirmed via x32dbg logs).
3. The `OnSteam`/`SteamID` check in `GameClient.dll` is most likely an
   **additional DRM/anti-piracy ownership check**, not a replacement for
   the main Aeria authentication.
4. The real session token/ticket is presumably written by
   `Launcher.exe` into the registry (near the already-found `USERID`)
   **after** a successful `code → access_token` exchange — but locally
   this step never happens yet, since the request to `code2token.php`
   never completes.

### code2token.php diagnosis — current status

- Hosts are configured correctly (`www.oclient.com` → `127.0.0.1`).
- OAuth login succeeds: `code` is obtained, the redirect to
  `/content_only_launcher` returns 200, `code`/`state` are read
  correctly.
- The request to exchange `code → access_token` **never reaches the
  server** (not visible in server logs or in Wireshark).
- Hypotheses: (a) Wireshark isn't capturing loopback traffic without
  special Npcap configuration; (b) a `state` or other validation check
  fails before the request is ever sent, and the code bails out earlier.

**Next step:** Process Monitor filtered on `Launcher.exe` and `TCP*`
operations, to determine whether a `connect()` is even attempted after
the login window closes.

# EXE/DLL Tree

[C:\PROGRAM FILES (X86)\STEAM\STEAMAPPS\COMMON\FEAR ONLINE](docs/tree.md)
|== aeria_launcher.exe
|== Aeria_web.ico
|== Compare2Requisite.lst
|== F.E.A.R. Online Website.URL
|== fearonline_install.ico
|== Get Aeria Points.URL
|== [Launcher.exe](docs/Launcher.exe.md)
|== Launcher.ini
|== sdkencryptedappticket.dll
|== steam_api.dll
|== steam_appid.txt
|== Uninst.exe
|== [ZAeria.dll](docs/ZAeria.dll.md)
|== [ZLauncher.dll](docs/ZLauncher.dll.md)
|== 
+---FEAR_Online
|== |== AssertWin32DLL.dll
|== |== AssertWin32DLLU.dll
|== |== binkw32.dll
|== |== dbghelp.dll
|== |== DumpGen.exe
|== |== eax.dll
|== |== [Engine.exe](docs/Engine.exe.md)
|== |== Game.ini
|== |== GameDatabase.dll
|== |== GameDefinitionFile.dll
|== |== LTMemory.dll
|== |== Monolith.PropertyGrid.v1.0.dll
|== |== Monolith.PropertyGrid.v1.0D.dll
|== |== Monolith.PropertyGrid.v1.0U.dll
|== |== Monolith.PropertyGrid.v1.0UD.dll
|== |== PerformanceMon.dll
|== |== sdkencryptedappticket.dll
|== |== SecuromPaul.dll
|== |== steam_api.dll
|== |== StringEditRuntime.dll
|== |== symsrv.dll
|== |== 
|== +---CSV
|== |== ==  OptionPatchList.csv
|== |
|== +---Game
|== |== |== ClientFx.fxd
|== |== |== ClientSession_MH.dll
|== |== |== dbghelp.dll
|== |== |== [GameClient.dll](docs/GameClient.dll.md)
|== |== |== [GameServer.dll](docs/GameServer.dll.md)
|== |== |== interface.bndl
|== |== |== SKU.cfg
|== |== |== SKULowViolence.cfg
|== |== |== Version.cfg
|== |== |== [ZNetwork.dll](docs/ZNetwork.dll.md)
