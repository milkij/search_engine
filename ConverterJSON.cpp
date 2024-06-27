//
// Created by Александр Широков on 30.03.2024.
//

#include "ConverterJSON.h"
#include <sstream>
#include <algorithm>

//CONSTRUCTOR
//#done
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


//#done
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
//#fix
            if(counter_of_words>ConverterJSON::maxWordsInFile) has_too_much_words = true;
            if(one_word.length()>ConverterJSON::maxLenOfWordInFile) has_length_too_long = true;
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


//#done
int ConverterJSON::GetResponsesLimit() const {
    return responsesLimit;
}



std::vector<std::string> ConverterJSON::GetRequests() {
    std::vector<std::string> list;
    std::ifstream f_request_json;
    f_request_json.open(ConverterJSON::requestsJsonPath, std::ios_base::in);
    if (!f_request_json.is_open()) {
        std::cerr << "[FAIL] requests file is missing" << std::endl;
    }
    else {
        nlohmann::ordered_json requests_buffer_json;
        requests_buffer_json = nlohmann::json::parse(f_request_json);
        //
        if(requests_buffer_json["requests"].size() > ConverterJSON::maxRequests) {
            std::cerr << "[FAIL] There are too may requests_buffer_json in file.";
            return list;
        }
        for (auto request : requests_buffer_json["requests"].items()) {
            //
            auto j_string = request.value().template get<std::string>();
            //
            if(request.value().size() > ConverterJSON::maxWordsInRequest){
                std::cerr << "[FAIL] There are too may words in " << std::stoi(request.key())+1 << "-st request.";
            } else {
                //do check small letters in string
                auto is_valid_string = true;
                auto start_pos_ascii = 97;
                auto end_pos_ascii = 122;
                auto space_ascii = 32;
                //using var j_string
                for(auto i = 0; i<j_string.length(); ++i)
                {
                    if(((int)j_string[i] >= start_pos_ascii and (int)j_string[i]<=end_pos_ascii)
                        || (int)j_string[i]==space_ascii) {
                        continue;
                    } else {
                        is_valid_string = false;
                        std::cerr << "[FAIL] \n"
                                     "litteral should be in latinael lowercase constant \n"
                                     << std::stoi(request.key())+1 << "-st request: wrong symbol is "
                                     << j_string[i]
                                     <<"\n";

                        break;
                    }
                }
                if(is_valid_string) list.push_back(j_string);
            }
        }
    }
    f_request_json.close();
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
        if (oneWord.length() > maxLenOfWordInFile) return false;
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


