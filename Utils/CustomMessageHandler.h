#include <QDateTime>
#include <QDebug>
#include <QTextStream>

void customMessageHandler(QtMsgType type, const QMessageLogContext &context,
                          const QString &msg) {
  Q_UNUSED(context);

  QByteArray localMsg = msg.toLocal8Bit();
  QString timeStr =
      QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss.zzz");

  QTextStream out(stdout);
  switch (type) {
  case QtDebugMsg:
    out << "[" << timeStr << "] [DEBUG] " << localMsg.constData() << "\n";
    break;
  case QtInfoMsg:
    out << "[" << timeStr << "] [INFO] " << localMsg.constData() << "\n";
    break;
  case QtWarningMsg:
    out << "[" << timeStr << "] [WARN] " << localMsg.constData() << "\n";
    break;
  case QtCriticalMsg:
    out << "[" << timeStr << "] [CRITICAL] " << localMsg.constData() << "\n";
    break;
  case QtFatalMsg:
    out << "[" << timeStr << "] [FATAL] " << localMsg.constData() << "\n";
    abort();
  }
}
