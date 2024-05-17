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
};



std::vector<std::string> ConverterJSON::GetTextDocuments() {
    std::vector<std::string> result;
    std::string dataFromFile;
    for (auto &i : filesPath) {
        std::ifstream tempFile(i, std::ios_base::in);
        if(!tempFile) {
            std::cerr << "[FAIL] "<< i <<" file is missing" << std::endl;
        }
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
            //std::cout << dataFromFile << std::endl;
        }
        //claer string
        dataFromFile.clear();
    }

    return result;
}



int ConverterJSON::GetResponsesLimit() const {
    return responsesLimit;
}



std::vector<std::string> ConverterJSON::GetRequests() {
    std::vector<std::string> list;
    std::ifstream in_req;
    in_req.open(requestsJsonPath, std::ios_base::in);
    if (!in_req.is_open()) {
        std::cerr << "[FAIL] requests file is missing" << std::endl;
    }
    else {
        nlohmann::json requests;
        in_req >> requests;
        if(requests["requests"].size()>GetResponsesLimit()) {
            std::cerr << "[FAIL] There are too may requests in file.";
            return list;
        }
        for (auto request : requests["requests"].items()) {
            if(request.value().size()>maxWordsInRequest){
                std::cerr << "[FAIL] There are too may words in " << std::stoi(request.key())+1 << "-st request.";
            } else {
                list.push_back(request.value().dump());
            }
        }
    }
    in_req.close();
    return list;

}

/*TODO*/
void ConverterJSON::putAnswers(std::vector<std::vector<std::pair<int, float>>> &answers) {
    auto requestData = GetRequests();
    auto textDocumentData = GetTextDocuments();
    auto jAnswer = json::parse(R"({"answer": []})");
    for (auto i=0; i<textDocumentData.size(); i++) {
        auto requestNumber = "request00"+std::to_string(i+1);
        jAnswer["answer"].push_back(requestNumber);
        //
        jAnswer["answer"][i] = json::parse(R"({"result": false, "relevance": []})");
        //todo find word in file
        auto isFind = true;
        //What do I to do?
        //
        jAnswer["answer"][i]["result"]=isFind;
    }
    std::cout << jAnswer.dump(2);
}

bool ConverterJSON::countWordsInString(std::string* str) const {
    std::stringstream stream(*str);
    std::string oneWord;
    int count = 0;
    while (stream >> oneWord) {
        ++count;
        if (oneWord.length() > maxLenOfWord) return false;
        if (count > maxWordsInFile) return false;
        //
    }
    return true;
}

bool ConverterJSON::checkRequestsLimit(json &data) const {
    auto respCount = data.size();
    if(GetResponsesLimit()<respCount) return false;
    else return true;
}


