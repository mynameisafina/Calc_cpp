#include "model.h"

s21::result s21::Model::calculate(const std::string& input,
                                  const std::string& x) {
  result res;
  input_ = input;
  x_num_ = x;
  x_.type = 0;
  try {
    gettingResult();
  } catch (...) {
    status_ = INVALID_INPUT;
  }
  if (status_ != OK) {
    res.error = errorCheck();
    res.res = sqrt(-1);
  } else {
    res.res = result_;
    res.error.clear();
  }
  return res;
}

s21::graph_res s21::Model::calucalte_graph(const std::string& input,
                                           const std::string& x_begin,
                                           const std::string& y_begin,
                                           const std::string& x_end,
                                           const std::string& y_end,
                                           const std::size_t& points) {
  input_ = input;
  try {
    parse();
  } catch (...) {
    status_ = INVALID_INPUT;
  }
  graph_res graph;
  graph.error.clear();
  if (status_ == OK) {
    double min_x, min_y, max_x, max_y;
    try {
      min_x = stod(x_begin);
      max_x = stod(x_end);
      min_y = stod(y_begin);
      max_y = stod(y_end);
    } catch (...) {
      status_ = INVALID_INPUT;
      output_.clear();
      graph.error = errorCheck();
      return graph;
    }
    if (std::fabs(min_x - max_x) < 1e-6 || min_x > max_x ||
        min_x < LimitBegin || max_x > LimitEnd ||
        std::fabs(min_y - max_y) < 1e-6 || min_y > max_y ||
        min_y < LimitBegin || max_y > LimitEnd)
      graph.error = "invalid input";
    else {
      double h = (max_x - min_x) / (points - 1);
      for (double i = min_x; i <= max_x && status_ == OK; i += h) {
        postfixCalc(i);
        graph.x.push_back(i);
        graph.y.push_back(result_);
      }
      output_.clear();
      if (status_ != OK)
        graph.error = errorCheck();
      else
        graph.error.clear();
    }
  } else {
    graph.error = errorCheck();
  }
  return graph;
}

std::string s21::Model::errorCheck() {
  switch (status_) {
    case X_NOT_FOUND:
      return "x not entered";
    case INVALID_INPUT:
      return "invalid input";
    case CALC_FAIL:
      return "calculation failed, try again";
    case EMPTY:
      return "enter an expression";
    default:
      return "";
  }
}

void s21::Model::gettingResult() {
  result_ = 0;
  parse();
  if (x_.type == 3 && status_ == OK) {
    try {
      x_.num = stod(x_num_);
    } catch (...) {
      status_ = INVALID_INPUT;
      output_.clear();
    }
  }
  if (status_ != OK) return;
  std::stack<double> numbers;
  postfixCalc(x_.num);
  output_.clear();
}

void s21::Model::postfixCalc(const double& x) {
  std::stack<double> numbers;
  for (size_t i = 0; i < output_.size(); i++) {
    if (output_[i].type == 1) {
      numbers.push(output_[i].num);
    } else if (output_[i].type == 3) {
      numbers.push(x);
    } else {
      calculationProcess(numbers, output_[i].num);
    }
  }
  if (status_ == OK) {
    result_ = numbers.top();
    numbers.pop();
  }
  if (!numbers.empty()) {
    status_ = CALC_FAIL;
  }
}

void s21::Model::calculationProcess(std::stack<double>& numbers,
                                    char operation) {
  if (numbers.empty()) {
    status_ = CALC_FAIL;
    return;
  }
  double a, c;
  a = numbers.top();
  numbers.pop();
  switch (operation) {
    case '+':
      c = BiCalc(a, numbers, '+');
      break;
    case '-':
      c = BiCalc(a, numbers, '-');
      break;
    case '*':
      c = BiCalc(a, numbers, '*');
      break;
    case '/':
      c = BiCalc(a, numbers, '/');
      break;
    case '^':
      c = BiCalc(a, numbers, '^');
      break;
    case '%':
      c = BiCalc(a, numbers, '%');
      break;
    case 'm':
      c = -a;
      break;
    case 'Q':
      c = sqrt(a);
      break;
    case 's':
      c = sin(a);
      break;
    case 'c':
      c = cos(a);
      break;
    case 't':
      c = tan(a);
      break;
    case 'S':
      c = asin(a);
      break;
    case 'C':
      c = acos(a);
      break;
    case 'T':
      c = atan(a);
      break;
    case 'n':
      c = log(a);
      break;
    case 'l':
      c = log10(a);
      break;
    default:
      status_ = CALC_FAIL;
      break;
  }
  if (status_ == OK) numbers.push(c);
}

double s21::Model::BiCalc(double& a, std::stack<double>& numbers,
                          const char& op) {
  if (numbers.empty()) {
    return a;
  }
  double b = numbers.top();
  numbers.pop();
  switch (op) {
    case '+':
      return a + b;
    case '-':
      return b - a;
    case '/':
      return b / a;
    case '*':
      return b * a;
    case '%':
      return fmod(b, a);
    case '^':
      return pow(b, a);
    default:
      status_ = CALC_FAIL;
      return 0;
  }
}