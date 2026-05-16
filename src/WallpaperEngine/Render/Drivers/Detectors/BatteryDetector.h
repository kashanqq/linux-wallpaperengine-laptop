#pragma once

#include "WallpaperEngine/Application/ApplicationContext.h"
#include <chrono>

namespace WallpaperEngine::Render::Drivers::Detectors {
class BatteryDetector {
public:
    explicit BatteryDetector (Application::ApplicationContext& appContext);
    virtual ~BatteryDetector () = default;

    /**
     * @return Return true, if laptop is working on battery
     */
    [[nodiscard]] virtual bool isOnBattery ();

    [[nodiscard]] Application::ApplicationContext& getApplicationContext () const;

private:
    Application::ApplicationContext& m_applicationContext;
    std::chrono::steady_clock::time_point m_lastCheckTime;
    bool m_lastBatteryStatus = false;
};
} // namespace WallpaperEngine::Render::Drivers::Detectors