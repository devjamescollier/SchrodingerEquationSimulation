#include "EquationSymbols.h"
#include "Function.h"
#include <vector>

class ComposedFunction : public Function {
public:
  double Evaluate(double x) const override;

  void addSymbol(Function *func);
  void addSymbol(char c);
  ~ComposedFunction();

private:
  struct StackItem {
    double num;
    char op;
    StackItem() {}
    StackItem(double num, char op) : num(num), op(op) {}
  };
  std::vector<EquationSymbols> f_symbols;
  int precedence(char op) const;
};
