#pragma once
#include "Function.h"

struct EquationSymbols {
  Function *func;
  char op;

  EquationSymbols();
  EquationSymbols(Function *func, char c);
};
