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
        istringstream iss(input);
        vector<string> arguments;
        copy(istream_iterator<string>(iss),
             istream_iterator<string>(),
             back_inserter<vector<string> >(arguments));

        // Rotate vector to the left, to get the command (first item)
        // at the end, so we can pop_back it
        string command_str = vector_shift(arguments);
        const char *command = command_str.c_str();

        // Check if the last argument is '&'
        if (arguments.size() > 0 && arguments.at(arguments.size() -1) == "&") {
          forkIt = true;

          // Remove '&'
          arguments.pop_back();
        }

        // Transform arguments vector<string> to char *const *
        char ** crguments = new char*[arguments.size()];
        for(size_t i = 0; i < arguments.size(); ++i){
            crguments[i] = new char[arguments[i].size() + 1];
            strcpy(crguments[i], arguments[i].c_str());
        }

        // Fork it, make it, do it, makes us
        // harder, better, faster, stronger
        pid_t childPID = fork();
        if (childPID == 0) {
          // Execute
          // v: char array as arguments
          // p: also search in $PATH
          execvp(command, crguments);

          // Exit child pid
          exit(childPID);
        } else  {
          if (forkIt) {
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
