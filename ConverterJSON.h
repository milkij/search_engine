//
// Created by Александр Широков on 30.03.2024.
//

#ifndef SEARCH_ENGINE_CONVERTERJSON_H
#define SEARCH_ENGINE_CONVERTERJSON_H

#pragma once
#include <iostream>
#include <nlohmann/json.hpp>
#include <fstream>
#include <vector>
#include <string>

//using json = nlohmann::json;

class ConverterJSON {
public:
    ConverterJSON();
/**
* Метод получения содержимого файлов
* @return Возвращает список с содержимым файлов перечисленных * в config.json
*/
    std::vector<std::string> GetTextDocuments();

/**
* Метод считывает поле max_responses для определения предельного * количества ответов на один запрос
* @return
*/
    int GetResponsesLimit() const;
/**
* Метод получения запросов из файла requests.json
* @return возвращает список запросов из файла requests.json */
    std::vector<std::string> GetRequests();
/**
* Положить в файл answers.json результаты поисковых запросов */
    void putAnswers(std::vector<std::vector<std::pair<int, float>>> &answers);
    //
private:
    /*PATHS*/
    const std::string configJsonPath = "..//config.json";
    const std::string requestsJsonPath = "..//requests.json";
    const std::string answersJsonPath = "..//answers.json";

    /*CONFIG DATA*/
    std::string appName;
    std::string appVersion;
    std::vector<std::string> filesPathVector;
    int responsesLimit;


    /*RULS*/
    const int maxLenOfWordInFile = 100;
    const int maxWordsInFile = 1000;
    //
    const int maxWordsInRequest = 10;
    const int maxRequests = 100;
    //



    /*
     * Метод подсчета слов и длины слов в строке
     * @return возвращает true если each word <= maxLenOfWordInFile && str <= maxWordsInFile
     */
    bool countWordsInString(std::string* str) const;

    /*
     * Метод поиска слов в файле
     * @return возвращает true если слова из requests есть в файле
     */
    bool findWordInFile(std::string& str);
    //
    /*
    * Метод сравнения кол-ва запрсов из файла request.json с responsesLimit
    * @return возвращает true или false.
    */
    bool checkRequestsLimit(nlohmann::json &data) const;
};


#endif //SEARCH_ENGINE_CONVERTERJSON_H
