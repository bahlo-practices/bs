#include <iostream>
#include <unistd.h>
#include <vector>
#include <sstream>
#include <iterator>
#include <algorithm>
#include <exception>

#include "colors.h"

using namespace std;

// Helper function to convert from string to char
char *convert(const std::string & s) {
   char *pc = new char[s.size()+1];
   std::strcpy(pc, s.c_str());
   return pc;
}

int main() {
  try {
    string input = "";

    // Print emblem
    cout << BLUE << " _______            _" << endl;
    cout <<         "|__   __|          | |" << endl;
    cout <<         "   | |_ __ __ _ ___| |__" << endl;
    cout <<         "   | | '__/ _` / __| '_ \\" << endl;
    cout <<         "   | | | | (_| \\__ \\ | | |" << endl;
    cout <<         "   |_|_|  \\__,_|___/_| |_|" << RESET << endl << endl;
    // Print welcome message
    cout << BLUE << "Welcome to Trash! Exit with <logout>" << RESET << endl;

    while (true) {
      cout << "$ ";
      cin >> input;

      if (input == "logout") {
        break;
      } else if (input != "") {
        // Split string
        input = "ls -la /home/arne";
        istringstream iss(input);

        // Split into arguments
        vector<string> arguments;
        copy(istream_iterator<string>(iss),
                 istream_iterator<string>(),
                 back_inserter<vector<string> >(arguments));

        // Rotate vector to the left, to get the command at the end,
        // so we can pop_back it
        string command = arguments.at(0);
        std::rotate(arguments.begin(), arguments.begin() + 1, arguments.end());
        arguments.pop_back();

        // Transform arguments vector<string> to char *const *
        char ** crguments = new char*[arguments.size()];
        for(size_t i = 0; i < arguments.size(); ++i){
            crguments[i] = new char[arguments[i].size() + 1];
            strcpy(crguments[i], arguments[i].c_str());
        }

        cout << YELLOW << arguments.size() << RESET << endl;
        cout << GREEN << command << RESET << endl;
        execvp(command.c_str(), crguments);
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
