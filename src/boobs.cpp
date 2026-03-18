#include <Geode/Geode.hpp>
#include <Geode/modify/MenuLayer.hpp>
#include "TextureWorkshopLayer.hpp"
#include "boobs.hpp"

using namespace geode::prelude;

// Hook into the MenuLayer to add the Workshop button
class $modify(MyMenuLayer, MenuLayer) {
    bool init() {
        if (!MenuLayer::init()) {
            return false;
        }

        // 2.2081: Use the standard right-side-menu ID
        auto menu = this->getChildByID("right-side-menu");
        if (menu) {
            auto wsButton = CCMenuItemSpriteExtra::create(
                CCSprite::createWithSpriteFrameName("TWS_MainButton.png"_spr),
                this,
                menu_selector(MyMenuLayer::onMyButton)
            );
            
            wsButton->setID("workshop-button"); // Cleaned up ID for 2.2081
            menu->addChild(wsButton);
            
            // Critical for 2.2081: Refresh the menu layout so the button shows up
            menu->updateLayout();
        }

        // Load your saved filter settings
        if (Mod::get()->hasSavedValue("version-filter")) {
            boobs::versionFilter = Mod::get()->getSavedValue<bool>("version-filter");
        }

        return true;
    }

    void onMyButton(CCObject*) {
        auto workshopLayer = TextureWorkshopLayer::scene();
        // 2.2081: Standard fade transition
        auto transition = CCTransitionFade::create(0.5f, workshopLayer);
        CCDirector::get()->pushScene(transition);
    }
};