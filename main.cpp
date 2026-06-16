#include <Geode/Geode.hpp>
#include <Geode/modify/GJBaseGameLayer.hpp>
#include <Geode/modify/PlayerObject.hpp>
#include <Geode/modify/MenuLayer.hpp>
#include <vector>

using namespace geode::prelude;

bool g_noclipActive = true;
bool g_botRecording = false;
bool g_botReplaying = false;
bool g_layoutModeActive = false;
float g_customTargetFPS = 240.0f; 
float g_gameSpeedMultiplier = 1.0f;

struct PlayerInputEvent {
    int frame;
    bool isPress;
    int playerButton; 
};
std::vector<PlayerInputEvent> g_recordedMacro;
int g_currentFrame = 0;
size_t g_macroIndex = 0;

class $modify(VOXEngine, GJBaseGameLayer) {
    void update(float dt) {
        float customDeltaTime = dt * g_gameSpeedMultiplier;
        GJBaseGameLayer::update(customDeltaTime);
        if (g_botRecording || g_botReplaying) {
            g_currentFrame++;
        }
        if (g_botReplaying && g_macroIndex < g_recordedMacro.size()) {
            auto& nextEvent = g_recordedMacro[g_macroIndex];
            if (g_currentFrame >= nextEvent.frame) {
                this->handleButton(nextEvent.isPress, nextEvent.playerButton, true);
                g_macroIndex++;
            }
        }
    }
    void handleButton(bool isPress, int button, bool isPlayer) {
        if (g_botRecording) {
            g_recordedMacro.push_back({g_currentFrame, isPress, button});
        }
        GJBaseGameLayer::handleButton(isPress, button, isPlayer);
    }
};

class $modify(VOXPlayer, PlayerObject) {
    void destroyPlayer(bool p0, bool p1) {
        if (g_noclipActive) return;
        PlayerObject::destroyPlayer(p0, p1);
    }
};

class $modify(VOXVisualMenu, MenuLayer) {
    bool init() {
        if (!MenuLayer::init()) return false;
        auto winSize = CCDirector::sharedDirector()->getWinSize();
        auto menuContainer = CCMenu::create();
        menuContainer->setPosition({winSize.width / 2, winSize.height / 2});

        auto noclipBtn = CCMenuItemSpriteExtra::create(
            CCLabelBMFont::create("Noclip [Active]", "bigFont.fnt"),
            this, menu_selector(VOXVisualMenu::onToggleNoclip)
        );
        noclipBtn->setPosition({0, 60});
        menuContainer->addChild(noclipBtn);

        auto speedBtn = CCMenuItemSpriteExtra::create(
            CCLabelBMFont::create("Speedhack: 1.0x", "bigFont.fnt"),
            this, menu_selector(VOXVisualMenu::onToggleSpeed)
        );
        speedBtn->setPosition({0, -100});
        menuContainer->addChild(speedBtn);
        this->addChild(menuContainer, 100); 
        return true;
    }
    void onToggleNoclip(CCObject* sender) {
        g_noclipActive = !g_noclipActive;
        auto label = static_cast<CCLabelBMFont*>(static_cast<CCMenuItemSpriteExtra*>(sender)->getChildren()->objectAtIndex(0));
        label->setString(g_noclipActive ? "Noclip [Active]" : "Noclip [Disabled]");
    }
    void onToggleSpeed(CCObject* sender) {
        auto label = static_cast<CCLabelBMFont*>(static_cast<CCMenuItemSpriteExtra*>(sender)->getChildren()->objectAtIndex(0));
        if (g_gameSpeedMultiplier == 1.0f) {
            g_gameSpeedMultiplier = 0.5f;
            label->setString("Speedhack: 0.5x");
        } else if (g_gameSpeedMultiplier == 0.5f) {
            g_gameSpeedMultiplier = 2.0f;
            label->setString("Speedhack: 2.0x");
        } else {
            g_gameSpeedMultiplier = 1.0f;
            label->setString("Speedhack: 1.0x");
        }
    }
};
