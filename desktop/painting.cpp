#include "painting.hpp"

#include <QFontDatabase>
#include <QImage>
#include <QPainter>
#include <QPainterPath>
#include <array>
#include <optional>

namespace {
struct Surfaces {
    std::array<QImage, 4> images;
    QPainter painter;
    std::optional<QGradient> gradient;

    Surfaces() {
        for (auto& image : images) {
            image = QImage(1200, 500, QImage::Format_ARGB32_Premultiplied);
            image.fill(Qt::transparent);
        }
    }
};

Surfaces& surfaces() {
    static Surfaces layers;
    return layers;
}

// The scene uses CSS #RRGGBB, #RRGGBBAA and rgba(r,g,b,a).
QColor color(const char* css) {
    const QByteArray value(css);
    if (value.startsWith("rgba(")) {
        const auto fields = value.mid(5, value.size() - 6).split(',');
        return QColor(fields[0].toInt(), fields[1].toInt(), fields[2].toInt(),
                      qRound(fields[3].toDouble() * 255));
    }
    QColor result(QString::fromLatin1(value.first(7)));
    if (value.size() == 9) result.setAlpha(value.last(2).toInt(nullptr, 16));
    return result;
}

QBrush brush(const char* css) {
    return css ? QBrush(color(css)) : QBrush(*surfaces().gradient);
}

void setPen(const char* css, double width) {
    auto& painter = surfaces().painter;
    auto pen = painter.pen();
    pen.setStyle(Qt::SolidLine);
    pen.setColor(color(css));
    pen.setWidthF(width);
    painter.setPen(pen);
}
} // namespace

const QImage& desktop::image(canvas::Layer layer) {
    return surfaces().images[static_cast<int>(layer)];
}

void desktop::finishPainting() {
    auto& painter = surfaces().painter;
    if (painter.isActive()) painter.end();
}

namespace canvas {
void selectLayer(Layer layer) {
    desktop::finishPainting();
    auto& target = surfaces();
    auto& painter = target.painter;
    painter.begin(&target.images[static_cast<int>(layer)]);
    painter.setRenderHint(QPainter::Antialiasing);
    painter.setPen(QPen(Qt::black, 1, Qt::SolidLine, Qt::FlatCap, Qt::MiterJoin));
}

void drawLayer(Layer layer) { surfaces().painter.drawImage(0, 0, desktop::image(layer)); }
void save() { surfaces().painter.save(); }
void restore() { surfaces().painter.restore(); }
void opacity(double alpha) { surfaces().painter.setOpacity(alpha); }
void transform(double a, double b, double c, double d, double x, double y) {
    surfaces().painter.setTransform(QTransform(a, b, c, d, x, y), true);
}

void roundedLines() {
    auto& painter = surfaces().painter;
    auto pen = painter.pen();
    pen.setCapStyle(Qt::RoundCap);
    pen.setJoinStyle(Qt::RoundJoin);
    painter.setPen(pen);
}

void line(double x1, double y1, double x2, double y2, const char* css, double width) {
    setPen(css, width);
    surfaces().painter.drawLine(QLineF(x1, y1, x2, y2));
}

void rect(double x, double y, double w, double h, const char* css) {
    surfaces().painter.fillRect(QRectF(x, y, w, h), brush(css));
}

void ellipse(double x, double y, double rx, double ry, const char* css) {
    QPainterPath shape;
    shape.addEllipse(QPointF(x, y), rx, ry);
    surfaces().painter.fillPath(shape, brush(css));
}

void path(const Point* points, unsigned count, const char* css, double width) {
    QPainterPath shape(QPointF(points[0].x, points[0].y));
    for (unsigned i = 1; i < count; ++i) shape.lineTo(points[i].x, points[i].y);
    auto& painter = surfaces().painter;
    if (width > 0) {
        setPen(css, width);
        painter.strokePath(shape, painter.pen());
    } else {
        shape.closeSubpath();
        painter.fillPath(shape, brush(css));
    }
}

void text(const char* value, double x, double y, const char* font, const char* css) {
    auto& painter = surfaces().painter;
    const QString spec = QString::fromLatin1(font);
    const int end = spec.indexOf("px");
    QFont face = QFontDatabase::systemFont(QFontDatabase::FixedFont);
    face.setPixelSize(spec.left(end).section(' ', -1).toInt());
    face.setBold(spec.startsWith("bold "));
    painter.save();
    painter.setFont(face);
    painter.setPen(color(css));
    painter.drawText(QPointF(x, y), QString::fromUtf8(value));
    painter.restore();
}

void linearGradient(double x1, double y1, double x2, double y2) {
    surfaces().gradient = QLinearGradient(x1, y1, x2, y2);
}
void radialGradient(double x, double y, double r1, double r2) {
    surfaces().gradient = QRadialGradient(QPointF(x, y), r2, QPointF(x, y), r1);
}
void colorStop(double offset, const char* css) { surfaces().gradient->setColorAt(offset, color(css)); }
} // namespace canvas
