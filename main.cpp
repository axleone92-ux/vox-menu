#include <Geode/Geode.hpp>
#include <Geode/modify/GJBaseGameLayer.hpp>
#include <Geode/modify/PlayerObject.hpp>
#include <Geode/modify/MenuLayer.hpp>
#include <vector>

using namespace geode::prelude;

// Global States
bool g_noclipActive = true;
float g_gameSpeedMultiplier = 1.0f;

// Simple structure for the speedhack loop
class $modify(VOXEngine, GJBaseGameLayer) {
    void update(float dt) {
        float customDeltaTime = dt * g_gameSpeedMultiplier;
        GJBaseGameLayer::update(customDeltaTime);
    }
};

// Simple structure for Noclip
class $modify(VOXPlayer, PlayerObject) {
    void destroyPlayer(bool p0, bool p1) {
        if (g_noclipActive) return;
        PlayerObject::destroyPlayer(p0, p1);
    }
};

// Clean UI Layout Menu
class $modify(VOXVisualMenu, MenuLayer) {

    // 1. FUNCTIONS PLACED AT THE TOP SO THE BUTTONS CAN SEE THEM
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

    // 2. THE INITIALIZATION LAYER BELOW THE FUNCTIONS
    bool init() {
        if (!MenuLayer::init()) return false;

        auto winSize = CCDirector::sharedDirector()->getWinSize();
        auto menuContainer = CCMenu::create();
        menuContainer->setPosition({winSize.width / 2, winSize.height / 2});

        // Noclip Button Layout
        auto noclipBtn = CCMenuItemSpriteExtra::create(
            CCLabelBMFont::create("Noclip [Active]", "bigFont.fnt"),
            this, menu_selector(VOXVisualMenu::onToggleNoclip)
        );
        noclipBtn->setPosition({0, 30});
        menuContainer->addChild(noclipBtn);

        // Speed Button Layout
        auto speedBtn = CCMenuItemSpriteExtra::create(
            CCLabelBMFont::create("Speedhack: 1.0x", "bigFont.fnt"),
            this, menu_selector(VOXVisualMenu::onToggleSpeed)
        );
        speedBtn->setPosition({0, -30});
        menuContainer->addChild(speedBtn);

        this->addChild(menuContainer, 100); 
        return true;
    }
};

