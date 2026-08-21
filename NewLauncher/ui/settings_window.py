# -*- coding: utf-8 -*-
"""
settings_window.py
Окно настроек.

ВАЖНО — два разных по смыслу параметра:
 - Display mode (windowed) — настройка ИГРЫ. Передаётся движку как
   +windowed 0/1 при запуске (см. config.build_launch_args).
 - Language — настройка ЛАУНЧЕРА. Меняет только язык интерфейса самого
   лаунчера (тексты, кнопки), см. i18n.py. На игру не влияет: движок
   всегда получает фиксированный +Lan en (config.ENGINE_LANGUAGE_CODE).

Принимает scale от MainWindow (тот же коэффициент, что и главное окно),
чтобы диалог выглядел пропорционально на 4K/8K так же, как основное окно.
"""

from PyQt5.QtCore import Qt
from PyQt5.QtWidgets import (
    QDialog, QLabel, QVBoxLayout, QHBoxLayout, QPushButton,
    QButtonGroup, QRadioButton, QFrame
)

from config import load_config, save_config
from i18n import tr


class SettingsWindow(QDialog):
    # Базовый размер диалога, соответствующий scale = 1.0
    BASE_W, BASE_H = 420, 340

    def __init__(self, parent=None, scale: float = 1.0):
        super().__init__(parent)
        self.scale = max(0.75, min(scale, 3.5))
        self.setWindowFlags(
            Qt.Dialog | Qt.CustomizeWindowHint | Qt.WindowTitleHint | Qt.WindowCloseButtonHint
        )

        self.cfg = load_config()

        self._build_ui()
        self._load_values()
        self._apply_scale()

    # ----------------------------------------------------------------- s()
    def _s(self, v):
        return max(1, round(v * self.scale))

    # ------------------------------------------------------------------ UI
    def _build_ui(self):
        self.setWindowTitle(tr("settings_title"))

        root = QVBoxLayout(self)
        self._root_layout = root
        root.setSpacing(18)

        self.title_label = QLabel(tr("settings_title"))
        self.title_label.setObjectName("SettingsTitle")
        root.addWidget(self.title_label)

        divider = QFrame()
        divider.setObjectName("Divider")
        divider.setFrameShape(QFrame.HLine)
        root.addWidget(divider)

        # --- Windowed mode (GAME setting) ---
        self.section_display = self._section_label(tr("section_display_mode"))
        root.addWidget(self.section_display)
        windowed_row = QHBoxLayout()
        self.windowed_group = QButtonGroup(self)
        self.rb_windowed_no = QRadioButton(tr("windowed_no_default"))
        self.rb_windowed_yes = QRadioButton(tr("windowed_yes"))
        self.windowed_group.addButton(self.rb_windowed_no, 0)
        self.windowed_group.addButton(self.rb_windowed_yes, 1)
        windowed_row.addWidget(self.rb_windowed_no)
        windowed_row.addWidget(self.rb_windowed_yes)
        root.addLayout(windowed_row)

        # --- Language (LAUNCHER setting, not sent to the game) ---
        self.section_language = self._section_label(tr("section_language"))
        root.addWidget(self.section_language)
        lang_row = QHBoxLayout()
        self.lang_group = QButtonGroup(self)
        self.rb_lang_eng = QRadioButton(tr("lang_option_eng_default"))
        self.rb_lang_ua = QRadioButton(tr("lang_option_ua"))
        self.lang_group.addButton(self.rb_lang_eng, 0)
        self.lang_group.addButton(self.rb_lang_ua, 1)
        lang_row.addWidget(self.rb_lang_eng)
        lang_row.addWidget(self.rb_lang_ua)
        root.addLayout(lang_row)

        root.addStretch(1)

        divider2 = QFrame()
        divider2.setObjectName("Divider")
        divider2.setFrameShape(QFrame.HLine)
        root.addWidget(divider2)

        # --- Buttons ---
        btn_row = QHBoxLayout()
        btn_row.addStretch(1)
        self.btn_cancel = QPushButton(tr("btn_cancel"))
        self.btn_cancel.setObjectName("SecondaryButton")
        self.btn_save = QPushButton(tr("btn_save"))
        self.btn_save.setObjectName("PrimaryButton")
        self.btn_cancel.setCursor(Qt.PointingHandCursor)
        self.btn_save.setCursor(Qt.PointingHandCursor)
        self.btn_cancel.clicked.connect(self.reject)
        self.btn_save.clicked.connect(self._on_save)
        btn_row.addWidget(self.btn_cancel)
        btn_row.addWidget(self.btn_save)
        root.addLayout(btn_row)

        self.radio_buttons = (
            self.rb_windowed_no, self.rb_windowed_yes,
            self.rb_lang_eng, self.rb_lang_ua,
        )

    @staticmethod
    def _section_label(text: str) -> QLabel:
        lbl = QLabel(text)
        lbl.setObjectName("SectionLabel")
        return lbl

    # --------------------------------------------------------------- scale
    def _apply_scale(self):
        s = self._s
        self.setFixedSize(s(self.BASE_W), s(self.BASE_H))
        self._root_layout.setContentsMargins(s(28), s(24), s(28), s(20))

        self.title_label.setStyleSheet(f"font-size:{s(16)}px;")
        for lbl in (self.section_display, self.section_language):
            lbl.setStyleSheet(f"font-size:{s(11)}px;")
        for rb in self.radio_buttons:
            ind = s(14)
            rb.setStyleSheet(
                f"QRadioButton {{ font-size:{s(12)}px; spacing:{s(8)}px; color:#c9c9cc; }}"
                f"QRadioButton:hover {{ color:#ffffff; }}"
                f"QRadioButton::indicator {{ width:{ind}px; height:{ind}px; border-radius:{s(8)}px; "
                f"border:1px solid #4a4a4f; background-color:#17171a; }}"
                f"QRadioButton::indicator:checked {{ background-color:#c0392b; border:1px solid #e04b3a; }}"
            )

        pad_v, pad_h = s(8), s(22)
        for btn in (self.btn_cancel, self.btn_save):
            btn.setStyleSheet(f"padding:{pad_v}px {pad_h}px; font-size:{s(12)}px;")

    # --------------------------------------------------------------- logic
    def _load_values(self):
        if self.cfg.get("windowed"):
            self.rb_windowed_yes.setChecked(True)
        else:
            self.rb_windowed_no.setChecked(True)

        if self.cfg.get("language") == "ua":
            self.rb_lang_ua.setChecked(True)
        else:
            self.rb_lang_eng.setChecked(True)

    def _on_save(self):
        self.cfg["windowed"] = self.rb_windowed_yes.isChecked()   # -> игра
        self.cfg["language"] = "ua" if self.rb_lang_ua.isChecked() else "eng"  # -> лаунчер
        save_config(self.cfg)
        self.accept()
        