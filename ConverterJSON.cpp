//
// Created by Александр Широков on 30.03.2024.
//

#include "ConverterJSON.h"
#include <sstream>
#include <algorithm>

//CONSTRUCTOR
ConverterJSON::ConverterJSON() {
    nlohmann::json configJsonData;
    std::fstream configFile(configJsonPath);
    //is there config file?
    if(!configFile) {
        std::cerr << "config file is missing";
        return;
    }
    configJsonData = nlohmann::json::parse(configFile);
    configFile.close();
    //Does file contain the #attr=config
    if(!configJsonData.contains("config")) {
        std::cerr << "config file is empty";
        return;
    }
    /*READ & WRITE SETTINGS*/
    //read name of search_engine
    appName=configJsonData["config"]["name"].dump();
    //read version of search_engine
    appVersion=configJsonData["config"]["version"].dump();
    //read max_responses for search_engine
    responsesLimit=(int)configJsonData["config"]["max_responses"];
    //set default val
    if(responsesLimit<=0) responsesLimit=5;
    //vector
    filesPathVector = configJsonData["files"];

    //STREAM
    std::cout << appName << " ver.- " << appVersion << "\n responsesLimit: "
                << responsesLimit << std::endl;
};



std::vector<std::string> ConverterJSON::GetTextDocuments() {
    //
    std::vector<std::string> result;
    std::string current_data_from_file;
    bool has_too_much_words = false;
    bool has_length_too_long = false;
    //
    for (const auto &i : ConverterJSON::filesPathVector) {
        std::ifstream buffer(i, std::ios_base::in); //ios::in: файл открывается для ввода (чтения).
        if(!buffer) //проверим, что по указанному пути файл открылся на чтение, если его нет просто вывыедем сообщение.
        {
            std::cerr << "[FAIL] "<< i <<" file is missing" << std::endl;
        }
        //read file
        //buffer >> current_data_from_file;
        std::getline(buffer, current_data_from_file);
        //close file
        buffer.close();
        //
        //CHECK current_data_from_file
        //checks <= 1000 words
        //checks len of words <= 100
        std::string one_word;
        std::stringstream str(current_data_from_file);
        int counter_of_words = 0;

        while (str >> one_word) {
            ++counter_of_words;
            if(counter_of_words>1000) has_too_much_words = true;
            if(one_word.length()>100) has_length_too_long = true;
        }
        //
        if(has_too_much_words) {
            std::cerr << "File " << i << " has too much words." << std::endl;

        } else if(has_length_too_long){
            std::cerr << "The length in file " << i << "is too long." << std::endl;
        } else {
            //add array
            result.emplace_back(current_data_from_file);
        }
        //claer string
        current_data_from_file.clear();
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
    auto jAnswer = nlohmann::json::parse(R"({"answer": []})");
    for (auto i=0; i<textDocumentData.size(); i++) {
        auto requestNumber = "request00"+std::to_string(i+1);
        jAnswer["answer"].push_back(requestNumber);
        //
        jAnswer["answer"][i] = nlohmann::json::parse(R"({"result": false, "relevance": []})");
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

bool ConverterJSON::checkRequestsLimit(nlohmann::json &data) const {
    auto respCount = data.size();
    if(GetResponsesLimit()<respCount) return false;
    else return true;
}


