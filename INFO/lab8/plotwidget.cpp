#include "plotwidget.h"

#include <QPainter>
#include <QPainterPath>
#include <QtMath>
#include <algorithm>

static QPointF worldToScreen(const QPointF& w, const QRect& r, double scale)
{
    const double cx = r.center().x();
    const double cy = r.center().y();
    return QPointF(cx + w.x() * scale, cy - w.y() * scale);
}

static double niceTickStep(double worldPerPixel)
{
    // Хотим ~50px между метками
    double desired = worldPerPixel * 50.0;
    if (desired <= 0) return 1.0;

    double pow10 = std::pow(10.0, std::floor(std::log10(desired)));
    double n = desired / pow10;

    double step;
    if (n < 1.5) step = 1.0;
    else if (n < 3.0) step = 2.0;
    else if (n < 7.0) step = 5.0;
    else step = 10.0;

    return step * pow10;
}

static void drawAxes(QPainter& p, const QRect& r, double scale)
{
    const QPointF c = r.center();

    // оси
    p.setPen(QPen(Qt::black, 2));
    p.drawLine(QPointF(r.left(), c.y()), QPointF(r.right(), c.y()));
    p.drawLine(QPointF(c.x(), r.top()), QPointF(c.x(), r.bottom()));

    // стрелки
    const double arr = 10;
    p.drawLine(QPointF(r.right()-arr, c.y()-arr/2), QPointF(r.right(), c.y()));
    p.drawLine(QPointF(r.right()-arr, c.y()+arr/2), QPointF(r.right(), c.y()));
    p.drawLine(QPointF(c.x()-arr/2, r.top()+arr), QPointF(c.x(), r.top()));
    p.drawLine(QPointF(c.x()+arr/2, r.top()+arr), QPointF(c.x(), r.top()));

    // подписи
    p.setFont(QFont("Arial", 10));
    p.drawText(QPointF(r.right()-20, c.y()-6), "X");
    p.drawText(QPointF(c.x()+6, r.top()+16), "Y");

    const double worldPerPixel = 1.0 / scale;
    const double stepW = niceTickStep(worldPerPixel);

    p.setPen(QPen(Qt::black, 1));

    // X ticks
    int kMaxX = static_cast<int>((r.width()/2.0 * worldPerPixel) / stepW);
    for (int k = 1; k <= kMaxX; ++k) {
        double x = k * stepW;
        QPointF s1 = worldToScreen(QPointF(+x, 0), r, scale);
        QPointF s2 = worldToScreen(QPointF(-x, 0), r, scale);

        p.drawLine(QPointF(s1.x(), c.y()-4), QPointF(s1.x(), c.y()+4));
        p.drawLine(QPointF(s2.x(), c.y()-4), QPointF(s2.x(), c.y()+4));

        p.drawText(QPointF(s1.x()+2, c.y()+16), QString::number(x));
        p.drawText(QPointF(s2.x()+2, c.y()+16), QString::number(-x));
    }

    // Y ticks
    int kMaxY = static_cast<int>((r.height()/2.0 * worldPerPixel) / stepW);
    for (int k = 1; k <= kMaxY; ++k) {
        double y = k * stepW;
        QPointF s1 = worldToScreen(QPointF(0, +y), r, scale);
        QPointF s2 = worldToScreen(QPointF(0, -y), r, scale);

        p.drawLine(QPointF(c.x()-4, s1.y()), QPointF(c.x()+4, s1.y()));
        p.drawLine(QPointF(c.x()-4, s2.y()), QPointF(c.x()+4, s2.y()));

        p.drawText(QPointF(c.x()+8, s1.y()-2), QString::number(y));
        p.drawText(QPointF(c.x()+8, s2.y()-2), QString::number(-y));
    }
}


// №3: роза 3 лепестка: r = a sin(3φ), φ∈[0,2π]
static QVector<QPointF> curve3(double a)
{
    QVector<QPointF> pts;
    const int N = 2400;
    pts.reserve(N+1);

    for (int i = 0; i <= N; ++i) {
        double phi = (2.0 * M_PI) * i / N;
        double r = a * std::sin(3.0 * phi);
        double x = r * std::cos(phi);
        double y = r * std::sin(phi);
        pts.push_back(QPointF(x, y));
    }
    return pts;
}

// №6: улитка Паскаля: x = 2a cos²t + b cos t, y = 2a sin t cos t + b sin t
static QVector<QPointF> curve6(double a, double b)
{
    QVector<QPointF> pts;
    const int N = 2400;
    pts.reserve(N+1);

    for (int i = 0; i <= N; ++i) {
        double t = (2.0 * M_PI) * i / N;
        double ct = std::cos(t), st = std::sin(t);
        double x = 2.0*a*ct*ct + b*ct;
        double y = 2.0*a*st*ct + b*st;
        pts.push_back(QPointF(x, y));
    }
    return pts;
}

// №9: декартов лист: x = 3a t/(1+t^3), y = 3a t^2/(1+t^3), t∈R (ограничим для рисования)
static QVector<QPointF> curve9(double a)
{
    QVector<QPointF> pts;
    const int N = 3000;
    pts.reserve(N+1);

    double T = 6.0;
    for (int i = 0; i <= N; ++i) {
        double t = -T + (2*T) * i / N;
        double denom = 1.0 + std::pow(t, 3);
        if (std::fabs(denom) < 1e-3) continue; // около t=-1 разрыв
        double x = 3.0 * a * t / denom;
        double y = 3.0 * a * t * t / denom;
        pts.push_back(QPointF(x, y));
    }
    return pts;
}

PlotWidget::PlotWidget(QWidget *parent)
    : QWidget(parent)
{
    setAutoFillBackground(true);
    QPalette pal = palette();
    pal.setColor(QPalette::Window, Qt::white);
    setPalette(pal);
}

void PlotWidget::setState(const State &s)
{
    st = s;
    update();
}

void PlotWidget::paintEvent(QPaintEvent * /*event*/)
{
    const double WORLD_LIMIT = 120.0;

    QPainter p(this);
    p.setRenderHint(QPainter::Antialiasing, true);

    const QRect r = rect().adjusted(20, 20, -20, -20);

    // Собираем точки (только нужные 3 кривые)
    QVector<QPointF> p3 = st.show1 ? curve3(st.a)       : QVector<QPointF>{};
    QVector<QPointF> p6 = st.show2 ? curve6(st.a, st.b) : QVector<QPointF>{};
    QVector<QPointF> p9 = st.show3 ? curve9(st.a)       : QVector<QPointF>{};

    // Подбор масштаба по максимумам
    double maxAbsX = 1.0, maxAbsY = 1.0;
    auto scan = [&](const QVector<QPointF>& pts){
        for (const auto& w : pts) {
            double ax = std::fabs(w.x());
            double ay = std::fabs(w.y());
            if (ax > WORLD_LIMIT || ay > WORLD_LIMIT)
                continue;   // <-- ключевое
            maxAbsX = std::max(maxAbsX, ax);
            maxAbsY = std::max(maxAbsY, ay);
        }
    };

    scan(p3); scan(p6); scan(p9);

    const double marginPx = 30.0;
    double sx = (r.width()/2.0  - marginPx) / maxAbsX;
    double sy = (r.height()/2.0 - marginPx) / maxAbsY;
    double scale = std::max(1e-6, std::min(sx, sy));

    // Оси
    drawAxes(p, r, scale);

    // Функция отрисовки ломаной
    auto drawCurve = [&](const QVector<QPointF>& pts, const QColor& col){
        QPainterPath path;
        bool started = false;

        for (const auto& w : pts) {
            double ax = std::fabs(w.x());
            double ay = std::fabs(w.y());

            if (ax > WORLD_LIMIT || ay > WORLD_LIMIT) {
                started = false;   // разрыв линии
                continue;
            }

            QPointF s = worldToScreen(w, r, scale);
            if (!started) {
                path.moveTo(s);
                started = true;
            } else {
                path.lineTo(s);
            }
        }

        if (!path.isEmpty()) {
            p.setPen(QPen(col, 2));
            p.drawPath(path);
        }
    };


    drawCurve(p3, st.color1);
    drawCurve(p6, st.color2);
    drawCurve(p9, st.color3);

    p.setPen(QPen(Qt::black, 1));
    p.setFont(QFont("Arial", 9));

    int y = r.top() + 20;

    auto line = [&](const QString& text){
        p.drawText(r.left() + 10, y, text);
        y += 16;
    };

    if (st.show1) line("График 1: №3 (роза 3 лепестка)");
    if (st.show2) line("График 2: №6 (улитка Паскаля)");
    if (st.show3) line("График 3: №9 (декартов лист)");
}
