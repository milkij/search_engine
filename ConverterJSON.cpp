//
// Created by Александр Широков on 30.03.2024.
//

#include "ConverterJSON.h"
#include <sstream>
#include <typeinfo>


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
        for (const auto &i : jsonRequestData["requests"])
        {
            std::string tempString = i.dump();
            tempVector.emplace_back(i.dump());
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


