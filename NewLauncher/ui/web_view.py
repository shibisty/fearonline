# -*- coding: utf-8 -*-
"""
web_view.py
QWebEngineView с ручным управлением зумом:
 - Ctrl + колесо мыши — зум страницы (перехватываем сами, чтобы не было
   двойного зума поверх встроенного в Chromium поведения, и чтобы можно
   было поймать факт изменения и сохранить его в конфиг).
 - zoom_in() / zoom_out() / zoom_reset() — используются также для
   хоткеев Ctrl+ / Ctrl- / Ctrl+0 в главном окне.
 - zoomChanged(float) — сигнал, по которому MainWindow сохраняет
   текущий уровень зума в конфиг.
"""

# -*- coding: utf-8 -*-
"""
web_view.py
QWebEngineView с ручным управлением зумом:
 - Ctrl + колесо мыши — зум страницы.
 - zoom_in() / zoom_out() / zoom_reset() — используются также для
   хоткеев Ctrl+ / Ctrl- / Ctrl+0 в главном окне.
 - zoomChanged(float) — сигнал, по которому MainWindow сохраняет
   текущий уровень зума в конфиг.

ВАЖНО про колесо мыши: события колеса приходят не в сам QWebEngineView,
а во внутренний дочерний виджет (RenderWidgetHostView), который отдаёт
focusProxy(). Поэтому просто переопределить wheelEvent() на QWebEngineView
недостаточно — событие туда чаще всего не долетает, а зум в этом случае
делает сам Chromium "под капотом", без нашего сигнала (и, соответственно,
без сохранения). Ставим eventFilter на focusProxy() и сами гасим событие
(return True), чтобы Chromium не зумил параллельно с нами.
"""

from PyQt5.QtCore import Qt, QEvent, pyqtSignal
from PyQt5.QtWebEngineWidgets import QWebEngineView


class ZoomableWebView(QWebEngineView):
    zoomChanged = pyqtSignal(float)

    ZOOM_MIN = 0.25
    ZOOM_MAX = 5.0
    ZOOM_STEP = 0.1
    ZOOM_DEFAULT = 1.0

    def __init__(self, parent=None):
        super().__init__(parent)
        self._install_wheel_filter()
        # focusProxy() пересоздаётся при навигации/загрузке страницы —
        # переустанавливаем фильтр каждый раз, чтобы не потерять перехват
        self.loadFinished.connect(lambda ok: self._install_wheel_filter())

    def _install_wheel_filter(self):
        proxy = self.focusProxy()
        if proxy is not None:
            proxy.installEventFilter(self)

    def showEvent(self, event):
        super().showEvent(event)
        self._install_wheel_filter()

    def eventFilter(self, obj, event):
        if event.type() == QEvent.Wheel and event.modifiers() & Qt.ControlModifier:
            delta = event.angleDelta().y()
            if delta > 0:
                self.zoom_in()
            elif delta < 0:
                self.zoom_out()
            return True  # гасим событие, чтобы Chromium не зумил параллельно
        return super().eventFilter(obj, event)

    @classmethod
    def clamp(cls, value: float) -> float:
        return round(max(cls.ZOOM_MIN, min(cls.ZOOM_MAX, value)), 2)

    def set_zoom(self, factor: float, emit: bool = True):
        factor = self.clamp(factor)
        if abs(self.zoomFactor() - factor) > 0.001:
            self.setZoomFactor(factor)
            if emit:
                self.zoomChanged.emit(factor)

    def zoom_in(self):
        self.set_zoom(self.zoomFactor() + self.ZOOM_STEP)

    def zoom_out(self):
        self.set_zoom(self.zoomFactor() - self.ZOOM_STEP)

    def zoom_reset(self):
        self.set_zoom(self.ZOOM_DEFAULT)

    def wheelEvent(self, event):
        # Оставлено как страховка (например, если колесо крутят не над
        # содержимым страницы, а над самим виджетом контейнера).
        if event.modifiers() & Qt.ControlModifier:
            delta = event.angleDelta().y()
            if delta > 0:
                self.zoom_in()
            elif delta < 0:
                self.zoom_out()
            event.accept()
            return
        super().wheelEvent(event)
        