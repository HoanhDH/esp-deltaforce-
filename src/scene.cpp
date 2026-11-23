#include "canvas.hpp"

namespace {
using namespace canvas;
constexpr int width = 1200;
constexpr double duration = 16;
constexpr const char* highlight = "#d9ff97";

extern "C" __attribute__((import_module("host"), import_name("sin"))) double sine(double angle);
extern "C" __attribute__((import_module("host"), import_name("showState")))
void showState(double seconds, double total, bool playing, bool esp);

class Random {
public:
    explicit Random(unsigned seed) : state(seed) {}
    double next() {
        state = state * 1664525u + 1013904223u;
        return state / 4294967296.0;
    }
private:
    unsigned state;
};

void crane(double x, double y, double size, const char* color) {
    save();
    transform(size, 0, 0, size, x, y);
    auto strut = [color](double x1, double y1, double x2, double y2, double weight) {
        line(x1, y1, x2, y2, color, weight);
    };
    strut(-22, 0, -7, -106, 5);
    strut(21, 0, 6, -106, 5);
    for (int i = 0; i < 5; ++i) {
        const double spread = 19 - i * 2.5;
        strut(-spread, -i * 19, spread - 2, -(i + 1) * 19, 2);
        strut(spread, -i * 19, -spread + 2, -(i + 1) * 19, 2);
    }
    strut(-40, -108, 99, -146, 4);
    strut(-39, -118, 99, -149, 3);
    for (int i = 0; i < 12; ++i)
        strut(-39 + i * 11.5, -118 - i * 2.55, -28 + i * 11.5, -111 - i * 3.1, 1.6);
    strut(-29, -118, 0, -153, 2);
    strut(0, -153, 88, -146, 1.5);
    strut(0, -153, 0, -119, 3);
    rect(-15, -117, 23, 12, color);
    strut(78, -140, 78, -79, 1);
    strut(80, -141, 80, -79, 1);
    rect(75, -80, 8, 3, color);
    restore();
}

void paintHarbor() {
    Random random(45);
    linearGradient(0, 0, 0, 330);
    colorStop(0, "#48616c");
    colorStop(.46, "#91a39b");
    colorStop(.82, "#dfba92");
    colorStop(1, "#e4c29b");
    rect(0, 0, width, 335, nullptr);
    radialGradient(866, 139, 3, 235);
    colorStop(0, "#ffdcaaa8");
    colorStop(1, "#f9daaa00");
    rect(630, 0, 480, 380, nullptr);
    ellipse(866, 139, 35, 35, "#f6d9a6");
    for (int i = 0; i < 65; ++i) {
        const double x = random.next() * 1240;
        const double y = random.next() * 230;
        const double rx = 8 + random.next() * 100;
        ellipse(x, y, rx, .5 + random.next() * 3.5, "rgba(206,219,200,.08)");
    }
    polygon({{0,215}, {92,201}, {152,208}, {250,183}, {309,192}, {410,168}, {437,173},
             {492,164}, {568,188}, {630,180}, {708,205}, {806,178}, {889,199}, {978,175},
             {1043,186}, {1138,166}, {1200,180}, {1200,280}, {0,280}}, "#9da99b");
    for (double x = -24; x < width; x += 18 + random.next() * 16) {
        const double y = 205 + random.next() * 33;
        rect(x, y, 24 + random.next() * 50, 262 - y, "#87978c");
        line(x + 7, y, x + 7, y - random.next() * 16, "#82958a");
    }
    crane(722, 249, .78, "#7b8b80");
    crane(905, 249, .93, "#737f75");
    crane(1075, 250, .70, "#7e8e83");
    rect(0, 246, width, 65, "#96a697");
    for (int i = 0; i < 330; ++i) {
        const double x = random.next() * width;
        const double y = 248 + random.next() * 57;
        const double length = 2 + random.next() * 45;
        rect(x, y, length, .5 + random.next(), "rgba(226,213,170,.29)");
    }
    for (int x = 0; x < width; x += 70) {
        rect(x, 281, 4, 31, "#566a62");
        line(x, 284, x + 70, 284, "#63776d");
        line(x, 295, x + 70, 295, "#6e7e71");
    }
    polygon({{0,313}, {1200,292}, {1200,500}, {0,500}}, "#56625b");
    linearGradient(0, 300, 0, 500);
    colorStop(0, "#808779");
    colorStop(.47, "#52635c");
    colorStop(1, "#293a37");
    polygon({{0,320}, {1200,301}, {1200,500}, {0,500}}, nullptr);
    for (int i = 0; i < 850; ++i) {
        const double x = random.next() * width;
        const double y = 307 + random.next() * 190;
        const double length = random.next() * 8 + .5;
        rect(x, y, length, random.next() * 1.2 + .25, "rgba(209,212,181,.17)");
    }
    for (int i = 0; i < 7; ++i)
        line(150 + i * 210, 306, -460 + i * 310, 500, "#40514d");
    polygon({{1200,355}, {1200,361}, {0,444}, {0,432}}, "#ada680");
    polygon({{0,151}, {186,134}, {285,179}, {285,324}, {0,344}}, "#475f5c");
    polygon({{186,134}, {285,179}, {285,324}, {186,311}}, "#344c4a");
    polygon({{0,147}, {186,130}, {289,177}, {284,183}, {185,139}, {0,156}}, "#596e66");
    for (int x = 12; x < 185; x += 12)
        line(x, 152 - x * .09, x, 321 - x * .09, "#405653", 2);
    for (int x = 15; x < 170; x += 34) {
        rect(x, 175 - x * .08, 23, 21, "#293e3d");
        line(x + 11, 175 - x * .08, x + 11, 196 - x * .08, "#73837a");
    }
    for (int i = 0; i < 5; ++i) {
        const int x = 1008 + i * 36;
        rect(x, 280, 29, 14, "#736f59");
        rect(x, 280, 29, 2, "#aaa487");
    }
    constexpr int lamps[] = {303, 988};
    for (int x : lamps) {
        line(x, 307, x, 207, "#445952", 3);
        line(x, 208, x + 12, 202, "#445952", 2.5);
        rect(x + 10, 201, 10, 4, "#5d6b5e");
        rect(x + 11, 205, 8, 2, "#f2d7a5");
    }
    constexpr Point birds[] = {{580,88}, {595,82}, {614,90}};
    for (auto [x, y] : birds) {
        line(x - 4, y - 1, x, y + 1, "#647a76");
        line(x, y + 1, x + 4, y - 2, "#647a76");
    }
}

void paintContainer() {
    Random random(831);
    polygon({{278,345}, {783,356}, {893,389}, {324,383}}, "rgba(22,38,36,.47)");
    polygon({{289,232}, {326,215}, {788,225}, {750,243}}, "#a18c72");
    polygon({{750,243}, {788,225}, {787,342}, {750,362}}, "#5b554c");
    save();
    transform(1, .023, 0, 1, 289, 232);
    linearGradient(0, 0, 0, 124);
    colorStop(0, "#a28b6d");
    colorStop(.12, "#918068");
    colorStop(.85, "#83765f");
    colorStop(1, "#625e52");
    rect(0, 0, 461, 120, nullptr);
    for (double x = 9; x < 455; x += 17) {
        polygon({{x,7}, {x+4,10}, {x+4,109}, {x,113}}, "#706957");
        rect(x + 4, 10, 6, 99, "#857760");
        line(x + 11, 9, x + 11, 109, "#a48f72", 2);
    }
    rect(0, 0, 461, 4, "#b29b7b");
    rect(0, 115, 461, 5, "#59594c");
    for (int i = 0; i < 180; ++i) {
        const double x = random.next() * 459;
        const double y = random.next() * 117;
        rect(x, y, random.next() * 6 + 1, 1, "rgba(62,62,49,.22)");
    }
    constexpr int ends[] = {0, 452};
    for (int x : ends) {
        rect(x, 0, 9, 120, "#7c7260");
        rect(x + 2, 3, 4, 4, "#464c42");
        rect(x + 2, 111, 4, 4, "#464c42");
    }
    text("E-4", 17, 32, "bold 22px monospace", "#d3c09c");
    for (int i = 0; i < 3; ++i)
        rect(20, 39 + i * 3, 23 - i * 4, 1, "#c0ad8a");
    restore();
    for (int x = 758; x < 786; x += 10) {
        const double top = 242 - (x - 750) * .48;
        line(x, top + 4, x, top + 111, "#827561", 2);
    }
    linearGradient(0, 400, 0, 500);
    colorStop(0, "rgba(14,26,26,0)");
    colorStop(1, "rgba(14,26,26,.62)");
    rect(0, 400, width, 100, nullptr);
}

void walker(double x, double seconds, bool outlined) {
    const double stride = sine(seconds * 4.8);
    const double lift = __builtin_fabs(stride) * 1.5;
    const char* body = outlined ? highlight : "#283b39";
    const char* back = outlined ? "#afdc7e" : "#455550";
    save();
    transform(1, 0, 0, 1, x, 336);
    roundedLines();
    constexpr int sides[] = {-1, 1};
    for (int side : sides) {
        const double swing = stride * side;
        const double knee = swing * 6 + 4;
        const double foot = swing * 13;
        const double footY = -2 + (swing < 0 ? swing * 4 : 0);
        const char* shade = side < 0 ? back : body;
        line(0, -29 - lift, knee, -15, shade, 6);
        line(knee, -15, foot, footY, shade, 5);
        line(foot, footY, foot + 4, footY, shade, 4);
    }
    polygon({{-5,-45-lift}, {2,-47-lift}, {7,-41-lift}, {4,-26-lift}, {-4,-26-lift}}, body);
    ellipse(3, -52 - lift, 5, 6, body);
    line(0, -42 - lift, -2 - stride * 5, -35 - lift, back, 4);
    line(-2 - stride * 5, -35 - lift, 3 - stride * 8, -31 - lift, back, 3.5);
    restore();
}

void marker(double x, double seconds) {
    save();
    opacity(.92);
    walker(x, seconds, true);
    constexpr int signs[] = {1, -1};
    for (int dx : signs) {
        const double cx = x - 22 * dx;
        for (int dy : signs) {
            const double cy = dy > 0 ? 269 : 341;
            const Point corner[] = {{cx + dx * 8, cy}, {cx, cy}, {cx, cy + dy * 8}};
            path(corner, 3, highlight, 1.5);
        }
    }
    rect(x - 22, 249, 57, 15, "#21372f");
    text("NGƯỜI 01", x - 18, 260, "10px monospace", highlight);
    restore();
}

double elapsed = 0;
double startedAt = 0;
bool playing = false;
bool esp = false;

double clampTime(double seconds) {
    if (seconds < 0) return 0;
    if (seconds > duration) return duration;
    return seconds;
}

void render() {
    const double x = 523 + elapsed * 28;
    drawLayer(Layer::Harbor);
    ellipse(x + 5, 337, 21, 2.5, "rgba(21,36,32,.28)");
    walker(x, elapsed, false);
    drawLayer(Layer::Container);
    if (esp) marker(x, elapsed);
    showState(elapsed, duration, playing, esp);
}
} // namespace

#define EXPORT(name) extern "C" __attribute__((export_name(#name)))
EXPORT(init) void init() {
    selectLayer(Layer::Harbor);
    paintHarbor();
    selectLayer(Layer::Container);
    paintContainer();
    selectLayer(Layer::Screen);
    render();
}

EXPORT(setView) void setView(bool highlighted) {
    esp = highlighted;
    render();
}

EXPORT(reset) void reset() {
    playing = false;
    elapsed = 0;
    render();
}

EXPORT(seekTo) void seekTo(double seconds, double now) {
    elapsed = clampTime(seconds);
    startedAt = now - elapsed;
    if (elapsed >= duration) playing = false;
    render();
}

EXPORT(toggle) void toggle(double now) {
    if (playing) {
        elapsed = clampTime(now - startedAt);
        playing = false;
    } else {
        if (elapsed >= duration) elapsed = 0;
        startedAt = now - elapsed;
        playing = true;
    }
    render();
}

EXPORT(tick) void tick(double now) {
    if (!playing) return;
    elapsed = clampTime(now - startedAt);
    if (elapsed >= duration) playing = false;
    render();
}
