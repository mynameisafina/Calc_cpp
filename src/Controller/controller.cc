#include "controller.h"

s21::result s21::Controller::passInput(const std::string& input,
                                       const std::string& x) {
  return model_->calculate(input, x);
};

s21::graph_res s21::Controller::calculateGraph(const std::string& input,
                                               const std::string& x_begin,
                                               const std::string& y_begin,
                                               const std::string& x_end,
                                               const std::string& y_end,
                                               const std::size_t& points) {
  return model_->calucalte_graph(input, x_begin, y_begin, x_end, y_end, points);
}