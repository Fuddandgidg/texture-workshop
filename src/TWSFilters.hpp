#pragma once

#include <Geode/Geode.hpp>
#include <Geode/ui/GeodeUI.hpp>
#include "boobs.hpp"
#include "TextureWorkshopLayer.hpp"

using namespace geode::prelude;

class TWSFilters : public Popup<> {
public:
    static inline TWSFilters* get = nullptr;

    bool setup() override {
        get = this;
        this->setTitle("Filters");
        m_title->setScale(0.85f);

        auto winSize = CCDirector::get()->getWinSize();
        
        // Use the new Scale9Sprite constructor for 2.2081 compatibility
        auto bg = typeinfo_cast<CCScale9Sprite*>(CCScale9Sprite::create("square02_small.png"));
        if (bg) {
            bg->setContentSize({110, 180});
            bg->setOpacity(100);
            bg->setPosition(m_mainLayer->getContentSize() / 2);
            bg->setPositionY(bg->getPositionY() - 13);
            m_mainLayer->addChild(bg);

            auto menu = CCMenu::create();
            menu->setContentSize(bg->getContentSize());
            menu->setPosition(0, 0);
            bg->addChild(menu);

            auto uc = CCLabelBMFont::create("Under Construction", "bigFont.fnt");
            uc->setScale(0.275f);
            uc->setPosition(menu->getContentSize() / 2);
            menu->addChild(uc);
            
            // 2.2081: Always call updateLayout on menus
            menu->updateLayout();
        }

        return true;
    }

    void onToggle(CCObject* toggle) {
        // Updated logic to ensure we don't crash if the main layer is null
        boobs::versionFilter = !boobs::versionFilter;
        Mod::get()->setSavedValue<bool>("version-filter", boobs::versionFilter);
        
        if (TextureWorkshopLayer::get) {
            TextureWorkshopLayer::get->onGetTPsFinished();
        }
    }

    static TWSFilters* create() {
        auto ret = new TWSFilters();
        // Using "TWS_Box.png"_spr requires the sprite to be in your mod.json resources
        if (ret && ret->initAnchored(140, 230, "TWS_Box.png"_spr)) {
            ret->autorelease();
            return ret;
        }
        CC_SAFE_DELETE(ret);
        return nullptr;
    }
    
    // Ensure the pointer is cleared when closed
    void onClose(CCObject* obj) override {
        get = nullptr;
        Popup::onClose(obj);
    }
};