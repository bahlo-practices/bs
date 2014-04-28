#include <iostream>
#include <unistd.h>
#include <vector>
#include <sstream>
#include <iterator>
#include <algorithm>
#include <exception>

#include "colors.h"

using namespace std;

vector<string> split_str(string input, char delimiter) {
  istringstream iss(input);
  vector<string> arguments;

  copy(istream_iterator<string>(iss),
       istream_iterator<string>(),
       back_inserter<vector<string> >(arguments));

  return arguments;
}

int main(int argc, char* argv[]) {
  try {
    // Print emblem and welcome message
    cout << BLUE << " _______            _" << endl;
    cout <<         "|__   __|          | |" << endl;
    cout <<         "   | |_ __ __ _ ___| |__" << endl;
    cout <<         "   | | '__/ _` / __| '_ \\" << endl;
    cout <<         "   | | | | (_| \\__ \\ | | |" << endl;
    cout <<         "   |_|_|  \\__,_|___/_| |_|" << RESET << endl << endl;
    cout << BLUE << "Welcome to Trash! Exit with <logout>" << RESET << endl;

    while (true) {
      string input = "";
      cout << "$ ";

      // Get command
      // cin >> input would stop reading at spaces
      getline(cin, input);

      if (input == "logout") {
        break; // free
      } else if (input != "") {
        int status;
        bool forkIt = false;

        // Split string by space
        vector<string> arguments = split_str(input, ' ');

        // Check if the last argument is '&'
        if (arguments.size() > 0 && arguments.at(arguments.size() -1) == "&") {
          forkIt = true;

          // Remove '&'
          arguments.pop_back();
        }

        // Transform arguments vector<string> to char *const *
        char ** crguments = new char*[arguments.size() + 1];
        for(size_t i = 0; i < arguments.size(); ++i){
            crguments[i] = new char[arguments[i].size() + 1];
            strcpy(crguments[i], arguments[i].c_str());
        }
        crguments[arguments.size() + 1] = NULL; // Null-terminate array

        // Fork it, make it, do it, makes us
        // harder, better, faster, stronger
        pid_t childPID = fork();
        switch (childPID) {
          case -1:
            cerr << "[" << childPID << "]" << " Error creating child process" << endl;
          case 0:
            // We're at the child process
            // Execute
            // v: char pointer array as arguments
            // p: also search in $PATH
            execvp(crguments[0], crguments);

            // Exit child pid
            exit(childPID);
            break;
          default:
            if (!forkIt) {
              waitpid(childPID, &status, 0);
            }
            break;
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
