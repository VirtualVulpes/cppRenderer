#ifndef CPPRENDERER_SETTINGS_H
#define CPPRENDERER_SETTINGS_H

struct DebugSettings {
  bool drawLights{false};
};

struct RenderSettings {
  bool drawTextures{true};
  bool drawWireframe{false};
  DebugSettings debug;
};

#endif