#ifndef ENTRY_H
#define ENTRY_H

#include <engine/scene.h>
#include "windowstate.h"
#include <memory>

namespace entry {

extern Scene s_scene;
extern float s_worldTransform[16];
extern float s_fps;
extern size_t s_iFrame;
extern double s_sum;
extern size_t s_counter;
extern size_t s_epoch;
extern double s_deltaTime;
extern std::chrono::time_point<std::chrono::high_resolution_clock> s_lastTime;

extern bool s_showStats;
extern std::string s_renderer;
extern std::string s_vendorID;
extern bool s_vsyncEnable;
extern int s_mssaLevel;
extern int s_textureSamplerFlags;

extern bool s_bgfxInitialized;

uint32_t getResetFlags();

void init(View& view);
void shutdown();
void update(const View& view);

} // entry

#endif // ENTRY_H
