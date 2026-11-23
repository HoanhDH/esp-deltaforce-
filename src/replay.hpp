#pragma once

namespace replay {
enum class View : int { Tung, Muc, Compare };

#if defined(__wasm__)
#define REPLAY_EXPORT(name) extern "C" __attribute__((export_name(#name)))
#else
#define REPLAY_EXPORT(name)
#endif

REPLAY_EXPORT(init) void init();
REPLAY_EXPORT(setView) void setView(View selected);
REPLAY_EXPORT(stepFrame) void stepFrame(int direction);
REPLAY_EXPORT(reset) void reset();
REPLAY_EXPORT(seekTo) void seekTo(double seconds, double now);
REPLAY_EXPORT(toggle) void toggle(double now);
REPLAY_EXPORT(tick) void tick(double now);
#undef REPLAY_EXPORT

#if defined(__wasm__)
extern "C" __attribute__((import_module("host"), import_name("showState")))
#endif
void showState(int frame, int lastFrame, int fps, bool playing, View view);
} // namespace replay
