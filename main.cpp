/*
 * CMP 3005 PROJECT OF :
 * BARAN BATI
 * UGUR BILGIN
 * UMUT BARIS BASAK
 *
 */

#include <iostream>
#include <string>
#include <sstream>
#include <fstream>
#include <algorithm>
#include <vector>
#include <cstdlib>
#include <unordered_map>
#include <unordered_set>
#include <chrono>
#include "porter2_stemmer.cpp"

using namespace std;
using namespace std::chrono;

vector<string> splitSentences(const string &str);

vector<string> splitWords(const string &str);

string getScriptFromFile(const string &fileName);

unordered_map<string, vector<int>> wordToSentenceIndex(vector<string> sentences);

int findSentenceOfQuestion(unordered_map<string, vector<int>> map, const string &question);

string makeLower(string word);

vector<string> stemmedWords(const vector<string> &words);

const unordered_set<string> stopWordsSet = {"i", "me", "my", "myself", "we", "our", "ours", "ourselves", "you", "your",
                                            "yours", "yourself", "yourselves", "he", "him", "his", "himself", "she",
                                            "her", "hers", "herself", "it", "its", "itself", "they", "them", "their",
                                            "theirs", "themselves", "what", "which", "who", "whom", "this", "that",
                                            "these", "those", "am", "is", "are", "was", "were", "be", "been", "being",
                                            "have", "has", "had", "having", "do", "does", "did", "doing", "a", "an",
                                            "the", "and", "but", "if", "or", "because", "as", "until", "while", "of",
                                            "at", "by", "for", "with", "about", "against", "between", "into", "through",
                                            "during", "before", "after", "above", "below", "to", "from", "up", "down",
                                            "in", "out", "on", "off", "over", "under", "again", "further", "then",
                                            "once", "here", "there", "when", "where", "why", "how", "all", "any",
                                            "both", "each", "few", "more", "most", "other", "some", "such", "no", "nor",
                                            "not", "only", "own", "same", "so", "than", "too", "very", "s", "t", "can",
                                            "will", "just", "don", "should", "now"};

int main() {

    std::ifstream inQ("questions.txt");
    vector<string> questions;
    string temp;
    while (!inQ.eof()) {
        getline(inQ, temp);
        questions.push_back(temp);
    }

    string script = getScriptFromFile("the_truman_show_script.txt");

    auto start = high_resolution_clock::now();

    vector<string> sentences = splitSentences(script);
    unordered_map<string, vector<int>> map = wordToSentenceIndex(sentences);

    for (int i = 0; i < questions.size(); i++) {
        string question = questions[i].substr(0, questions[i].size() - 1);
        int answerIndex = findSentenceOfQuestion(map, question);
        string answer = sentences[answerIndex];
        cout << "q" << i + 1 << " = " << questions[i] << endl;
//        cout << "answerSentence" << i + 1 << " = " << answer << endl;
        vector<string> questionWords = splitWords(question);
        vector<string> answerWords = splitWords(answer);
        vector<string> stemmedQuestionWords = stemmedWords(questionWords);
        vector<string> stemmedAnswerWords = stemmedWords(answerWords);
        string result;
        for (int j = 0; j < stemmedAnswerWords.size(); j++) {
            if (find(stemmedQuestionWords.begin(), stemmedQuestionWords.end(), stemmedAnswerWords[j]) ==
                stemmedQuestionWords.end()) {
                result.append(answerWords[j]);
                result.append("  ");
            }
        }
        cout << "a" << i + 1 << " =" << result << endl;
    }


    auto stop = high_resolution_clock::now();
    auto duration = duration_cast<microseconds>(stop - start);
    cout << "Time taken by function: " << duration.count() << " microseconds" << endl;

}

vector<string> stemmedWords(const vector<string> &words) {
    vector<string> result;
    result.reserve(words.size());
    for (auto &word : words) {
        string temp = word;
        Porter2Stemmer::stem(const_cast<string &>(temp));
        result.push_back(temp);
    }
    return result;
}

string makeLower(string word) {
    for (char &i : word) {
        i = tolower(i);
    }
    return word;
}

int findSentenceOfQuestion(unordered_map<string, vector<int>> map, const string &question) {
    std::unordered_map<int, int> indicesMap;
    vector<string> questionWords = splitWords(question);
    for (const auto &word : questionWords) {
        auto element = map.find(word);
        if (element != map.end()) {
            vector<int> indices = element->second;
            for (int &index : indices) {
//            cout << indices[l] << " ";
                indicesMap[index]++;
            }
        }

    }
    int maxCount = 0;
    int maxIndex = 0;

    for (const auto&[key, value] : indicesMap) {
        if (value > maxCount) {
            maxCount = value;
            maxIndex = key;
        }
    }
    return maxIndex;
}

unordered_map<string, vector<int>> wordToSentenceIndex(vector<string> sentences) {
    unordered_map<string, vector<int>> hashMap;
    hashMap.reserve(4096);
    hashMap.max_load_factor(0.25);
    for (int i = 0; i < sentences.size(); i++) {
        vector<string> words = splitWords(sentences[i]);
        for (auto &word : words) {
            hashMap[word].push_back(i);
        }
    }
    return hashMap;
}

string getScriptFromFile(const string &fileName) {
    std::ifstream in(fileName);
    std::stringstream buffer;
    buffer << in.rdbuf();
    std::string script = buffer.str();
    return script;
}

vector<string> splitWords(const string &str) {
    vector<string> result;
    size_t start = 0;
    std::size_t found = str.find(' ', start);
    while (found != std::string::npos) {
        string word = str.substr(start, found - start);
        char lastLetter = word[word.size() - 1];
        string cleanWord;
        if (lastLetter == ',') {
            cleanWord = word.substr(0, word.size() - 1);
        } else { cleanWord = word; }
        if (stopWordsSet.find(makeLower(cleanWord)) == stopWordsSet.end()) {
            result.push_back(cleanWord);
        }
        start = found + 1;
        found = str.find(' ', start);
    }

    string lastWord = str.substr(start, str.length() - start);
    result.push_back(lastWord);


    return result;
}

vector<string> splitSentences(const string &str) {
    vector<string> result;
    size_t start = 0;
    std::size_t found = str.find_first_of(".!?", start);
    while (found != std::string::npos) {
        string sentence = str.substr(start, found - start);
        result.push_back(sentence);
        start = found + 1;
        found = str.find_first_of(".!?", start);
    }
    return result;
}