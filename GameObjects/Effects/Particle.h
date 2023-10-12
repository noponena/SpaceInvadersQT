#ifndef PARTICLE_H
#define PARTICLE_H

#include <QDebug>
#include <QPointF>
#include <QPainter>
#include <QRandomGenerator>

namespace GameObjects {
namespace Effects {
class Particle {
public:
    Particle(QPointF position, int lifespan, QColor color)
        :m_position(position), m_lifespan(lifespan), m_maxLifespan(lifespan), m_color(color)
    {
        m_velocity = randomVelocity(50);
    };

    QPointF randomVelocity(float maxSpeed) {
        float angle = QRandomGenerator::global()->generateDouble() * 2 * M_PI;
        float speed = QRandomGenerator::global()->generateDouble() * maxSpeed;

        return QPointF(cos(angle) * speed, sin(angle) * speed);
    }

    void move() {
        m_position += m_velocity;
        //qDebug() << "particle position:" << m_position;
    };
    bool isDead() const {
        return m_lifespan <= 0;
    };
    void draw(QPainter &painter) {
        painter.save();

        QColor currentColor = m_color;
        int alpha = static_cast<int>(255.0 * static_cast<double>(m_lifespan) / static_cast<double>(m_maxLifespan));
        currentColor.setAlpha(alpha);

        painter.setBrush(currentColor);
        painter.setPen(Qt::NoPen);

        painter.drawEllipse(m_position, 1, 1);
        painter.restore();
    }

    void update() {
        this->move();
        m_lifespan -= 1;
        if (m_lifespan < 0) m_lifespan = 0;
    }

    QPointF position() const;

private:
    QPointF m_position;
    QPointF m_velocity;
    int m_lifespan;
    int m_maxLifespan;
    QColor m_color;
};

inline QPointF Particle::position() const
{
    return m_position;
}

}
}


#endif // PARTICLE_H
