#include <iostream>
#include "ConverterJSON.h"

int main() {
    setlocale(LC_ALL, "ru_RU.UTF-8");
    ConverterJSON* converterJson = new ConverterJSON();
    converterJson->GetTextDocuments();
    return 0;
}
