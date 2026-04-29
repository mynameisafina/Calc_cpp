#include "view.h"

#include "./ui_view.h"

namespace s21 {

View::View(Controller *controller, QWidget *parent)
    : QMainWindow(parent), ui(new Ui::View), controller_(controller) {
  ui->setupUi(this);
  ui->input->setFocus();
  ui->equal->setDefault(true);
  connect(ui->two, SIGNAL(clicked()), this, SLOT(type_symbol()));
  connect(ui->one, SIGNAL(clicked()), this, SLOT(type_symbol()));
  connect(ui->nine, SIGNAL(clicked()), this, SLOT(type_symbol()));
  connect(ui->four, SIGNAL(clicked()), this, SLOT(type_symbol()));
  connect(ui->five, SIGNAL(clicked()), this, SLOT(type_symbol()));
  connect(ui->seven, SIGNAL(clicked()), this, SLOT(type_symbol()));
  connect(ui->eight, SIGNAL(clicked()), this, SLOT(type_symbol()));
  connect(ui->zero, SIGNAL(clicked()), this, SLOT(type_symbol()));
  connect(ui->six, SIGNAL(clicked()), this, SLOT(type_symbol()));
  connect(ui->three, SIGNAL(clicked()), this, SLOT(type_symbol()));
  connect(ui->dot, SIGNAL(clicked()), this, SLOT(type_symbol()));
  connect(ui->leftpar, SIGNAL(clicked()), this, SLOT(type_symbol()));
  connect(ui->rightpar, SIGNAL(clicked()), this, SLOT(type_symbol()));
  connect(ui->minus, SIGNAL(clicked()), this, SLOT(type_symbol()));
  connect(ui->divide, SIGNAL(clicked()), this, SLOT(type_symbol()));
  connect(ui->multiply, SIGNAL(clicked()), this, SLOT(type_symbol()));
  connect(ui->plus, SIGNAL(clicked()), this, SLOT(type_symbol()));
  connect(ui->exp, SIGNAL(clicked()), this, SLOT(type_symbol()));
  connect(ui->x, SIGNAL(clicked()), this, SLOT(type_symbol()));
  connect(ui->mod, SIGNAL(clicked()), this, SLOT(type_symbol()));

  connect(ui->tan, SIGNAL(clicked()), this, SLOT(type_function()));
  connect(ui->atan, SIGNAL(clicked()), this, SLOT(type_function()));
  connect(ui->sin, SIGNAL(clicked()), this, SLOT(type_function()));
  connect(ui->asin, SIGNAL(clicked()), this, SLOT(type_function()));
  connect(ui->cos, SIGNAL(clicked()), this, SLOT(type_function()));
  connect(ui->acos, SIGNAL(clicked()), this, SLOT(type_function()));
  connect(ui->sqrt, SIGNAL(clicked()), this, SLOT(type_function()));
  connect(ui->ln, SIGNAL(clicked()), this, SLOT(type_function()));
  connect(ui->log, SIGNAL(clicked()), this, SLOT(type_function()));

  connect(ui->allclear, SIGNAL(clicked()), this, SLOT(clear()));

  connect(ui->equal, SIGNAL(clicked()), this, SLOT(result()));
  connect(ui->graph_button, SIGNAL(clicked()), this, SLOT(draw_graph()));
}

void View::type_symbol() {
  QPushButton *button = (QPushButton *)sender();
  ui->input->insert(button->text());
}

void View::type_function() {
  QPushButton *button = (QPushButton *)sender();
  ui->input->insert(button->text() + '(');
}

void View::clear() { ui->input->setText(""); }

void View::keyPressEvent(QKeyEvent *event) {
  if ((event->key() == Qt::Key_Enter) || (event->key() == Qt::Key_Return)) {
    result();
  } else {
    event->ignore();
  }
}

void View::result() {
  ui->printed_res->setText(" ");
  std::string input = ui->input->text().toStdString();
  std::string x = ui->for_x->text().toStdString();
  s21::result res = controller_->passInput(input, x);
  if (res.error.empty()) {
    ui->printed_res->setText(QString::number(res.res, 'g', 7));
  } else {
    ui->printed_res->setText(QString::fromStdString(res.error));
  }
}

void View::draw_graph() {
  graph_ = new graph(controller_, this);
  std::string input = ui->input->text().toStdString();
  std::string x_begin = graph_->GetXBegin();
  std::string x_end = graph_->GetXEnd();
  std::string y_begin = graph_->GetYBegin();
  std::string y_end = graph_->GetYEnd();
  std::size_t num_points = graph_->GetPoints();
  s21::graph_res res = controller_->calculateGraph(input, x_begin, y_begin,
                                                   x_end, y_end, num_points);
  if (res.error.empty()) {
    graph_->SetInput(input);
    graph_->PlotNewGraph(res.x, res.y);
    graph_->show();
  } else {
    ui->printed_res->setText(QString::fromStdString(res.error));
  }
}

View::~View() { delete ui; }

}  // namespace s21
