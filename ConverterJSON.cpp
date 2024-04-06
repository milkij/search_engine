//
// Created by Александр Широков on 30.03.2024.
//

#include "ConverterJSON.h"
#include <sstream>


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
    std::fstream requestFile(requestsJsonPath);
    json jsonRequestData = json::parse(requestFile);
    requestFile.close();
    //put data into tempVector from requests.json
    tempVector = jsonRequestData["requests"];
    return tempVector;
}

void ConverterJSON::putAnswers(std::vector<std::vector<std::pair<int, float>>> answers) {

}

bool ConverterJSON::countWordsInString(std::string *str) {

    std::stringstream  stream(*str);
    std::string        oneWord;
    int       count = 0;
    while(stream >> oneWord) {
        ++count;
        if(oneWord.length()>maxLenOfWord) return false;
        if(count>maxLenOfWord) return false;
    }
    return true;
}


