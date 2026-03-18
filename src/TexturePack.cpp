#include <Geode/Geode.hpp>
#include "TexturePack.hpp"
#include "boobs.hpp"
#include "TextureWorkshopLayer.hpp"

using namespace geode::prelude;

void TexturePack::downloadPack()
{
    // Prevent duplicate downloads
    if (std::find(boobs::downloads.begin(), boobs::downloads.end(), this) != boobs::downloads.end()) {
        log::error("Already downloading the pack: {}", name);
        return;
    }

    // 2.2081 Fix: Get the correct Geode directory for texture-loader packs
    // Using getModRuntimeDir() or getConfigDir() safely
    auto loaderMod = Loader::get()->getLoadedMod("geode.texture-loader");
    if (!loaderMod) {
        Notification::create("Texture Loader not found!", CCSprite::createWithSpriteFrameName("GJ_deleteIcon_001.png"))->show();
        return;
    }

    // Use the mod's save directory/packs folder
    auto packsDir = loaderMod->getSaveDir() / "packs";
    if (!std::filesystem::exists(packsDir)) {
        std::filesystem::create_directories(packsDir);
    }

    std::filesystem::path fullPath = packsDir / (name + ".zip");

    m_downloadTP.bind([this, fullPath] (web::WebTask::Event* e) {
        if (web::WebResponse* res = e->getValue()) {
            if (res->ok() && res->into(fullPath)) {
                std::string versionSaveThing = fmt::format("{} Version", name);
                Mod::get()->setSavedValue<std::string>(versionSaveThing, version);
                
                Notification::create("Download Successful", CCSprite::createWithSpriteFrameName("GJ_completesIcon_001.png"))->show();
                
                if (popup)
                    popup->keyBackClicked();
                
                if (TextureWorkshopLayer::get)
                    TextureWorkshopLayer::get->onRefresh(nullptr);
            } else {
                Notification::create("Download Failed", CCSprite::createWithSpriteFrameName("GJ_deleteIcon_001.png"))->show();
                if (std::filesystem::exists(fullPath)) {
                    std::filesystem::remove(fullPath);
                }
            }

            // Safe erase using the iterator
            auto it = std::find(boobs::downloads.begin(), boobs::downloads.end(), this);
            if (it != boobs::downloads.end()) boobs::downloads.erase(it);
            
        } else if (e->isCancelled()) {
            log::info("Request cancelled for {}", name);
            auto it = std::find(boobs::downloads.begin(), boobs::downloads.end(), this);
            if (it != boobs::downloads.end()) boobs::downloads.erase(it);
        } else if (auto progress = e->getProgress()) {
            // Update UI sliders if they exist
            float val = progress->downloadProgress().value_or(0.0f) / 100.0f;
            if (slider) slider->setValue(val);
            if (slider2) slider2->setValue(val);
        }
    });

    auto req = web::WebRequest();
    // 2.2081: Use the mod's setting for cert verification
    req.certVerification(Mod::get()->getSettingValue<bool>("cert-verification"));
    
    m_downloadTP.setFilter(req.get(download));
    boobs::downloads.push_back(this);
}

bool TexturePack::isDownloading()
{
    return std::find(boobs::downloads.begin(), boobs::downloads.end(), this) != boobs::downloads.end();
}