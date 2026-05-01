#include "ComposedFunction.h"
#include "EquationSymbols.h"
#include <assert.h>
#include <stack>
#include <vector>
ComposedFunction::~ComposedFunction() {
  for (int i = 0; i < f_symbols.size(); i++) {
    if (f_symbols[i].func != nullptr) {
      delete f_symbols[i].func;
    }
  }
}

void ComposedFunction::addSymbol(Function *func) {
  f_symbols.push_back(EquationSymbols(func, '\0'));
}

void ComposedFunction::addSymbol(char c) {
  f_symbols.push_back(EquationSymbols(nullptr, c));
}

double ComposedFunction::Evaluate(double x) const {

  if (f_symbols.size() == 1) {
    return (f_symbols[0].func)->Evaluate(x);
  }

  std::stack<StackItem> eq_stack;
  eq_stack.push(StackItem((f_symbols[0].func)->Evaluate(x), '\0'));
  eq_stack.push(StackItem(0.0, f_symbols[1].op));
  eq_stack.push(StackItem((f_symbols[2].func)->Evaluate(x), '\0'));

  int symbol_ind = 3;
  while (eq_stack.size() > 1 || symbol_ind < f_symbols.size()) {
    // assume the topmost item of the stack is a number
    // consider a number that is on the right side of the left operator
    /*
    if (f_symbols[symbol_ind].op == '(') { // right side of the operator is
    parantheses eq_stack.push(StackItem(0.0, '(')); continue;
    }
    */
    double num = eq_stack.top().num;
    eq_stack.pop();
    if (eq_stack.empty()) { // there are still more symbols to be added
      eq_stack.push(
          StackItem(num, '\0')); // put num back since we are doing nothing
      eq_stack.push(StackItem(0.0, f_symbols[symbol_ind].op));
      eq_stack.push(StackItem((f_symbols[symbol_ind + 1].func)->Evaluate(x),
                              '\0')); // put num back since we are doing nothing

      symbol_ind = symbol_ind + 2;
    } else if (symbol_ind < f_symbols.size()) {
      char r_op = f_symbols[symbol_ind].op;
      char l_op = eq_stack.top().op;

      if (precedence(r_op) <=
          precedence(
              l_op)) { // given ... a * b + c, or ... a * b * c, evaluate a * b
        double a, b;
        b = num;
        eq_stack.pop();
        a = eq_stack.top().num;

        double new_num;
        if (l_op == '+') {
          new_num = a + b;
        } else if (l_op == '-') {
          new_num = a - b;
        } else if (l_op == '*') {
          new_num = a * b;
        } else if (l_op == '/') {
          new_num = a / b;
        }

        eq_stack.pop();
        eq_stack.push(StackItem(new_num, '\0'));
      } else { // given ...a + b * c
        eq_stack.push(
            StackItem(num, '\0')); // put num back since we are doing nothing
        eq_stack.push(StackItem(0.0, f_symbols[symbol_ind].op));
        eq_stack.push(
            StackItem((f_symbols[symbol_ind + 1].func)->Evaluate(x),
                      '\0')); // put num back since we are doing nothing

        symbol_ind = symbol_ind + 2;
      }
    } else { // ran out of additional symbols, just evaluate the expression
      // we are guarenteed that an expression like a * b + c does not exist,
      // where left has higher precedence than the right
      //  so the expression is of the form a + b * c, a + b, or a * b
      double a, b;
      char op;
      b = num;
      op = eq_stack.top().op;
      eq_stack.pop();
      a = eq_stack.top().num;
      eq_stack.pop();
      double new_num;
      if (op == '+') {
        new_num = a + b;
      } else if (op == '-') {
        new_num = a - b;
      } else if (op == '*') {
        new_num = a * b;
      } else if (op == '/') {
        new_num = a / b;
      }
      eq_stack.push(StackItem(new_num, '\0'));
    }
  }

  return eq_stack.top().num;
}

int ComposedFunction::precedence(char op1) const {
  assert(op1 != '\0');
  if (op1 == '+' || op1 == '-')
    return 1;
  else if (op1 == '*' || op1 == '/')
    return 2;
  else
    return 0;
}
