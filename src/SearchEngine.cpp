//
// Created by Александр Широков on 31.07.2024.
//
#include "SearchEngine.h"
#include <iostream>
#include <sstream>
#include <cmath>

std::vector<std::vector<RelativeIndex>> SearchServer::search(const std::vector<std::string> &queries_input) {
    std::vector<std::vector<RelativeIndex>> ri_result;
    std::vector<std::vector<std::string>> request_words_vector;
    std::vector<std::vector<std::string>> sorted_request_words_vector;

    //_index.print_freq_dictionary();
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
            //std::cout << word.first << ' ' << word.second << std::endl;
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

        for (auto _doc_id = 0; _doc_id < docs_count; _doc_id++) { //считаем по документам
            int abs_relevance = 0; //абсолютная релевантность
            //std::cout << "doc_id:"<<_doc_id<<' ';
            for (const auto &request_word: words_request) { //Работаем с каждым словом

                for (const auto &fdc : freq_dict_copy[request_word]){
                    if (fdc.doc_id==_doc_id && freq_dict_copy.contains(request_word)) {
                        //std::cout << request_word << '{' << fdc.doc_id << ',' << fdc.count << "} ";
                        abs_relevance+=fdc.count;
                    } else {
                        //std::cout << request_word << '{' << fdc.doc_id << ',' << 0 << "} ";
                    }
                }
            }
            if(abs_relevance>0) {
                RelativeIndex rl(_doc_id, abs_relevance);
                rel_index.push_back(rl);
                //std::cout << abs_relevance;
                //std::cout << std::endl;
                if (abs_relevance > max_abs_frequency) max_abs_frequency = abs_relevance;
            }
        }
        //std::cout << "max_abs_frequency " << max_abs_frequency << std::endl;
        for (auto &rl : rel_index) { //получим Относительная релевантность

           rl.rank =  rl.rank/max_abs_frequency;
        }
        ri_result.emplace_back(rel_index);
    }
    return ri_result;
}
