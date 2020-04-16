#include "CookieDumper.h"
#include "Decryptor.h"
#include "MyUlti.h"
#include <iostream>

int __stdcall COOKIE_DUMPER::AddEntry(void *p, int nCol, char **ColValue,
                                      char **ColName) {
  std::vector<COOKIE_ENTRY> *Container =
      reinterpret_cast<std::vector<COOKIE_ENTRY> *>(p);

  COOKIE_ENTRY T;
  if (ColValue) {
    T.Host = ColValue[0];
    T.Name = ColValue[1];
    T.HasExpired = atoll(ColValue[2]);
    T.EncryptedValue = FromCharArray(ColValue[3], atoi(ColValue[4]));
    Container->push_back(T);
  } else
    std::cerr << "Error when processing database: Empty row!" << std::endl;

  return 0;
}

int COOKIE_DUMPER::Dump() {
  Cookie.clear();
  char *ErrMsg = nullptr;
  const char Command[] = "SELECT host_key, name, has_expires, encrypted_value, "
                         "length(encrypted_value) FROM cookies";

  int rc = sqlite3_exec(pDatabase, Command, &AddEntry,
                        reinterpret_cast<void *>(&Cookie), &ErrMsg);
  if (rc || ErrMsg) {
    std::cerr << "Error with when reading database: " << ErrMsg << std::endl;
    sqlite3_free(ErrMsg);
    return rc;
  }
  sqlite3_free(ErrMsg);

  return rc;
}

int COOKIE_DUMPER::Show() {
  std::cout << "COOKIE" << std::endl;

  DECRYPTOR Decryptor;
  int rc = Decryptor.GetMasterKey();
  if (rc)
    return rc;

  for (const COOKIE_ENTRY &T : Cookie) {
    std::cout << std::string(60, '-') << std::endl;
    std::cout << "Host: " << T.Host << std::endl;
    std::cout << "Name: " << T.Name << std::endl;
    std::cout << "Has Expired: " << T.HasExpired << std::endl;
    std::cout << "Encrypted value: " << T.EncryptedValue << std::endl;
    std::cout << "Try to decrypt: " << Decryptor.Decrypt(T.EncryptedValue)
              << std::endl;
  }
  std::cout << std::string(60, '-') << std::endl;
  return 0;
}
