#pragma once

#include <Geode/Geode.hpp>
#include <matjson.hpp>
#include "TexturePack.hpp"

using namespace geode::prelude;

class boobs {
    public:
        // 2.2081: Use inline static for global access across all mod files
        inline static bool downloaded = false;
        inline static bool versionFilter = true;
        
        // This holds your test.json data once it's loaded
        inline static matjson::Value tpJson; 
        
        // Keeps track of active downloads to prevent duplicates
        inline static std::vector<TexturePack*> downloads = { };
};