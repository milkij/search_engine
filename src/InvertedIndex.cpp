//
// Created by Александр Широков on 07.07.2024.
//

#include "../include/InvertedIndex.h"
#include <sstream>
#include <algorithm>
#include <iostream>
#include <vector>
#include <thread>
#include <mutex>


Entry::Entry(size_t _doc_id, size_t _count) : doc_id(_doc_id), count(_count)
{
    //std::cout << "Struct Entry Constructor \n doc_id = " << _doc_id << "; count = " << _count << std::endl;
}

void InvertedIndex::UpdateDocumentBase(std::vector<std::string> input_docs) {
    std::vector<std::string> temp_string_vector;
    //threads
    std::vector<std::thread> threadVector;
    //mutex
    std::mutex mutex_vector_collection;
    //
    //for (const auto &in_doc : input_docs) docs.emplace_back(in_doc);
    //
    auto write_words_into_collect = [&mutex_vector_collection, &temp_string_vector](const std::string &data){
        std::string one_word;
        std::stringstream str(data);
        while (str >> one_word) {
            mutex_vector_collection.lock();
            temp_string_vector.push_back(one_word);
            mutex_vector_collection.unlock();
        }
    };
    //
    for (const auto &_doc : input_docs) {
        docs.emplace_back(_doc);
        //
        threadVector.emplace_back(std::thread(write_words_into_collect,_doc));
    }
    //will join threads
    for (auto i=0; i<threadVector.size();++i) {
        threadVector[i].join();
    }


    //sort temp_string_vector
    std::sort(temp_string_vector.begin(), temp_string_vector.end());
    //do unique temp_string_vector
    auto it = std::unique(temp_string_vector.begin(),temp_string_vector.end());
    temp_string_vector.resize(std::distance(temp_string_vector.begin(), it));

    //Clear collection before UpdateDocumentBase
    freq_dictionary.clear();
    //start countWords by sorted vector
    for (const auto &word : temp_string_vector) {
        freq_dictionary[word]= this->GetWordCount(word);
    }
}

std::vector<Entry> InvertedIndex::GetWordCount(const std::string &word) {
    std::vector<Entry> temp_container;
    int document_id=0;
    for (const auto doc : docs)
    {
        int word_counter=0;
        std::string one_word;
        std::stringstream str(doc);
        while (str >> one_word) if(one_word == word) ++word_counter;
        if(word_counter>0) {
            //std::cout << word << '-';
            Entry entry(document_id,word_counter);
            temp_container.emplace_back(entry);
        }
        ++document_id;
    }

    return temp_container;
}

void InvertedIndex::print_freq_dictionary() {
    for (const auto &i : freq_dictionary) {
        std::cout << i.first << ' ';
        for (const auto &j : i.second) {
            std::cout << '{' << j.doc_id << ',' << j.count << '}' << ' ';
        }
        std::cout << std::endl;
    }
}
