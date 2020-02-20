#include <iostream>
#include <regex>
#include <fstream>
#include <vector>
#include <sstream>

using namespace std;

int main() {
  ifstream input;
  input.open("listing.txt");
  
  if (!input.is_open()) return 1;

  string line;

  while(getline(input, line)) {
    regex re("\\s(\\d{6})");
    vector<unsigned short int> data;

    for(sregex_iterator it = sregex_iterator(line.begin(), line.end(), re); it != sregex_iterator(); it++) {
      smatch match = *it;
      for (int i=1; i < match.size(); i++) {
          unsigned short int a;
          stringstream ss(match[i]);
          ss >> std::oct >> a;
          data.push_back(a);
      }
    }

    if (data.size() > 1) {
      unsigned short int addr = data[0];
      for(int i=1; i < data.size(); i++) {
        cout << "d " << std::oct << addr << " " << data[i] << endl;
        addr += 2;
      }
    }
  }

  input.close();
  return 0;
}