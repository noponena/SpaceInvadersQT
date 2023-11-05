#include "StellarPool.h"

namespace GameObjects {
namespace Collectables {
Stellar::Stellar(const Position &position)
    : Collectable(position, 1) {}

bool Stellar::shouldBeDeleted()
{
    return m_collected;
}

QPointF GameObjects::Collectables::Stellar::getPixmapScaledSize() const { return QPointF(3.0, 3.0); }

QString GameObjects::Collectables::Stellar::getPixmapResourcePath() const
{
    return QString(":/Images/coin.png");
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



