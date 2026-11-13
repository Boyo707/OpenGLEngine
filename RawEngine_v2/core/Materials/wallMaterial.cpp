//
// Created by boydv on 13/11/2026.
//

#include "wallMaterial.h"

namespace material {
    WallMaterial::WallMaterial(const std::string& pName) {
        name = pName;
        albedoTexture = new core::Texture("textures/StoneAlbedo.png");
        normalMap = new core::Texture("textures/StoneNormal.png");
    }
}