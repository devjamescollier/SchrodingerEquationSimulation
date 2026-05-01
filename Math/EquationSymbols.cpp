#include "EquationSymbols.h"
#include "Function.h"

EquationSymbols::EquationSymbols() {
  func = nullptr;
  op = '\0';
}

EquationSymbols::EquationSymbols(Function *func, char c) : func(func), op(c) {}
