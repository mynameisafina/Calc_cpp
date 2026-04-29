#include "graph.h"

#include "ui_graph.h"

namespace s21 {

graph::graph(Controller* controller, QWidget* parent)
    : QDialog(parent),
      ui(new Ui::graph),
      g_controller_(controller),
      x_end_(5),
      x_begin_(-5),
      y_begin_(-5),
      y_end_(5) {
  ui->setupUi(this);
  ui->min_x->setText(QString::number(x_begin_));
  ui->max_x->setText(QString::number(x_end_));
  ui->min_y->setText(QString::number(y_begin_));
  ui->max_y->setText(QString::number(y_end_));

  connect(ui->plot_new_one, SIGNAL(clicked()), this, SLOT(PlotGraph()));
}

std::string graph::GetXBegin() { return std::to_string(x_begin_); }

std::string graph::GetYBegin() { return std::to_string(y_begin_); }

std::string graph::GetXEnd() { return std::to_string(x_end_); }

std::string graph::GetYEnd() { return std::to_string(y_end_); }

std::size_t graph::GetPoints() {
  return (std::size_t)ui->widget->geometry().width();
}

void graph::SetInput(std::string input) { input_ = input; }

void graph::PlotNewGraph(std::vector<double> x, std::vector<double> y) {
  x_ = QVector<double>(x.begin(), x.end());
  y_ = QVector<double>(y.begin(), y.end());
  ui->widget->clearGraphs();

  ui->widget->xAxis->setRange(x_begin_, x_end_);
  ui->widget->yAxis->setRange(y_begin_, y_end_);
  ui->widget->xAxis->setLabel("x");
  ui->widget->yAxis->setLabel("y");
  ui->widget->setInteraction(QCP::iRangeDrag, true);
  ui->widget->setInteraction(QCP::iRangeZoom, true);
  ui->widget->addGraph();
  ui->widget->graph(0)->setPen(QPen(QColor(151, 146, 237)));
  ui->widget->graph(0)->setLineStyle(QCPGraph::LineStyle::lsNone);
  ui->widget->graph(0)->setScatterStyle(
      QCPScatterStyle(QCPScatterStyle::ScatterShape::ssDisc, 2));

  ui->widget->graph(0)->addData(x_, y_);
  ui->widget->replot();
}

void graph::PlotGraph() {
  std::string x_begin, x_end, y_begin, y_end;
  x_begin = ui->min_x->text().toStdString();
  x_end = ui->max_x->text().toStdString();
  y_begin = ui->min_y->text().toStdString();
  y_end = ui->max_y->text().toStdString();
  std::size_t num_points = GetPoints();
  s21::graph_res res = g_controller_->calculateGraph(input_, x_begin, y_begin,
                                                     x_end, y_end, num_points);
  if (res.error.empty()) {
    x_begin_ = ui->min_x->text().toDouble();
    x_end_ = ui->max_x->text().toDouble();
    y_begin_ = ui->min_y->text().toDouble();
    y_end_ = ui->max_y->text().toDouble();
    PlotNewGraph(res.x, res.y);
  } else {
    QMessageBox msgBox;
    msgBox.setFixedSize(QSize(600, 120));
    msgBox.addButton("ok", QMessageBox::YesRole);
    msgBox.setText("invalid x or y values");
    msgBox.setStyleSheet("QDialogButtonBox { qproperty-centerButtons: true; }");
    msgBox.exec();
  }
}

graph::~graph() { delete ui; }

}  // namespace s21
