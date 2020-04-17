#include "Decryptor.h"
#include "CryptoPP\\aes.h"
#include "CryptoPP\\cryptlib.h"
#include "CryptoPP\\filters.h"
#include "CryptoPP\\gcm.h"
#include "CryptoPP\\secblock.h"
#include "MyUlti.h"
#include <Windows.h>
#include <fstream>
#include <iostream>
#include <string>

int DECRYPTOR::GetMasterKey() {
  if (!MASTER_KEY.empty())
    return 0;

  std::fstream f;
  std::string S;
  std::string LOCAL_STAGE_FILE =
      GetUserHomepath() +
      "\\AppData\\Local\\Google\\Chrome\\User Data\\Local State";

  f.open(LOCAL_STAGE_FILE, std::ios::in);
  if (!f.is_open()) {
    std::cerr << "Can't open " << LOCAL_STAGE_FILE << std::endl;
    return -1;
  }
  getline(f, S);
  f.close();

  size_t left = S.find("encrypted_key");
  if (left == std::string::npos) {
    std::cerr << "Master key not found!" << std::endl;
    return -2;
  }
  left += 16;

  size_t right = S.find("\"", left + 1);
  if (right == std::string::npos) {
    std::cerr << "Invalid Master Key format!" << std::endl;
    return -3;
  }

  S = S.substr(left, right - left);
  S = B64Decode(S);
  if (S.empty() || (S.length() < 6)) {
    std::cerr << "Master Key corrupted: Invalid base64 format!" << std::endl;
    return -4;
  }

  S = Unprotect(S.substr(5));
  if (S.length() != 32) {
    std::cerr << "Master Key corrupted: Invalid length!" << std::endl;
    return -5;
  }

  MASTER_KEY = S;
  return 0;
}

std::string DECRYPTOR::Unprotect(const std::string &S) {
  BYTE *Cipher = new BYTE[S.length() + 1];
  for (size_t i = 0; i < S.length(); ++i)
    Cipher[i] = S[i];
  Cipher[S.length()] = 0;

  DATA_BLOB Input, Output;
  Input.pbData = Cipher;
  Input.cbData = S.length() + 1;
  if (!CryptUnprotectData(&Input, NULL, NULL, NULL, NULL, 0, &Output)) {
    std::cerr << "CryptUnprotectData failed with code 0x" << std::hex
              << GetLastError() << std::endl;
    delete[] Cipher;
    LocalFree(Output.pbData);
    return std::string();
  }
  delete[] Cipher;

  std::string Key;
  for (size_t i = 0; i < Output.cbData; ++i)
    Key.push_back(Output.pbData[i]);
  LocalFree(Output.pbData);

  return Key;
}

std::string DECRYPTOR::Decrypt(const std::string &PW) {
  if (PW.length() < 16) {
    std::cerr << "Invalid encrypted password!" << std::endl;
    return std::string();
  }

  CryptoPP::SecByteBlock key(MASTER_KEY.size());
  for (size_t i = 0; i < key.size(); ++i)
    key[i] = MASTER_KEY[i];

  CryptoPP::SecByteBlock iv(12);
  for (int i = 0; i < 12; ++i)
    iv[i] = PW[i + 3];

  CryptoPP::GCM<CryptoPP::AES>::Decryption Decryptor;
  Decryptor.SetKeyWithIV(key, key.size(), iv, iv.size());

  std::string Password = PW.substr(15), PlainPassword;
  CryptoPP::StringSource Source(
      Password, true,
      new CryptoPP::AuthenticatedDecryptionFilter(
          Decryptor, new CryptoPP::StringSink(PlainPassword)));

  return PlainPassword;
}
