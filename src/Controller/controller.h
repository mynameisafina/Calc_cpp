#ifndef _SRC_CONTROLLER_CONTROLLER_H_
#define _SRC_CONTROLLER_CONTROLLER_H_

#include "../Model/model.h"

namespace s21 {

class Controller {
 public:
  Controller(Model* model) : model_(model){};

  result passInput(const std::string& input, const std::string& x);
  graph_res calculateGraph(const std::string& input, const std::string& x_begin,
                           const std::string& y_begin, const std::string& x_end,
                           const std::string& y_end, const std::size_t& points);

 private:
  Model* model_;
};
}  // namespace s21

#endif  // _SRC_CONTROLLER_CONTROLLER_H_