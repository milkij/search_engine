#include <iostream>
#include "ConverterJSON.h"

int main() {

    setlocale(LC_ALL, "ru_RU.UTF-8");
    ConverterJSON* converterJson = new ConverterJSON();
    //std::vector<std::string> a = converterJson->GetTextDocuments();
    //std::vector<std::string> v = converterJson->GetRequests();
    /*for (auto const& i : v){
        std::cout << i << std::endl;
    }*/

    std::vector<std::vector<std::pair<int, float>>> a;
    converterJson->putAnswers(a);


    /*auto answer = json::parse(R"({"answer": []})");
    auto requestNumber = 1000;
    std::string rr = "{\"request"+std::to_string(requestNumber)+"\": []}";
    std::cout << rr << std::endl;
    auto r = "{\"request\": []}";
    auto request = json::parse(rr);
    //auto request = json::parse(R"({"request": []})");
    nlohmann::ordered_json relevance = json::parse(R"({"result": false, "relevance": []})");
    auto docId = json::parse(R"({"docid": 0,"rank": 0.141})");
    relevance["result"]=false;
    relevance["relevance"].emplace_back(docId);
    request["request001"].emplace_back(relevance);
    answer["answer"].emplace_back(request);
    std::cout << answer.dump(2);*/


 /*   auto jAnswer = json::parse(R"({"answer": []})");
    for (auto i=0; i<4; i++) {
        auto requestNumber = "request00"+std::to_string(i+1);
        jAnswer["answer"].push_back(requestNumber);
        //
        jAnswer["answer"][i] = json::parse(R"({"result": false, "relevance": []})");
        //todo find word in file
        bool isFind = true;
        //
        jAnswer["answer"][i]["result"]=isFind;
    }
    std::cout << jAnswer.dump(2);*/



}
