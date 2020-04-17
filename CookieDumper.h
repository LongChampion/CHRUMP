#pragma once
#ifndef COOKIE_DUMPER_H
#define COOKIE_DUMPER_H

#include "DUMPER.h"
#include <string>
#include <vector>

// Structure to save a cookie
struct COOKIE_ENTRY {
  std::string Host;
  std::string Name;
  __int64 HasExpired;
  std::string EncryptedValue;
};

// Class to dump cookies
class COOKIE_DUMPER : public DUMPER {
private:
  std::vector<COOKIE_ENTRY> Cookie;
  static int __stdcall AddEntry(void *, int, char **, char **);

public:
  COOKIE_DUMPER(){};
  ~COOKIE_DUMPER(){};

  int Dump() override;
  int Show() override;
};
#endif // !COOKIE_DUMPER_H
