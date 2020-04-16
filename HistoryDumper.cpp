#include "HistoryDumper.h"
#include "DECRYPTOR.h"
#include "MyUlti.h"
#include <iostream>

int __stdcall HISTORY_DUMPER::AddURL(void *p, int nCol, char **ColValue,
                                     char **ColName) {
  std::vector<std::string> *Container =
      reinterpret_cast<std::vector<std::string> *>(p);

  if (ColValue)
    Container->push_back(FromCharArray(ColValue[0], atoi(ColValue[1])));
  else
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

int HISTORY_DUMPER::Dump() {
  URL.clear();
  URL.push_back("This URL is used for padding only!");
  char *ErrMsg = nullptr;
  const char Command1[] = "SELECT url,length(url) FROM urls";

  int rc = sqlite3_exec(pDatabase, Command1, &AddURL,
                        reinterpret_cast<void *>(&URL), &ErrMsg);
  if (rc || ErrMsg) {
    std::cerr << "Error with when reading database: " << ErrMsg << std::endl;
    sqlite3_free(ErrMsg);
    return rc;
  }
  sqlite3_free(ErrMsg);

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
    std::cout << "URL: " << URL[T.URL_Index] << std::endl;
  }
  std::cout << std::string(60, '-') << std::endl;

  return 0;
}
