// Software engineering
// Lab 1
#define _USE_MATH_DEFINES
#include <cmath>
#include <cstdlib>
#include <iostream>
#include <sstream>
#include <stack>
#include <string>
using namespace std;

// Load element from stack and pop them
inline void pop(stack<double>& s, double& x) {
  x = s.top();
  s.pop();
}

// Load two elements from stack and pop them
inline void pop2(stack<double>& s, double& a, double& b) {
  b = s.top();
  s.pop();
  a = s.top();
  s.pop();
}

// Parse row
bool interpreter(int n, string* row, double x) {
  string* token;
  stack<double> s;
  // Read n tokens
  for (int i = 0; i < n; i++) {
    token = &row[i];
    double a, b;
    // If token math operation - working with stack
    if (*token == "+") {
      pop2(s, a, b);
      s.push(a + b);
    } else if (*token == "-") {
      pop2(s, a, b);
      s.push(a - b);
    } else if (*token == "*") {
      pop2(s, a, b);
      s.push(a * b);
    } else if (*token == "sin") {
      pop(s, a);
      s.push(sin(a));
    } else if (*token == "cos") {
      pop(s, a);
      s.push(cos(a));
    } else if (*token == "tan") {
      pop(s, a);
      s.push(tan(a));
    } else if (*token == "x") {
      // If this is x, use special mini number (parametr x)
      s.push(x);
    } else {
      // In any other situation (example: just number)
      s.push(atof(token->c_str()));
    }
  }
  // Check result
  if (s.top() == 0)
    return true;
  else
    return false;
}

int main() {
  int n;
  string* row;
  stringstream answer;
  // freopen("input.txt", "r", stdin); [FOR FILE READ&INPUT]

  // Get count of tokens
  cin >> n;
  do {
    // Read all row to string array
    row = new string[n];
    for (int i = 0; i < n; i++) cin >> row[i];
    // Call row interpreter
    if (interpreter(n, row, .0123) && interpreter(n, row, .001)) {
      answer << "Identity\n";
    } else {
      // Check tangens with PI
      if (interpreter(n, row, M_PI))
        answer << "Identity\n";
      else
        answer << "Not an identity\n";
    }
    delete[] row;
    // Get count of tokens
    cin >> n;
  } while (n != 0);  // Detect zero terminator

  // Answer
  cout << answer.str();

  return 0;
}