#include "GameOfLife.hpp"
#include <QTextCodec>
#include <QTranslator>
#include <QLocale>
#include <QLibraryInfo>

int main(int argc, char *argv[]){
  QApplication app(argc, argv);

  //////////////////////////////////////////////////////////////////////////////

  // To translate to user's language, but only things like button yes, quit, ...
  // Our sentences will have to be translated
  QString locale = QLocale::system().name().section('_', 0, 0);
  QTranslator translator;
  translator.load(QString("qt_") + locale, QLibraryInfo::location(QLibraryInfo::TranslationsPath));
  app.installTranslator(&translator);

  //////////////////////////////////////////////////////////////////////////////

  GameOfLife gameOfLife;
  gameOfLife.show();

  //////////////////////////////////////////////////////////////////////////////

  return app.exec();
}
