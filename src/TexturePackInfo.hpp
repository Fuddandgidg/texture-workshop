#pragma once

#include <Geode/Geode.hpp>
#include <Geode/utils/web.hpp>
#include "TexturePack.hpp"
#include "boobs.hpp"
#include "TextureWorkshopLayer.hpp"

using namespace geode::prelude;

class TexturePackInfo : public Popup<TexturePack*> {
public:
    TexturePack* texturePack;
    CCMenuItemSpriteExtra* downloadButton;
    CCMenuItemSpriteExtra* deleteButton;
    Slider* downloadProgressSlider;

    bool setup(TexturePack* tp) override {
        m_noElasticity = true;
        texturePack = tp;
        texturePack->popup = this;

        // 2.2081 Path Fix
        auto loaderMod = Loader::get()->getLoadedMod("geode.texture-loader");
        std::filesystem::path filePath = loaderMod->getSaveDir() / "packs" / (tp->name + ".zip");

        auto winSize = CCDirector::get()->getWinSize();
        
        // Logo setup
        auto texturePackIconSpr = CCSprite::createWithSpriteFrameName("TWS_PlaceholderLogo.png"_spr);
        texturePackIconSpr->setScale(0.65f);
        texturePackIconSpr->setPosition(this->getContentSize() / 2 + ccp(-116, 80));
        m_mainLayer->addChild(texturePackIconSpr);

        if (tp->featured) {
            auto featuredSpr = CCSprite::createWithSpriteFrameName("TWS_Featured.png"_spr);
            featuredSpr->setScale(0.65f);
            featuredSpr->setPosition(texturePackIconSpr->getPosition());
            m_mainLayer->addChild(featuredSpr);
        }

        // Text labels
        auto texturePackName = CCLabelBMFont::create(tp->name.c_str(), "bigFont.fnt");
        texturePackName->setScale(0.6f);
        texturePackName->setAnchorPoint({0, 0.5f});
        texturePackName->setPosition(texturePackIconSpr->getPosition() + ccp(35, 17));
        m_mainLayer->addChild(texturePackName);

        auto texturePackVer = CCLabelBMFont::create(tp->version.c_str(), "bigFont.fnt");
        texturePackVer->setScale(0.4f);
        texturePackVer->setColor({0, 200, 255});
        texturePackVer->setAnchorPoint({0, 0.5f});
        texturePackVer->setPosition(texturePackName->getPosition() + ccp(0, -13));
        m_mainLayer->addChild(texturePackVer);

        // Description line and area
        auto line = CCSprite::createWithSpriteFrameName("floorLine_001.png");
        line->setPosition(this->getContentSize() / 2 + ccp(0, 45));
        line->setScale(0.675f);
        m_mainLayer->addChild(line);

        auto desc = MDTextArea::create(fmt::format("# {}\n{}", tp->name, tp->description), {300, 150});
        desc->setPosition(line->getPosition() + ccp(0, -81));
        m_mainLayer->addChild(desc);

        // Buttons
        auto viewBtnMenu = CCMenu::create();
        m_mainLayer->addChild(viewBtnMenu);

        auto downloadButtonSpr = CCSprite::createWithSpriteFrameName("TWS_DownloadButton.png"_spr);
        downloadButtonSpr->setScale(.5f);
        downloadButton = CCMenuItemSpriteExtra::create(downloadButtonSpr, this, menu_selector(TexturePackInfo::onDownload));

        auto deleteButtonSpr = CCSprite::createWithSpriteFrameName("TWS_DeleteButton.png"_spr);
        deleteButtonSpr->setScale(.5f);
        deleteButton = CCMenuItemSpriteExtra::create(deleteButtonSpr, this, menu_selector(TexturePackInfo::onDelete));

        if (!std::filesystem::exists(filePath)) {
            viewBtnMenu->addChild(downloadButton);
        } else {
            viewBtnMenu->addChild(deleteButton);
        }
        
        viewBtnMenu->setPosition(line->getPosition() + ccp(150, 53));
        viewBtnMenu->updateLayout();

        // Progress Slider
        downloadProgressSlider = Slider::create(this, nullptr);
        downloadProgressSlider->setScale(0.5f);
        m_mainLayer->addChildAtPosition(downloadProgressSlider, Anchor::TopRight, {-65, -20});
        
        texturePack->slider2 = downloadProgressSlider;
        updateDownloadIndicator();

        return true;
    }

    void onDownload(CCObject*);

    void onDelete(CCObject*) {
        geode::createQuickPopup(
            "Delete Pack",
            fmt::format("Are you sure you want to delete {}?", texturePack->name),
            "Nope", "Yeah",
            [this](auto, bool btn2) {
                if (btn2) {
                    auto loaderMod = Loader::get()->getLoadedMod("geode.texture-loader");
                    std::filesystem::remove(loaderMod->getSaveDir() / "packs" / (texturePack->name + ".zip"));
                    Notification::create("Deleted!", CCSprite::createWithSpriteFrameName("GJ_completesIcon_001.png"))->show();
                    this->onClose(nullptr);
                    if (TextureWorkshopLayer::get) TextureWorkshopLayer::get->onRefresh(nullptr);
                }
            }
        );
    }

    void updateDownloadIndicator() {
        if (downloadButton) downloadButton->setVisible(!texturePack->isDownloading());
        if (downloadProgressSlider) downloadProgressSlider->setVisible(texturePack->isDownloading());
    }
};