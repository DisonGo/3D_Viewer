#include <StyleLoader.h>

#include <QApplication>
#include <QSurfaceFormat>

#include "DrawConfig.h"
#include "Engine.h"
#include "MainWindow.h"
#include "Spacers/EngineSpacer.h"

int main(int argc, char *argv[]) {
  QSurfaceFormat format;
  format.setDepthBufferSize(16);
  format.setStencilBufferSize(4);
  format.setSwapBehavior(QSurfaceFormat::TripleBuffer);
  format.setSamples(4);
  format.setVersion(4, 1);
  format.setProfile(QSurfaceFormat::CoreProfile);
  QSurfaceFormat::setDefaultFormat(format);
  QApplication a(argc, argv);
  const QString style_file_path = ":/style/style.qss";
  // ":/style/icons/collapse_arrow.png";
  // const QString style_file_path =
  //     "C:/gits/3D_Viewer/src/qt_app/resources/style/style.qss";
  // const QString style_file_path =
  //     "/Users/evverenn/Desktop/Projects/Junk/3D_Viewer/src/qt_app/resources/"
  //     "style/style.qss";
  s21::DrawConfig config;
  s21::Engine engine(config);
  s21::EngineSpacer spacer(engine);

  s21::MainWindow w(spacer);

  auto *style_loader = new s21::StyleLoader(&w, style_file_path);
  style_loader->Attach();
  w.show();
  return a.exec();
}
