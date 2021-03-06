#include "LoginDumper.h"
#include "DECRYPTOR.h"
#include "MyUlti.h"
#include <iostream>

int __stdcall LOGIN_DUMPER::AddEntry(void *p, int nCol, char **ColValue,
                                     char **ColName) {
  std::vector<LOGIN_ENTRY> *Container =
      reinterpret_cast<std::vector<LOGIN_ENTRY> *>(p);

  LOGIN_ENTRY T;
  if (ColValue) {
    T.URL = FromCharArray(ColValue[0], atoi(ColValue[1]));
    T.Username = FromCharArray(ColValue[2], atoi(ColValue[3]));
    T.Password = FromCharArray(ColValue[4], atoi(ColValue[5]));
    Container->push_back(T);
  } else
    std::cerr << "Error when processing database: Empty row!" << std::endl;

  return 0;
}

int LOGIN_DUMPER::Dump() {
  LoginData.clear();
  char *ErrMsg = nullptr;
  const char Command[] =
      "SELECT "
      "origin_url,length(origin_url),username_value,length(username_value),"
      "password_value,length(password_value) FROM logins";

  int rc = sqlite3_exec(pDatabase, Command, &AddEntry,
                        reinterpret_cast<void *>(&LoginData), &ErrMsg);
  if (rc || ErrMsg) {
    std::cerr << "Error with when reading database: " << ErrMsg << std::endl;
    sqlite3_free(ErrMsg);
    return rc;
  }
  sqlite3_free(ErrMsg);

  return 0;
}

int LOGIN_DUMPER::Show() {
  std::cout << "LOGIN DATA" << std::endl;
  if (LoginData.empty()) {
    std::cout << "No login data!" << std::endl;
    return 0;
  }

  DECRYPTOR Decryptor;
  int rc = Decryptor.GetMasterKey();
  if (rc)
    return rc;

  std::string Decoded;
  for (const LOGIN_ENTRY &T : LoginData) {
    std::cout << std::string(60, '-') << std::endl;
    std::cout << "URL: " << T.URL << std::endl;
    std::cout << "Username: " << T.Username << std::endl;
    std::cout << "Encrypted password: " << std::endl;
    HexDump(T.Password);
    if (T.Password.empty())
      Decoded = "[!] You didn't save this password!";
    else {
      try {
        Decoded = Decryptor.Decrypt(T.Password);
      } catch (...) {
        Decoded = "Can't decrypt this password!";
      }
    }
    std::cout << "Try to decrypt: " << Decoded << std::endl;
  }
  std::cout << std::string(60, '-') << std::endl;

  return 0;
}
