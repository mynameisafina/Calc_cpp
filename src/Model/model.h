#ifndef _SRC_MODEL_MODEL_H_
#define _SRC_MODEL_MODEL_H_

#include <algorithm>
#include <cmath>
#include <map>
#include <stack>
#include <string>
#include <vector>

namespace s21 {

struct result {
  double res;
  std::string error;
};

struct graph_res {
  std::vector<double> x;
  std::vector<double> y;
  std::string error;
};

class Model {
 public:
  Model() : input_({}), output_({}), result_(0.0), x_({}), status_(OK){};

  result calculate(const std::string& input, const std::string& x);
  graph_res calucalte_graph(const std::string& input,
                            const std::string& x_begin,
                            const std::string& y_begin,
                            const std::string& x_end, const std::string& y_end,
                            const std::size_t& points);

 private:
  enum ErrorType { OK, INVALID_INPUT, CALC_FAIL, X_NOT_FOUND, EMPTY };

  struct store_postfix {
    double num;
    int type;  // 1 - num, 2 - op/func, 3 - var
  };

  enum PrevType { start, num, lpar, rpar, bi, un, func, var };

  const std::map<char, int> precedence{
      {'Q', 6}, {'s', 6}, {'S', 6}, {'c', 6}, {'C', 6}, {'t', 6},
      {'T', 6}, {'n', 6}, {'l', 6}, {'m', 5}, {'^', 4}, {'*', 3},
      {'%', 3}, {'/', 3}, {'+', 2}, {'-', 2}, {'(', 1}};

  const std::map<char, std::string> func_letter{
      {'Q', "sqrt"}, {'s', "sin"},  {'c', "cos"}, {'t', "tan"}, {'S', "asin"},
      {'C', "acos"}, {'T', "atan"}, {'n', "ln"},  {'l', "log"}};

  const double LimitBegin = -1e6;
  const double LimitEnd = 1e6;

  void parse();
  void gettingResult();

  void calculationProcess(std::stack<double>& numbers, char operation);
  void postfixCalc(const double& x);

  std::pair<int, char> GetLetter(std::string& str, size_t& index) const;
  void PushOperator(std::stack<std::pair<char, int>>& operators, char c);
  void EmptyOpStack(std::stack<std::pair<char, int>>& store_operators);

  bool isLPar(size_t& index, PrevType& prev) const noexcept;
  bool isRPar(size_t& index, PrevType& prev) const noexcept;
  bool isVar(size_t& index, PrevType& prev) const noexcept;
  bool isUn(size_t& index, PrevType& prev) const noexcept;
  bool isBi(size_t& index, PrevType& prev) const noexcept;
  bool isFunc(size_t& index, PrevType& prev) const noexcept;

  void handleRPar(std::stack<std::pair<char, int>>& store_operators);
  void handleNumber(size_t& i, Model::PrevType& prev);
  void handleUn(size_t& index,
                std::stack<std::pair<char, int>>& store_operators);
  void handleBi(size_t& index,
                std::stack<std::pair<char, int>>& store_operators);
  void handleFunc(size_t& index,
                  std::stack<std::pair<char, int>>& store_operators);

  double BiCalc(double& a, std::stack<double>& numbers, const char& op);
  void inputCheck();
  std::string errorCheck();

  std::string input_;
  std::vector<store_postfix> output_;
  double result_;
  store_postfix x_;
  ErrorType status_;
  std::string x_num_;
};

}  // namespace s21

#endif  // _SRC_MODEL_MODEL_H_
