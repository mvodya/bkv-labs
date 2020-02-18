// Software engineering
// Lab 5

#include <fstream>
#include <iostream>
#include <regex>
#include <experimental/filesystem>
#include <vector>
#include <sstream>

using namespace std;
namespace fs = std::experimental::filesystem;

bool bison_check() {
  cout << "Required bison version: 3.2.x\n";
  system("bison --version > ./tmp/bison_version");
  ifstream ifs("./tmp/bison_version");
  string in((istreambuf_iterator<char>(ifs)), (istreambuf_iterator<char>()));
  regex r("(\\d+)\\.(\\d+)\\.(\\d+)");
  smatch matches;
  if(regex_search(in, matches, r) && matches.size() >= 1) {
    cout << "Your version: " << matches[1] << "." << matches[2] << "." << matches[3] << endl;
    if (atoi(matches[1].str().c_str()) >=3 && atoi(matches[2].str().c_str()) >= 2) cout << "\x1B[32mBison version check completed\x1B[0m\n";
    else {
      cout << "\x1B[31mBison version check failed!\x1B[0m Your bison is too old\n";
      return false;
    }
    return true;
  }
  cout << "\x1B[31mBison error.\x1B[0m\n";
  cout << "What could have happened:\n - Bison is not installed. Try `apt install bison` or `yum install bison`\n - ./tmp is no available\n - Any other mystery thing...\n";

  return false;
}

vector<pair<string, string>> reg_functions;

void generate_functions() {
  fs::path path = "./calc/functions";
  regex r("(\\w+)\\.h");
  regex r_class("class (\\w+)\\s?:\\s?public Function[1|2]?");
  smatch match, match_class;
  cout << "Register calculator functions: \n";
  // List functions folder
  for (auto entry : fs::directory_iterator(path)) {
    // Get file path
    string name = entry.path().string();
    // Get filename
    if(regex_search(name, match, r) && match.size() >= 1) {
      string m = match[1].str();
      if (m != "fun1" && m != "fun2" && m != "function" && m != "const") {
        // File founded, get class name
        ifstream ifs("./calc/functions/" + m + ".h");
        string in((istreambuf_iterator<char>(ifs)), (istreambuf_iterator<char>()));
        if(regex_search(in, match_class, r_class) && match.size() >= 1) {
          reg_functions.push_back(make_pair(m, match_class[1].str()));
          cout << " - " << m << " (" << match_class[1].str() << ")" << endl;
        }
      }
    }
  }

  // Generate substitution
  stringstream include_substitution;
  stringstream map_substitution;
  ifstream ifs("./calc/stackmap.h");
  string in((istreambuf_iterator<char>(ifs)), (istreambuf_iterator<char>()));
  ofstream ofs("./calc/stackmap.h");

  regex r_include("\\/\\*GEN_INCLUDES_AREA\\*\\/([\\w|\\W]*)\\/\\*GEN_INCLUDES_AREA_END\\*\\/");
  regex r_map("\\/\\*GEN_FUNCTIONS_AREA\\*\\/([\\w|\\W]*)\\/\\*GEN_FUNCTIONS_AREA_END\\*\\/");

  cout << "Available functions: ";
  include_substitution << "/*GEN_INCLUDES_AREA*/\n";
  /* Generate help definition */
  include_substitution << "#define HELP_DATA \"Simple calculation example: 5 + 2 * 3\\nWrite variable: x = 10^2\\nAvailable functions: ";
  for (auto &i : reg_functions) {
    include_substitution << i.first;
    if (&i != &reg_functions.back()) {
      include_substitution << ", ";
    } else {
      include_substitution << "\\n";
    }
  }
  include_substitution << "Available constants: PI, E, PHI\\n" << "\"\n";
  /* Generate help definition END */
  map_substitution << "/*GEN_FUNCTIONS_AREA*/\n";
  for (auto &i : reg_functions) {
    cout << i.first;
    include_substitution << "#include \"functions/" << i.first << ".h\"\n";
    map_substitution << "{\"" << i.first << "\", new " << i.second << "()},\n";
    if (&i != &reg_functions.back()) {
      cout << ", ";
    } else {
      cout << endl;
    }
  }

  cout << "Making stackmap.h substitution... ";
  include_substitution << "/*GEN_INCLUDES_AREA_END*/";
  map_substitution << "/*GEN_FUNCTIONS_AREA_END*/";
  cout << "\x1B[32mdone\x1B[0m\n";

  in = regex_replace(in, r_include, include_substitution.str());
  in = regex_replace(in, r_map, map_substitution.str());

  // Write stackmap file
  ofs << in;
}

int main() {
  cout << "Checking enviroment\n";
  
  if (!bison_check()) {
    cout << "You can use precompilled bison file. Use? (y/n): ";
    char c;
    cin >> c;
    if (c == 'y') {
      // Yes
      cout << "Copying pre.parser.tab.cc to parser.tab.cc\n";
      system("cp calc/pre.parser.tab.cc calc/parser.tab.cc");
    } else {
      // No
      cout << "Stop.\n";
      return 0;
    }
  } else {
    // Run bison
    cout << "Running bison generation\n";
    system("bison calc/parser.yy -o calc/parser.tab.cc");
    cout << "Bison generation done\n";
  }

  generate_functions();

  // Build g++ calc
  cout << "Running g++ compilation\n";
  system("g++ -std=c++17 calc/main.cpp -o calculator");
  cout << "g++ compilation done\n";

  cout << "Builder program complete\nOutput file: \x1B[4m\x1B[1mcalculator\x1B[0m\n";
}