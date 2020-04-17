#include "HistoryDumper.h"
#include "DECRYPTOR.h"
#include "MyUlti.h"
#include <algorithm>
#include <iostream>

int __stdcall HISTORY_DUMPER::AddURL(void *p, int nCol, char **ColValue,
                                     char **ColName) {
  std::vector<URL_ENTRY> *Container =
      reinterpret_cast<std::vector<URL_ENTRY> *>(p);

  URL_ENTRY T;
  if (ColValue) {
    T.ID = atoll(ColValue[0]);
    T.Content = FromCharArray(ColValue[1], atoi(ColValue[2]));
    Container->push_back(T);
  } else
    std::cerr << "Error when processing database: Empty row!" << std::endl;

  return 0;
}

int __stdcall HISTORY_DUMPER::AddEntry(void *p, int nCol, char **ColValue,
                                       char **ColName) {
  std::vector<HISTORY_ENTRY> *Container =
      reinterpret_cast<std::vector<HISTORY_ENTRY> *>(p);

  HISTORY_ENTRY T;
  if (ColValue) {
    T.AccessTime = ColValue[0];
    T.URL_Index = atoll(ColValue[1]);
    Container->push_back(T);
  } else
    std::cerr << "Error when processing database: Empty row!" << std::endl;

  return 0;
}

int HISTORY_DUMPER::URL_Search(const __int64 &ID) {
  size_t L = 0, R = URL.size() - 1, M;

  while (L <= R) {
    M = (L + R) >> 1;
    if (URL[M].ID == ID)
      return M;
    if (URL[M].ID < ID)
      L = M + 1;
    else
      R = M - 1;
  }

  return -1;
}

int HISTORY_DUMPER::Dump() {
  URL.clear();
  char *ErrMsg = nullptr;
  const char Command1[] = "SELECT id,url,length(url) FROM urls";

  int rc = sqlite3_exec(pDatabase, Command1, &AddURL,
                        reinterpret_cast<void *>(&URL), &ErrMsg);
  if (rc || ErrMsg) {
    std::cerr << "Error with when reading database: " << ErrMsg << std::endl;
    sqlite3_free(ErrMsg);
    return rc;
  }
  sqlite3_free(ErrMsg);

  std::sort(URL.begin(), URL.end(),
            [&](const URL_ENTRY &X, const URL_ENTRY &Y) -> bool {
              return X.ID < Y.ID;
            });

  History.clear();
  ErrMsg = nullptr;
  const char Command2[] = "SELECT datetime(visit_time/1000000-11644473600, "
                          "'unixepoch'),url FROM visits";

  rc = sqlite3_exec(pDatabase, Command2, &AddEntry,
                    reinterpret_cast<void *>(&History), &ErrMsg);
  if (rc || ErrMsg) {
    std::cerr << "Error with when reading database: " << ErrMsg << std::endl;
    sqlite3_free(ErrMsg);
    return rc;
  }
  sqlite3_free(ErrMsg);

  return 0;
}

int HISTORY_DUMPER::Show() {
  std::cout << "HISTORY" << std::endl;
  if (History.empty()) {
    std::cout << "No history!" << std::endl;
    return 0;
  }

  for (const HISTORY_ENTRY &T : History) {
    std::cout << std::string(60, '-') << std::endl;
    std::cout << "Access Time (UTC): " << T.AccessTime << std::endl;
    int idx = URL_Search(T.URL_Index);
    if (idx < 0)
      std::cerr << "Can't found URL with ID = " << T.URL_Index << std::endl;
    else
      std::cout << "URL: " << URL[idx].Content << std::endl;
  }
  std::cout << std::string(60, '-') << std::endl;

  return 0;
}
