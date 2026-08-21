# -*- coding: utf-8 -*-
"""
theme.py
Единая точка применения тёмной темы для ВСЕГО приложения (главное окно +
любые дочерние диалоги, например Settings).

Почему окно вдруг становилось "светлым":
 - style.qss применялся только к MainWindow через self.setStyleSheet(...).
 - Нативный стиль Windows (windowsvista/windows11) частично игнорирует
   QSS-правила для базовой палитры (фон, текст), поэтому часть виджетов
   (особенно в модальном диалоге настроек) рисовалась системной светлой
   темой поверх наших правил.
 - Если resources/style.qss не находился по пути (например, при неверной
   структуре папок), приложение просто молча оставалось без темы.

Решение:
 1) app.setStyle("Fusion") — единственный встроенный стиль Qt, который
    стабильно уважает QSS для всех виджетов.
 2) Тёмная QPalette как база — подстраховка для того, что не покрыто
    style.qss (QMessageBox и другие системные элементы).
 3) style.qss поверх — детальные цвета/рамки наших кастомных виджетов.

Всё это применяется один раз на уровне QApplication, поэтому действует
одинаково и на главное окно, и на все дочерние окна.
"""

import os

from PyQt5.QtGui import QPalette, QColor
from PyQt5.QtWidgets import QApplication

from config import resource_path

_DARK_PALETTE_COLORS = {
    QPalette.Window: "#121214",
    QPalette.WindowText: "#eaeaea",
    QPalette.Base: "#0e0e10",
    QPalette.AlternateBase: "#1c1c1f",
    QPalette.ToolTipBase: "#1c1c1f",
    QPalette.ToolTipText: "#eaeaea",
    QPalette.Text: "#d8d8da",
    QPalette.Button: "#1c1c1f",
    QPalette.ButtonText: "#eaeaea",
    QPalette.BrightText: "#ff5c4d",
    QPalette.Link: "#e04b3a",
    QPalette.Highlight: "#8b1a1a",
    QPalette.HighlightedText: "#ffffff",
}


def apply_dark_theme(app: QApplication) -> None:
    app.setStyle("Fusion")

    palette = QPalette()
    for role, color in _DARK_PALETTE_COLORS.items():
        palette.setColor(role, QColor(color))
    palette.setColor(QPalette.Disabled, QPalette.Text, QColor("#5a5a5e"))
    palette.setColor(QPalette.Disabled, QPalette.ButtonText, QColor("#5a5a5e"))
    palette.setColor(QPalette.Disabled, QPalette.WindowText, QColor("#5a5a5e"))
    app.setPalette(palette)

    qss_path = resource_path(os.path.join("resources", "style.qss"))
    if os.path.exists(qss_path):
        with open(qss_path, "r", encoding="utf-8") as f:
            app.setStyleSheet(f.read())
    else:
        print(f"[theme] WARNING: style.qss not found at: {qss_path}")
        