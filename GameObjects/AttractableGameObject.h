#ifndef GAMEOBJECTS_ATTRACTABLEGAMEOBJECT_H
#define GAMEOBJECTS_ATTRACTABLEGAMEOBJECT_H

#include "GameObject.h"

namespace GameObjects {

class AttractableGameObject : public GameObject
{
public:
    AttractableGameObject(const Position &position);
    void update(const UpdateContext &context) override;
    void updateMovement(const UpdateContext &context);
    void applyMovement(float deltaTimeInSeconds, const QPointF &velocity);
    void handleMagneticMovement(const QPointF &totalMagneticForce, float deltaTimeInSeconds);

protected:
    std::unordered_set<ObjectType> m_magneticTargets;
    bool m_hasInitiatedMovement = false;
    bool m_stopped = false;
    float m_dampingFactor = 0.9f;
    bool m_attractionEnabled;

private:
    QPointF m_magneticVelocity;
    const std::vector<std::pair<QPointF, Magnetism>> getMagneticObjectData(const UpdateContext &context);
};

} // namespace GameObjects

#endif // GAMEOBJECTS_ATTRACTABLEGAMEOBJECT_H
