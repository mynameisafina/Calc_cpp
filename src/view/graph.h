#ifndef GRAPH_H
#define GRAPH_H

#include <QDialog>

#include "../Controller/controller.h"
#include "qcustomplot.h"

namespace Ui {
class graph;
}

namespace s21 {
class graph : public QDialog {
  Q_OBJECT

 public:
  explicit graph(Controller* controller, QWidget* parent = nullptr);
  ~graph();
  std::string GetXBegin();
  std::string GetYBegin();
  std::string GetXEnd();
  std::string GetYEnd();
  std::size_t GetPoints();
  void SetInput(std::string input);
  void PlotNewGraph(std::vector<double> x, std::vector<double> y);

 private slots:
  void PlotGraph();

 private:
  Ui::graph* ui;
  Controller* g_controller_;
  QVector<double> x_, y_;
  double x_begin_, x_end_, y_begin_, y_end_;
  std::string input_;
};
}  // namespace s21

#endif  // GRAPH_H
