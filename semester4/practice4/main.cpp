/*
 * File:   main.cpp
 * Author: fbi
 *
 * Created on 12. Juni 2014, 16:06
 */
#include <cstdlib>
#include <iostream>
#include <math.h>
#include <vector>
using namespace std;
struct doneAllocations {
  void * allocationStart;
  void * allocationEnd;
  int size;
  int line;
};
void * start;
int length;
int strategie;
char charToWrite = 'a';
char numberToWrite = '1';
vector<doneAllocations> da;
bool memorySizing = true;
/*
 *
 */
int allocationcounter = 0;
void mystatus() {
  int totalAllocated = 0;
  int last = 0;
  for (int i = 0; i < da.size(); i++) {
    totalAllocated = totalAllocated + da.at(i).size;
    last = i;
  }
  cout << "Start: " << start << "\tEnd: " << start + length;
  cout << "\nTotalmem: " << length << " Bytes";
  cout << "\nTotal allocated: " << totalAllocated << "Bytes";
  cout << "\nAllocation: " << allocationcounter;
  cout << "\nAllocation-start: " << da.at(last).allocationStart
    << " Allocation-end: " << da.at(last).allocationEnd;
  cout << "\nmymalloc(" << da.at(last).size << "); in line   " << da.at(last).line;
}
void myinit(unsigned int totalmem, int strategy) {
  strategie = strategy;
  void * first;
  first = malloc(totalmem);
  start = first;
  length = totalmem;
  char * write;
  write = (char*) first;
  for (int i = 0; i < totalmem; i++) {
    if (i < totalmem) {
      * write = 'I';
      write++;
    }
  }
}
void mycleanup() {
  char * deleteIT = (char*) start;
  delete deleteIT;
  start = NULL;
  for (int i = 0; i < da.size(); i++) {
    da.pop_back();
  }
}
void *myalloc(unsigned int size, int line) {
  char* read = (char*) start;
  char* write;
  int empty = 0;
  bool bwrite = false;
  int placeToWrite = 0;
  //first fit - initialisiere speicher mit I als Platzhalter
  if (strategie == 0) {
    for (int i = 0; i < length; i++) {
      if (* read == 'I') {
        empty++;
        read++;
      }
      if (empty == size) {
        placeToWrite = i - empty + 1;
        //cout<<"\nplaceToWrite: "<<placeToWrite;
        bwrite = true;
        i = size;
        break;
      }
      if (* read != 'I') {
        read++;
      }
    }
  }
  //buddy algorithmus - Initialisiere Speicher mit verschiedenen Zahlen für die Unterteilung
  if (strategie == 1) {
    int k, newlength;

    if (length &&!(length & (length - 1))) {
      k = log2(length);
      newlength = length;
    } else { //size auf nächste zweier potenz aufrunden
      k = 5;
      newlength = length;
    }
    //SPEICHEREINTEILUNG HIER NOCH: fall speicher bereich keine zweier potenz
    if (memorySizing) { // im ersten Schritt ist dies die speicher unterteilung
      bool firstRun = true;
      for (int i = 0; i < k; i++) {
        if (firstRun) {
          for (int j = 0; j < length; j++) {
            *read = numberToWrite; //change to numberToWrite
            read++;
          }
          firstRun = false;
          numberToWrite++;
        } else {
          newlength = newlength / 2;
          read = (char*) start;
          for (int a = 0; a < newlength; a++) {
            * read = numberToWrite; //change to numberToWrite
            read++;
          }
        }
        if (numberToWrite == '9') {
          numberToWrite == '1';
        } else {
          numberToWrite++;
        }
      }
      memorySizing = false;
    }
    //PRÜFUNG AUF ZWEIER POTENZ
    if (size &&!(size & (size - 1))) {
      //zahl ist zweierpotenz
    } else { //size auf nächste zweier potenz aufrunden
      do {
        size++;
      } while (!size &&!(size & (size - 1)));
    }
    //BELEGUNG
    char * search = (char*) start;
    int searchCounter = 0;
    int totallength = 0;
    bool writeNow = false;
    if (da.size() != 0) {
      for (int z = 0; z < da.size(); z++) {
        totallength = totallength + da.at(z).size;
      }
      for (int i = 0; i < length; i++) {
        if (writeNow) {
          bwrite = true;
          break;
        }
        for (int i = 0; i < da.size(); i++) { // wenn speicherbereich belegt;
          if (size > (length - totallength)) {
            break;
          }
          if (da.at(i).allocationStart == search) {
            search = search + da.at(i).size;
            searchCounter++;
            //cout<<"Speicherbereich belegt";
          } else { //finde passenden speicherplatz
            search++;
            read = (char*) search;
            for (int j = 0; j < size; j++) {
              if (*read == *search) {
                empty++;
                read++;
              }
              if (empty == size) {
                for (int a = 0; a < searchCounter; a++) {
                  placeToWrite = placeToWrite + da.at(a).size;
                }
                cout << "\nplaceToWrite: " << placeToWrite;
                writeNow = true;
                bwrite = true;
                i = size;
                break;
              }
              if (*read != *search) { // falls grenze von freiem speicherbereich erreicht
                bool found = false;
                char * temp = read;
                search = (char*) start;
                for (int c = 0; c < da.size(); c++) { // wenn speicherbereich belegt;
                  if (da.at(c).allocationStart == read) {
                    found = true;
                  }
                }
                if (found) {
                } else {
                  search = read;
                }
              }
            }
          }
        }
      }
    } else { //falls noch keine allocationen durchgeführt wurden
      placeToWrite = 0;
      bwrite = true;
    }
  }
  if (bwrite) {
    write = (char*) start;
    write = write + placeToWrite;
    for (int i = 0; i < size; i++) {
      *write = charToWrite;
      write++;
    }
  }
  if (bwrite) {
    doneAllocations da0;
    da0.allocationStart = start + placeToWrite;
    da0.size = size;
    da0.allocationEnd = start + placeToWrite + size;
    da0.line = line;
    da.push_back(da0);
  }
  mystatus();
  charToWrite++;
  allocationcounter++;
  return start + placeToWrite;
}
void myfree(void *p) {
  //cout << "\n da size: " << da.size();
  char* read = (char*) p;
  bool freed = false;
  char memory;
  //VECTOR LÖSCHEN
  vector<doneAllocations>::iterator it;
  for (int i = 0; i < da.size(); i++) {
    if (da.at(i).allocationStart == p) {
      da.erase(da.begin() + i);
    }
  }
  //cout << "\n da size new: " << da.size();
  //SPEICHER BEREINIGEN
  if (strategie == 0) {
    if (*read != 'I') {
      memory = *read;
      do {
        if (*read == memory) {
          *read = 'I';
          read++;
        }
        if (*read != memory) {
          freed = true;
          break;
        }
      } while (!freed);
    } else {
      cout << "\nMemory not allocated!";
    }
  }
  if (strategie == 1) {
    numberToWrite--;
    if (numberToWrite == '1') {
      numberToWrite == '2';
    }
    memory = *read;
    do {
      if (*read == memory) {
        *read = numberToWrite;
        read++;
      }
      if (*read != memory) {
        freed = true;
        break;
      }
    } while (!freed);
  }
}
void printMemory() {
  char * write = (char*) start;
  cout << "\n";
  cout << "\n" << "länge: " << length << "\n";
  for (int i = 0; i < length; i++) {
    cout << *write;
    ++write;
  }
}
void testFirstFit() {
  void * a0 = myalloc(8, __LINE__);
  //myfree(a0);
  void * a1 = myalloc(8, __LINE__);
  myfree(a1);
  void * a2 = myalloc(4, __LINE__);
  void * a3 = myalloc(4, __LINE__);
  //    char* test = (char*) a2;
  //    cout<<"\n a0: "<<*test;
}
void testBuddyAlgorithm() {
  void * a0 = myalloc(8, __LINE__);
  void * a1 = myalloc(8, __LINE__);
  myfree(a0);
  void * a2 = myalloc(4, __LINE__);
  void * a3 = myalloc(3, __LINE__);
}
int main(int argc, char** argv) {
  myinit(16, 1);
  testBuddyAlgorithm();
  //testFirstFit();
  printMemory();
  mycleanup();
  return 0;
}
