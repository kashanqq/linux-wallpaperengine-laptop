//
// Created by kashtan on 5/13/26.
//

#include "BatteryDetector.h"
#include "
#include <fstream>
#include <string>
#include <iostream>

using namespace WallpaperEngine;
using namespace WallpaperEngine::Render::Drivers::Detectors;

BatteryDetector::BatteryDetector (Application::ApplicationContext& appContext) :
    m_applicationContext (appContext) { }

Application::ApplicationContext& BatteryDetector::getApplicationContext () const {
    return this->m_applicationContext;
}

bool BatteryDetector::isOnBattery () {
    auto now = std::chrono::steady_clock::now();

    // Если 5 секунд не прошло, отдаем сохраненное значение
    if (std::chrono::duration_cast<std::chrono::seconds>(now - m_lastCheckTime).count() < 5) {
        return m_lastBatteryStatus;
    }

    m_lastCheckTime = now;

    std::ifstream file("/sys/class/power_supply/AC/online");
    if (!file.is_open()) file.open("/sys/class/power_supply/ACAD/online");
    if (!file.is_open()) file.open("/sys/class/power_supply/ADP1/online");

    if (file.is_open()) {
        std::string status;
        std::getline(file, status);
        // if in file 0, we are on battery
        m_lastBatteryStatus = (status == "0");

        sLog.out ("[БАТАРЕЯ] Статус в файле: " << status << " | Пауза: " << m_lastBatteryStatus);

        return m_lastBatteryStatus;
    }

    // If files do not exist, continue working
    std::cout << "[БАТАРЕЯ] ОШИБКА: Файл питания вообще не найден!" << std::endl;
    m_lastBatteryStatus = false;
    return m_lastBatteryStatus;
}