#include "LoginDumper.h"
#include "MyUlti.h"
#include <iostream>

int main() {
  try {
    LOGIN_DUMPER LoginDumper;
    std::string HOMEPATH = GetUserHomepath();
    std::string LOGIN_DATA_FILE =
        HOMEPATH +
        "\\AppData\\Local\\Google\\Chrome\\User Data\\Default\\Login Data";
    LoginDumper.OpenDatabase(LOGIN_DATA_FILE);
    LoginDumper.Dump();
    LoginDumper.Show();
  } catch (...) {
    std::cerr << "Somethings went wrong!" << std::endl;
  }

  getchar();
  return 0;
}
