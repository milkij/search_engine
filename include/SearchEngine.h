//
// Created by Александр Широков on 31.07.2024.
//

#ifndef SEARCH_ENGINE_SEARCHENGINE_H
#define SEARCH_ENGINE_SEARCHENGINE_H
#pragma once
#include "InvertedIndex.h"

struct RelativeIndex{
    size_t doc_id;
    float rank=0.0;
    RelativeIndex(size_t _doc_id, float _rank)
    {
        doc_id=_doc_id;
        rank=_rank;
    };
    bool operator ==(const RelativeIndex& other) const {
        return (doc_id == other.doc_id && rank == other.rank);
    }
};
class SearchServer {
public:
/**
* @param idx в конструктор класса передаётся ссылка на класс
InvertedIndex,
* чтобы SearchServer мог узнать частоту слов встречаемых в
запросе
*/
    SearchServer(InvertedIndex& idx, int _responsesLimit) : _index(idx),responses_limit(_responsesLimit){ };

/**
* Метод обработки поисковых запросов
* @param queries_input поисковые запросы взятые из файла
requests.json
* @return возвращает отсортированный список релевантных ответов для
заданных запросов
*/
    std::vector<std::vector<RelativeIndex>> search(const
                                                   std::vector<std::string>& queries_input);

private:
    InvertedIndex _index;
    int responses_limit;
};

#endif //SEARCH_ENGINE_SEARCHENGINE_H
