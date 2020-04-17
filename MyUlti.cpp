#include "MyUlti.h"
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
      return std::string();
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
    return std::string();
  }
  HOMEPATH += std::string(buffer);
  free(buffer);

  buffer = nullptr;
  if (_dupenv_s(&buffer, &counter, "HOMEPATH")) {
    std::cerr << "$HOMEPATH environment variable not found!" << std::endl;
    free(buffer);
    return std::string();
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

int HexDump(const std::string &S) {
  size_t N;
  std::cout.fill('0');

  for (size_t pos = 0; pos < S.length(); pos += 16) {
    std::cout << "  ";
    std::cout.width(8);
    std::cout << std::hex << pos << ": ";

    N = std::_Min_value(S.length(), pos + 16);
    for (size_t i = pos; i < N; ++i) {
      std::cout.width(2);
      std::cout << std::hex << (int(S[i]) & 0xff) << ' ';
    }
    std::cout << std::string(3 * (pos + 16 - N), ' ');

    std::cout << "| ";
    for (size_t i = pos; i < N; ++i)
      if ((32 <= S[i]) && (S[i] <= 126))
        std::cout << S[i];
      else
        std::cout << '.';
    std::cout << std::endl;
  }

  return 0;
}

std::string LastErrorMessage() {
  DWORD ErrorCode = GetLastError();
  if (ErrorCode == 0)
    return std::string();

  LPSTR Buffer = nullptr;
  size_t size = FormatMessageA(
      FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM |
          FORMAT_MESSAGE_IGNORE_INSERTS,
      NULL, ErrorCode, MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
      (LPSTR)&Buffer, 0, NULL);

  std::string Message(Buffer, size);
  LocalFree(Buffer);
  return Message;
}
