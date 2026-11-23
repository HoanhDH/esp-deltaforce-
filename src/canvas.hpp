#pragma once

// The browser provides these Canvas 2D primitives to the WebAssembly module.
// Coordinates use the original 1200 x 500 scene regardless of display density.
namespace canvas {

struct Point {
    double x;
    double y;
};

enum class Layer : int { Screen, Harbor, Container, Comparison };

#define CANVAS_IMPORT(name) __attribute__((import_module("canvas"), import_name(#name)))
extern "C" {
CANVAS_IMPORT(selectLayer) void selectLayer(Layer layer);
CANVAS_IMPORT(drawLayer) void drawLayer(Layer layer);
CANVAS_IMPORT(save) void save();
CANVAS_IMPORT(restore) void restore();
CANVAS_IMPORT(transform) void transform(double a, double b, double c, double d, double x, double y);
CANVAS_IMPORT(roundedLines) void roundedLines();
CANVAS_IMPORT(opacity) void opacity(double alpha);
CANVAS_IMPORT(line) void line(double x1, double y1, double x2, double y2, const char* color,
                              double thickness = 1);
CANVAS_IMPORT(ellipse) void ellipse(double x, double y, double rx, double ry, const char* color);
CANVAS_IMPORT(rect) void rect(double x, double y, double w, double h, const char* color);
CANVAS_IMPORT(path) void path(const Point* points, unsigned count, const char* color,
                              double strokeWidth);
CANVAS_IMPORT(text) void text(const char* value, double x, double y, const char* font, const char* color);

// A null fill uses the gradient most recently defined on the current layer.
CANVAS_IMPORT(linearGradient) void linearGradient(double x1, double y1, double x2, double y2);
CANVAS_IMPORT(radialGradient) void radialGradient(double x, double y, double r1, double r2);
CANVAS_IMPORT(colorStop) void colorStop(double offset, const char* color);
}
#undef CANVAS_IMPORT

template<unsigned N>
void polygon(const Point (&points)[N], const char* fill) {
    path(points, N, fill, 0);
}

} // namespace canvas
