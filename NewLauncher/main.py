# -*- coding: utf-8 -*-
"""
main.py
Точка входа лаунчера F.E.A.R. Online.
Требует Windows + PyQt5 + PyQtWebEngine.
"""

import sys

from PyQt5.QtWidgets import QApplication

from config import load_config
from theme import apply_dark_theme
from i18n import load_language
from ui.main_window import MainWindow


def main():
    if sys.platform != "win32":
        print("Этот лаунчер поддерживает только Windows.")
        sys.exit(1)

    app = QApplication(sys.argv)
    app.setApplicationName("FEAR Online Launcher")

    # Тема применяется на уровне QApplication — одинаково влияет на
    # главное окно и на все дочерние окна (Settings и т.д.)
    apply_dark_theme(app)

    # Язык интерфейса лаунчера (НЕ язык игры — см. config.ENGINE_LANGUAGE_CODE)
    cfg = load_config()
    load_language(cfg.get("language", "eng"))

    window = MainWindow()
    window.show()

    sys.exit(app.exec_())


if __name__ == "__main__":
    main()
    