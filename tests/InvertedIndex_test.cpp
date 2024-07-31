#include "../include/InvertedIndex.h"
#include "gtest/gtest.h"

using namespace std;
void TestInvertedIndexFunctionality(
        const vector<string>& test_docs,
        const vector<string>& test_requests,
        const std::vector<vector<Entry>>& expected
) {
    std::vector<std::vector<Entry>> result;
    InvertedIndex idx;
    idx.UpdateDocumentBase(test_docs);
    for(auto& request : test_requests) {
        std::vector<Entry> word_count = idx.GetWordCount(request);
        result.push_back(word_count);
    }
    ASSERT_EQ(result, expected);
}

TEST(TestCaseInvertedIndex, TestBasic) {
    const vector<string> docs = {
            "london is the capital of great britain",
            "big ben is the nickname for the Great bell of the striking clock"
    };
    const vector<string> requests = {"london", "the"};
    const vector<vector<Entry>> expected = {
            {
                    {0, 1}
            }, {
                    {0, 1}, {1, 3}
            }
    };
    TestInvertedIndexFunctionality(docs, requests, expected);
}

TEST(TestCaseInvertedIndex, TestBasic2) {
    const vector<string> tb2_docs = {
            "milk milk milk milk water water water",
            "milk water water",
            "milk milk milk milk milk water water water water water",
            "americano cappuchino"
    };
    const vector<string> requests = {"milk", "water", "cappuchino"};
    const vector<vector<Entry>> expected = {
            {
                    {0, 4}, {1, 1}, {2, 5}
            },
            {
                    {0, 3}, {1, 2}, {2, 5}
            },
            {
                    {3, 1}
            }
    };
    TestInvertedIndexFunctionality(tb2_docs, requests, expected);
}


TEST(TestCaseInvertedIndex, TestInvertedIndexMissingWord) {
    const vector<string> docs = {
            "a b c d e f g h i j k l",
            "statement"
    };
    const vector<string> requests = {"m", "statement"};
    const vector<vector<Entry>> expected = {
            {
            }, {
                    {1, 1}
            }
    };
    TestInvertedIndexFunctionality(docs, requests, expected);
}


TEST(InvertedIndex, updateDocumentBase) {
    EXPECT_EQ(1, 1);
}

TEST(InvertedIndex, getWordCount) {
    EXPECT_EQ(1, 1);
}

