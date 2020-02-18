// Software engineering
// Lab 2
#include <iostream>
#include <stack>
#include <string>

using namespace std;

namespace priority {
// Priority for input tokens
int input(char c) {
  switch (c) {
    case '+':
    case '-':
      return 1;
    case '*':
    case '/':
      return 3;
    case '(':
      return 7;
    case ')':
      return 0;
    default:
      return 5;
  }
}
// Priority for stack tokens
inline int stack(char c) { return (input(c) + 1) % 8; }
}  // namespace priority

void interpreter(string& token, stack<double>& s) {
  double r;
  switch (token[0]) {
    case '+':
      r = s.top();
      s.pop();
      r += s.top();
      s.pop();
      s.push(r);
      break;
    case '-':
      r = s.top();
      s.pop();
      r = s.top() - r;
      s.pop();
      s.push(r);
      break;
    case '*':
      r = s.top();
      s.pop();
      r *= s.top();
      s.pop();
      s.push(r);
      break;
    case '/':
      r = s.top();
      s.pop();
      r = s.top() / r;
      s.pop();
      s.push(r);
      break;
    default:
      s.push(atof(token.c_str()));
      break;
  }
}

int main(int argc, char* argv[]) {
  string* input;
  stack<string> tokens;
  stack<double> calculate;

  if (argc < 2) {
    cout << "Need arguments. For example: 5 + 5\n";
    return 1;
  }

  // Read arguments to array
  input = new string[argc];
  for (int i = 1; i < argc; i++) input[i - 1] = argv[i];
  input[argc - 1] = ")";

  tokens.push("(");

  // Generate polish notation
  for (int i = 0; i < argc; i++) {
    // Calculate token priority
    int p = priority::stack(tokens.top()[0]) - priority::input(input[i][0]);
    if (p < 0) {
      tokens.push(input[i]);
    } else if (p > 0) {
      // Work with token
      interpreter(tokens.top(), calculate);
      tokens.pop();
      i--;
    } else {
      tokens.pop();
    }
  }

  cout << calculate.top() << endl;

  return 0;
}