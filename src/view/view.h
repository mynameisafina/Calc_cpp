#ifndef VIEW_H
#define VIEW_H

#include <QKeyEvent>
#include <QMainWindow>

#include "../Controller/controller.h"
#include "graph.h"

QT_BEGIN_NAMESPACE
namespace Ui {
class View;
}
QT_END_NAMESPACE

namespace s21 {
class View : public QMainWindow {
  Q_OBJECT

 public:
  View(Controller* controller, QWidget* parent = nullptr);
  ~View();

 protected:
  void keyPressEvent(QKeyEvent* event);

 private slots:
  void type_symbol();
  void type_function();
  void clear();
  void result();
  void draw_graph();

 private:
  Ui::View* ui;
  Controller* controller_;
  graph* graph_;
};
}  // namespace s21

#endif  // VIEW_H
