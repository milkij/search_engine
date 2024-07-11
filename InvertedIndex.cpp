//
// Created by Александр Широков on 07.07.2024.
//

#include "InvertedIndex.h"
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
    for (const auto &_doc : input_docs) this->docs.emplace_back(_doc);
    //
    std::vector<std::string> temp_string_vector;
    for (auto doc=0; doc<this->docs.size(); ++doc) {
        std::string one_word;
        std::stringstream str(this->docs[doc]);
        std::mutex add_file_access;
        std::cout <<"Id: "<< std::this_thread::get_id() << std::endl;
        auto fx=[&str, &one_word, &temp_string_vector, &add_file_access] () {
            std::cout <<"Id: "<< std::this_thread::get_id() << std::endl;
            add_file_access.lock();
            while (str >> one_word) temp_string_vector.push_back(one_word);
            add_file_access.unlock();
        };
        std::thread new_file(fx);
        new_file.join();

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

    for (const auto &i : freq_dictionary) {
        std::cout << i.first << ' ';
        for (const auto &j : i.second) {
            std::cout << '{' << j.doc_id << ',' << j.count << '}' << ' ';
        }
        std::cout << std::endl;
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
