#include <iostream>
#include <unistd.h>
#include <vector>
#include <sstream>
#include <iterator>
#include <algorithm>
#include <exception>

#include "colors.h"

using namespace std;

int main() {
  try {
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
      string input = "";
      cout << "$ ";

      // Get command
      // cin >> input would stop reading at spaces
      getline(cin, input);

      if (input == "logout") {
        break;
      } else if (input != "") {
        int status;
        // Split string
        istringstream iss(input);

        // Split into arguments
        vector<string> arguments;
        copy(istream_iterator<string>(iss),
                 istream_iterator<string>(),
                 back_inserter<vector<string> >(arguments));

        // Rotate vector to the left, to get the command (first item)
        // at the end, so we can pop_back it
        string command = arguments.at(0);
        std::rotate(arguments.begin(), arguments.begin() + 1, arguments.end());
        arguments.pop_back();

        // Transform arguments vector<string> to char *const *
        char ** crguments = new char*[arguments.size()];
        for(size_t i = 0; i < arguments.size(); ++i){
            crguments[i] = new char[arguments[i].size() + 1];
            strcpy(crguments[i], arguments[i].c_str());
        }

        // Fork it, make it, do it, makes us
        // harder, better, faster, Stronger
        pid_t childPID = fork();
        if (childPID == 0) {
          // Execute
          // v: char array as arguments
          // p: also search in $PATH
          execvp(command.c_str(), crguments);

          // Exit child pid
          exit(childPID);
        } else  {
          waitpid(childPID, &status, 0);
        }
      }
    }
  } catch(const string &e) {
    cerr << RED << e << RESET << endl;
    return 1;
  } catch(const std::exception &e) {
    cerr << RED << e.what() << RESET << endl;
    return 2;
  } catch(...) {
    cerr << RED << "An unknown error occured" << RESET << endl;
    return 3;
  }

  return 0;
}
