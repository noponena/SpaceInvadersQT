#include "Stellar.h"
#include "Game/Audio/SoundInfo.h"

namespace GameObjects {
namespace Collectables {
Stellar::Stellar(const Position &position)
    : Collectable(position, 1)
{
    m_objectType = ObjectType::COLLECTABLE;
    m_pixmapResourcePath = ":/Images/coin.png";
    m_pixmapScale = QPointF(5.0, 5.0);
    m_destructionSoundInfo = Game::Audio::SoundInfo({m_soundEnabled, Game::Audio::SoundEffect::STELLAR_COIN_COLLECTED});
}

bool Stellar::shouldBeDeleted()
{
    return m_collected || m_lifeSpanExceeded || m_position.isBeyondAnyLimit();
}

QPixmap Stellar::getPixmap() const {
    static QPixmap pixmap = GameObject::getPixmap();
    return pixmap;
}

void Stellar::collideWith(GameObject &other)
{
    other.collideWithStellarToken(*this);
}

}
}



