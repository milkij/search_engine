#include <iostream>
#include "ConverterJSON.h"

int main() {
    setlocale(LC_ALL, "ru_RU.UTF-8");
    ConverterJSON* converterJson = new ConverterJSON();
    //converterJson->GetTextDocuments();
    std::vector<std::string> v = converterJson->GetRequests();
    for (auto const& i : v){
        std::cout << i << std::endl;
    }




}
