#include <iostream>
#include "ConverterJSON.h"

int main() {
    setlocale(LC_ALL, "ru_RU.UTF-8");
    ConverterJSON* converterJson = new ConverterJSON();
    //converterJson->GetTextDocuments();
    std::vector<std::string> v = converterJson->GetRequests();
    //std::cout << v[0].size();

/*    auto j2 = R"(
  {
    "requests":
    [["word1", "word2", "word3", "word4"]]

  }
)"_json;
    std::cout << j2["requests"][0][0] ;*/
}
