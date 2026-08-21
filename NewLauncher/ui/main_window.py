# -*- coding: utf-8 -*-
"""
main_window.py
Главное окно лаунчера:
 - адресная строка (read-only)
 - кнопки вперёд/назад для навигации по встроенной странице (index.html)
 - шестерёнка -> открывает окно настроек (windowed = игра, language = лаунчер)
 - кнопка START -> запускает Engine.exe с параметрами (см. config.build_launch_args)

Окно свободно растягивается (640x480 .. 8K), запоминает последний размер
(config.window_width/window_height) и масштабирует шрифты/кнопки/панели
пропорционально текущему размеру — это чинит "мелкие кнопки на 4K",
потому что раньше они были захардкожены в пикселях под маленькое окно.

Тема оформления (цвета/рамки) применяется глобально в theme.apply_dark_theme()
(main.py); здесь мы только дополнительно переопределяем font-size/размеры
для масштабирования — это не конфликтует с общей темой.

Только Windows.
"""

import os
import subprocess
import sys

from PyQt5.QtCore import QUrl, Qt, QTimer
from PyQt5.QtGui import QKeySequence
from PyQt5.QtWidgets import (
    QApplication, QMainWindow, QWidget, QVBoxLayout, QHBoxLayout, QLineEdit,
    QPushButton, QLabel, QMessageBox, QSizeGrip, QShortcut
)

from config import load_config, save_config, build_launch_args, resource_path
from i18n import tr, load_language
from ui.settings_window import SettingsWindow
from ui.web_view import ZoomableWebView


class MainWindow(QMainWindow):
    # Абсолютные границы, которые должно уметь окно (от 640x480 до 8K)
    MIN_W, MIN_H = 640, 480
    MAX_W, MAX_H = 7680, 4320

    # Базовый дизайн, под который изначально подбирались размеры
    # кнопок/шрифтов (эталон для scale = 1.0)
    BASE_W, BASE_H = 1000, 620

    # Доля экрана, которую окно занимает при первом запуске (10-20%)
    SCREEN_FRACTION = 0.15

    # Границы коэффициента масштабирования элементов интерфейса
    MIN_SCALE, MAX_SCALE = 0.75, 3.5

    # Как часто (мс) сохранять размер окна / зум в конфиг после того, как
    # пользователь перестал тянуть за рамку/грип или крутить колесо
    SAVE_DEBOUNCE_MS = 500

    def __init__(self):
        super().__init__()
        self.cfg = load_config()
        self._current_scale = None

        self.setWindowFlags(Qt.Window | Qt.FramelessWindowHint)

        self._save_timer = QTimer(self)
        self._save_timer.setSingleShot(True)
        self._save_timer.timeout.connect(self._persist_config)

        self._build_ui()
        self._setup_zoom_shortcuts()
        self._load_start_page()
        self._configure_geometry()
        self.retranslate_ui()

    # ------------------------------------------------------------- sizing
    def _configure_geometry(self):
        """
        При старте использует сохранённый размер окна (config.window_width/
        window_height), если он есть и валиден для текущего экрана; иначе
        вычисляет размер как ~10-20% площади экрана. Всегда в границах
        640x480 .. 7680x4320 (8K), окно остаётся свободно растягиваемым.
        """
        self.setMinimumSize(self.MIN_W, self.MIN_H)
        self.setMaximumSize(self.MAX_W, self.MAX_H)

        screen = QApplication.primaryScreen().availableGeometry()

        saved_w = self.cfg.get("window_width")
        saved_h = self.cfg.get("window_height")

        if (
            isinstance(saved_w, int) and isinstance(saved_h, int)
            and self.MIN_W <= saved_w <= self.MAX_W
            and self.MIN_H <= saved_h <= self.MAX_H
        ):
            target_w = min(saved_w, screen.width())
            target_h = min(saved_h, screen.height())
        else:
            target_w = int(screen.width() * self.SCREEN_FRACTION)
            target_w = max(self.MIN_W, min(target_w, self.MAX_W, screen.width()))

            target_h = int(target_w / (self.BASE_W / self.BASE_H))
            target_h = max(self.MIN_H, min(target_h, self.MAX_H, screen.height()))

            # пересчитываем ширину под итоговую высоту, чтобы сохранить пропорции
            target_w = int(target_h * (self.BASE_W / self.BASE_H))
            target_w = max(self.MIN_W, min(target_w, self.MAX_W, screen.width()))

        self.resize(target_w, target_h)

        frame = self.frameGeometry()
        frame.moveCenter(screen.center())
        self.move(frame.topLeft())

        self._apply_scale(self._compute_scale(target_w, target_h))

    def _compute_scale(self, width: int, height: int) -> float:
        scale = min(width / self.BASE_W, height / self.BASE_H)
        return max(self.MIN_SCALE, min(scale, self.MAX_SCALE))

    def resizeEvent(self, event):
        super().resizeEvent(event)
        self._apply_scale(self._compute_scale(self.width(), self.height()))
        # сохраняем размер с задержкой, чтобы не писать файл на каждый пиксель
        self._save_timer.start(self.SAVE_DEBOUNCE_MS)

    def _persist_config(self):
        """Пишет на диск текущий размер окна и зум (оба уже лежат в self.cfg)."""
        self.cfg["window_width"] = self.width()
        self.cfg["window_height"] = self.height()
        save_config(self.cfg)

    def closeEvent(self, event):
        self._persist_config()
        super().closeEvent(event)

    def _apply_scale(self, scale: float):
        """Пересчитывает размеры панелей/кнопок и размер шрифта под текущий scale."""
        if self._current_scale == scale:
            return
        self._current_scale = scale

        def s(v):
            return max(1, round(v * scale))

        self.titlebar_widget.setFixedHeight(s(34))
        self.toolbar_widget.setFixedHeight(s(46))
        self.footer_widget.setFixedHeight(s(64))

        self.title_label.setStyleSheet(f"font-size:{s(11)}px;")
        self.status_label.setStyleSheet(f"font-size:{s(11)}px;")

        self.address_bar.setFixedHeight(s(34))
        self.address_bar.setStyleSheet(f"font-size:{s(12)}px; padding:0 {s(10)}px;")

        for b in (self.btn_back, self.btn_forward):
            b.setFixedSize(s(34), s(34))
            b.setStyleSheet(f"font-size:{s(13)}px;")

        self.btn_settings.setFixedSize(s(34), s(34))
        self.btn_settings.setStyleSheet(f"font-size:{s(16)}px; border-radius:{s(17)}px;")

        self.btn_start.setFixedSize(s(160), s(42))
        self.btn_start.setStyleSheet(f"font-size:{s(14)}px; letter-spacing:{s(3)}px;")

        for b in (self.btn_min, self.btn_close):
            b.setFixedSize(s(28), s(24))
            b.setStyleSheet(f"font-size:{s(12)}px;")

        self.size_grip.setFixedSize(s(16), s(16))

    # ------------------------------------------------------------------ UI
    def _build_ui(self):
        central = QWidget()
        central.setObjectName("Root")
        self.setCentralWidget(central)

        root_layout = QVBoxLayout(central)
        root_layout.setContentsMargins(0, 0, 0, 0)
        root_layout.setSpacing(0)

        root_layout.addWidget(self._build_titlebar())
        root_layout.addWidget(self._build_toolbar())

        # --- Web content area ---
        self.webview = ZoomableWebView()
        self.webview.setObjectName("WebView")
        self.webview.urlChanged.connect(self._on_url_changed)
        self.webview.zoomChanged.connect(self._on_zoom_changed)
        # применяем сохранённый зум сразу (без emit, чтобы не писать файл повторно)
        saved_zoom = ZoomableWebView.clamp(self.cfg.get("zoom_factor", 1.0))
        self.webview.setZoomFactor(saved_zoom)
        root_layout.addWidget(self.webview, stretch=1)

        root_layout.addWidget(self._build_footer())

    def _build_titlebar(self) -> QWidget:
        bar = QWidget()
        bar.setObjectName("TitleBar")
        self.titlebar_widget = bar
        layout = QHBoxLayout(bar)
        layout.setContentsMargins(14, 0, 8, 0)

        self.title_label = QLabel()
        self.title_label.setObjectName("TitleBarLabel")
        layout.addWidget(self.title_label)
        layout.addStretch(1)

        self.btn_min = QPushButton("—")
        self.btn_min.setObjectName("WinButton")
        self.btn_min.clicked.connect(self.showMinimized)

        self.btn_close = QPushButton("✕")
        self.btn_close.setObjectName("WinButtonClose")
        self.btn_close.clicked.connect(self.close)

        for b in (self.btn_min, self.btn_close):
            b.setCursor(Qt.PointingHandCursor)
            layout.addWidget(b)

        # allow dragging the frameless window by the titlebar
        bar.mousePressEvent = self._titlebar_mouse_press
        bar.mouseMoveEvent = self._titlebar_mouse_move
        self._drag_pos = None

        return bar

    def _build_toolbar(self) -> QWidget:
        bar = QWidget()
        bar.setObjectName("ToolBar")
        self.toolbar_widget = bar
        layout = QHBoxLayout(bar)
        layout.setContentsMargins(10, 6, 10, 6)
        layout.setSpacing(8)

        self.btn_back = QPushButton("◀")
        self.btn_forward = QPushButton("▶")
        for b in (self.btn_back, self.btn_forward):
            b.setObjectName("NavButton")
            b.setCursor(Qt.PointingHandCursor)

        self.btn_back.clicked.connect(lambda: self.webview.back())
        self.btn_forward.clicked.connect(lambda: self.webview.forward())

        self.address_bar = QLineEdit()
        self.address_bar.setObjectName("AddressBar")
        self.address_bar.setReadOnly(True)
        self.address_bar.setText(self.cfg.get("display_url", "https://address.com"))

        self.btn_settings = QPushButton("⚙")
        self.btn_settings.setObjectName("SettingsButton")
        self.btn_settings.setCursor(Qt.PointingHandCursor)
        self.btn_settings.clicked.connect(self._open_settings)

        layout.addWidget(self.btn_back)
        layout.addWidget(self.btn_forward)
        layout.addWidget(self.address_bar, stretch=1)
        layout.addWidget(self.btn_settings)

        return bar

    def _build_footer(self) -> QWidget:
        bar = QWidget()
        bar.setObjectName("Footer")
        self.footer_widget = bar
        layout = QHBoxLayout(bar)
        layout.setContentsMargins(16, 10, 16, 10)

        self.status_label = QLabel()
        self.status_label.setObjectName("StatusLabel")
        layout.addWidget(self.status_label)
        layout.addStretch(1)

        self.btn_start = QPushButton()
        self.btn_start.setObjectName("StartButton")
        self.btn_start.setCursor(Qt.PointingHandCursor)
        self.btn_start.clicked.connect(self._on_start_clicked)
        layout.addWidget(self.btn_start)

        # Ручка для изменения размера окна (нужна, т.к. окно без рамки ОС)
        self.size_grip = QSizeGrip(self)
        self.size_grip.setObjectName("SizeGrip")
        layout.addWidget(self.size_grip, 0, Qt.AlignBottom | Qt.AlignRight)

        return bar

    # ---------------------------------------------------------- i18n / текст
    def retranslate_ui(self):
        """Обновляет весь видимый текст под текущий язык лаунчера (i18n)."""
        self.setWindowTitle(tr("window_title"))
        self.title_label.setText(tr("title_bar"))
        self.btn_settings.setToolTip(tr("settings_tooltip"))
        self.btn_back.setToolTip(tr("nav_back_tooltip"))
        self.btn_forward.setToolTip(tr("nav_forward_tooltip"))
        self.btn_start.setText(tr("start_button"))
        self.status_label.setText(self._status_text())

    def _status_text(self) -> str:
        mode = tr("mode_windowed") if self.cfg.get("windowed") else tr("mode_fullscreen")
        lang_key = "lang_name_ua" if self.cfg.get("language", "eng") == "ua" else "lang_name_eng"
        lang = tr(lang_key)
        zoom_pct = int(round(self.cfg.get("zoom_factor", 1.0) * 100))
        return (
            f"{tr('status_mode_label')}: {mode}   |   "
            f"{tr('status_language_label')}: {lang}   |   "
            f"{tr('status_zoom_label')}: {zoom_pct}%"
        )

    # --------------------------------------------------------------- logic
    def _load_start_page(self):
        index_path = resource_path(os.path.join("resources", "index.html"))
        if os.path.exists(index_path):
            self.webview.load(QUrl.fromLocalFile(index_path))
        else:
            self.webview.setHtml(
                "<body style='background:#0a0a0a;color:#c0392b;"
                "font-family:Consolas,monospace;display:flex;"
                "align-items:center;justify-content:center;height:100vh;'>"
                f"<h2>{tr('err_index_not_found')}</h2></body>"
            )

    def _on_url_changed(self, url: QUrl):
        # Показываем "красивый" адрес, а не file:// путь к локальному index.html
        if url.isLocalFile():
            self.address_bar.setText(self.cfg.get("display_url", "https://address.com"))
        else:
            self.address_bar.setText(url.toString())

    def _on_zoom_changed(self, factor: float):
        """Ctrl+колесо или Ctrl+/Ctrl- — обновляем конфиг, статус-бар и сохраняем с задержкой."""
        self.cfg["zoom_factor"] = round(factor, 2)
        self.status_label.setText(self._status_text())
        self._save_timer.start(self.SAVE_DEBOUNCE_MS)

    def _setup_zoom_shortcuts(self):
        """
        Ctrl+ / Ctrl- / Ctrl+0 — зум клавиатурой (дублирует Ctrl+колесо).

        ВАЖНО: не добавляем одновременно QKeySequence.ZoomIn/ZoomOut и
        свои явные строки — на Windows они совпадают буква-в-букву
        ("Ctrl+-"), а два QShortcut с идентичной комбинацией в одном
        окне дают "неоднозначный" шорткат, и Qt не вызывает НИ ОДИН
        обработчик. Поэтому дедуплицируем по итоговой строке комбинации.
        """
        def register(sequences, slot):
            for seq in sequences:
                if seq.isEmpty():
                    continue
                key_text = seq.toString()
                if key_text in seen:
                    continue
                seen.add(key_text)
                sc = QShortcut(seq, self)
                sc.activated.connect(slot)
                self._zoom_shortcuts.append(sc)

        self._zoom_shortcuts = []
        seen = set()

        register(
            [QKeySequence(QKeySequence.ZoomIn), QKeySequence("Ctrl++"), QKeySequence("Ctrl+=")],
            self.webview.zoom_in,
        )
        register(
            [QKeySequence(QKeySequence.ZoomOut), QKeySequence("Ctrl+-")],
            self.webview.zoom_out,
        )
        register(
            [QKeySequence("Ctrl+0")],
            self.webview.zoom_reset,
        )

    def _open_settings(self):
        dlg = SettingsWindow(self, scale=self._current_scale or 1.0)
        if dlg.exec_() == SettingsWindow.Accepted:
            self.cfg = load_config()
            # язык лаунчера мог измениться -> перезагружаем словарь и весь текст
            load_language(self.cfg.get("language", "eng"))
            self.retranslate_ui()

    def _on_start_clicked(self):
        if sys.platform != "win32":
            QMessageBox.warning(
                self, tr("err_unsupported_platform_title"),
                tr("err_unsupported_platform_text")
            )
            return

        engine_path = self.cfg.get("engine_path", "")
        working_dir = self.cfg.get("working_directory", "")

        if not os.path.exists(engine_path):
            QMessageBox.critical(
                self, tr("err_engine_not_found_title"),
                tr("err_engine_not_found_text", path=engine_path)
            )
            return

        args = build_launch_args(self.cfg)
        cmd = [engine_path] + args

        try:
            subprocess.Popen(cmd, cwd=working_dir)
        except OSError as e:
            QMessageBox.critical(
                self, tr("err_failed_start_title"),
                tr("err_failed_start_text", error=e)
            )
            return

        self.close()

    # -------------------------------------------------- frameless dragging
    def _titlebar_mouse_press(self, event):
        if event.button() == Qt.LeftButton:
            self._drag_pos = event.globalPos() - self.frameGeometry().topLeft()
            event.accept()

    def _titlebar_mouse_move(self, event):
        if event.buttons() == Qt.LeftButton and self._drag_pos is not None:
            self.move(event.globalPos() - self._drag_pos)
            event.accept()
            