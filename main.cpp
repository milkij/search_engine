#include <iostream>
#include "ConverterJSON.h"

int main(int argc, char* argv[]) {
    setlocale(LC_ALL, "ru_RU.UTF-8");
    ConverterJSON converterJson =  ConverterJSON();
    converterJson.GetTextDocuments();
    for (const auto it : converterJson.GetRequests())
    {
        std::cout << it << std::endl;
    };

}
