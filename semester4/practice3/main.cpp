/*
 * File:   main.cpp
 * Author: fbi
 *
 * Created on 10. April 2014, 23:13
 */

#include <vector>
#include <cstdlib>
#include <iostream>
#include <unistd.h>
#include <string.h>
#include <sys/wait.h>
#include <sys/types.h>
#include <stdio.h>
#include <signal.h>
#include <iterator>
#include <algorithm>
using namespace std;

pid_t mypid = 0;
int childCounter = 0;
vector<int> pids;

void handle_SIGCONT(int i) {
    //kill(getpid(), SIGCONT);
    //cout << "caught SIGCONT\n";
    //cout << "$";
}

void handle_SIGCHLD(int i) {
    //cout << "caught SIGCHLD\n";
    int status;
        while (waitpid(-1, &status, WNOHANG) > 0) {
            if (WIFEXITED(status)) childCounter--;
        }

        // something happened with child 'pid', do something about it...
        // Details are in 'status', see waitpid() manpage
   // cout << "$";

}

void handle_SIGINT(int i) {
    childCounter--;
    //cout << "caught SIGINT \n";
    //    cout << "____DEBUG: size of pid vector: " << pids.size() << "\n";
    //
    //
    //    int a=0;
    //    if (pids.size() > 0) {
    //        for (a = 0; a < pids.size(); a++) {
    //            if(mypid == pids.at(i)) break;
    //        }
    //    }
    //    
    //    
    //    if(find(pids.begin(),pids.end(), mypid) != pids.end()){
    //        pids.erase(find(pids.begin(),pids.end(), mypid) );
    //    }
    //    
    //    cout<<"_____DEBUG: Deleting pid: "<<mypid<<" from Vector\n";
    cout<<"$";
    kill(mypid, SIGINT);
}

void handle_SIGSTOP(int i) {
    //cout << "caught SIGTSTP \n" << mypid;
    
    //cout << "$" << mypid;
    kill(mypid, SIGSTOP);
}

void executeProgram(string command) {

    string value = "";
    int countOfArguments = 0;
    string::iterator it = command.begin();

    //Anzahl der übergebenen Argumente ermitteln
    while (it != command.end()) {

        if (*it != ' ') {
            countOfArguments++;
            while (it != command.end() && *it != ' ') {
                it++;
            }
        }

        while (it != command.end() && *it == ' ') {
            it++;
        }
    }

    // check if last char is an '&' an decrement countofargs is true.
    if (command.at(command.size() - 1) == '&' || command.at(command.size() -
            1) == ' ') {
        int lastChar = command.size() - 1;
        while (command.at(lastChar) == ' ') {
            lastChar--;
        }
        if (command.at(lastChar) == '&') {
            countOfArguments--;
        }
    }

    //    cout << "Count = " << countOfArguments << "\n";


    //    cout << "---------Args geschreiben---------------\n";

    it = command.begin();
    char ** args = new char*[countOfArguments + 1];
    int index = 0;
    while (it != command.end() && index < countOfArguments) {
        if (*it != ' ') {
            while (it != command.end() && *it != ' ') {
                //                cout << *it;
                value += *it;
                it++;
            }

            args[index] = new char[value.length() + 1];
            strcpy(args[index], value.c_str());
            //            cout << "args[" << index << "] = " << value << ".\n";
            value = "";
            index++;
        }

        while (it != command.end() && *it == ' ') {
            //cout << *it;
            it++;
        }


    }

    args[countOfArguments] = NULL;
    //    cout << "--------Array----------\n";
    for (int i = 0; i < countOfArguments; i++) {
        //        cout << "args[" << i << "] = " << args[i] << ".\n";
    }
    execvp(args[0], args);
    exit;
}

int main(int argc, char** argv) {

    pid_t pid;
    cout << "Welcome to myshell. Exit with \"logout\".\n$ ";

    signal(SIGINT, handle_SIGINT);
    signal(SIGTSTP, handle_SIGSTOP);
    signal(SIGCHLD, handle_SIGCHLD);
    signal(SIGCONT, handle_SIGCONT);


    string program = "";
    bool login = true;
    int status;
    bool wait = true;

    while (login) {

        getline(cin, program);

        if (program == "logout") {
            if (childCounter > 0) {
                cout << "Logout not possible. " << childCounter << " running Processes";
                cout << "\n$";
            } else {
                login = false;
                cout << "All done. Good bye!\n";
                break;
            }
        } else if (program == "fg") {
            //cout << "\nTrying to resume children. Sending pid to: " << mypid << "\n";
            kill(mypid, SIGCONT);
            waitpid(pid, &status, WUNTRACED);


        } else if (program == "bg") {
            //cout << "\nTrying to resume children. Sending pid to: " << mypid << "\n";
            kill(mypid, SIGCONT);

        } else {
            switch (pid = fork()) {
                case -1:
                    cout << "Fehler bei fork()\n";
                    break;
                case 0:
                    executeProgram(program);
                    break;
                default:
                    childCounter++;
                    mypid = pid;
                    //cout << "[" << mypid << "]";

                    if (program.at(program.size() - 1) == '&' ||
                            program.at(program.size() - 1) == ' ') {
                        // wait = true;
                        int lastChar = program.size() - 1;
                        while (program.at(lastChar) == ' ') {
                            lastChar--;
                        }
                        if (program.at(lastChar) == '&') {
                            wait = false;
                        }
                    }

                    if (wait) {
                        waitpid(pid, &status, WUNTRACED);
                        //waitpid(pid, &status, 0);

                    }

                    cout << "$ ";
                    wait = true;


            };
        }
    };
    return 0;
}