#include "DUMPER.h"
#include <iostream>
#include <string>

int DUMPER::OpenDatabase(const std::string &Filename) {
  CloseDatabase();
  int rc = sqlite3_open(Filename.c_str(), &pDatabase);
  if (rc != SQLITE_OK) {
    std::cerr << "Error when opening database: " << sqlite3_errmsg(pDatabase)
              << std::endl;
    sqlite3_close(pDatabase);
    pDatabase = nullptr;
  }
  return rc;
}

int DUMPER::CloseDatabase() {
  if (pDatabase) {
    int rc = sqlite3_close(pDatabase);
    if (rc != SQLITE_OK) {
      std::cerr << "Error when closing database: " << sqlite3_errmsg(pDatabase)
                << std::endl;
      pDatabase = nullptr;
    }
    return rc;
  } else
    return 0;
}
