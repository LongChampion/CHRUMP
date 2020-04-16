#pragma once
#ifndef DECRYPTOR_H
#define DECRYPTOR_H

#include <string>

// Class that can decrypt two types of password saved in Chrome
class DECRYPTOR {
private:
  std::string MASTER_KEY; // Primary key use to decrypt

  // Call CryptUnprotectData() to decrypt data
  std::string Unprotect(const std::string &);

public:
  DECRYPTOR() {}
  ~DECRYPTOR(){};

  // Get Master Key for further decryption
  int GetMasterKey();

  // Try to decrypt a string and return the result,
  // return an empty string if fail
  std::string Decrypt(const std::string &);
};
#endif // !DECRYPTOR_H
