#ifndef TSL_SETTINGS_ENTITY_H
#define TSL_SETTINGS_ENTITY_H

#include <gf/Entity.h>
#include <gf/ResourceManager.h>

namespace tsl {

    class SettingsEntity : public gf::Entity {
        public:
            SettingsEntity(gf::ResourceManager& resources);
            
            void render(gf::RenderTarget &target, const gf::RenderStates &states) override;
        
        private:
            gf::Texture& m_paper;
            gf::Font& m_font;
                           
    };

}

#endif // TSL_SETTINGS_ENTITY_H