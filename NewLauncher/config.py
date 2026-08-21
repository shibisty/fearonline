# -*- coding: utf-8 -*-
"""
config.py
Загрузка/сохранение настроек лаунчера и построение аргументов запуска движка.
Логика аргументов повторяет GamePatcher.cpp, но windowed/язык теперь берутся
из пользовательских настроек, а не захардкожены.
"""

import json
import os
import sys

APP_DIR = os.path.dirname(os.path.abspath(sys.argv[0]))
CONFIG_PATH = os.path.join(APP_DIR, "launcher_config.json")

DEFAULT_CONFIG = {
    # --- UI / gameplay settings ---
    "windowed": False,     # False = "no" (default), True = "yes" — НАСТРОЙКА ИГРЫ
    "language": "eng",     # "eng" (default) | "ua" — ЯЗЫК ИНТЕРФЕЙСА ЛАУНЧЕРА

    # --- Engine launch settings (аналог Config из GamePatcher.cpp) ---
    "engine_path": r"C:\Program Files (x86)\Steam\steamapps\common\FEAR Online\FEAR_Online\Engine.exe",
    "working_directory": r"C:\Program Files (x86)\Steam\steamapps\common\FEAR Online\FEAR_Online",
    "uid": "test123",
    "launcher_id": "1",
    "login_server_ip": "127.0.0.1",
    "login_server_port": "30003",

    # --- Запоминание размера окна между запусками ---
    "window_width": None,
    "window_height": None,

    # --- Запоминание зума веб-страницы (Ctrl+колесо / Ctrl+ / Ctrl-) ---
    "zoom_factor": 1.0,

    # --- Отображаемый (не редактируемый) адрес в адресной строке лаунчера ---
    "display_url": "https://address.com",
}

# Коды языка UI лаунчера, которые поддерживаются (см. resources/locales/*.json)
SUPPORTED_UI_LANGUAGES = ("eng", "ua")

# Язык самого игрового движка (+Lan). Это НЕ связано с языком лаунчера —
# движок всегда получает фиксированное значение, как в оригинальном
# GamePatcher.cpp ("+Lan", "en").
ENGINE_LANGUAGE_CODE = "en"


def load_config() -> dict:
    cfg = DEFAULT_CONFIG.copy()
    if os.path.exists(CONFIG_PATH):
        try:
            with open(CONFIG_PATH, "r", encoding="utf-8") as f:
                data = json.load(f)
            cfg.update(data)
        except Exception:
            # Битый/повреждённый конфиг — используем значения по умолчанию
            pass
    return cfg


def save_config(cfg: dict) -> bool:
    try:
        with open(CONFIG_PATH, "w", encoding="utf-8") as f:
            json.dump(cfg, f, indent=4, ensure_ascii=False)
        return True
    except Exception:
        return False


def build_launch_args(cfg: dict) -> list:
    """
    Строит список аргументов командной строки для Engine.exe,
    аналогично Config::arguments в GamePatcher.cpp.

    Важно: windowed — это НАСТРОЙКА ИГРЫ и берётся из cfg.
    Язык (+Lan) — параметр движка, фиксирован (ENGINE_LANGUAGE_CODE) и
    НЕ связан с языком интерфейса лаунчера (тот настраивается отдельно,
    см. i18n.py).
    """
    windowed_flag = "1" if cfg.get("windowed") else "0"

    return [
        "+PB", "IN",
        "+UIPB", "none",
        "+BANNER", "0",
        "+UID", str(cfg.get("uid", "test123")),
        "+LauncherID", str(cfg.get("launcher_id", "1")),
        "+windowed", windowed_flag,
        "+Lan", ENGINE_LANGUAGE_CODE,
        "+LoginServerIP", str(cfg.get("login_server_ip", "127.0.0.1")),
        "+LoginServerPort", str(cfg.get("login_server_port", "30003")),
    ]


def resource_path(relative_path: str) -> str:
    """
    Возвращает абсолютный путь к ресурсу, работает как из исходников,
    так и из собранного PyInstaller-EXE (sys._MEIPASS).
    """
    if hasattr(sys, "_MEIPASS"):
        base_path = sys._MEIPASS
    else:
        base_path = os.path.dirname(os.path.abspath(__file__))
    return os.path.join(base_path, relative_path)
    