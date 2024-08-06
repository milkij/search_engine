//
// Created by Александр Широков on 07.07.2024.
//

#ifndef SEARCH_ENGINE_INVERTEDINDEX_H
#define SEARCH_ENGINE_INVERTEDINDEX_H
#include <vector>
#include <map>

struct Entry {
    size_t doc_id, count;
    Entry(size_t _doc_id, size_t _count);

// Данный оператор необходим для проведения тестовых сценариев
bool operator ==(const Entry& other) const {
    return (doc_id == other.doc_id && count == other.count);
} };

class InvertedIndex {
public:
    InvertedIndex() = default;

    /**
    * Обновить или заполнить базу документов, по которой будем совершать поиск
    * @param texts_input содержимое документов
     * */
    void UpdateDocumentBase(std::vector<std::string> input_docs);

    /**
    * Метод определяет количество вхождений слова word в загруженной базе документов
    * @param word слово, частоту вхождений которого необходимо определить
    * @return возвращает подготовленный список с частотой слов
    */
    std::vector<Entry> GetWordCount(const std::string& word);


private:
    std::vector<std::string> docs;
    std::map<std::string, std::vector<Entry>> freq_dictionary;

public:
    void print_freq_dictionary();
    std::map<std::string, std::vector<Entry>> get_freq_dictionary();
    int get_count_docs();
};


#endif //SEARCH_ENGINE_INVERTEDINDEX_H
