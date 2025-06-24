#ifndef GAME_CORE_BENCHMARKPROMPTVIEW_H
#define GAME_CORE_BENCHMARKPROMPTVIEW_H

#include "MenuView.h"
#include <QGraphicsTextItem>

namespace Game {
namespace Core {

class BenchmarkPromptView : public MenuView {
  Q_OBJECT
public:
  BenchmarkPromptView(QRect screenGeometry, QWidget *parent = nullptr);

private:
  QGraphicsTextItem *m_promptText;
};

} // namespace Core
} // namespace Game

#endif // GAME_CORE_BENCHMARKPROMPTVIEW_H
