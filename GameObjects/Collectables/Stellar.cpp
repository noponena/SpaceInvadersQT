#include "Stellar.h"
#include "Game/Audio/SoundInfo.h"

namespace GameObjects {
namespace Collectables {
Stellar::Stellar(const Position &position)
    : Collectable(position, 1)
{
    m_pixmapResourcePath = ":/Images/coin.png";
    m_pixmapScale = QPointF(5.0, 5.0);
    m_destructionSoundInfo = Game::Audio::SoundInfo({true, Game::Audio::SoundEffect::STELLAR_COIN_COLLECTED, 1.0f});
}

bool Stellar::shouldBeDeleted()
{
    return m_collected || m_position.isBeyondAnyLimit();
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



