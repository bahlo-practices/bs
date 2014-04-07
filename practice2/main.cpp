#include <iostream>
#include <exception>

#include "colors.h"

using namespace std;

int main() {
  setlocale(0, "DE");

  try {
    string input = "";
    cout << BLUE << "Welcome to Trash! Exit with <logout>" << RESET << endl;
    while (true) {
      cout << "$ ";
      cin >> input;

      if (input == "logout") {
        break;
      } else if (input != "") {
        cout << "COMMAND: " << input << endl;
      }
    }
    return 0;
  } catch(const string &e) {
    cerr << RED << e << endl;
    return 1;
  } catch(const std::exception &e) {
    cerr << RED << e.what() << endl;
  } catch(...) {
    cerr << RED << "Ein unbekannter Fehler ist aufgetreten." << endl;
    return 2;
  }

  return 0;
}
