//
// Created by Александр Широков on 31.07.2024.
//
#include "SearchEngine.h"
#include "ConverterJSON.h"
#include <iostream>
#include <sstream>
#include <cmath>

std::vector<std::vector<RelativeIndex>> SearchServer::search(const std::vector<std::string> &queries_input) {
    std::vector<std::vector<RelativeIndex>> ri_result;
    std::vector<std::vector<std::string>> request_words_vector;
    std::vector<std::vector<std::string>> sorted_request_words_vector;


    //1. Разбивает поисковый запрос на отдельные слова.
    for (auto &req : queries_input) {
        std::vector<std::string> words_vector;
        std::string request_word;
        std::stringstream str(req);
        while (str >> request_word) {
            words_vector.emplace_back(request_word);
        }
        //2. Формирует из них список уникальных.
        std::sort(words_vector.begin(),words_vector.end());
        auto it = std::unique(words_vector.begin(),words_vector.end());
        words_vector.resize(std::distance(words_vector.begin(), it));
        request_words_vector.emplace_back(words_vector);
    }

    /*3. Сортирует слова в порядке увеличения частоты встречаемости: от самых
         редких до самых частых. По возрастанию значения поля count поля
         freq_dictionary
         */

    for(const auto &uniq_req : request_words_vector) {
        std::vector<std::string> sorted_words_vector;
        std::multimap<int, std::string> sort_frequency_dictionary;
        for (const auto &uniq_word : uniq_req) {
        int count = 0;
        for (const auto &dict: _index.get_freq_dictionary()) {
            if (dict.first == uniq_word) {
                for (const auto &frequency: dict.second) {
                    count += frequency.count;
                }
                break;
            }
        }
            std::pair<int, std::string> match_dict(count, uniq_word);
            sort_frequency_dictionary.insert(match_dict);
        }
        for (const auto &word : sort_frequency_dictionary)
        {
            sorted_words_vector.emplace_back(word.second);
        }

        sorted_request_words_vector.emplace_back(sorted_words_vector);
    }

   //count relevance
    auto freq_dict_copy = _index.get_freq_dictionary();
    auto docs_count = _index.get_count_docs();

    int max_abs_frequency=0;

    for (const auto &words_request : sorted_request_words_vector) { //берем регвесты

        std::vector<RelativeIndex> rel_index;
        std::multimap<float,size_t, std::greater<int>> sorted_response_desc;

        for (auto _doc_id = 0; _doc_id < docs_count; _doc_id++) { //считаем по документам
            int abs_relevance = 0; //абсолютная релевантность
            for (const auto &request_word: words_request) { //Работаем с каждым словом

                for (const auto &fdc : freq_dict_copy[request_word]){
                    if (fdc.doc_id==_doc_id && freq_dict_copy.contains(request_word)) {
                        abs_relevance+=fdc.count;
                    } else {
                    }
                }
            }
            if(abs_relevance>0) {
                std::pair<size_t,float> cur_relevance(abs_relevance,_doc_id);
                sorted_response_desc.insert(cur_relevance);
                if (abs_relevance > max_abs_frequency) max_abs_frequency = abs_relevance;
            }
        }
        //просто печатаем
        auto counter_response_limit=0;
        for (const auto &data : sorted_response_desc) {
            ++counter_response_limit;
            if(counter_response_limit>responses_limit) break;
            RelativeIndex rl(data.second, data.first/max_abs_frequency);
            rel_index.emplace_back(rl);

        }
        ri_result.emplace_back(rel_index);
    }
    return ri_result;
}
