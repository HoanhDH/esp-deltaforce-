#pragma once
#include "canvas.hpp"

class QImage;

namespace desktop {
const QImage& image(canvas::Layer layer);
void finishPainting();
}
