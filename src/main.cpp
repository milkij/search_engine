#include "ConverterJSON.h"
#include "InvertedIndex.h"
#include <sstream>
#include "Timer.h"
#include "SearchEngine.h"






int main(int argc, char* argv[]) {
    Timer my_timer;
    setlocale(LC_ALL, "ru_RU.UTF-8");

    ConverterJSON converterJson =  ConverterJSON();
    InvertedIndex invertedIndex = InvertedIndex();
    invertedIndex.UpdateDocumentBase(converterJson.GetTextDocuments());
    SearchServer searchServer(invertedIndex, converterJson.GetResponsesLimit());
    std::vector<std::vector<std::pair<int, float>>> answers;
    auto RI = searchServer.search(converterJson.GetRequests());
    converterJson.putAnswers(RI);


    return 0;
}
