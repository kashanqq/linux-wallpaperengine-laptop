//
// Created by kashtan on 5/13/26.
//

#include "BatteryDetector.h"
#include "WallpaperEngine/Logging/Log.h"
#include <fstream>
#include <iostream>
#include <string>

using namespace WallpaperEngine;
using namespace WallpaperEngine::Render::Drivers::Detectors;

BatteryDetector::BatteryDetector (Application::ApplicationContext& appContext) : m_applicationContext (appContext) { }

Application::ApplicationContext& BatteryDetector::getApplicationContext () const { return this->m_applicationContext; }

bool BatteryDetector::isOnBattery () {
    auto now = std::chrono::steady_clock::now ();

    // If 5 seconds have not passed, return the stored value
    if (std::chrono::duration_cast<std::chrono::seconds> (now - m_lastCheckTime).count () < 5) {
	return m_lastBatteryStatus;
    }

    m_lastCheckTime = now;

    std::ifstream file ("/sys/class/power_supply/AC/online");
    if (!file.is_open ()) {
	file.open ("/sys/class/power_supply/ACAD/online");
    }
    if (!file.is_open ()) {
	file.open ("/sys/class/power_supply/ADP1/online");
    }

    if (file.is_open ()) {
	std::string status;
	std::getline (file, status);
	// if in file 0, we are on battery
	m_lastBatteryStatus = (status == "0");

	if (m_lastBatteryStatus != m_lastLoggedStatus) {
	    sLog.out ("[BATTERY] Power source changed to: ", m_lastBatteryStatus ? "Battery" : "AC");
	    m_lastLoggedStatus = m_lastBatteryStatus;
	}

	return m_lastBatteryStatus;
    }

    // If files do not exist, continue working
    sLog.error ("[BATTERY] ERROR: Power source file not found!");
    m_lastBatteryStatus = false;
    return m_lastBatteryStatus;
}