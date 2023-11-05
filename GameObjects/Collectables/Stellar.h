#ifndef STELLAR_H
#define STELLAR_H

#include "Collectable.h"

namespace GameObjects {
namespace Collectables {

class Stellar : public Collectable, public std::enable_shared_from_this<Stellar>
{
public:
    Stellar(const Position &position);

    // GameObject interface
public:
    bool shouldBeDeleted() override;
protected:
    QPointF getPixmapScaledSize() const override;
    QString getPixmapResourcePath() const override;
    QPixmap getPixmap() const override;
};
}

}


#endif // STELLAR_H
