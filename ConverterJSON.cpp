//
// Created by Александр Широков on 30.03.2024.
//

#include "ConverterJSON.h"
#include <sstream>
#include <algorithm>


ConverterJSON::ConverterJSON() {
    json configJsonData;
    std::fstream configFile(configJsonPath);
    if(!configFile) {
        std::cerr << "config file is missing";
        return;
    }
    configJsonData = json::parse(configFile);
    configFile.close();
    if(!configJsonData.contains("config")) {
        std::cerr << "config file is empty";
        return;
    }
    appName=configJsonData["config"]["name"].dump();
    appVersion=configJsonData["config"]["version"].dump();
    responsesLimit=(int)configJsonData["config"]["max_responses"];
    if(responsesLimit<=0) responsesLimit=5;
    filesPath = configJsonData["files"];


    std::cout << appName << " ver.- " << appVersion << "\n responsesLimit: "
                << responsesLimit << std::endl;

    /*for(auto &element : configJsonData)
    {
        for (auto &element2 : element.items()) {
            if(element2.key()=="max_responses")
            std::cout << element2.value() << std::endl;
        }
    }*/

};



std::vector<std::string> ConverterJSON::GetTextDocuments() {
    std::vector<std::string> result;
    std::string dataFromFile;
    for (auto &i : filesPath) {
        std::ifstream tempFile(i);
        if(!tempFile) std::cerr << "file " << i << "is not exists" << std::endl;
        //read file
        std::getline(tempFile,dataFromFile);
        //close file
        tempFile.close();
        //
        std::vector<std::string> tempArray;
        //checks < 1000 words
        //
        if(!countWordsInString(&dataFromFile)) {
            std::cerr << "incorect data in file " << i << std::endl;
        //add array
        } else {
            result.push_back(dataFromFile);
            std::cout << dataFromFile << std::endl;
        }
        //claer string
        dataFromFile.clear();
    }

    return result;
}



int ConverterJSON::GetResponsesLimit() {
    return responsesLimit;
}



std::vector<std::string> ConverterJSON::GetRequests() {
    std::vector<std::string> tempVector;
    std::string buffer;
    //считаем данные из файла request.json
    std::fstream requestFile(requestsJsonPath);
    auto jsonRequestData = json::parse(requestFile);
    requestFile.close();
    //проверим, что кол-во запросов допустимо
    if(!checkRequestsLimit(jsonRequestData["requests"]) && jsonRequestData["requests"].size()<1000)
    {
        std::cerr << "Responses limit is to mutch. \n"
                     "Max is " << GetResponsesLimit();
        return tempVector;
        //если проверки пройдены запишем каждый массив строк в tempVector вектор
    } else
    {
        auto step=0;
        /*
         *
         * нужно сделать парс списков request
         * проверить длинну слова < 100 символов
         * одиин запрос менее 11 слов
         *
         */
        auto requests = jsonRequestData["requests"];
        for (auto i=0; i<requests.size(); ++i)
        {
//если кол-ва слов больше maxWordsInRequest пропускаем этот запрос
            if (requests[i].size()>this->maxWordsInRequest) {
                std::cerr << "request00" << (i+1) << " has more then " << this->maxWordsInRequest << " words";
            } else {

                 for (auto j=0; j<requests[i].size(); ++j)
                 {
                     std::string str = requests[i][j].dump();
                     str.erase(std::remove(str.begin(), str.end(), '"'), str.end());
                     //если длина слова больше maxLenWordInRequest пропускаем это слова, не записываем .
                     if(str.length()>this->maxLenWordInRequest) {
                         std::cerr << "Lenth of word " << str << " has more then " << this->maxLenWordInRequest
                         << " symbols";
                     } else {
                         buffer+=str+' ';
                     }
                 }
            }


            tempVector.emplace_back(buffer);
        }
        return tempVector;
    }
    //

}

/*TODO*/
void ConverterJSON::putAnswers(std::vector<std::vector<std::pair<int, float>>> answers) {

}

bool ConverterJSON::countWordsInString(std::string* str) {
    std::stringstream stream(*str);
    std::string oneWord;
    int count = 0;
    while (stream >> oneWord) {
        ++count;
        if (oneWord.length() > maxLenOfWord) return false;
        if (count > maxWordsInFile) return false;
        //
        return true;
    }
}

bool ConverterJSON::checkRequestsLimit(json &data) {
    auto respCount = data.size();
    if(GetResponsesLimit()<respCount) return false;
    else return true;
}


