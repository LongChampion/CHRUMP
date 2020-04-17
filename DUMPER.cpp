#include "DUMPER.h"
#include "MyUlti.h"
#include <Windows.h>
#include <iostream>
#include <string>

int DUMPER::OpenDatabase(const std::string &Filename) {
  CloseDatabase();
  BackupDatabase(Filename);
  int rc = sqlite3_open((Filename + ".tmp").c_str(), &pDatabase);
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

int DUMPER::BackupDatabase(const std::string &S) {
  size_t N = S.length();
  LPTSTR OldName = new TCHAR[N + 1];
  LPTSTR NewName = new TCHAR[N + 5];
  for (size_t i = 0; i < N; ++i)
    OldName[i] = NewName[i] = S[i];
  NewName[N] = '.';
  NewName[N + 1] = 't';
  NewName[N + 2] = 'm';
  NewName[N + 3] = 'p';
  OldName[N] = NewName[N + 4] = 0;
  int ErrorCode = !CopyFile(OldName, NewName, false);
  if (ErrorCode)
    std::cerr << "Can't backup database: " << LastErrorMessage() << std::endl;
  delete[] OldName, delete[] NewName;
  return ErrorCode;
}
