# -*- coding: utf-8 -*-
"""
i18n.py
Локализация ИНТЕРФЕЙСА ЛАУНЧЕРА (не игры).

Настройка "Language" в окне Settings меняет только текст самого лаунчера
(заголовки, кнопки, подписи). Игровой движок всегда получает фиксированный
+Lan en (см. config.ENGINE_LANGUAGE_CODE) — это отдельная, не связанная
настройка.

Переводы хранятся в JSON-файлах: resources/locales/<code>.json
"""

import json
import os

from config import resource_path

DEFAULT_LANGUAGE = "eng"
SUPPORTED_LANGUAGES = ("eng", "ua")

_current_lang = DEFAULT_LANGUAGE
_strings = {}
_fallback_strings = {}


def _load_json(lang_code: str) -> dict:
    path = resource_path(os.path.join("resources", "locales", f"{lang_code}.json"))
    try:
        with open(path, "r", encoding="utf-8") as f:
            return json.load(f)
    except Exception as e:
        print(f"[i18n] WARNING: failed to load '{path}': {e}")
        return {}


def load_language(lang_code: str) -> None:
    """Загружает словарь перевода для указанного языка лаунчера."""
    global _current_lang, _strings, _fallback_strings

    if lang_code not in SUPPORTED_LANGUAGES:
        lang_code = DEFAULT_LANGUAGE

    _current_lang = lang_code
    _strings = _load_json(lang_code)

    # английский всегда держим как запасной вариант, если ключа нет в переводе
    if lang_code != DEFAULT_LANGUAGE:
        _fallback_strings = _load_json(DEFAULT_LANGUAGE)
    else:
        _fallback_strings = _strings


def current_language() -> str:
    return _current_lang


def tr(key: str, **kwargs) -> str:
    """Возвращает переведённую строку по ключу с подстановкой kwargs через .format()."""
    text = _strings.get(key)
    if text is None:
        text = _fallback_strings.get(key, key)
    if kwargs:
        try:
            text = text.format(**kwargs)
        except Exception:
            pass
    return text
    