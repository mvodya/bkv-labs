// Software engineering
// Lab 5

%require "3.2"
%language "c++"

%code {

#include <string>
#include <queue>
#include <regex>
#include <cmath>

using namespace std;

}

%define api.value.type variant

%define api.token.constructor

%token <double> NUMBER
%token <calc::Symbol*> FUNC VAR UNDF
%type <double> expr assign
%type <double> prog
%right '='
%left  '+' '-'
%left  '*' '/'
%right '^'
%left UNMIN

%code {

namespace yy {

queue<string> sequence;
double result = -1;
bool berror = 0;
std::string error_msg = "";

auto yylex() -> parser::symbol_type {
  if (sequence.size() <= 0) return 0;
  string top = sequence.front();
  sequence.pop();

  if (isdigit(top[0])) {
    double number = atof(top.c_str());
    return parser::make_NUMBER(number);
  }

  if(isalpha(top[0])) {
    if(calc::stackMap.count(top)) {
        auto sm = calc::stackMap[top];
        if (sm->getType() == calc::Symbol::Type::S_FUNC) {
          // For function
          return parser::make_FUNC(sm);
        } else {
          // For var/const
          if (sm->rewritable()) {
            // Var
            return parser::make_VAR(sm);
          } else {
            // Const
            return parser::make_NUMBER(sm->call());
          }
        }
    } else {
      // Define new var
      calc::Var *v = new calc::Var();
      calc::stackMap[top] = v;
      return parser::make_VAR(calc::stackMap[top]);
    }
  }

  return top[0];
}
}

}

%%

prog:
'\n' { }
| expr '\n' { $$ = $1; result = $1; }
| prog expr '\n' { $$ = $2; result = $2; }
;

assign:
VAR '=' expr { $$ = $3; $1->set($3); }
;

expr:
NUMBER { $$ = $1; }
| VAR  { $$ = $1->call(); }
| FUNC { $$ = $1->call(); }
| FUNC '(' expr ')' { $$ = $1->call($3); }
| FUNC '(' expr ',' expr ')' { $$ = $1->call($3, $5); }
| expr '+' expr { $$ = $1 + $3; }
| expr '-' expr { $$ = $1 - $3; }
| expr '*' expr { $$ = $1 * $3; }
| expr '/' expr { if($3 != 0) $$ = $1 / $3; else yy::parser::error("Division by zero!"); }
| expr '^' expr { $$ = pow($1, $3); }
| '-' expr %prec UNMIN { $$ = -$2; }
| assign
| '(' expr ')' { $$ = $2; }
;
%%

namespace yy
{
auto parser::error (const std::string& msg) -> void {
  //std::cerr << msg << '\n';
  berror = true;
  error_msg = msg;
}
void sequencer(string& in) {
  // original: \d+\.\d+|\d+|\w+|\+|\-|\(|\)|\*|\/|\^|\=|\,
  /* d.d | d | char&d | + - ( ) * / ^ = ,
   */
  regex r("\\d+\\.\\d+|\\d+|\\w+|\\+|\\-|\\(|\\)|\\*|\\/|\\^|\\=|\\,");

  sregex_iterator i = sregex_iterator(in.begin(), in.end(), r);
  for (; i != sregex_iterator(); i++) {
    smatch match = *i;
    sequence.push(match.str(0));
  }
  sequence.push("\n");
}
}