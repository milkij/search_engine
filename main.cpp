#include <iostream>
#include "ConverterJSON.h"
#include "InvertedIndex.h"
#include <sstream>
#include <utility>
#include <thread>




struct RelativeIndex{
    size_t _doc_id;
    float _rank;
    RelativeIndex(size_t doc_id, float rank) : _doc_id(doc_id), _rank(rank){

    }

    bool operator == (const RelativeIndex& other) const {
        return (_doc_id == other._doc_id && _rank == other._rank);
    }
};


int main(int argc, char* argv[]) {
    setlocale(LC_ALL, "ru_RU.UTF-8");
    std::cout <<"Main Id: "<< std::this_thread::get_id() << std::endl;




    ConverterJSON converterJson =  ConverterJSON();
    InvertedIndex invertedIndex = InvertedIndex();

    invertedIndex.UpdateDocumentBase(converterJson.GetTextDocuments());



    return 0;
}
