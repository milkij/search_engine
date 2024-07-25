#include <iostream>
#include "ConverterJSON.h"
#include "InvertedIndex.h"
#include <sstream>
#include "Timer.h"






int main(int argc, char* argv[]) {
    Timer my_timer;
    setlocale(LC_ALL, "ru_RU.UTF-8");

    ConverterJSON converterJson =  ConverterJSON();
    InvertedIndex invertedIndex = InvertedIndex();
    //for (const auto &i : converterJson.GetTextDocuments()) std::cout << i << std::endl;
    invertedIndex.UpdateDocumentBase(converterJson.GetTextDocuments());


    return 0;
}
