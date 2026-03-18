#include "TexturePackInfo.hpp"
#include "TexturePackCell.hpp"

using namespace geode::prelude;

void TexturePackInfo::onDownload(CCObject*) {
    // 2.2081 Safety: Check if the texture pack exists before trying to download
    if (!texturePack) return;

    texturePack->downloadPack();

    // Update the button and progress bar in the popup
    updateDownloadIndicator();

    // 2.2081 Safety: Check if the cell still exists in the list before updating it
    if (texturePack->cell) {
        auto cell = typeinfo_cast<TexturePackCell*>(texturePack->cell);
        if (cell) {
            cell->updateDownloadIndicator();
        }
    }
}