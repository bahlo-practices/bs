#include <iostream>
#include <unistd.h>
#include <exception>

#include "colors.h"

using namespace std;

int main() {
  try {
    string input = "";

    cout << BLUE << " _______            _" << endl;
    cout <<           "|__   __|          | |" << endl;
    cout <<           "   | |_ __ __ _ ___| |__" << endl;
    cout <<           "   | | '__/ _` / __| '_ \\" << endl;
    cout <<           "   | | | | (_| \\__ \\ | | |" << endl;
    cout <<           "   |_|_|  \\__,_|___/_| |_|" << RESET << endl << endl;
    cout << BLUE << "Welcome to Trash! Exit with <logout>" << RESET << endl;

    while (true) {
      cout << "$ ";
      cin >> input;

      if (input == "logout") {
        break;
      } else if (input != "") {
        const char *cInput = input.c_str();
        cout << YELLOW << cInput << endl;
        execlp(cInput, cInput, NULL);
      }
    }
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
