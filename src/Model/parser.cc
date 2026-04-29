#include <iomanip>

#include "model.h"

void s21::Model::parse() {
  std::stack<std::pair<char, int>> store_operators;
  PrevType prev = start;
  status_ = OK;
  inputCheck();
  if (input_.empty()) status_ = EMPTY;
  for (size_t i = 0; i < input_.length() && status_ == OK; i++) {
    if (isLPar(i, prev)) {
      store_operators.push({'(', precedence.at('(')});
      prev = lpar;
    } else if (isRPar(i, prev)) {
      handleRPar(store_operators);
      prev = rpar;
    } else if (isdigit(input_[i])) {
      handleNumber(i, prev);
      prev = num;
    } else if (isVar(i, prev)) {
      output_.push_back({'x', 3});
      x_.type = 3;
      prev = var;
    } else if (isBi(i, prev)) {
      handleBi(i, store_operators);
      prev = bi;
    } else if (isUn(i, prev)) {
      handleUn(i, store_operators);
      prev = un;
    } else if (isFunc(i, prev)) {
      handleFunc(i, store_operators);
      prev = func;
    } else if (isspace(input_[i])) {
      continue;
    } else {
      status_ = INVALID_INPUT;
    }
  }
  if (prev == func || prev == lpar || prev == bi || prev == un)
    status_ = INVALID_INPUT;
  EmptyOpStack(store_operators);
  if (status_ != OK) output_.clear();
}

void s21::Model::EmptyOpStack(
    std::stack<std::pair<char, int>>& store_operators) {
  while (!store_operators.empty() && status_ == OK) {
    char poppedElement = store_operators.top().first;
    output_.push_back({double(poppedElement), 2});
    store_operators.pop();
  }
}

void s21::Model::inputCheck() {
  bool check = (std::count(input_.begin(), input_.end(), '(') ==
                std::count(input_.begin(), input_.end(), ')'));
  if (!check || input_.length() >= 256) status_ = INVALID_INPUT;
}

bool s21::Model::isLPar(size_t& index,
                        s21::Model::PrevType& prev) const noexcept {
  return (input_[index] == '(' && (prev != var && prev != rpar && prev != num));
}

bool s21::Model::isRPar(size_t& index,
                        s21::Model::PrevType& prev) const noexcept {
  return (input_[index] == ')' && (prev == num || prev == rpar || prev == var));
}

bool s21::Model::isVar(size_t& index,
                       s21::Model::PrevType& prev) const noexcept {
  return (input_[index] == 'x' &&
          (prev == start || prev == lpar || prev == bi || prev == un));
}

bool s21::Model::isUn(size_t& index,
                      s21::Model::PrevType& prev) const noexcept {
  bool symb = (input_[index] == '+' || input_[index] == '-');
  bool check = (prev == start || prev == bi || prev == lpar || prev == var);
  return (symb && check);
}

bool s21::Model::isBi(size_t& index,
                      s21::Model::PrevType& prev) const noexcept {
  std::string op_symb = "+-*/^";
  bool symb = (op_symb.find(input_[index]) != std::string::npos ||
               !input_.compare(index, 3, "mod"));
  bool check = (prev == num || prev == rpar || prev == var);
  return symb && check;
}

bool s21::Model::isFunc(size_t& index,
                        s21::Model::PrevType& prev) const noexcept {
  bool check = (prev == start || prev == lpar || prev == bi || prev == un);
  bool found = false;
  std::string substr = input_.substr(index);
  for (std::map<char, std::string>::const_iterator it = func_letter.begin();
       it != func_letter.end(); ++it) {
    if (substr.find((*it).second) == 0) {
      found = true;
      break;
    }
  }
  return found && check;
}

void s21::Model::handleFunc(size_t& index,
                            std::stack<std::pair<char, int>>& store_operators) {
  std::pair<int, char> function = GetLetter(input_, index);
  index = index + function.first - 1;
  PushOperator(store_operators, function.second);
}

void s21::Model::handleRPar(std::stack<std::pair<char, int>>& store_operators) {
  while (store_operators.top().first != '(') {
    char poppedElement = store_operators.top().first;
    store_operators.pop();
    output_.push_back({double(poppedElement), 2});
  }
  store_operators.pop();
}

void s21::Model::handleNumber(size_t& i, s21::Model::PrevType& prev) {
  if (prev == num || prev == var || prev == rpar || prev == func) {
    status_ = INVALID_INPUT;
    return;
  }
  size_t j;
  double num = stod(input_.substr(i), &j);
  i += j - 1;
  output_.push_back({num, 1});
}

void s21::Model::handleUn(size_t& index,
                          std::stack<std::pair<char, int>>& store_operators) {
  if (input_[index] == '-') {
    PushOperator(store_operators, 'm');
  }
}

void s21::Model::handleBi(size_t& index,
                          std::stack<std::pair<char, int>>& store_operators) {
  char symb = input_[index];
  if (!input_.compare(index, 3, "mod")) {
    symb = '%';
    index += 2;
  }
  PushOperator(store_operators, symb);
}

void s21::Model::PushOperator(std::stack<std::pair<char, int>>& operators,
                              char c) {
  if (operators.empty()) {
    operators.push({c, precedence.at(c)});
  } else {
    while (!operators.empty() && operators.top().second >= precedence.at(c)) {
      std::pair<char, int> poppedElement = operators.top();
      output_.push_back({double(poppedElement.first), 2});
      operators.pop();
    }
    operators.push({c, precedence.at(c)});
  }
}

std::pair<int, char> s21::Model::GetLetter(std::string& str,
                                           size_t& index) const {
  if (str.compare(index, 4, "sqrt") == 0)
    return {4, 'Q'};
  else if (str.compare(index, 3, "sin") == 0)
    return {3, 's'};
  else if (str.compare(index, 3, "cos") == 0)
    return {3, 'c'};
  else if (str.compare(index, 3, "tan") == 0)
    return {3, 't'};
  else if (str.compare(index, 4, "asin") == 0)
    return {4, 'S'};
  else if (str.compare(index, 4, "acos") == 0)
    return {4, 'C'};
  else if (str.compare(index, 4, "atan") == 0)
    return {4, 'T'};
  else if (str.compare(index, 2, "ln") == 0)
    return {2, 'n'};
  else if (str.compare(index, 3, "log") == 0)
    return {3, 'l'};
  return {0, 0};
}