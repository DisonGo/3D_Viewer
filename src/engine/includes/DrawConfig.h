#ifndef DRAWCONFIG_H
#define DRAWCONFIG_H
#include <QColor>
#include <QDebug>
class DrawConfig {
 public:
  static DrawConfig& Instance() {
    static DrawConfig instance_;
    return instance_;
  };
  float point_size = 1;
  float line_width = 1;
  bool points = true;
  bool lines = true;
  bool triangles = true;
  bool triangles_strip = false;
  bool round_circle = false;
  bool dashed_lines = false;
  bool index_draw = false;
  bool vertex_only_draw = true;
  QColor fragment_color = QColor(255, 255, 255);
  QColor line_color = QColor(255, 255, 255);
  QColor dot_color = QColor(255, 255, 255);
  QColor back_color = QColor(0, 0, 0);

 private:
  DrawConfig(){};
  DrawConfig(const DrawConfig&);
  const DrawConfig& operator=(const DrawConfig&);
  ~DrawConfig(){};
};
#endif  // DRAWCONFIG_H
