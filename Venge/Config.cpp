#include <fstream>
#include <ShlObj.h>

#include "json/json.h"

#include "Config.h"

Config::Config(const char* name) noexcept
{
    if (PWSTR pathToDocuments; SUCCEEDED(SHGetKnownFolderPath(FOLDERID_Documents, 0, nullptr, &pathToDocuments))) {
        path = pathToDocuments;
        path /= name;
        CoTaskMemFree(pathToDocuments);
    }

    if (!std::filesystem::is_directory(path)) {
        std::filesystem::remove(path);
        std::filesystem::create_directory(path);
    }

    std::transform(std::filesystem::directory_iterator{ path },
                   std::filesystem::directory_iterator{ },
                   std::back_inserter(configs),
                   [](const auto& entry) { return entry.path().filename().string(); });
}

void Config::load(size_t id) noexcept
{
    if (!std::filesystem::is_directory(path)) {
        std::filesystem::remove(path);
        std::filesystem::create_directory(path);
    }

    std::ifstream in{ path / configs[id] };

    if (!in.good())
        return;

    Json::Value json;

    in >> json;
    in.close();

    for (size_t i = 0; i < aimhelper.size(); i++) {
        const auto& aimhelperJson = json["Aimbot"][i];
        auto& aimhelperConfig = aimhelper[i];

        if (aimhelperJson.isMember("Enabled")) aimhelperConfig.enabled = aimhelperJson["Enabled"].asBool();
        if (aimhelperJson.isMember("On key")) aimhelperConfig.onKey = aimhelperJson["On key"].asBool();
        if (aimhelperJson.isMember("Key")) aimhelperConfig.key = aimhelperJson["Key"].asInt();
        if (aimhelperJson.isMember("Key mode")) aimhelperConfig.keyMode = aimhelperJson["Key mode"].asInt();
        if (aimhelperJson.isMember("Aimlock")) aimhelperConfig.aimlock = aimhelperJson["Aimlock"].asBool();
        if (aimhelperJson.isMember("Silent")) aimhelperConfig.silent = aimhelperJson["Silent"].asBool();
        if (aimhelperJson.isMember("Friendly fire")) aimhelperConfig.friendlyFire = aimhelperJson["Friendly fire"].asBool();
        if (aimhelperJson.isMember("Visible only")) aimhelperConfig.visibleOnly = aimhelperJson["Visible only"].asBool();
        if (aimhelperJson.isMember("Scoped only")) aimhelperConfig.scopedOnly = aimhelperJson["Scoped only"].asBool();
        if (aimhelperJson.isMember("Ignore flash")) aimhelperConfig.ignoreFlash = aimhelperJson["Ignore flash"].asBool();
        if (aimhelperJson.isMember("Ignore smoke")) aimhelperConfig.ignoreSmoke = aimhelperJson["Ignore smoke"].asBool();
        if (aimhelperJson.isMember("Auto shot")) aimhelperConfig.autoShot = aimhelperJson["Auto shot"].asBool();
        if (aimhelperJson.isMember("Auto scope")) aimhelperConfig.autoScope = aimhelperJson["Auto scope"].asBool();
        if (aimhelperJson.isMember("Recoil-based fov")) aimhelperConfig.recoilbasedFov = aimhelperJson["Recoil-based fov"].asBool();
        if (aimhelperJson.isMember("Fov")) aimhelperConfig.fov = aimhelperJson["Fov"].asFloat();
        if (aimhelperJson.isMember("Smooth")) aimhelperConfig.smooth = aimhelperJson["Smooth"].asFloat();
        if (aimhelperJson.isMember("Bone")) aimhelperConfig.bone = aimhelperJson["Bone"].asInt();
        if (aimhelperJson.isMember("Recoil control X")) aimhelperConfig.recoilControlX = aimhelperJson["Recoil control X"].asFloat();
        if (aimhelperJson.isMember("Recoil control Y")) aimhelperConfig.recoilControlY = aimhelperJson["Recoil control Y"].asFloat();
        if (aimhelperJson.isMember("Max aim inaccuracy")) aimhelperConfig.maxAimInaccuracy = aimhelperJson["Max aim inaccuracy"].asFloat();
        if (aimhelperJson.isMember("Max shot inaccuracy")) aimhelperConfig.maxShotInaccuracy = aimhelperJson["Max shot inaccuracy"].asFloat();
        if (aimhelperJson.isMember("Min damage")) aimhelperConfig.minDamage = aimhelperJson["Min damage"].asInt();
        if (aimhelperJson.isMember("Killshot")) aimhelperConfig.killshot = aimhelperJson["Killshot"].asBool();
        if (aimhelperJson.isMember("Between shots")) aimhelperConfig.betweenShots = aimhelperJson["Between shots"].asBool();
    }

    for (size_t i = 0; i < fastshotbot.size(); i++) {
        const auto& fastshotbotJson = json["Triggerbot"][i];
        auto& fastshotbotConfig = fastshotbot[i];

        if (fastshotbotJson.isMember("Enabled")) fastshotbotConfig.enabled = fastshotbotJson["Enabled"].asBool();
        if (fastshotbotJson.isMember("On key")) fastshotbotConfig.onKey = fastshotbotJson["On key"].asBool();
        if (fastshotbotJson.isMember("Key")) fastshotbotConfig.key = fastshotbotJson["Key"].asInt();
        if (fastshotbotJson.isMember("Friendly fire")) fastshotbotConfig.friendlyFire = fastshotbotJson["Friendly fire"].asBool();
        if (fastshotbotJson.isMember("Scoped only")) fastshotbotConfig.scopedOnly = fastshotbotJson["Scoped only"].asBool();
        if (fastshotbotJson.isMember("Ignore flash")) fastshotbotConfig.ignoreFlash = fastshotbotJson["Ignore flash"].asBool();
        if (fastshotbotJson.isMember("Ignore smoke")) fastshotbotConfig.ignoreSmoke = fastshotbotJson["Ignore smoke"].asBool();
        if (fastshotbotJson.isMember("Hitgroup")) fastshotbotConfig.hitgroup = fastshotbotJson["Hitgroup"].asInt();
        if (fastshotbotJson.isMember("Shot delay")) fastshotbotConfig.shotDelay = fastshotbotJson["Shot delay"].asInt();
        if (fastshotbotJson.isMember("Min damage")) fastshotbotConfig.minDamage = fastshotbotJson["Min damage"].asInt();
        if (fastshotbotJson.isMember("Killshot")) fastshotbotConfig.killshot = fastshotbotJson["Killshot"].asBool();
    }

    {
        const auto& timereverseJson = json["Backtrack"];
        if (timereverseJson.isMember("Enabled")) timereverse.enabled = timereverseJson["Enabled"].asBool();
        if (timereverseJson.isMember("Ignore smoke")) timereverse.ignoreSmoke = timereverseJson["Ignore smoke"].asBool();
        if (timereverseJson.isMember("Recoil based fov")) timereverse.recoilBasedFov = timereverseJson["Recoil based fov"].asBool();
        if (timereverseJson.isMember("Time limit")) timereverse.timeLimit = timereverseJson["Time limit"].asInt();
    }


    for (size_t i = 0; i < bodyoutline.size(); i++) {
        const auto& bodyoutlineJson = json["Glow"][i];
        auto& bodyoutlineConfig = bodyoutline[i];

        if (bodyoutlineJson.isMember("Enabled")) bodyoutlineConfig.enabled = bodyoutlineJson["Enabled"].asBool();
        if (bodyoutlineJson.isMember("healthBased")) bodyoutlineConfig.healthBased = bodyoutlineJson["healthBased"].asBool();
        if (bodyoutlineJson.isMember("thickness")) bodyoutlineConfig.thickness = bodyoutlineJson["thickness"].asFloat();
        if (bodyoutlineJson.isMember("alpha")) bodyoutlineConfig.alpha = bodyoutlineJson["alpha"].asFloat();
        if (bodyoutlineJson.isMember("style")) bodyoutlineConfig.style = bodyoutlineJson["style"].asInt();
        if (bodyoutlineJson.isMember("Color")) {
            const auto& colorJson = bodyoutlineJson["Color"];
            auto& colorConfig = bodyoutlineConfig.color;

            if (colorJson.isMember("Color")) {
                colorConfig.color[0] = colorJson["Color"][0].asFloat();
                colorConfig.color[1] = colorJson["Color"][1].asFloat();
                colorConfig.color[2] = colorJson["Color"][2].asFloat();
            }

            if (colorJson.isMember("Rainbow")) colorConfig.rainbow = colorJson["Rainbow"].asBool();
            if (colorJson.isMember("Rainbow speed")) colorConfig.rainbowSpeed = colorJson["Rainbow speed"].asFloat();
        }
    }

    for (size_t i = 0; i < colouredbody.size(); i++) {
        const auto& aimhelperJson = json["Chams"][i];
        auto& aimhelperConfig = colouredbody[i];

        for (size_t j = 0; j < colouredbody[0].materials.size(); j++) {
            const auto& materialsJson = aimhelperJson[j];
            auto& materialsConfig = colouredbody[i].materials[j];

            if (materialsJson.isMember("Enabled")) materialsConfig.enabled = materialsJson["Enabled"].asBool();
            if (materialsJson.isMember("Health based")) materialsConfig.healthBased = materialsJson["Health based"].asBool();
            if (materialsJson.isMember("Blinking")) materialsConfig.blinking = materialsJson["Blinking"].asBool();
            if (materialsJson.isMember("Material")) materialsConfig.material = materialsJson["Material"].asInt();
            if (materialsJson.isMember("Wireframe")) materialsConfig.wireframe = materialsJson["Wireframe"].asBool();
            if (materialsJson.isMember("Color")) {
                const auto& colorJson = materialsJson["Color"];
                auto& colorConfig = materialsConfig.color;

                if (colorJson.isMember("Color")) {
                    colorConfig.color[0] = colorJson["Color"][0].asFloat();
                    colorConfig.color[1] = colorJson["Color"][1].asFloat();
                    colorConfig.color[2] = colorJson["Color"][2].asFloat();
                }

                if (colorJson.isMember("Rainbow")) colorConfig.rainbow = colorJson["Rainbow"].asBool();
                if (colorJson.isMember("Rainbow speed")) colorConfig.rainbowSpeed = colorJson["Rainbow speed"].asFloat();
            }
            if (materialsJson.isMember("Alpha")) materialsConfig.alpha = materialsJson["Alpha"].asFloat();
        }
    }

    for (size_t i = 0; i < esp.players.size(); i++) {
        const auto& espJson = json["esp"]["Players"][i];
        auto& espConfig = esp.players[i];
        
        if (espJson.isMember("Enabled")) espConfig.enabled = espJson["Enabled"].asBool();
        if (espJson.isMember("Font")) espConfig.font = espJson["Font"].asInt();

        if (espJson.isMember("Snaplines")) {
            const auto& snaplinesJson = espJson["Snaplines"];
            auto& snaplinesConfig = espConfig.snaplines;

            if (snaplinesJson.isMember("Enabled")) snaplinesConfig.enabled = snaplinesJson["Enabled"].asBool();

            if (snaplinesJson.isMember("Color")) {
                snaplinesConfig.color[0] = snaplinesJson["Color"][0].asFloat();
                snaplinesConfig.color[1] = snaplinesJson["Color"][1].asFloat();
                snaplinesConfig.color[2] = snaplinesJson["Color"][2].asFloat();
            }

            if (snaplinesJson.isMember("Rainbow")) snaplinesConfig.rainbow = snaplinesJson["Rainbow"].asBool();
            if (snaplinesJson.isMember("Rainbow speed")) snaplinesConfig.rainbowSpeed = snaplinesJson["Rainbow speed"].asFloat();
        }

        if (espJson.isMember("Eye traces")) {
            const auto& eyeTracesJson = espJson["Eye traces"];
            auto& eyeTracesConfig = espConfig.eyeTraces;

            if (eyeTracesJson.isMember("Enabled")) eyeTracesConfig.enabled = eyeTracesJson["Enabled"].asBool();

            if (eyeTracesJson.isMember("Color")) {
                eyeTracesConfig.color[0] = eyeTracesJson["Color"][0].asFloat();
                eyeTracesConfig.color[1] = eyeTracesJson["Color"][1].asFloat();
                eyeTracesConfig.color[2] = eyeTracesJson["Color"][2].asFloat();
            }

            if (eyeTracesJson.isMember("Rainbow")) eyeTracesConfig.rainbow = eyeTracesJson["Rainbow"].asBool();
            if (eyeTracesJson.isMember("Rainbow speed")) eyeTracesConfig.rainbowSpeed = eyeTracesJson["Rainbow speed"].asFloat();
        }

        if (espJson.isMember("Box")) {
            const auto& boxJson = espJson["Box"];
            auto& boxConfig = espConfig.box;

            if (boxJson.isMember("Enabled")) boxConfig.enabled = boxJson["Enabled"].asBool();

            if (boxJson.isMember("Color")) {
                boxConfig.color[0] = boxJson["Color"][0].asFloat();
                boxConfig.color[1] = boxJson["Color"][1].asFloat();
                boxConfig.color[2] = boxJson["Color"][2].asFloat();
            }

            if (boxJson.isMember("Rainbow")) boxConfig.rainbow = boxJson["Rainbow"].asBool();
            if (boxJson.isMember("Rainbow speed")) boxConfig.rainbowSpeed = boxJson["Rainbow speed"].asFloat();
        }

        if (espJson.isMember("Box type")) espConfig.boxType = espJson["Box type"].asInt();

        if (espJson.isMember("Name")) {
            const auto& nameJson = espJson["Name"];
            auto& nameConfig = espConfig.name;

            if (nameJson.isMember("Enabled")) nameConfig.enabled = nameJson["Enabled"].asBool();

            if (nameJson.isMember("Color")) {
                nameConfig.color[0] = nameJson["Color"][0].asFloat();
                nameConfig.color[1] = nameJson["Color"][1].asFloat();
                nameConfig.color[2] = nameJson["Color"][2].asFloat();
            }

            if (nameJson.isMember("Rainbow")) nameConfig.rainbow = nameJson["Rainbow"].asBool();
            if (nameJson.isMember("Rainbow speed")) nameConfig.rainbowSpeed = nameJson["Rainbow speed"].asFloat();
        }

        if (espJson.isMember("Health")) {
            const auto& healthJson = espJson["Health"];
            auto& healthConfig = espConfig.health;

            if (healthJson.isMember("Enabled")) healthConfig.enabled = healthJson["Enabled"].asBool();

            if (healthJson.isMember("Color")) {
                healthConfig.color[0] = healthJson["Color"][0].asFloat();
                healthConfig.color[1] = healthJson["Color"][1].asFloat();
                healthConfig.color[2] = healthJson["Color"][2].asFloat();
            }

            if (healthJson.isMember("Rainbow")) healthConfig.rainbow = healthJson["Rainbow"].asBool();
            if (healthJson.isMember("Rainbow speed")) healthConfig.rainbowSpeed = healthJson["Rainbow speed"].asFloat();
        }

        if (espJson.isMember("Health bar")) {
            const auto& healthBarJson = espJson["Health bar"];
            auto& healthBarConfig = espConfig.healthBar;

            if (healthBarJson.isMember("Enabled")) healthBarConfig.enabled = healthBarJson["Enabled"].asBool();

            if (healthBarJson.isMember("Color")) {
                healthBarConfig.color[0] = healthBarJson["Color"][0].asFloat();
                healthBarConfig.color[1] = healthBarJson["Color"][1].asFloat();
                healthBarConfig.color[2] = healthBarJson["Color"][2].asFloat();
            }

            if (healthBarJson.isMember("Rainbow")) healthBarConfig.rainbow = healthBarJson["Rainbow"].asBool();
            if (healthBarJson.isMember("Rainbow speed")) healthBarConfig.rainbowSpeed = healthBarJson["Rainbow speed"].asFloat();
        }

        if (espJson.isMember("Armor")) {
            const auto& armorJson = espJson["Armor"];
            auto& armorConfig = espConfig.armor;

            if (armorJson.isMember("Enabled")) armorConfig.enabled = armorJson["Enabled"].asBool();

            if (armorJson.isMember("Color")) {
                armorConfig.color[0] = armorJson["Color"][0].asFloat();
                armorConfig.color[1] = armorJson["Color"][1].asFloat();
                armorConfig.color[2] = armorJson["Color"][2].asFloat();
            }

            if (armorJson.isMember("Rainbow")) armorConfig.rainbow = armorJson["Rainbow"].asBool();
            if (armorJson.isMember("Rainbow speed")) armorConfig.rainbowSpeed = armorJson["Rainbow speed"].asFloat();
        }

        if (espJson.isMember("Armor bar")) {
            const auto& armorBarJson = espJson["Armor bar"];
            auto& armorBarConfig = espConfig.armorBar;

            if (armorBarJson.isMember("Enabled")) armorBarConfig.enabled = armorBarJson["Enabled"].asBool();

            if (armorBarJson.isMember("Color")) {
                armorBarConfig.color[0] = armorBarJson["Color"][0].asFloat();
                armorBarConfig.color[1] = armorBarJson["Color"][1].asFloat();
                armorBarConfig.color[2] = armorBarJson["Color"][2].asFloat();
            }

            if (armorBarJson.isMember("Rainbow")) armorBarConfig.rainbow = armorBarJson["Rainbow"].asBool();
            if (armorBarJson.isMember("Rainbow speed")) armorBarConfig.rainbowSpeed = armorBarJson["Rainbow speed"].asFloat();
        }

        if (espJson.isMember("Money")) {
            const auto& moneyJson = espJson["Money"];
            auto& moneyConfig = espConfig.money;

            if (moneyJson.isMember("Enabled")) moneyConfig.enabled = moneyJson["Enabled"].asBool();

            if (moneyJson.isMember("Color")) {
                moneyConfig.color[0] = moneyJson["Color"][0].asFloat();
                moneyConfig.color[1] = moneyJson["Color"][1].asFloat();
                moneyConfig.color[2] = moneyJson["Color"][2].asFloat();
            }

            if (moneyJson.isMember("Rainbow")) moneyConfig.rainbow = moneyJson["Rainbow"].asBool();
            if (moneyJson.isMember("Rainbow speed")) moneyConfig.rainbowSpeed = moneyJson["Rainbow speed"].asFloat();
        }

        if (espJson.isMember("Head dot")) {
            const auto& headDotJson = espJson["Head dot"];
            auto& headDotConfig = espConfig.headDot;

            if (headDotJson.isMember("Enabled")) headDotConfig.enabled = headDotJson["Enabled"].asBool();

            if (headDotJson.isMember("Color")) {
                headDotConfig.color[0] = headDotJson["Color"][0].asFloat();
                headDotConfig.color[1] = headDotJson["Color"][1].asFloat();
                headDotConfig.color[2] = headDotJson["Color"][2].asFloat();
            }

            if (headDotJson.isMember("Rainbow")) headDotConfig.rainbow = headDotJson["Rainbow"].asBool();
            if (headDotJson.isMember("Rainbow speed")) headDotConfig.rainbowSpeed = headDotJson["Rainbow speed"].asFloat();
        }

        if (espJson.isMember("Active weapon")) {
            const auto& activeWeaponJson = espJson["Active weapon"];
            auto& activeWeaponConfig = espConfig.activeWeapon;

            if (activeWeaponJson.isMember("Enabled")) activeWeaponConfig.enabled = activeWeaponJson["Enabled"].asBool();

            if (activeWeaponJson.isMember("Color")) {
                activeWeaponConfig.color[0] = activeWeaponJson["Color"][0].asFloat();
                activeWeaponConfig.color[1] = activeWeaponJson["Color"][1].asFloat();
                activeWeaponConfig.color[2] = activeWeaponJson["Color"][2].asFloat();
            }

            if (activeWeaponJson.isMember("Rainbow")) activeWeaponConfig.rainbow = activeWeaponJson["Rainbow"].asBool();
            if (activeWeaponJson.isMember("Rainbow speed")) activeWeaponConfig.rainbowSpeed = activeWeaponJson["Rainbow speed"].asFloat();
        }

        if (espJson.isMember("Outline")) {
            const auto& outlineJson = espJson["Outline"];
            auto& outlineConfig = espConfig.outline;

            if (outlineJson.isMember("Enabled")) outlineConfig.enabled = outlineJson["Enabled"].asBool();

            if (outlineJson.isMember("Color")) {
                outlineConfig.color[0] = outlineJson["Color"][0].asFloat();
                outlineConfig.color[1] = outlineJson["Color"][1].asFloat();
                outlineConfig.color[2] = outlineJson["Color"][2].asFloat();
            }

            if (outlineJson.isMember("Rainbow")) outlineConfig.rainbow = outlineJson["Rainbow"].asBool();
            if (outlineJson.isMember("Rainbow speed")) outlineConfig.rainbowSpeed = outlineJson["Rainbow speed"].asFloat();
        }

        if (espJson.isMember("Distance")) {
            const auto& distanceJson = espJson["Distance"];
            auto& distanceConfig = espConfig.distance;

            if (distanceJson.isMember("Enabled")) distanceConfig.enabled = distanceJson["Enabled"].asBool();

            if (distanceJson.isMember("Color")) {
                distanceConfig.color[0] = distanceJson["Color"][0].asFloat();
                distanceConfig.color[1] = distanceJson["Color"][1].asFloat();
                distanceConfig.color[2] = distanceJson["Color"][2].asFloat();
            }

            if (distanceJson.isMember("Rainbow")) distanceConfig.rainbow = distanceJson["Rainbow"].asBool();
            if (distanceJson.isMember("Rainbow speed")) distanceConfig.rainbowSpeed = distanceJson["Rainbow speed"].asFloat();
        }

        if (espJson.isMember("Max distance")) espConfig.maxDistance = espJson["Max distance"].asFloat();
    }

    {
        const auto& espJson = json["esp"]["Weapons"];
        auto& espConfig = esp.weapon;

        if (espJson.isMember("Enabled")) espConfig.enabled = espJson["Enabled"].asBool();
        if (espJson.isMember("Font")) espConfig.font = espJson["Font"].asInt();
        if (espJson.isMember("Snaplines")) {
            const auto& snaplinesJson = espJson["Snaplines"];
            auto& snaplinesConfig = espConfig.snaplines;

            if (snaplinesJson.isMember("Enabled")) snaplinesConfig.enabled = snaplinesJson["Enabled"].asBool();

            if (snaplinesJson.isMember("Color")) {
                snaplinesConfig.color[0] = snaplinesJson["Color"][0].asFloat();
                snaplinesConfig.color[1] = snaplinesJson["Color"][1].asFloat();
                snaplinesConfig.color[2] = snaplinesJson["Color"][2].asFloat();
            }

            if (snaplinesJson.isMember("Rainbow")) snaplinesConfig.rainbow = snaplinesJson["Rainbow"].asBool();
            if (snaplinesJson.isMember("Rainbow speed")) snaplinesConfig.rainbowSpeed = snaplinesJson["Rainbow speed"].asFloat();
        }

        if (espJson.isMember("Box")) {
            const auto& boxJson = espJson["Box"];
            auto& boxConfig = espConfig.box;

            if (boxJson.isMember("Enabled")) boxConfig.enabled = boxJson["Enabled"].asBool();

            if (boxJson.isMember("Color")) {
                boxConfig.color[0] = boxJson["Color"][0].asFloat();
                boxConfig.color[1] = boxJson["Color"][1].asFloat();
                boxConfig.color[2] = boxJson["Color"][2].asFloat();
            }

            if (boxJson.isMember("Rainbow")) boxConfig.rainbow = boxJson["Rainbow"].asBool();
            if (boxJson.isMember("Rainbow speed")) boxConfig.rainbowSpeed = boxJson["Rainbow speed"].asFloat();
        }

        if (espJson.isMember("Box type")) espConfig.boxType = espJson["Box type"].asInt();

        if (espJson.isMember("Outline")) {
            const auto& outlineJson = espJson["Outline"];
            auto& outlineConfig = espConfig.outline;

            if (outlineJson.isMember("Enabled")) outlineConfig.enabled = outlineJson["Enabled"].asBool();

            if (outlineJson.isMember("Color")) {
                outlineConfig.color[0] = outlineJson["Color"][0].asFloat();
                outlineConfig.color[1] = outlineJson["Color"][1].asFloat();
                outlineConfig.color[2] = outlineJson["Color"][2].asFloat();
            }

            if (outlineJson.isMember("Rainbow")) outlineConfig.rainbow = outlineJson["Rainbow"].asBool();
            if (outlineJson.isMember("Rainbow speed")) outlineConfig.rainbowSpeed = outlineJson["Rainbow speed"].asFloat();
        }

        if (espJson.isMember("Name")) {
            const auto& nameJson = espJson["Name"];
            auto& nameConfig = espConfig.name;

            if (nameJson.isMember("Enabled")) nameConfig.enabled = nameJson["Enabled"].asBool();

            if (nameJson.isMember("Color")) {
                nameConfig.color[0] = nameJson["Color"][0].asFloat();
                nameConfig.color[1] = nameJson["Color"][1].asFloat();
                nameConfig.color[2] = nameJson["Color"][2].asFloat();
            }

            if (nameJson.isMember("Rainbow")) nameConfig.rainbow = nameJson["Rainbow"].asBool();
            if (nameJson.isMember("Rainbow speed")) nameConfig.rainbowSpeed = nameJson["Rainbow speed"].asFloat();
        }

        if (espJson.isMember("Distance")) {
            const auto& distanceJson = espJson["Distance"];
            auto& distanceConfig = espConfig.distance;

            if (distanceJson.isMember("Enabled")) distanceConfig.enabled = distanceJson["Enabled"].asBool();

            if (distanceJson.isMember("Color")) {
                distanceConfig.color[0] = distanceJson["Color"][0].asFloat();
                distanceConfig.color[1] = distanceJson["Color"][1].asFloat();
                distanceConfig.color[2] = distanceJson["Color"][2].asFloat();
            }

            if (distanceJson.isMember("Rainbow")) distanceConfig.rainbow = distanceJson["Rainbow"].asBool();
            if (distanceJson.isMember("Rainbow speed")) distanceConfig.rainbowSpeed = distanceJson["Rainbow speed"].asFloat();
        }

        if (espJson.isMember("Max distance")) espConfig.maxDistance = espJson["Max distance"].asFloat();
    }

    for (size_t i = 0; i < esp.dangerZone.size(); i++) {
        const auto& espJson = json["esp"]["Danger Zone"][i];
        auto& espConfig = esp.dangerZone[i];

        if (espJson.isMember("Enabled")) espConfig.enabled = espJson["Enabled"].asBool();
        if (espJson.isMember("Font")) espConfig.font = espJson["Font"].asInt();
        if (espJson.isMember("Snaplines")) {
            const auto& snaplinesJson = espJson["Snaplines"];
            auto& snaplinesConfig = espConfig.snaplines;

            if (snaplinesJson.isMember("Enabled")) snaplinesConfig.enabled = snaplinesJson["Enabled"].asBool();

            if (snaplinesJson.isMember("Color")) {
                snaplinesConfig.color[0] = snaplinesJson["Color"][0].asFloat();
                snaplinesConfig.color[1] = snaplinesJson["Color"][1].asFloat();
                snaplinesConfig.color[2] = snaplinesJson["Color"][2].asFloat();
            }

            if (snaplinesJson.isMember("Rainbow")) snaplinesConfig.rainbow = snaplinesJson["Rainbow"].asBool();
            if (snaplinesJson.isMember("Rainbow speed")) snaplinesConfig.rainbowSpeed = snaplinesJson["Rainbow speed"].asFloat();
        }
        
        if (espJson.isMember("Box")) {
            const auto& boxJson = espJson["Box"];
            auto& boxConfig = espConfig.box;

            if (boxJson.isMember("Enabled")) boxConfig.enabled = boxJson["Enabled"].asBool();

            if (boxJson.isMember("Color")) {
                boxConfig.color[0] = boxJson["Color"][0].asFloat();
                boxConfig.color[1] = boxJson["Color"][1].asFloat();
                boxConfig.color[2] = boxJson["Color"][2].asFloat();
            }

            if (boxJson.isMember("Rainbow")) boxConfig.rainbow = boxJson["Rainbow"].asBool();
            if (boxJson.isMember("Rainbow speed")) boxConfig.rainbowSpeed = boxJson["Rainbow speed"].asFloat();
        }

        if (espJson.isMember("Box type")) espConfig.boxType = espJson["Box type"].asInt();

        if (espJson.isMember("Outline")) {
            const auto& outlineJson = espJson["Outline"];
            auto& outlineConfig = espConfig.outline;

            if (outlineJson.isMember("Enabled")) outlineConfig.enabled = outlineJson["Enabled"].asBool();

            if (outlineJson.isMember("Color")) {
                outlineConfig.color[0] = outlineJson["Color"][0].asFloat();
                outlineConfig.color[1] = outlineJson["Color"][1].asFloat();
                outlineConfig.color[2] = outlineJson["Color"][2].asFloat();
            }

            if (outlineJson.isMember("Rainbow")) outlineConfig.rainbow = outlineJson["Rainbow"].asBool();
            if (outlineJson.isMember("Rainbow speed")) outlineConfig.rainbowSpeed = outlineJson["Rainbow speed"].asFloat();
        }

        if (espJson.isMember("Name")) {
            const auto& nameJson = espJson["Name"];
            auto& nameConfig = espConfig.name;

            if (nameJson.isMember("Enabled")) nameConfig.enabled = nameJson["Enabled"].asBool();

            if (nameJson.isMember("Color")) {
                nameConfig.color[0] = nameJson["Color"][0].asFloat();
                nameConfig.color[1] = nameJson["Color"][1].asFloat();
                nameConfig.color[2] = nameJson["Color"][2].asFloat();
            }

            if (nameJson.isMember("Rainbow")) nameConfig.rainbow = nameJson["Rainbow"].asBool();
            if (nameJson.isMember("Rainbow speed")) nameConfig.rainbowSpeed = nameJson["Rainbow speed"].asFloat();
        }

        if (espJson.isMember("Distance")) {
            const auto& distanceJson = espJson["Distance"];
            auto& distanceConfig = espConfig.distance;

            if (distanceJson.isMember("Enabled")) distanceConfig.enabled = distanceJson["Enabled"].asBool();

            if (distanceJson.isMember("Color")) {
                distanceConfig.color[0] = distanceJson["Color"][0].asFloat();
                distanceConfig.color[1] = distanceJson["Color"][1].asFloat();
                distanceConfig.color[2] = distanceJson["Color"][2].asFloat();
            }

            if (distanceJson.isMember("Rainbow")) distanceConfig.rainbow = distanceJson["Rainbow"].asBool();
            if (distanceJson.isMember("Rainbow speed")) distanceConfig.rainbowSpeed = distanceJson["Rainbow speed"].asFloat();
        }

        if (espJson.isMember("Max distance")) espConfig.maxDistance = espJson["Max distance"].asFloat();
    }

    for (size_t i = 0; i < esp.projectiles.size(); i++) {
        const auto& espJson = json["esp"]["Projectiles"][i];
        auto& espConfig = esp.projectiles[i];

        if (espJson.isMember("Enabled")) espConfig.enabled = espJson["Enabled"].asBool();
        if (espJson.isMember("Font")) espConfig.font = espJson["Font"].asInt();
        if (espJson.isMember("Snaplines")) {
            const auto& snaplinesJson = espJson["Snaplines"];
            auto& snaplinesConfig = espConfig.snaplines;

            if (snaplinesJson.isMember("Enabled")) snaplinesConfig.enabled = snaplinesJson["Enabled"].asBool();

            if (snaplinesJson.isMember("Color")) {
                snaplinesConfig.color[0] = snaplinesJson["Color"][0].asFloat();
                snaplinesConfig.color[1] = snaplinesJson["Color"][1].asFloat();
                snaplinesConfig.color[2] = snaplinesJson["Color"][2].asFloat();
            }

            if (snaplinesJson.isMember("Rainbow")) snaplinesConfig.rainbow = snaplinesJson["Rainbow"].asBool();
            if (snaplinesJson.isMember("Rainbow speed")) snaplinesConfig.rainbowSpeed = snaplinesJson["Rainbow speed"].asFloat();
        }
        if (espJson.isMember("Box")) {
            const auto& boxJson = espJson["Box"];
            auto& boxConfig = espConfig.box;

            if (boxJson.isMember("Enabled")) boxConfig.enabled = boxJson["Enabled"].asBool();

            if (boxJson.isMember("Color")) {
                boxConfig.color[0] = boxJson["Color"][0].asFloat();
                boxConfig.color[1] = boxJson["Color"][1].asFloat();
                boxConfig.color[2] = boxJson["Color"][2].asFloat();
            }

            if (boxJson.isMember("Rainbow")) boxConfig.rainbow = boxJson["Rainbow"].asBool();
            if (boxJson.isMember("Rainbow speed")) boxConfig.rainbowSpeed = boxJson["Rainbow speed"].asFloat();
        }

        if (espJson.isMember("Box type")) espConfig.boxType = espJson["Box type"].asInt();

        if (espJson.isMember("Outline")) {
            const auto& outlineJson = espJson["Outline"];
            auto& outlineConfig = espConfig.outline;

            if (outlineJson.isMember("Enabled")) outlineConfig.enabled = outlineJson["Enabled"].asBool();

            if (outlineJson.isMember("Color")) {
                outlineConfig.color[0] = outlineJson["Color"][0].asFloat();
                outlineConfig.color[1] = outlineJson["Color"][1].asFloat();
                outlineConfig.color[2] = outlineJson["Color"][2].asFloat();
            }

            if (outlineJson.isMember("Rainbow")) outlineConfig.rainbow = outlineJson["Rainbow"].asBool();
            if (outlineJson.isMember("Rainbow speed")) outlineConfig.rainbowSpeed = outlineJson["Rainbow speed"].asFloat();
        }

        if (espJson.isMember("Name")) {
            const auto& nameJson = espJson["Name"];
            auto& nameConfig = espConfig.name;

            if (nameJson.isMember("Enabled")) nameConfig.enabled = nameJson["Enabled"].asBool();

            if (nameJson.isMember("Color")) {
                nameConfig.color[0] = nameJson["Color"][0].asFloat();
                nameConfig.color[1] = nameJson["Color"][1].asFloat();
                nameConfig.color[2] = nameJson["Color"][2].asFloat();
            }

            if (nameJson.isMember("Rainbow")) nameConfig.rainbow = nameJson["Rainbow"].asBool();
            if (nameJson.isMember("Rainbow speed")) nameConfig.rainbowSpeed = nameJson["Rainbow speed"].asFloat();
        }

        if (espJson.isMember("Distance")) {
            const auto& distanceJson = espJson["Distance"];
            auto& distanceConfig = espConfig.distance;

            if (distanceJson.isMember("Enabled")) distanceConfig.enabled = distanceJson["Enabled"].asBool();

            if (distanceJson.isMember("Color")) {
                distanceConfig.color[0] = distanceJson["Color"][0].asFloat();
                distanceConfig.color[1] = distanceJson["Color"][1].asFloat();
                distanceConfig.color[2] = distanceJson["Color"][2].asFloat();
            }

            if (distanceJson.isMember("Rainbow")) distanceConfig.rainbow = distanceJson["Rainbow"].asBool();
            if (distanceJson.isMember("Rainbow speed")) distanceConfig.rainbowSpeed = distanceJson["Rainbow speed"].asFloat();
        }

        if (espJson.isMember("Max distance")) espConfig.maxDistance = espJson["Max distance"].asFloat();
    }

    {
        const auto& visualsJson = json["visuals"];
        if (visualsJson.isMember("disablePostProcessing")) visuals.disablePostProcessing = visualsJson["disablePostProcessing"].asBool();
        if (visualsJson.isMember("inverseRagdollGravity")) visuals.inverseRagdollGravity = visualsJson["inverseRagdollGravity"].asBool();
        if (visualsJson.isMember("noFog")) visuals.noFog = visualsJson["noFog"].asBool();
        if (visualsJson.isMember("no3dSky")) visuals.no3dSky = visualsJson["no3dSky"].asBool();
        if (visualsJson.isMember("No aim punch")) visuals.noAimPunch = visualsJson["No aim punch"].asBool();
        if (visualsJson.isMember("No view punch")) visuals.noViewPunch = visualsJson["No view punch"].asBool();
        if (visualsJson.isMember("noHands")) visuals.noHands = visualsJson["noHands"].asBool();
        if (visualsJson.isMember("noSleeves")) visuals.noSleeves = visualsJson["noSleeves"].asBool();
        if (visualsJson.isMember("noWeapons")) visuals.noWeapons = visualsJson["noWeapons"].asBool();
        if (visualsJson.isMember("noSmoke")) visuals.noSmoke = visualsJson["noSmoke"].asBool();
        if (visualsJson.isMember("noBlur")) visuals.noBlur = visualsJson["noBlur"].asBool();
        if (visualsJson.isMember("noScopeOverlay")) visuals.noScopeOverlay = visualsJson["noScopeOverlay"].asBool();
        if (visualsJson.isMember("noGrass")) visuals.noGrass = visualsJson["noGrass"].asBool();
        if (visualsJson.isMember("noShadows")) visuals.noShadows = visualsJson["noShadows"].asBool();
        if (visualsJson.isMember("wireframeSmoke")) visuals.wireframeSmoke = visualsJson["wireframeSmoke"].asBool();
        if (visualsJson.isMember("Zoom")) visuals.zoom = visualsJson["Zoom"].asBool();
        if (visualsJson.isMember("Zoom key")) visuals.zoomKey = visualsJson["Zoom key"].asInt();
        if (visualsJson.isMember("thirdperson")) visuals.thirdperson = visualsJson["thirdperson"].asBool();
        if (visualsJson.isMember("thirdpersonKey")) visuals.thirdpersonKey = visualsJson["thirdpersonKey"].asInt();
        if (visualsJson.isMember("thirdpersonDistance")) visuals.thirdpersonDistance = visualsJson["thirdpersonDistance"].asInt();
        if (visualsJson.isMember("viewmodelFov")) visuals.viewmodelFov = visualsJson["viewmodelFov"].asInt();
        if (visualsJson.isMember("Fov")) visuals.fov = visualsJson["Fov"].asInt();
        if (visualsJson.isMember("farZ")) visuals.farZ = visualsJson["farZ"].asInt();
        if (visualsJson.isMember("flashReduction")) visuals.flashReduction = visualsJson["flashReduction"].asInt();
        if (visualsJson.isMember("brightness")) visuals.brightness = visualsJson["brightness"].asFloat();
        if (visualsJson.isMember("skybox")) visuals.skybox = visualsJson["skybox"].asInt();
        if (visualsJson.isMember("World")) {
            const auto& worldJson = visualsJson["World"];

            if (worldJson.isMember("Color")) {
                visuals.world.color[0] = worldJson["Color"][0].asFloat();
                visuals.world.color[1] = worldJson["Color"][1].asFloat();
                visuals.world.color[2] = worldJson["Color"][2].asFloat();
            }
            if (worldJson.isMember("Rainbow")) visuals.world.rainbow = worldJson["Rainbow"].asBool();
            if (worldJson.isMember("Rainbow speed")) visuals.world.rainbowSpeed = worldJson["Rainbow speed"].asFloat();
        }
        if (visualsJson.isMember("Deagle spinner")) visuals.deaglespinner = visualsJson["Deagle spinner"].asBool();
        if (visualsJson.isMember("Screen effect")) visuals.screenEffect = visualsJson["Screen effect"].asInt();
        if (visualsJson.isMember("Hit marker")) visuals.hitMarker = visualsJson["Hit marker"].asInt();
        if (visualsJson.isMember("Hit marker time")) visuals.hitMarkerTime = visualsJson["Hit marker time"].asFloat();
    }

    for (size_t i = 0; i < SkinChanger.size(); i++) {
        const auto& SkinChangerJson = json["SkinChanger"][i];
        auto& SkinChangerConfig = SkinChanger[i];

        if (SkinChangerJson.isMember("Enabled")) SkinChangerConfig.enabled = SkinChangerJson["Enabled"].asBool();
        if (SkinChangerJson.isMember("definition_vector_index")) SkinChangerConfig.definition_vector_index = SkinChangerJson["definition_vector_index"].asInt();
        if (SkinChangerJson.isMember("definition_index")) SkinChangerConfig.definition_index = SkinChangerJson["definition_index"].asInt();
        if (SkinChangerJson.isMember("entity_quality_vector_index")) SkinChangerConfig.entity_quality_vector_index = SkinChangerJson["entity_quality_vector_index"].asInt();
        if (SkinChangerJson.isMember("entity_quality_index")) SkinChangerConfig.entity_quality_index = SkinChangerJson["entity_quality_index"].asInt();
        if (SkinChangerJson.isMember("paint_kit_vector_index")) SkinChangerConfig.paint_kit_vector_index = SkinChangerJson["paint_kit_vector_index"].asInt();
        if (SkinChangerJson.isMember("paint_kit_index")) SkinChangerConfig.paint_kit_index = SkinChangerJson["paint_kit_index"].asInt();
        if (SkinChangerJson.isMember("definition_override_vector_index")) SkinChangerConfig.definition_override_vector_index = SkinChangerJson["definition_override_vector_index"].asInt();
        if (SkinChangerJson.isMember("definition_override_index")) SkinChangerConfig.definition_override_index = SkinChangerJson["definition_override_index"].asInt();
        if (SkinChangerJson.isMember("seed")) SkinChangerConfig.seed = SkinChangerJson["seed"].asInt();
        if (SkinChangerJson.isMember("stat_trak")) SkinChangerConfig.stat_trak = SkinChangerJson["stat_trak"].asInt();
        if (SkinChangerJson.isMember("wear")) SkinChangerConfig.wear = SkinChangerJson["wear"].asFloat();
        if (SkinChangerJson.isMember("custom_name")) strcpy_s(SkinChangerConfig.custom_name, sizeof(SkinChangerConfig.custom_name), SkinChangerJson["custom_name"].asCString());

        if (SkinChangerJson.isMember("stickers")) {
            for (size_t j = 0; j < SkinChangerConfig.stickers.size(); j++) {
                const auto& stickerJson = SkinChangerJson["stickers"][j];
                auto& stickerConfig = SkinChangerConfig.stickers[j];

                if (stickerJson.isMember("kit")) stickerConfig.kit = stickerJson["kit"].asInt();
                if (stickerJson.isMember("kit_vector_index")) stickerConfig.kit_vector_index = stickerJson["kit_vector_index"].asInt();
                if (stickerJson.isMember("wear")) stickerConfig.wear = stickerJson["wear"].asFloat();
                if (stickerJson.isMember("scale")) stickerConfig.scale = stickerJson["scale"].asFloat();
                if (stickerJson.isMember("rotation")) stickerConfig.rotation = stickerJson["rotation"].asFloat();
            }
        }
    }

    {
        const auto& soundJson = json["Sound"];

        if (soundJson.isMember("Chicken volume")) sound.chickenVolume = soundJson["Chicken volume"].asInt();

        if (soundJson.isMember("Players")) {
            for (size_t i = 0; i < sound.players.size(); i++) {
                const auto& playerJson = soundJson["Players"][i];
                auto& playerConfig = sound.players[i];

                if (playerJson.isMember("Master volume")) playerConfig.masterVolume = playerJson["Master volume"].asInt();
                if (playerJson.isMember("Headshot volume")) playerConfig.headshotVolume = playerJson["Headshot volume"].asInt();
                if (playerJson.isMember("Weapon volume")) playerConfig.weaponVolume = playerJson["Weapon volume"].asInt();
                if (playerJson.isMember("Footstep volume")) playerConfig.footstepVolume = playerJson["Footstep volume"].asInt();
            }
        }
    }


    {
        const auto& styleJson = json["Style"];

        if (styleJson.isMember("Menu style")) style.menuStyle = styleJson["Menu style"].asInt();
        if (styleJson.isMember("Menu colors")) style.menuColors = styleJson["Menu colors"].asInt();

        if (styleJson.isMember("Colors")) {
            const auto& colorsJson = styleJson["Colors"];

            ImGuiStyle& style = ImGui::GetStyle();

            for (int i = 0; i < ImGuiCol_COUNT; i++) {
                if (const char* name = ImGui::GetStyleColorName(i); colorsJson.isMember(name)) {
                    const auto& colorJson = styleJson["Colors"][name];
                    style.Colors[i].x = colorJson[0].asFloat();
                    style.Colors[i].y = colorJson[1].asFloat();
                    style.Colors[i].z = colorJson[2].asFloat();
                    style.Colors[i].w = colorJson[3].asFloat();
                }
            }
        }
    }

    {
        const auto& miscJson = json["Misc"];

        if (miscJson.isMember("Menu key")) misc.menuKey = miscJson["Menu key"].asInt();
        if (miscJson.isMember("Anti AFK kick")) misc.antiAfkKick = miscJson["Anti AFK kick"].asBool();
        if (miscJson.isMember("Auto strafe")) misc.autoStrafe = miscJson["Auto strafe"].asBool();
        if (miscJson.isMember("Bunny hop")) misc.fastmovementjump = miscJson["Bunny hop"].asBool();
        if (miscJson.isMember("Hitchance")) misc.fastmovementjum_hitchance = miscJson["Hitchance"].asInt();
        if (miscJson.isMember("Max Hops")) misc.fastmovementjum_maxhops = miscJson["Max Hops"].asInt();
        if (miscJson.isMember("Min Hops")) misc.fastmovementjum_minhops = miscJson["Min Hops"].asInt();
        if (miscJson.isMember("Restricted Limit")) misc.fastmovementjum_restricted = miscJson["Restricted Limit"].asInt();
        if (miscJson.isMember("Custom clan tag")) misc.customClanTag = miscJson["Custom clan tag"].asBool();
        if (miscJson.isMember("Clock tag")) misc.clocktag = miscJson["Clock tag"].asBool();
        if (miscJson.isMember("Clan tag")) strcpy_s(misc.clanTag, sizeof(misc.clanTag), miscJson["Clan tag"].asCString());
        if (miscJson.isMember("Animated clan tag")) misc.animatedClanTag = miscJson["Animated clan tag"].asBool();
        if (miscJson.isMember("Fast duck")) misc.fastDuck = miscJson["Fast duck"].asBool();
        if (miscJson.isMember("spacemove")) misc.spacemove = miscJson["spacemove"].asBool();
        if (miscJson.isMember("Sniper crosshair")) misc.scopegunpoint = miscJson["Sniper crosshair"].asBool();
        if (miscJson.isMember("Recoil crosshair")) misc.gunmovepoint = miscJson["Recoil crosshair"].asBool();
        if (miscJson.isMember("Auto pistol")) misc.autoPistol = miscJson["Auto pistol"].asBool();
        if (miscJson.isMember("Auto reload")) misc.autoReload = miscJson["Auto reload"].asBool();
        if (miscJson.isMember("Auto accept")) misc.autoAccept = miscJson["Auto accept"].asBool();
        if (miscJson.isMember("Radar hack")) misc.radarHack = miscJson["Radar hack"].asBool();
        if (miscJson.isMember("Reveal ranks")) misc.revealRanks = miscJson["Reveal ranks"].asBool();
        if (miscJson.isMember("Reveal money")) misc.revealMoney = miscJson["Reveal money"].asBool();

        if (const auto& spectatorList{ miscJson["Spectator list"] }; spectatorList.isObject()) {
            if (const auto& enabled{ spectatorList["Enabled"] }; enabled.isBool())
                misc.spectatorList.enabled = enabled.asBool();

            if (const auto& color{ spectatorList["Color"] }; color.isArray()) {
                misc.spectatorList.color[0] = color[0].asFloat();
                misc.spectatorList.color[1] = color[1].asFloat();
                misc.spectatorList.color[2] = color[2].asFloat();
            }
            if (const auto& rainbow{ spectatorList["Rainbow"] }; rainbow.isBool())
                misc.spectatorList.rainbow = rainbow.asBool();

            if (const auto& rainbowSpeed{ spectatorList["Rainbow speed"] }; rainbowSpeed.isDouble())
                misc.spectatorList.rainbowSpeed = rainbowSpeed.asFloat();
        }

        if (const auto& watermark{ miscJson["Watermark"] }; watermark.isObject()) {
            if (const auto& enabled{ watermark["Enabled"] }; enabled.isBool())
                misc.watermark = enabled.asBool();

            
        }

        if (miscJson.isMember("Fix animation LOD")) misc.fixAnimationLOD = miscJson["Fix animation LOD"].asBool();
        if (miscJson.isMember("Fix bone matrix")) misc.fixBoneMatrix = miscJson["Fix bone matrix"].asBool();
        if (miscJson.isMember("Fix movement")) misc.fixMovement = miscJson["Fix movement"].asBool();
        if (miscJson.isMember("Disable model occlusion")) misc.disableModelOcclusion = miscJson["Disable model occlusion"].asBool();
        if (miscJson.isMember("Kill message")) misc.killMessage = miscJson["Kill message"].asBool();
        if (miscJson.isMember("Kill message string")) strcpy_s(misc.killMessageString, sizeof(misc.killMessageString), miscJson["Kill message string"].asCString());
        if (miscJson.isMember("Name stealer"))  misc.nameStealer = miscJson["Name stealer"].asBool();
        if (miscJson.isMember("Disable HUD blur"))  misc.disablePanoramablur = miscJson["Disable HUD blur"].asBool();
        if (miscJson.isMember("Vote text")) strcpy_s(misc.voteText, sizeof(misc.voteText), miscJson["Vote text"].asCString());
        if (miscJson.isMember("Ban color")) misc.banColor = miscJson["Ban color"].asInt();
        if (miscJson.isMember("Ban text")) strcpy_s(misc.banText, sizeof(misc.banText), miscJson["Ban text"].asCString());
        if (miscJson.isMember("Fast plant")) misc.fastPlant = miscJson["Fast plant"].asBool();

        if (const auto& bombTimer{ miscJson["Bomb timer"] }; bombTimer.isObject()) {
            if (const auto& enabled{ bombTimer["Enabled"] }; enabled.isBool())
                misc.bombTimer.enabled = enabled.asBool();

            if (const auto& color{ bombTimer["Color"] }; color.isArray()) {
                misc.bombTimer.color[0] = color[0].asFloat();
                misc.bombTimer.color[1] = color[1].asFloat();
                misc.bombTimer.color[2] = color[2].asFloat();
            }
            if (const auto& rainbow{ bombTimer["Rainbow"] }; rainbow.isBool())
                misc.bombTimer.rainbow = rainbow.asBool();

            if (const auto& rainbowSpeed{ bombTimer["Rainbow speed"] }; rainbowSpeed.isDouble())
                misc.bombTimer.rainbowSpeed = rainbowSpeed.asFloat();
        }

        if (miscJson.isMember("Quick reload")) misc.quickReload = miscJson["Quick reload"].asBool();
        if (miscJson.isMember("Prepare revolver")) misc.prepareRevolver = miscJson["Prepare revolver"].asBool();
        if (miscJson.isMember("Prepare revolver key")) misc.prepareRevolverKey = miscJson["Prepare revolver key"].asInt();
        if (miscJson.isMember("Hit sound")) misc.hitSound = miscJson["Hit sound"].asInt();
        if (miscJson.isMember("Choked packets")) misc.chokedPackets = miscJson["Choked packets"].asInt();
        if (miscJson.isMember("Choked packets key")) misc.chokedPacketsKey = miscJson["Choked packets key"].asInt();
        if (miscJson.isMember("Quick healthshot key")) misc.quickHealthshotKey = miscJson["Quick healthshot key"].asInt();
        if (miscJson.isMember("Grenade predict")) misc.nadePredict = miscJson["Grenade predict"].asBool();
        if (miscJson.isMember("Grenade trajectory")) misc.nadeTrajectory = miscJson["Grenade trajectory"].asBool();
        if (miscJson.isMember("Fix tablet signal")) misc.fixTabletSignal = miscJson["Fix tablet signal"].asBool();
        if (miscJson.isMember("Max angle delta")) misc.maxAngleDelta = miscJson["Max angle delta"].asFloat();
    }

    
}

void Config::save(size_t id) const noexcept
{
    if (!std::filesystem::is_directory(path)) {
        std::filesystem::remove(path);
        std::filesystem::create_directory(path);
    }

    std::ofstream out{ path / configs[id] };

    if (!out.good())
        return;

    Json::Value json;

    for (size_t i = 0; i < aimhelper.size(); i++) {
        auto& aimhelperJson = json["Aimbot"][i];
        const auto& aimhelperConfig = aimhelper[i];

        aimhelperJson["Enabled"] = aimhelperConfig.enabled;
        aimhelperJson["On key"] = aimhelperConfig.onKey;
        aimhelperJson["Key"] = aimhelperConfig.key;
        aimhelperJson["Key mode"] = aimhelperConfig.keyMode;
        aimhelperJson["Aimlock"] = aimhelperConfig.aimlock;
        aimhelperJson["Silent"] = aimhelperConfig.silent;
        aimhelperJson["Friendly fire"] = aimhelperConfig.friendlyFire;
        aimhelperJson["Visible only"] = aimhelperConfig.visibleOnly;
        aimhelperJson["Scoped only"] = aimhelperConfig.scopedOnly;
        aimhelperJson["Ignore flash"] = aimhelperConfig.ignoreFlash;;
        aimhelperJson["Ignore smoke"] = aimhelperConfig.ignoreSmoke;
        aimhelperJson["Auto shot"] = aimhelperConfig.autoShot;
        aimhelperJson["Auto scope"] = aimhelperConfig.autoScope;
        aimhelperJson["Recoil-based fov"] = aimhelperConfig.recoilbasedFov;
        aimhelperJson["Fov"] = aimhelperConfig.fov;
        aimhelperJson["Smooth"] = aimhelperConfig.smooth;
        aimhelperJson["Bone"] = aimhelperConfig.bone;
        aimhelperJson["Recoil control X"] = aimhelperConfig.recoilControlX;
        aimhelperJson["Recoil control Y"] = aimhelperConfig.recoilControlY;
        aimhelperJson["Max aim inaccuracy"] = aimhelperConfig.maxAimInaccuracy;
        aimhelperJson["Max shot inaccuracy"] = aimhelperConfig.maxShotInaccuracy;
        aimhelperJson["Min damage"] = aimhelperConfig.minDamage;
        aimhelperJson["Killshot"] = aimhelperConfig.killshot;
        aimhelperJson["Between shots"] = aimhelperConfig.betweenShots;
    }

    for (size_t i = 0; i < fastshotbot.size(); i++) {
        auto& fastshotbotJson = json["Triggerbot"][i];
        const auto& fastshotbotConfig = fastshotbot[i];

        fastshotbotJson["Enabled"] = fastshotbotConfig.enabled;
        fastshotbotJson["On key"] = fastshotbotConfig.onKey;
        fastshotbotJson["Key"] = fastshotbotConfig.key;
        fastshotbotJson["Friendly fire"] = fastshotbotConfig.friendlyFire;
        fastshotbotJson["Scoped only"] = fastshotbotConfig.scopedOnly;
        fastshotbotJson["Ignore flash"] = fastshotbotConfig.ignoreFlash;
        fastshotbotJson["Ignore smoke"] = fastshotbotConfig.ignoreSmoke;
        fastshotbotJson["Hitgroup"] = fastshotbotConfig.hitgroup;
        fastshotbotJson["Shot delay"] = fastshotbotConfig.shotDelay;
        fastshotbotJson["Min damage"] = fastshotbotConfig.minDamage;
        fastshotbotJson["Killshot"] = fastshotbotConfig.killshot;
    }

    {
        auto& timereverseJson = json["Backtrack"];
        timereverseJson["Enabled"] = timereverse.enabled;
        timereverseJson["Ignore smoke"] = timereverse.ignoreSmoke;
        timereverseJson["Recoil based fov"] = timereverse.recoilBasedFov;
        timereverseJson["Time limit"] = timereverse.timeLimit;
    }

    for (size_t i = 0; i < bodyoutline.size(); i++) {
        auto& bodyoutlineJson = json["Glow"][i];
        const auto& bodyoutlineConfig = bodyoutline[i];

        bodyoutlineJson["Enabled"] = bodyoutlineConfig.enabled;
        bodyoutlineJson["healthBased"] = bodyoutlineConfig.healthBased;
        bodyoutlineJson["thickness"] = bodyoutlineConfig.thickness;
        bodyoutlineJson["alpha"] = bodyoutlineConfig.alpha;
        bodyoutlineJson["style"] = bodyoutlineConfig.style;

        {
            auto& colorJson = bodyoutlineJson["Color"];
            const auto& colorConfig = bodyoutlineConfig.color;

            colorJson["Color"][0] = colorConfig.color[0];
            colorJson["Color"][1] = colorConfig.color[1];
            colorJson["Color"][2] = colorConfig.color[2];

            colorJson["Rainbow"] = colorConfig.rainbow;
            colorJson["Rainbow speed"] = colorConfig.rainbowSpeed;
        }
    }

    for (size_t i = 0; i < colouredbody.size(); i++) {
        auto& colouredbodyJson = json["Chams"][i];
        const auto& colouredbodyConfig = colouredbody[i];

        for (size_t j = 0; j < colouredbody[0].materials.size(); j++) {
            auto& materialsJson = colouredbodyJson[j];
            const auto& materialsConfig = colouredbody[i].materials[j];

            materialsJson["Enabled"] = materialsConfig.enabled;
            materialsJson["Health based"] = materialsConfig.healthBased;
            materialsJson["Blinking"] = materialsConfig.blinking;
            materialsJson["Material"] = materialsConfig.material;
            materialsJson["Wireframe"] = materialsConfig.wireframe;

            {
                auto& colorJson = materialsJson["Color"];
                const auto& colorConfig = materialsConfig.color;

                colorJson["Color"][0] = colorConfig.color[0];
                colorJson["Color"][1] = colorConfig.color[1];
                colorJson["Color"][2] = colorConfig.color[2];

                colorJson["Rainbow"] = colorConfig.rainbow;
                colorJson["Rainbow speed"] = colorConfig.rainbowSpeed;
            }

            materialsJson["Alpha"] = materialsConfig.alpha;
        }
    }

    for (size_t i = 0; i < esp.players.size(); i++) {
        auto& espJson = json["esp"]["Players"][i];
        const auto& espConfig = esp.players[i];

        espJson["Enabled"] = espConfig.enabled;
        espJson["Font"] = espConfig.font;

        {
            auto& snaplinesJson = espJson["Snaplines"];
            const auto& snaplinesConfig = espConfig.snaplines;

            snaplinesJson["Enabled"] = snaplinesConfig.enabled;
            snaplinesJson["Color"][0] = snaplinesConfig.color[0];
            snaplinesJson["Color"][1] = snaplinesConfig.color[1];
            snaplinesJson["Color"][2] = snaplinesConfig.color[2];
            snaplinesJson["Rainbow"] = snaplinesConfig.rainbow;
            snaplinesJson["Rainbow speed"] = snaplinesConfig.rainbowSpeed;
        }

        {
            auto& eyeTracesJson = espJson["Eye traces"];
            const auto& eyeTracesConfig = espConfig.eyeTraces;

            eyeTracesJson["Enabled"] = eyeTracesConfig.enabled;
            eyeTracesJson["Color"][0] = eyeTracesConfig.color[0];
            eyeTracesJson["Color"][1] = eyeTracesConfig.color[1];
            eyeTracesJson["Color"][2] = eyeTracesConfig.color[2];
            eyeTracesJson["Rainbow"] = eyeTracesConfig.rainbow;
            eyeTracesJson["Rainbow speed"] = eyeTracesConfig.rainbowSpeed;
        }

        {
            auto& boxJson = espJson["Box"];
            const auto& boxConfig = espConfig.box;

            boxJson["Enabled"] = boxConfig.enabled;
            boxJson["Color"][0] = boxConfig.color[0];
            boxJson["Color"][1] = boxConfig.color[1];
            boxJson["Color"][2] = boxConfig.color[2];
            boxJson["Rainbow"] = boxConfig.rainbow;
            boxJson["Rainbow speed"] = boxConfig.rainbowSpeed;
        }

        espJson["Box type"] = espConfig.boxType;

        {
            auto& nameJson = espJson["Name"];
            const auto& nameConfig = espConfig.name;

            nameJson["Enabled"] = nameConfig.enabled;
            nameJson["Color"][0] = nameConfig.color[0];
            nameJson["Color"][1] = nameConfig.color[1];
            nameJson["Color"][2] = nameConfig.color[2];
            nameJson["Rainbow"] = nameConfig.rainbow;
            nameJson["Rainbow speed"] = nameConfig.rainbowSpeed;
        }

        {
            auto& healthJson = espJson["Health"];
            const auto& healthConfig = espConfig.health;

            healthJson["Enabled"] = healthConfig.enabled;
            healthJson["Color"][0] = healthConfig.color[0];
            healthJson["Color"][1] = healthConfig.color[1];
            healthJson["Color"][2] = healthConfig.color[2];
            healthJson["Rainbow"] = healthConfig.rainbow;
            healthJson["Rainbow speed"] = healthConfig.rainbowSpeed;
        }

        {
            auto& healthBarJson = espJson["Health bar"];
            const auto& healthBarConfig = espConfig.healthBar;

            healthBarJson["Enabled"] = healthBarConfig.enabled;
            healthBarJson["Color"][0] = healthBarConfig.color[0];
            healthBarJson["Color"][1] = healthBarConfig.color[1];
            healthBarJson["Color"][2] = healthBarConfig.color[2];
            healthBarJson["Rainbow"] = healthBarConfig.rainbow;
            healthBarJson["Rainbow speed"] = healthBarConfig.rainbowSpeed;
        }

        {
            auto& armorJson = espJson["Armor"];
            const auto& armorConfig = espConfig.armor;

            armorJson["Enabled"] = armorConfig.enabled;
            armorJson["Color"][0] = armorConfig.color[0];
            armorJson["Color"][1] = armorConfig.color[1];
            armorJson["Color"][2] = armorConfig.color[2];
            armorJson["Rainbow"] = armorConfig.rainbow;
            armorJson["Rainbow speed"] = armorConfig.rainbowSpeed;
        }

        {
            auto& armorBarJson = espJson["Armor bar"];
            const auto& armorBarConfig = espConfig.armorBar;

            armorBarJson["Enabled"] = armorBarConfig.enabled;
            armorBarJson["Color"][0] = armorBarConfig.color[0];
            armorBarJson["Color"][1] = armorBarConfig.color[1];
            armorBarJson["Color"][2] = armorBarConfig.color[2];
            armorBarJson["Rainbow"] = armorBarConfig.rainbow;
            armorBarJson["Rainbow speed"] = armorBarConfig.rainbowSpeed;
        }

        {
            auto& moneyJson = espJson["Money"];
            const auto& moneyConfig = espConfig.money;

            moneyJson["Enabled"] = moneyConfig.enabled;
            moneyJson["Color"][0] = moneyConfig.color[0];
            moneyJson["Color"][1] = moneyConfig.color[1];
            moneyJson["Color"][2] = moneyConfig.color[2];
            moneyJson["Rainbow"] = moneyConfig.rainbow;
            moneyJson["Rainbow speed"] = moneyConfig.rainbowSpeed;
        }

        {
            auto& headDotJson = espJson["Head dot"];
            const auto& headDotConfig = espConfig.headDot;

            headDotJson["Enabled"] = headDotConfig.enabled;
            headDotJson["Color"][0] = headDotConfig.color[0];
            headDotJson["Color"][1] = headDotConfig.color[1];
            headDotJson["Color"][2] = headDotConfig.color[2];
            headDotJson["Rainbow"] = headDotConfig.rainbow;
            headDotJson["Rainbow speed"] = headDotConfig.rainbowSpeed;
        }

        {
            auto& activeWeaponJson = espJson["Active weapon"];
            const auto& activeWeaponConfig = espConfig.activeWeapon;

            activeWeaponJson["Enabled"] = activeWeaponConfig.enabled;
            activeWeaponJson["Color"][0] = activeWeaponConfig.color[0];
            activeWeaponJson["Color"][1] = activeWeaponConfig.color[1];
            activeWeaponJson["Color"][2] = activeWeaponConfig.color[2];
            activeWeaponJson["Rainbow"] = activeWeaponConfig.rainbow;
            activeWeaponJson["Rainbow speed"] = activeWeaponConfig.rainbowSpeed;
        }

        {
            auto& outlineJson = espJson["Outline"];
            const auto& outlineConfig = espConfig.outline;

            outlineJson["Enabled"] = outlineConfig.enabled;
            outlineJson["Color"][0] = outlineConfig.color[0];
            outlineJson["Color"][1] = outlineConfig.color[1];
            outlineJson["Color"][2] = outlineConfig.color[2];
            outlineJson["Rainbow"] = outlineConfig.rainbow;
            outlineJson["Rainbow speed"] = outlineConfig.rainbowSpeed;
        }

        {
            auto& distanceJson = espJson["Distance"];
            const auto& distanceConfig = espConfig.distance;

            distanceJson["Enabled"] = distanceConfig.enabled;
            distanceJson["Color"][0] = distanceConfig.color[0];
            distanceJson["Color"][1] = distanceConfig.color[1];
            distanceJson["Color"][2] = distanceConfig.color[2];
            distanceJson["Rainbow"] = distanceConfig.rainbow;
            distanceJson["Rainbow speed"] = distanceConfig.rainbowSpeed;
        }

        espJson["Max distance"] = espConfig.maxDistance;
    }

    {
        auto& espJson = json["esp"]["Weapons"];
        const auto& espConfig = esp.weapon;

        espJson["Enabled"] = espConfig.enabled;
        espJson["Font"] = espConfig.font;
        {
            auto& snaplinesJson = espJson["Snaplines"];
            const auto& snaplinesConfig = espConfig.snaplines;

            snaplinesJson["Enabled"] = snaplinesConfig.enabled;
            snaplinesJson["Color"][0] = snaplinesConfig.color[0];
            snaplinesJson["Color"][1] = snaplinesConfig.color[1];
            snaplinesJson["Color"][2] = snaplinesConfig.color[2];
            snaplinesJson["Rainbow"] = snaplinesConfig.rainbow;
            snaplinesJson["Rainbow speed"] = snaplinesConfig.rainbowSpeed;
        }

        {
            auto& boxJson = espJson["Box"];
            const auto& boxConfig = espConfig.box;

            boxJson["Enabled"] = boxConfig.enabled;
            boxJson["Color"][0] = boxConfig.color[0];
            boxJson["Color"][1] = boxConfig.color[1];
            boxJson["Color"][2] = boxConfig.color[2];
            boxJson["Rainbow"] = boxConfig.rainbow;
            boxJson["Rainbow speed"] = boxConfig.rainbowSpeed;
        }

        espJson["Box type"] = espConfig.boxType;

        {
            auto& outlineJson = espJson["Outline"];
            const auto& outlineConfig = espConfig.outline;

            outlineJson["Enabled"] = outlineConfig.enabled;
            outlineJson["Color"][0] = outlineConfig.color[0];
            outlineJson["Color"][1] = outlineConfig.color[1];
            outlineJson["Color"][2] = outlineConfig.color[2];
            outlineJson["Rainbow"] = outlineConfig.rainbow;
            outlineJson["Rainbow speed"] = outlineConfig.rainbowSpeed;
        }

        {
            auto& nameJson = espJson["Name"];
            const auto& nameConfig = espConfig.name;

            nameJson["Enabled"] = nameConfig.enabled;
            nameJson["Color"][0] = nameConfig.color[0];
            nameJson["Color"][1] = nameConfig.color[1];
            nameJson["Color"][2] = nameConfig.color[2];
            nameJson["Rainbow"] = nameConfig.rainbow;
            nameJson["Rainbow speed"] = nameConfig.rainbowSpeed;
        }

        {
            auto& distanceJson = espJson["Distance"];
            const auto& distanceConfig = espConfig.distance;

            distanceJson["Enabled"] = distanceConfig.enabled;
            distanceJson["Color"][0] = distanceConfig.color[0];
            distanceJson["Color"][1] = distanceConfig.color[1];
            distanceJson["Color"][2] = distanceConfig.color[2];
            distanceJson["Rainbow"] = distanceConfig.rainbow;
            distanceJson["Rainbow speed"] = distanceConfig.rainbowSpeed;
        }

        espJson["Max distance"] = espConfig.maxDistance;
    }

    for (size_t i = 0; i < esp.dangerZone.size(); i++) {
        auto& espJson = json["esp"]["Danger Zone"][i];
        const auto& espConfig = esp.dangerZone[i];

        espJson["Enabled"] = espConfig.enabled;
        espJson["Font"] = espConfig.font;
        {
            auto& snaplinesJson = espJson["Snaplines"];
            const auto& snaplinesConfig = espConfig.snaplines;

            snaplinesJson["Enabled"] = snaplinesConfig.enabled;
            snaplinesJson["Color"][0] = snaplinesConfig.color[0];
            snaplinesJson["Color"][1] = snaplinesConfig.color[1];
            snaplinesJson["Color"][2] = snaplinesConfig.color[2];
            snaplinesJson["Rainbow"] = snaplinesConfig.rainbow;
            snaplinesJson["Rainbow speed"] = snaplinesConfig.rainbowSpeed;
        }

        {
            auto& boxJson = espJson["Box"];
            const auto& boxConfig = espConfig.box;

            boxJson["Enabled"] = boxConfig.enabled;
            boxJson["Color"][0] = boxConfig.color[0];
            boxJson["Color"][1] = boxConfig.color[1];
            boxJson["Color"][2] = boxConfig.color[2];
            boxJson["Rainbow"] = boxConfig.rainbow;
            boxJson["Rainbow speed"] = boxConfig.rainbowSpeed;
        }

        espJson["Box type"] = espConfig.boxType;
        
        {
            auto& outlineJson = espJson["Outline"];
            const auto& outlineConfig = espConfig.outline;

            outlineJson["Enabled"] = outlineConfig.enabled;
            outlineJson["Color"][0] = outlineConfig.color[0];
            outlineJson["Color"][1] = outlineConfig.color[1];
            outlineJson["Color"][2] = outlineConfig.color[2];
            outlineJson["Rainbow"] = outlineConfig.rainbow;
            outlineJson["Rainbow speed"] = outlineConfig.rainbowSpeed;
        }

        {
            auto& nameJson = espJson["Name"];
            const auto& nameConfig = espConfig.name;

            nameJson["Enabled"] = nameConfig.enabled;
            nameJson["Color"][0] = nameConfig.color[0];
            nameJson["Color"][1] = nameConfig.color[1];
            nameJson["Color"][2] = nameConfig.color[2];
            nameJson["Rainbow"] = nameConfig.rainbow;
            nameJson["Rainbow speed"] = nameConfig.rainbowSpeed;
        }

        {
            auto& distanceJson = espJson["Distance"];
            const auto& distanceConfig = espConfig.distance;

            distanceJson["Enabled"] = distanceConfig.enabled;
            distanceJson["Color"][0] = distanceConfig.color[0];
            distanceJson["Color"][1] = distanceConfig.color[1];
            distanceJson["Color"][2] = distanceConfig.color[2];
            distanceJson["Rainbow"] = distanceConfig.rainbow;
            distanceJson["Rainbow speed"] = distanceConfig.rainbowSpeed;
        }

        espJson["Max distance"] = espConfig.maxDistance;
    }

    for (size_t i = 0; i < esp.projectiles.size(); i++) {
        auto& espJson = json["esp"]["Projectiles"][i];
        const auto& espConfig = esp.projectiles[i];

        espJson["Enabled"] = espConfig.enabled;
        espJson["Font"] = espConfig.font;
        {
            auto& snaplinesJson = espJson["Snaplines"];
            const auto& snaplinesConfig = espConfig.snaplines;

            snaplinesJson["Enabled"] = snaplinesConfig.enabled;
            snaplinesJson["Color"][0] = snaplinesConfig.color[0];
            snaplinesJson["Color"][1] = snaplinesConfig.color[1];
            snaplinesJson["Color"][2] = snaplinesConfig.color[2];
            snaplinesJson["Rainbow"] = snaplinesConfig.rainbow;
            snaplinesJson["Rainbow speed"] = snaplinesConfig.rainbowSpeed;
        }

        {
            auto& boxJson = espJson["Box"];
            const auto& boxConfig = espConfig.box;

            boxJson["Enabled"] = boxConfig.enabled;
            boxJson["Color"][0] = boxConfig.color[0];
            boxJson["Color"][1] = boxConfig.color[1];
            boxJson["Color"][2] = boxConfig.color[2];
            boxJson["Rainbow"] = boxConfig.rainbow;
            boxJson["Rainbow speed"] = boxConfig.rainbowSpeed;
        }

        espJson["Box type"] = espConfig.boxType;

        {
            auto& outlineJson = espJson["Outline"];
            const auto& outlineConfig = espConfig.outline;

            outlineJson["Enabled"] = outlineConfig.enabled;
            outlineJson["Color"][0] = outlineConfig.color[0];
            outlineJson["Color"][1] = outlineConfig.color[1];
            outlineJson["Color"][2] = outlineConfig.color[2];
            outlineJson["Rainbow"] = outlineConfig.rainbow;
            outlineJson["Rainbow speed"] = outlineConfig.rainbowSpeed;
        }

        {
            auto& nameJson = espJson["Name"];
            const auto& nameConfig = espConfig.name;

            nameJson["Enabled"] = nameConfig.enabled;
            nameJson["Color"][0] = nameConfig.color[0];
            nameJson["Color"][1] = nameConfig.color[1];
            nameJson["Color"][2] = nameConfig.color[2];
            nameJson["Rainbow"] = nameConfig.rainbow;
            nameJson["Rainbow speed"] = nameConfig.rainbowSpeed;
        }

        {
            auto& distanceJson = espJson["Distance"];
            const auto& distanceConfig = espConfig.distance;

            distanceJson["Enabled"] = distanceConfig.enabled;
            distanceJson["Color"][0] = distanceConfig.color[0];
            distanceJson["Color"][1] = distanceConfig.color[1];
            distanceJson["Color"][2] = distanceConfig.color[2];
            distanceJson["Rainbow"] = distanceConfig.rainbow;
            distanceJson["Rainbow speed"] = distanceConfig.rainbowSpeed;
        }

        espJson["Max distance"] = espConfig.maxDistance;
    }

    {
        auto& visualsJson = json["visuals"];
        visualsJson["disablePostProcessing"] = visuals.disablePostProcessing;
        visualsJson["inverseRagdollGravity"] = visuals.inverseRagdollGravity;
        visualsJson["noFog"] = visuals.noFog;
        visualsJson["no3dSky"] = visuals.no3dSky;
        visualsJson["No aim punch"] = visuals.noAimPunch;
        visualsJson["No view punch"] = visuals.noViewPunch;
        visualsJson["noHands"] = visuals.noHands;
        visualsJson["noSleeves"] = visuals.noSleeves;
        visualsJson["noWeapons"] = visuals.noWeapons;
        visualsJson["noSmoke"] = visuals.noSmoke;
        visualsJson["noBlur"] = visuals.noBlur;
        visualsJson["noScopeOverlay"] = visuals.noScopeOverlay;
        visualsJson["noGrass"] = visuals.noGrass;
        visualsJson["noShadows"] = visuals.noShadows;
        visualsJson["wireframeSmoke"] = visuals.wireframeSmoke;
        visualsJson["Zoom"] = visuals.zoom;
        visualsJson["Zoom key"] = visuals.zoomKey;
        visualsJson["thirdperson"] = visuals.thirdperson;
        visualsJson["thirdpersonKey"] = visuals.thirdpersonKey;
        visualsJson["thirdpersonDistance"] = visuals.thirdpersonDistance;
        visualsJson["viewmodelFov"] = visuals.viewmodelFov;
        visualsJson["Fov"] = visuals.fov;
        visualsJson["farZ"] = visuals.farZ;
        visualsJson["flashReduction"] = visuals.flashReduction;
        visualsJson["brightness"] = visuals.brightness;
        visualsJson["skybox"] = visuals.skybox;

        {
            auto& worldJson = visualsJson["World"];
            worldJson["Color"][0] = visuals.world.color[0];
            worldJson["Color"][1] = visuals.world.color[1];
            worldJson["Color"][2] = visuals.world.color[2];
            worldJson["Rainbow"] = visuals.world.rainbow;
            worldJson["Rainbow speed"] = visuals.world.rainbowSpeed;
        }

        visualsJson["Deagle spinner"] = visuals.deaglespinner;
        visualsJson["Screen effect"] = visuals.screenEffect;
        visualsJson["Hit marker"] = visuals.hitMarker;
        visualsJson["Hit marker time"] = visuals.hitMarkerTime;
    }

    for (size_t i = 0; i < SkinChanger.size(); i++) {
        auto& SkinChangerJson = json["SkinChanger"][i];
        const auto& SkinChangerConfig = SkinChanger[i];

        SkinChangerJson["Enabled"] = SkinChangerConfig.enabled;
        SkinChangerJson["definition_vector_index"] = SkinChangerConfig.definition_vector_index;
        SkinChangerJson["definition_index"] = SkinChangerConfig.definition_index;
        SkinChangerJson["entity_quality_vector_index"] = SkinChangerConfig.entity_quality_vector_index;
        SkinChangerJson["entity_quality_index"] = SkinChangerConfig.entity_quality_index;
        SkinChangerJson["paint_kit_vector_index"] = SkinChangerConfig.paint_kit_vector_index;
        SkinChangerJson["paint_kit_index"] = SkinChangerConfig.paint_kit_index;
        SkinChangerJson["definition_override_vector_index"] = SkinChangerConfig.definition_override_vector_index;
        SkinChangerJson["definition_override_index"] = SkinChangerConfig.definition_override_index;
        SkinChangerJson["seed"] = SkinChangerConfig.seed;
        SkinChangerJson["stat_trak"] = SkinChangerConfig.stat_trak;
        SkinChangerJson["wear"] = SkinChangerConfig.wear;
        SkinChangerJson["custom_name"] = SkinChangerConfig.custom_name;

        for (size_t j = 0; j < SkinChangerConfig.stickers.size(); j++) {
            auto& stickerJson = SkinChangerJson["stickers"][j];
            const auto& stickerConfig = SkinChangerConfig.stickers[j];

            stickerJson["kit"] = stickerConfig.kit;
            stickerJson["kit_vector_index"] = stickerConfig.kit_vector_index;
            stickerJson["wear"] = stickerConfig.wear;
            stickerJson["scale"] = stickerConfig.scale;
            stickerJson["rotation"] = stickerConfig.rotation;
        }
    }

    {
        auto& soundJson = json["Sound"];

        soundJson["Chicken volume"] = sound.chickenVolume;

        for (size_t i = 0; i < sound.players.size(); i++) {
            auto& playerJson = soundJson["Players"][i];
            const auto& playerConfig = sound.players[i];

            playerJson["Master volume"] = playerConfig.masterVolume;
            playerJson["Headshot volume"] = playerConfig.headshotVolume;
            playerJson["Weapon volume"] = playerConfig.weaponVolume;
            playerJson["Footstep volume"] = playerConfig.footstepVolume;
        }
    }

    {
        auto& styleJson = json["Style"];

        styleJson["Menu style"] = style.menuStyle;
        styleJson["Menu colors"] = style.menuColors;

        auto& colorsJson = styleJson["Colors"];

        const ImGuiStyle& style = ImGui::GetStyle();

        for (int i = 0; i < ImGuiCol_COUNT; i++) {
            auto& colorJson = styleJson["Colors"][ImGui::GetStyleColorName(i)];
            colorJson[0] = style.Colors[i].x;
            colorJson[1] = style.Colors[i].y;
            colorJson[2] = style.Colors[i].z;
            colorJson[3] = style.Colors[i].w;
        }
    }

    {
        auto& miscJson = json["Misc"];
        
        miscJson["Menu key"] = misc.menuKey;
        miscJson["Anti AFK kick"] = misc.antiAfkKick;
        miscJson["Auto strafe"] = misc.autoStrafe;
        miscJson["Bunny hop"] = misc.fastmovementjump;
        miscJson["Hitchance"] = misc.fastmovementjum_hitchance;
        miscJson["Max Hops"] = misc.fastmovementjum_maxhops;
        miscJson["Min Hops"] = misc.fastmovementjum_minhops;
        miscJson["Restricted Limit"] = misc.fastmovementjum_restricted;
        miscJson["Custom clan tag"] = misc.customClanTag;
        miscJson["Clock tag"] = misc.clocktag;
        miscJson["Clan tag"] = misc.clanTag;
        miscJson["Animated clan tag"] = misc.animatedClanTag;
        miscJson["Fast duck"] = misc.fastDuck;
        miscJson["spacemove"] = misc.spacemove;
        miscJson["Sniper crosshair"] = misc.scopegunpoint;
        miscJson["Recoil crosshair"] = misc.gunmovepoint;
        miscJson["Auto pistol"] = misc.autoPistol;
        miscJson["Auto reload"] = misc.autoReload;
        miscJson["Auto accept"] = misc.autoAccept;
        miscJson["Radar hack"] = misc.radarHack;
        miscJson["Reveal ranks"] = misc.revealRanks;
        miscJson["Reveal money"] = misc.revealMoney;

        {
            auto& spectatorListJson = miscJson["Spectator list"];
            spectatorListJson["Enabled"] = misc.spectatorList.enabled;
            spectatorListJson["Color"][0] = misc.spectatorList.color[0];
            spectatorListJson["Color"][1] = misc.spectatorList.color[1];
            spectatorListJson["Color"][2] = misc.spectatorList.color[2];
            spectatorListJson["Rainbow"] = misc.spectatorList.rainbow;
            spectatorListJson["Rainbow speed"] = misc.spectatorList.rainbowSpeed;
        }

        {
            auto& watermarkJson = miscJson["Watermark"];
            watermarkJson["Enabled"] = misc.watermark;
        }

        miscJson["Fix animation LOD"] = misc.fixAnimationLOD;
        miscJson["Fix bone matrix"] = misc.fixBoneMatrix;
        miscJson["Fix movement"] = misc.fixMovement;
        miscJson["Disable model occlusion"] = misc.disableModelOcclusion;
        miscJson["Kill message"] = misc.killMessage;
        miscJson["Kill message string"] = misc.killMessageString;
        miscJson["Name stealer"] = misc.nameStealer;
        miscJson["Disable HUD blur"] = misc.disablePanoramablur;
        miscJson["Vote text"] = misc.voteText;
        miscJson["Ban color"] = misc.banColor;
        miscJson["Ban text"] = misc.banText;
        miscJson["Fast plant"] = misc.fastPlant;

        {
            auto& bombTimerJson = miscJson["Bomb timer"];
            bombTimerJson["Enabled"] = misc.bombTimer.enabled;
            bombTimerJson["Color"][0] = misc.bombTimer.color[0];
            bombTimerJson["Color"][1] = misc.bombTimer.color[1];
            bombTimerJson["Color"][2] = misc.bombTimer.color[2];
            bombTimerJson["Rainbow"] = misc.bombTimer.rainbow;
            bombTimerJson["Rainbow speed"] = misc.bombTimer.rainbowSpeed;
        }

        miscJson["Quick reload"] = misc.quickReload;
        miscJson["Prepare revolver"] = misc.prepareRevolver;
        miscJson["Prepare revolver key"] = misc.prepareRevolverKey;
        miscJson["Hit sound"] = misc.hitSound;
        miscJson["Choked packets"] = misc.chokedPackets;
        miscJson["Choked packets key"] = misc.chokedPacketsKey;
        miscJson["Quick healthshot key"] = misc.quickHealthshotKey;
        miscJson["Grenade predict"] = misc.nadePredict;
        miscJson["Grenade trajectory"] = misc.nadeTrajectory;
        miscJson["Fix tablet signal"] = misc.fixTabletSignal;
        miscJson["Max angle delta"] = misc.maxAngleDelta;
    }


    out << json;
    out.close();
}

void Config::add(const char* name) noexcept
{
    if (*name && std::find(std::cbegin(configs), std::cend(configs), name) == std::cend(configs))
        configs.emplace_back(name);
}

void Config::remove(size_t id) noexcept
{
    std::filesystem::remove(path / configs[id]);
    configs.erase(configs.cbegin() + id);
}

void Config::rename(size_t item, const char* newName) noexcept
{
    std::filesystem::rename(path / configs[item], path / newName);
    configs[item] = newName;
}

void Config::reset() noexcept
{
    aimhelper = { };
    fastshotbot = { };
    timereverse = { };
    bodyoutline = { };
	colouredbody = { };
    esp = { };
    visuals = { };
    SkinChanger = { };
    sound = { };
    style = { };
    misc = { };
}
