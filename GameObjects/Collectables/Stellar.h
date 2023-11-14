#ifndef STELLAR_H
#define STELLAR_H

#include "Collectable.h"

#include <bits/shared_ptr.h>

namespace GameObjects {
namespace Collectables {

class Stellar : public Collectable, public std::enable_shared_from_this<Stellar>
{
public:
    Stellar(const Position &position);

protected:
    QPixmap getPixmap() const override;

    // GameObject interface
public:
    void collideWith(GameObject &other) override;
};
}

}


#endif // STELLAR_H
