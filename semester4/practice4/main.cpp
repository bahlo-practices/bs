#include <cstdlib>
#include <iostream>
#include <math.h>
#include <vector>

using namespace std;

struct doneAllocations {
  void * allocationStart;
  void * allocationEnd;
  int size;
};
void * start;
int length;
int strategie;
char charToWrite = 'a';
char numberToWrite = '1';
vector<doneAllocations> da;
bool memorySizing = true;
int allocationcounter = 0;

void mystatus() {
  cout << "\nTotalmem: " << length << " Bytes";
  for (int i = 0; i < da.size(); i++) {
    cout << "\nAllocation: " << i;
    cout << "\nAllocation-start: " << da.at(i).allocationStart
      << " Allocation-end: " << da.at(i).allocationEnd;
  }
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
  cout << "Start: " << start << "\tEnd: " << start + length;
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
    //cout<<"\nEMPTY: "<<empty;
  }
  //buddy algorithmus - Initialisiere Speicher mit verschiedenen Zahlen für die Unterteilung
  if (strategie == 1) {
    int k = log2(length);
    int newlength = length;
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
        numberToWrite++;
      }
      memorySizing = false;
    }

    read = (char*) start;
    //aktuelle belegung

    // HIER BAUSTELLE, PRÜFEN OB SPEICHER BEREICHE GROSS GENUG, SONST VERGRÖSSERN UND ZUSAMMENFÜGEN

    for (int i = 0; i < length; i++) { //<-- DÜRFTE MIST SEIN
      if (*read != numberToWrite) {
        empty++;
        read++;
      }
      if (empty == size) {
        placeToWrite = i - empty + 1;
        cout << "\nplaceToWrite: " << placeToWrite;
        bwrite = true;
        i = size;
        break;
      }
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
  doneAllocations da0;
  da0.allocationStart = start + placeToWrite;
  da0.size = size;
  da0.allocationEnd = start + placeToWrite + size;
  da.push_back(da0);
  mystatus();
  charToWrite++;
  allocationcounter++;
  return start + placeToWrite;
}
void myfree(void *p) {
  cout<<"\n da size: "<<da.size();

  char* read = (char*) p;
  bool freed = false;
  char memory;
  //VECTOR LÖSCHEN
  vector<doneAllocations>::iterator it;
  for(int i = 0; i < da.size(); i++){
    if(da.at(i).allocationStart == p){
      da.erase(da.begin()+i);
    }
  }

  cout<<"\n da size new: "<<da.size();
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
    }        while (!freed);
  }
}
void printMemory() {
  char * write = (char*) start;
  cout << "\n";
  cout << "\n" << "länge: " << length << "\n";
  for (int i = 0; i < length; i++) {
    cout << *write;
    write++;
  }
}
void testFirstFit() {
  void * a0 = myalloc(8, __LINE__);
  myfree(a0);
  void * a1 = myalloc(2, __LINE__);
  //myfree(a1);
  void * a2 = myalloc(6, __LINE__);
  //    char* test = (char*) a2;
  //    cout<<"\n a0: "<<*test;
}
void testBuddyAlgorithm() {
  myalloc(2, __LINE__);
}
int main(int argc, char** argv) {
  myinit(32, 1);
  testBuddyAlgorithm();
  //testFirstFit();
  printMemory();
  mycleanup();
  return 0;
}
