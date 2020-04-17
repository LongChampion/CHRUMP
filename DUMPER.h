#pragma once
#ifndef DUMPER_H
#define DUMPER_H

#include <string>
#include <winsqlite/winsqlite3.h>

// Base class for all dumpers, have some basic method of them
class DUMPER {
protected:
  sqlite3 *pDatabase; // Pointer to SQLite3 database to dump

  int CloseDatabase(); // Method to close the database

  // Method to backup database before process it
  int BackupDatabase(const std::string &);

  // ALL DUMPER MUST DEFINE THE FOLLOWING FUNCTION
  // Callback function to add an entry to data vector of dumper
  // static int __stdcall AddEntry(void *, int, char **, char **);

public:
  // Simple constructor and destructor
  DUMPER() : pDatabase(nullptr){};
  ~DUMPER() { CloseDatabase(); };

  // Method to open a database
  int OpenDatabase(const std::string &);

  // Method to dump a database and save result in data vector
  virtual int Dump() = 0;

  // Method to print all data vector to screen
  virtual int Show() = 0;
};
#endif // !DUMPER_H
