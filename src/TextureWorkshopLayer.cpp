#include "TextureWorkshopLayer.hpp"
#include "TexturePackCell.hpp"
#include "TWSFilters.hpp"
#include <matjson.hpp>

using namespace geode::prelude;

bool TextureWorkshopLayer::setup() {
    this->setTitle("Texture Workshop");
    m_noElasticity = false;

    auto winSize = CCDirector::get()->getWinSize();

    // Create the background for the list
    auto listBg = typeinfo_cast<CCScale9Sprite*>(CCScale9Sprite::create("square02_001.png"));
    listBg->setContentSize({350, 220});
    listBg->setOpacity(100);
    listBg->setPosition(m_mainLayer->getContentSize() / 2);
    m_mainLayer->addChild(listBg);

    // Filter Button
    auto filterSpr = CCSprite::createWithSpriteFrameName("GJ_plusBtn_001.png");
    filterSpr->setScale(0.7f);
    auto filterBtn = CCMenuItemSpriteExtra::create(
        filterSpr, this, menu_selector(TextureWorkshopLayer::onFilters)
    );
    
    auto menu = CCMenu::create();
    menu->addChild(filterBtn);
    m_mainLayer->addChild(menu);
    menu->setPosition({winSize.width / 2 + 185, winSize.height / 2 + 95});

    // Start loading the packs
    this->onRefresh(nullptr);

    return true;
}

void TextureWorkshopLayer::onRefresh(CCObject*) {
    // 2.2081: Use web::WebRequest for the API
    web::WebRequest().get("https://textureworkshop.plusgdps.dev")
        .listen([this](web::WebTask::Event* e) {
            if (auto res = e->getValue()) {
                if (res->ok()) {
                    this->onGetTPsFinished(res->string().value_or(""));
                }
            }
        });
}

void TextureWorkshopLayer::onGetTPsFinished(std::string const& data) {
    if (data.empty()) return;

    // 2.2081: Use matjson to parse the test.json you provided
    auto json = matjson::parse(data);
    auto scroll = ScrollLayer::create({340, 210});
    
    int yOffset = 0;
    for (auto const& [key, value] : json.as_object()) {
        auto tp = TexturePack::create();
        tp->name = value["packName"].as_string();
        tp->download = value["downloadLink"].as_string();
        tp->version = value["packVersion"].as_string();
        
        auto cell = TexturePackCell::create(tp, true);
        cell->setPositionY(yOffset);
        scroll->m_contentLayer->addChild(cell);
        yOffset -= 40;
    }

    scroll->m_contentLayer->setContentSize({340, static_cast<float>(abs(yOffset))});
    m_mainLayer->addChild(scroll);
    scroll->setPosition(m_mainLayer->getContentSize() / 2 - scroll->getContentSize() / 2);
}

void TextureWorkshopLayer::onFilters(CCObject*) {
    TWSFilters::create()->show();
}

TextureWorkshopLayer* TextureWorkshopLayer::create() {
    auto ret = new TextureWorkshopLayer();
    if (ret && ret->initAnchored(420, 280)) {
        ret->autorelease();
        return ret;
    }
    CC_SAFE_DELETE(ret);
    return nullptr;
}