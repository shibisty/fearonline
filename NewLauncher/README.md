# F.E.A.R. Online Launcher (PyQt5)

A F.E.A.R. Online-style launcher: a dark industrial theme with red accents,
an embedded web page (`resources/index.html`), a settings window, and launching
`Engine.exe` with parameters.

## Project Structure

```
FEAR_Launcher/
├── main.py                     # entry point (theme + language are applied here)
├── config.py                    # settings, building game launch arguments
├── theme.py                     # application-wide dark theme (Fusion + palette + QSS)
├── i18n.py                      # LAUNCHER UI LOCALIZATION
├── ui/
│   ├── main_window.py           # main window (webview, nav, settings, start)
│   └── settings_window.py       # settings window (windowed / language)
├── resources/
│   ├── style.qss                 # application theme
│   ├── index.html                 # launcher start page
│   └── locales/
│       ├── eng.json                # launcher UI translations (English)
│       └── ua.json                 # launcher UI translations (Українська)
├── launcher.spec                 # PyInstaller spec for .exe build
├── requirements.txt
└── launcher_config.json          # created automatically next to the .exe/script
```

`main_window.py` and `settings_window.py` must be located inside `ui/`,
and `eng.json` / `ua.json` must be located inside `resources/locales/`.

## Important: Two Different Settings Parameters

- **Display mode (windowed)** — a **game** setting. It is passed to the
  engine as `+windowed 0/1` when launching (`config.build_launch_args`).
- **Language** — a **launcher** setting. It only changes the launcher UI text
  itself (titles, buttons, status) through `i18n.py` /
  `resources/locales/*.json`. It does **not** affect the game — the engine
  always receives a fixed `+Lan en` (`config.ENGINE_LANGUAGE_CODE`), just as
  in the original `GamePatcher.cpp`.

To add a new UI language:
1. Copy `resources/locales/eng.json` to `resources/locales/<code>.json`
   and translate the values (do not modify the keys).
2. Add `"<code>"` to `SUPPORTED_UI_LANGUAGES` (`config.py`) and
   `SUPPORTED_LANGUAGES` (`i18n.py`).
3. Add a radio button in `ui/settings_window.py`.

## Why the Theme "Switched" to Light — and How It Was Fixed

Previously, the style (`style.qss`) was applied only to the main window through
`self.setStyleSheet(...)`. The native Windows style partially ignores QSS
for the base widget palette, so the settings dialog (and some other elements)
could be rendered with the light system theme on top. In addition, if
`resources/style.qss` could not be found at the expected path, the theme simply
was not applied.

Now everything is applied once in `theme.apply_dark_theme()` (called from
`main.py` **before** creating any windows):
1. `QApplication.setStyle("Fusion")` — the only built-in Qt style that
   consistently respects QSS for all widgets.
2. A dark `QPalette` as the base — a fallback for elements not covered by
   `style.qss` (such as `QMessageBox`).
3. `style.qss` on top — detailed colors/borders for custom widgets.

This guarantees that both the main window and any child dialogs are styled.
If the theme still does not apply, the console will display the warning
`[theme] WARNING: style.qss not found at: ...` — this means the `resources/`
folder structure does not match the expected layout.

## Installation (Windows)

```powershell
python -m venv venv
venv\Scripts\activate
pip install -r requirements.txt
```

## Running from Source

```powershell
python main.py
```

## Configuring Game Paths

Open `config.py` and change the default values to match your installation:

```python
"engine_path": r"C:\Program Files (x86)\Steam\steamapps\common\FEAR Online\FEAR_Online\Engine.exe",
"working_directory": r"C:\Program Files (x86)\Steam\steamapps\common\FEAR Online\FEAR_Online",
```

These values can later be overridden through `launcher_config.json`,
which is created automatically next to the executable when settings are
saved for the first time.

## Launch Logic

When **START** is pressed, the launcher builds the same arguments as the
original `GamePatcher.cpp`; only `+windowed` is taken from the settings,
while the engine language (`+Lan`) is fixed:

```
+PB IN +UIPB none +BANNER 0 +UID <uid> +LauncherID <id>
+windowed <0|1> +Lan <en|ua>
+LoginServerIP <ip> +LoginServerPort <port>
```

and launches `Engine.exe` through `subprocess.Popen(cmd, cwd=working_directory)`.

The loading bar is not implemented — file updates are handled by a separate
program, as specified in the task.

## Building an .exe (PyInstaller)

```powershell
pip install pyinstaller
pyinstaller launcher.spec
```

The finished build will appear in `dist/FEAR_Online_Launcher/`. The
`FEAR_Online_Launcher.exe` file is the launcher; resources (`style.qss`,
`index.html`) are already bundled through the `datas` section in
`launcher.spec`.

If you want to add an `.ico` icon, place it in `resources/icons/app.ico`
and uncomment the `icon=...` line in `launcher.spec`.

## Notes

- Windows only (uses `CreateProcess`/`subprocess.Popen` under Win32,
  as well as `QtWebEngine`, which brings along the Chromium runtime —
  a large build size is expected).
- The address bar is a `QLineEdit` in `readOnly` mode, so it cannot
  be edited manually, but it is updated programmatically (`_on_url_changed`).
- The ◀ / ▶ buttons use the built-in `QWebEngineView` history
  (`.back()` / `.forward()`).
- F.E.A.R. background images/artwork are not included in the project
  (copyrighted material) — add your own assets to `resources/` and reference
  them from `index.html` / `style.qss`.
