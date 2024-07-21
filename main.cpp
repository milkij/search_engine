#include <iostream>
#include "ConverterJSON.h"
#include "InvertedIndex.h"
#include <sstream>






int main(int argc, char* argv[]) {
    setlocale(LC_ALL, "ru_RU.UTF-8");

    ConverterJSON converterJson =  ConverterJSON();
    InvertedIndex invertedIndex = InvertedIndex();

    invertedIndex.UpdateDocumentBase(converterJson.GetTextDocuments());

    return 0;
}
