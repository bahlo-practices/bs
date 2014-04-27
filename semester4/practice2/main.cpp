#include <iostream>
#include <unistd.h>
#include <vector>
#include <sstream>
#include <iterator>
#include <algorithm>
#include <exception>

#include "colors.h"

using namespace std;

/**
 * Helper function to shift values from an array
 */
string vector_shift(vector<string> &v) {
  string shifted = v.at(0);
  rotate(v.begin(), v.begin() + 1, v.end());
  v.pop_back();

  return shifted;
}

int main(int argc, char* argv[]) {
  try {
    bool debug = true;

    // Check arguments
    if (argc > 1) {
      for (int i = 1;i < argc;++i) {
        if (strncmp(argv[1], "debug", 5)) {
          debug = true;
        }
      }
    }

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
        if (debug) cout << YELLOW << "Parsing arguments.." << endl;
        istringstream iss(input);
        vector<string> arguments;
        copy(istream_iterator<string>(iss),
             istream_iterator<string>(),
             back_inserter<vector<string> >(arguments));

        if (debug) {
          cout << YELLOW << "Arguments: " << endl;
          for(size_t i = 0; i < arguments.size(); ++i) {
            cout << " -- " << arguments.at(i) << endl;
          }
          cout << RESET;
        }

        // Rotate vector to the left, to get the command (first item)
        // at the end, so we can pop_back it
        if (debug) cout << YELLOW << "Shifting arguments.." << endl;
        string command = vector_shift(arguments);

        // Check if the last argument is '&'
        if (debug) cout << YELLOW << "Checking for &.." << endl;
        if (arguments.size() > 0 && arguments.at(arguments.size() -1) == "&") {
          if (debug) cout << YELLOW << "Found &, set fork flag" << endl;
          forkIt = true;

          // Remove '&'
          arguments.pop_back();
        }

        // Transform arguments vector<string> to char *const *
        if (debug) cout << YELLOW << "Transform vector to char array.." << endl;
        char ** crguments = new char*[arguments.size()];
        for(size_t i = 0; i < arguments.size(); ++i){
            crguments[i] = new char[arguments[i].size() + 1];
            strcpy(crguments[i], arguments[i].c_str());
        }

        // Fork it, make it, do it, makes us
        // harder, better, faster, stronger
        pid_t childPID = fork();
        if (childPID == 0) {
          if (debug) cout << YELLOW << "Executing.." << endl;
          // Execute
          // v: char array as arguments
          // p: also search in $PATH
          execvp(command.c_str(), crguments);

          // Exit child pid
          exit(childPID);
        } else  {
          if (forkIt) {
            if (debug) cout << YELLOW << "Waiting for process.." << endl;
            waitpid(childPID, &status, 0);
          }
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
