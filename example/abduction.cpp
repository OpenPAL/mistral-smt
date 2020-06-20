#include <iostream>
#include "constraint.h"
#include "term.h"

 /*
 * Compile as: g++ -I.. -I../cnode -I../solver -I../numeric-lib -I../term/ -std=c++0x -lgmp example.cpp ../build/libmistral.a ../build/parser/libparser.a -o my_project
 * The, run ./my_project
 */

void hello_mistral();
void hello_abduction();
void guards_demo();


int main(int argc, char* argv[]) { guards_demo(); }


void guards_demo() {
  Term* sg = VariableTerm::make("sg");
  Term* sr = VariableTerm::make("sr");
  Term* n = VariableTerm::make("n");

  Constraint conclusion {sg, n, ATOM_GEQ};
  Constraint guard_l {sr, n, ATOM_GEQ};

  map<Term*, int> costs {{sg, 0}, {sr, 0}};

  Constraint guard_g = conclusion.abduce(guard_l, set<Constraint>{}, costs);

  cout << guard_g << " & " << guard_l << " => " << conclusion << endl;
}


void hello_abduction() {
  Term* x = VariableTerm::make("x");
  Term* y = VariableTerm::make("y");
  Term* z = VariableTerm::make("z");

  Constraint xneg {x, ConstantTerm::make(0), ATOM_LEQ};
  Constraint ygt1 {y, ConstantTerm::make(1), ATOM_GT};
  Constraint premises = xneg & ygt1;

  map<Term*, long int> elems;
  elems[x] = 2;
  elems[y] = -1;
  elems[z] = 3;
  Term* t = ArithmeticTerm::make(elems);
  Constraint conclusion {t, ConstantTerm::make(10), ATOM_LEQ};

  Constraint explanation = conclusion.abduce(premises);
  cout << premises << " & " << explanation << " => " << conclusion << endl;
}


void hello_mistral() {
  Term* t1 = VariableTerm::make("a");
  Term* t2 = VariableTerm::make("b");

  map<Term*, long> elems;
  elems[t1] = 3;
  elems[t2] = 7;
  Term* t3 = ArithmeticTerm::make(elems, 2);  // 3a + 7b + 2

  vector<Term*> args;
  args.push_back(t1);
  Term* t4 = FunctionTerm::make("f", args, false);  // f(a)

  Constraint c1 {t4, t2, ATOM_LEQ};  // f(a) <= b
  Constraint c2 {t3, ConstantTerm::make(4), ATOM_EQ};  // 3a + 7b + 2 = 4
  Constraint c3 = c1 & c2;
  Constraint c4 = !c1;
  Constraint c5 = c4 | c2;

  bool res1 = c5.sat_discard();
  bool res2 = c3.valid_discard();
  bool res3 = c5.equivalent(c3);

  map<Term*, SatValue> assignment;
  bool res = c5.get_assignment(assignment);
  for (std::pair<Term*, SatValue> result : assignment) {
    cout << " Term: " << result.first->to_string()
         << " satisfying assignment: " << result.second.to_string()
         << endl;
  }
}
