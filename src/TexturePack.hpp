#pragma once

#include <Geode/Geode.hpp>
#include <Geode/utils/web.hpp>

using namespace geode::prelude;

class TexturePack : public cocos2d::CCObject {
public:
    std::string id;
    std::string name;
    std::string download;
    std::string logo;
    std::string description;
    std::string creator;
    std::string version;
    std::string gdVersion;
    
    // UI References
    Slider* slider = nullptr;
    Slider* slider2 = nullptr;
    FLAlertLayer* popup = nullptr;

    // 2.2081 Compatibility: Use web::WebTask for asynchronous downloads
    web::WebTask m_downloadTP;

    void downloadPack();
    bool isDownloading();

    static TexturePack* create() {
        auto ret = new TexturePack();
        ret->autorelease();
        return ret;
    }
};