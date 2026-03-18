#pragma once
#include <Geode/Geode.hpp>
#include "TexturePack.hpp"
#include "TextureWorkshopLayer.hpp"

using namespace geode::prelude;

class TexturePackCell : public CCLayerColor {
public:
    TexturePack* texturePack;
    EventListener<web::WebTask> m_getIcon;

    bool init(TexturePack* tp, bool thing) {
        if (!CCLayerColor::init()) return false;
        texturePack = tp;
        this->setContentSize({315, 35});
        
        auto loaderMod = Loader::get()->getLoadedMod("geode.texture-loader");
        auto filePath = loaderMod->getSaveDir() / "packs" / (tp->name + ".zip");

        // UI Setup
        auto nameLabel = CCLabelBMFont::create(tp->name.c_str(), "bigFont.fnt");
        nameLabel->setScale(0.375f);
        nameLabel->setPosition(50, 22);
        nameLabel->setAnchorPoint({0, 0.5f});
        this->addChild(nameLabel);

        auto creatorLabel = CCLabelBMFont::create(fmt::format("By {}", tp->creator).c_str(), "goldFont.fnt");
        creatorLabel->setScale(0.3f);
        
        auto menu = CCMenu::create();
        menu->setPosition(50, 8);
        menu->setAnchorPoint({0, 0.5f});
        this->addChild(menu);

        auto btn = CCMenuItemSpriteExtra::create(creatorLabel, this, nullptr);
        menu->addChild(btn);
        menu->updateLayout(); // Fix for 2.2081

        return true;
    }

    static TexturePackCell* create(TexturePack* tp, bool thing) {
        auto ret = new TexturePackCell();
        if (ret && ret->init(tp, thing)) {
            ret->autorelease();
            return ret;
        }
        CC_SAFE_DELETE(ret);
        return nullptr;
    }
};