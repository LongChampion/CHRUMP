#include <Windows.h>
#include <iostream>
#include <string>

static const std::string B64TABLE =
    "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/";
static std::string MASTER_KEY;

std::string B64Decode(const std::string &S) {
  std::string R;
  int T = 0, counter = 0;

  for (const char &ch : S) {
    if (ch == '=')
      break;

    size_t k = B64TABLE.find(ch);
    if (k == std::string::npos) {
      std::cerr << "Error in base64 format!" << std::endl;
      return "";
    }

    for (int i = 5; i >= 0; --i)
      T = (T << 1) | ((k >> i) & 1);
    counter += 6;

    if (counter >= 8) {
      R.push_back(char(T >> (counter - 8)));
      counter -= 8;
      T &= ((1 << counter) - 1);
    }
  }

  return R;
}

std::string GetUserHomepath() {
  std::string HOMEPATH;
  char *buffer = nullptr;
  size_t counter;

  if (_dupenv_s(&buffer, &counter, "HOMEDRIVE")) {
    std::cerr << "$HOMEDRIVE environment variable not found!" << std::endl;
    free(buffer);
    return "";
  }
  HOMEPATH += std::string(buffer);
  free(buffer);

  buffer = nullptr;
  if (_dupenv_s(&buffer, &counter, "HOMEPATH")) {
    std::cerr << "$HOMEPATH environment variable not found!" << std::endl;
    free(buffer);
    return "";
  }
  HOMEPATH += std::string(buffer);
  free(buffer);

  return HOMEPATH;
}

std::string FromCharArray(const char *S, const int &n) {
  std::string R(n, ' ');
  for (int i = 0; i < n; ++i)
    R[i] = S[i];
  return R;
}
