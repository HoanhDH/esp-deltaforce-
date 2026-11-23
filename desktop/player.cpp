#include "painting.hpp"
#include "replay.hpp"

#include <QApplication>
#include <QButtonGroup>
#include <QElapsedTimer>
#include <QHBoxLayout>
#include <QImage>
#include <QLabel>
#include <QPainter>
#include <QPushButton>
#include <QShortcut>
#include <QSignalBlocker>
#include <QSlider>
#include <QTimer>
#include <QVBoxLayout>

namespace {
using replay::View;
using canvas::Layer;

class Preview final : public QWidget {
public:
    Preview() {
        setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
        setMinimumHeight(190);
        setAccessibleName("Cảnh mô phỏng tại bến cảng");
    }

    void showFrame(View selected) {
        view = selected;
        update();
    }

protected:
    void paintEvent(QPaintEvent*) override {
        QPainter painter(this);
        painter.fillRect(rect(), QColor("#101613"));
        painter.setRenderHint(QPainter::SmoothPixmapTransform);
        const QRectF area(rect());
        if (view != View::Compare) {
            drawPanel(painter, area, Layer::Screen, view);
            return;
        }
        constexpr int gap = 4;
        // Pick the arrangement that gives each complete frame the most space.
        if (width() >= height() * 2.4) {
            const double w = (width() - gap) / 2.0;
            drawPanel(painter, QRectF(0, 0, w, height()), Layer::Screen, View::Tung);
            drawPanel(painter, QRectF(w + gap, 0, w, height()), Layer::Comparison, View::Muc);
        } else {
            const double h = (height() - gap) / 2.0;
            drawPanel(painter, QRectF(0, 0, width(), h), Layer::Screen, View::Tung);
            drawPanel(painter, QRectF(0, h + gap, width(), h), Layer::Comparison, View::Muc);
        }
    }

private:
    View view = View::Tung;

    static void drawPanel(QPainter& painter, QRectF area, Layer layer, View selected) {
        const auto& source = desktop::image(layer);
        QSizeF size = source.size();
        size.scale(area.size(), Qt::KeepAspectRatio);
        QRectF target(QPointF(), size);
        target.moveCenter(area.center());
        painter.drawImage(target, source);

        painter.save();
        QFont font = painter.font();
        font.setPixelSize(13);
        painter.setFont(font);
        const QString label = selected == View::Muc ? "Mực · Góc ESP" : "Tùng · Góc thường";
        const QRectF badge(target.left() + 10, target.top() + 10,
                           painter.fontMetrics().horizontalAdvance(label) + 18, 25);
        painter.fillRect(badge, QColor("#182524"));
        painter.setPen(selected == View::Muc ? QColor("#d5f396") : QColor("#edf1e5"));
        painter.drawText(badge, Qt::AlignCenter, label);
        painter.restore();
    }
};

class Player final : public QWidget {
public:
    Player() {
        setWindowTitle("Khoảng khuất");
        resize(1100, 640);
        auto* layout = new QVBoxLayout(this);
        layout->setContentsMargins(22, 18, 22, 18);
        layout->setSpacing(12);

        auto* header = new QHBoxLayout;
        auto* titles = new QVBoxLayout;
        auto* title = new QLabel("Khoảng khuất");
        title->setObjectName("title");
        titles->addWidget(title);
        titles->addWidget(new QLabel("Bến cảng · Mô phỏng ngoại tuyến"));
        header->addLayout(titles);
        header->addStretch();
        auto* views = new QButtonGroup(this);
        const QStringList names = {"Tùng", "Mực", "Đối chiếu"};
        for (int i = 0; i < names.size(); ++i) {
            auto* button = new QPushButton(names[i]);
            button->setCheckable(true);
            views->addButton(button, i);
            header->addWidget(button);
            shortcut(QKeySequence(QString::number(i + 1)), [button] { button->click(); });
        }
        views->button(0)->setChecked(true);
        connect(views, &QButtonGroup::idClicked, this, [](int id) {
            replay::setView(static_cast<View>(id));
        });
        layout->addLayout(header);
        layout->addWidget(&preview, 1);

        auto* transport = new QHBoxLayout;
        transport->addWidget(&play);
        auto* reset = new QPushButton("Về đầu");
        transport->addWidget(reset);
        transport->addWidget(&seek, 1);
        transport->addWidget(&clock);
        layout->addLayout(transport);

        auto* inspection = new QHBoxLayout;
        inspection->addWidget(&previous);
        inspection->addWidget(&next);
        inspection->addWidget(&counter);
        inspection->addStretch();
        layout->addLayout(inspection);
        seek.setAccessibleName("Vị trí trong cảnh");
        play.setObjectName("play");
        play.setToolTip("Phát hoặc tạm dừng (Space)");
        previous.setToolTip("Lùi một khung hình (,)");
        next.setToolTip("Tiến một khung hình (.)");

        connect(&play, &QPushButton::clicked, this, [this] { replay::toggle(now()); });
        connect(reset, &QPushButton::clicked, this, [] { replay::reset(); });
        connect(&previous, &QPushButton::clicked, this, [] { replay::stepFrame(-1); });
        connect(&next, &QPushButton::clicked, this, [] { replay::stepFrame(1); });
        shortcut(QKeySequence(Qt::Key_Space), [this] { play.click(); });
        shortcut(QKeySequence(Qt::Key_Comma), [this] { previous.click(); });
        shortcut(QKeySequence(Qt::Key_Period), [this] { next.click(); });
        connect(&seek, &QSlider::valueChanged, this, [this](int frame) {
            replay::seekTo(static_cast<double>(frame) / fps, now());
        });
        timer.setTimerType(Qt::PreciseTimer);
        timer.setInterval(16);
        connect(&timer, &QTimer::timeout, this, [this] { replay::tick(now()); });
        time.start();
    }

    void showState(int frame, int last, int rate, bool playing, View view) {
        fps = rate;
        const QSignalBlocker block(&seek);
        seek.setRange(0, last);
        seek.setValue(frame);
        previous.setEnabled(frame > 0);
        next.setEnabled(frame < last);
        play.setText(playing ? "Tạm dừng" : "Phát");
        counter.setText(QString("Khung %1 / %2 · %3 khung/giây").arg(frame).arg(last).arg(rate));
        clock.setText(QString("%1 / %2 s").arg(frame / double(rate), 0, 'f', 2)
                      .arg(last / double(rate), 0, 'f', 2));
        preview.showFrame(view);
        if (playing && !timer.isActive()) timer.start();
        if (!playing && timer.isActive()) timer.stop();
    }

private:
    template<class Callback> void shortcut(const QKeySequence& keys, Callback action) {
        auto* binding = new QShortcut(keys, this);
        connect(binding, &QShortcut::activated, this, action);
    }
    double now() const { return time.nsecsElapsed() / 1e9; }

    Preview preview;
    QPushButton play, previous{"Lùi khung"}, next{"Tiến khung"};
    QSlider seek{Qt::Horizontal};
    QLabel clock, counter;
    QElapsedTimer time;
    QTimer timer;
    int fps = 30;
};

Player* player;
} // namespace

void replay::showState(int frame, int last, int fps, bool playing, View view) {
    desktop::finishPainting();
    player->showState(frame, last, fps, playing, view);
}

int main(int argc, char** argv) {
    QApplication app(argc, argv);
    app.setApplicationName("Khoảng khuất");
    app.setStyle("Fusion");
    app.setStyleSheet(R"(
        QWidget { background: #171d1a; color: #edf1e7; font-size: 14px; }
        QLabel#title { font-size: 29px; font-weight: bold; }
        QPushButton { background: #26312a; padding: 7px 13px; border: 1px solid #506053; border-radius: 5px; }
        QPushButton:hover { background: #354239; }
        QPushButton:focus { border-color: #b8d892; }
        QPushButton:checked, QPushButton#play { background: #d5f396; color: #182314; }
        QPushButton:disabled { color: #899187; background: #252b27; }
        QSlider::groove:horizontal { height: 4px; background: #475047; }
        QSlider::sub-page:horizontal { background: #b7d882; }
        QSlider::handle:horizontal { width: 14px; margin: -5px 0; border-radius: 7px; background: #d5f396; }
    )");
    Player window;
    player = &window;
    replay::init();
    window.show();
    return app.exec();
}
