#include "CookieDumper.h"
#include "HistoryDumper.h"
#include "LoginDumper.h"
#include "MyUlti.h"
#include <iostream>

int main() {
  try {
    std::string HOMEPATH = GetUserHomepath();

    // Dump History
    HISTORY_DUMPER HistoryDumper;
    std::string HISTORY_FILE =
        HOMEPATH +
        "\\AppData\\Local\\Google\\Chrome\\User Data\\Default\\History";
    HistoryDumper.OpenDatabase(HISTORY_FILE);
    HistoryDumper.Dump();
    HistoryDumper.Show();
    std::cout << std::endl;

    // Dump Cookie
    COOKIE_DUMPER CookieDumper;
    std::string COOKIE_FILE =
        HOMEPATH +
        "\\AppData\\Local\\Google\\Chrome\\User Data\\Default\\Cookies";
    CookieDumper.OpenDatabase(COOKIE_FILE);
    CookieDumper.Dump();
    CookieDumper.Show();
    std::cout << std::endl;

    // Dump LoginInfo
    LOGIN_DUMPER LoginDumper;
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
