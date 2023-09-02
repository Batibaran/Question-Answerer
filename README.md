![image](https://cdn.bau.edu.tr/index/bau-OpenGraph.jpg)
 
 #CMP3005 Term Project





|Course  | CMP3005 Analysis of Algorithms |
|--------|--------------------------------|
|Project |CMP3005 Analysis of Algorithms  |
|Students| Baran BATI, Ugur BILGIN, Umut Baris BASAK|

## Programming Language

We chose C++ as our language because we all know it well compared to Java. Also considering the fact that C++ executes the code by using only a compiler. The compiler compiles and converts the code into machine code as opposed to Java where there is a compiler and an interpreter. Therefore C++ is faster than Java. 
We used JetBrains’ Clion as our IDE. Purely because we are familiarized with it over the years.


## Average Speed

The included timer in the code includes every step but the ifstream process. Given the fact that the project assignment asks for the algorithm’s time, we decided it’d make sense to exclude the file reading process. If the professor desires to check the time with file stream included; moving the timer start in **line 50** to the start of the main, at **line 47** will include the process as well.  
  
Our speed varies between **35000 and 52000 microseconds** . The average time is around **42000 microseconds** .


In DEBUG mode our speed varies between 32500 and 43000 microseconds . The average time is around 36500 microseconds . Right now for some reason we can’t run in Release mode to get the real speed of the code.


    34080 Microseconds 
    32930 Microseconds 
    40372 Microseconds
```bash
q1 = Who is suddenly aware that the hundreds of other beachgoers have stopped their activities to stare at him?
a1 =  Truman
q2 = With an apathetic shrug, what does Truman replace?
a2 =  receiver
q3 = He picks up the framed picture of his wife from where?
a3 =  desk
q4 = The sound of the children triggers what in his head?
a4 =  memory
q5 = What does Truman exit to investigate?"
a5 =  Oldsmobile
q6 = How many girls look up, surprised by the interruption?
a6 =  3
q7 = What time is read on a clock on the wall?
a7 =  4:12pm
q8 = What color is the cardigan she is already removing from the drycleaning bag on the back seat as Truman pulls away f
rom the curb?
a8 =  lavender
Time taken by function: 39508 microseconds
```



## Average Correctness

We tested with various questions from the_truman_show_script.txt where all the questions fit the template of the example questions given in the questions.txt and all questions were answered with 98% correctness. Due to stopwords some answers are not in exactly the same form with the script. Such as:

```bash
q9 = When does the reason for Christof's concern becomes all too apparent, where the struggling TRUMAN is being escorted away?

a9 = helicopter passes power plant
Time taken by function: 48891 microseconds
```
Where the fully correct answer would be “as the helicopter passes over the power plant”

**Important Note:**  
From time to time we get exit code -1073741819 and the program terminates. The exit code in mention is associated with **“access violation”**. That’s all we know about it. However when the code is run again the issue goes away. We neither know what triggers this nor how to get over it.  
We suspect that the problem is caused by a pointer issue somewhere, we are yet to find any evidence.

```bash
q1 = Who is suddenly aware that the hundreds of other beachgoers have stopped their activities to stare at him?
a1 =  Truman
q2 = With an apathetic shrug, what does Truman replace?
a2 =  receiver
q3 = He picks up the framed picture of his wife from where?
a3 =  desk
q4 = The sound of the children triggers what in his head?
a4 =  memory

Process finished with exit code  -1073741819 (0xC0000005)
```
### Included Libraries:
   
Other than the usual libraries we used;

<!-- UL -->
* **<unordered_map> :** unordered map corresponds to HashMap in c++ libraries where “map” is Red-Black Tree. HashMap was more fitting for our needs since we were concerned about speed and we didn’t need any sorting.
* **<unordered_set> :** we used the set for keeping our stopWords since the searching time in set is O(1).
* **“stem.cpp” :** this is the stemming library we took from [GitHub](http://snowball.tartarus.org/algorithms/english/stemmer.html).
* **<chrono.cpp :** is included for keeping time since Clion does not keep time.


```cpp
#include <iostream>
#include <string>
#include <sstream>
#include <fstream>
#include <algorithm>
#include <utility>
#include <vector>
#include <cstdlib>
#include <unordered_map>
#include <unordered_set>
#include <chrono>
#include "porter2_stemmer.cpp"
```

##Functions:

```cpp

vector<string> splitSentences(string str);

vector<string> splitWords(string str);

string getScriptFromFile(string fileName);

unordered_map<string, vector<int>> wordToSentenceIndex(vector<string> sentences);

int findSentenceOfQuestion(unordered_map<string, vector<int>> map, string question);

string makeLower(string word);

string stemmer(string word);

vector<string> stemmedWords(vector<string> words);

```
The function names were tried to be written quite explanatory.  

<!-- UL -->

* **splitSentences:** This function takes a string, the movie script in particular, and splits it into sentences and fills a vector with it.
    
```cpp
vector<string> splitSentences(string str) {
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
```
---
* **splitWords:** This function takes a string ,a sentence, then splits it word by word while cleaning the words from commas(,) and eliminates stop words.  

```cpp
vector<string> splitWords(string str) {
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
```
--------------------
* **getScriptFromFile:**  this function gets the filename and grabs the script from the .txt . buffer is used for ease of use and “claimed” speed increase.  

```cpp
string getScriptFromFile(string fileName) {
    std::ifstream in(fileName);
    std::stringstream buffer;
    buffer << in.rdbuf();
    std::string script = buffer.str();
    return script;
}
```
---
* **wordToSentenceIndex:** This function gets the vector of the sentences. It creates a hash map where the key is the words and value is their indices of the sentences where the word belongs.   
    * To explain better here’s a simple example:  
“Baran” = {1,2,3} : “Baran” is mentioned in sentences 1,2,3  
“Ugur” = {3,4,5,6} : “Ugur” is mentioned in sentences 3,4,5,6  
“Baris” = {3,7} : “Baris” is mentioned in sentences 3,7  
We can see all our names are included in the 3rd sentence. This will be the base for our problem solving.

```cpp
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
```
---
* **findSentenceOfQuestion:** This is the key function of our project. It takes the map and the question. Then returns the index of the answer sentence. It works by mapping the number of times where the words of the question appear. (scriptteki cümlelerin hangileri, soru cümlesindeki kelimeleri kaç defa içeriyor.) Then it finds the sentence with the most matches and returns it’s index.  
 ```cpp
 int findSentenceOfQuestion(unordered_map<string, vector<int>> map, string question) {
     std::unordered_map<int, int> indicesMap; //scriptteki cümlelerin hangileri, soru cümlesindeki kelimeleri kaç defa içeriyor.
     vector<string> questionWords = splitWords(question);
     for (auto word : questionWords) {
         auto element = map.find(word);
         if (element != map.end()) {
             vector<int> indices = element->second;
             for (int &indice : indices) {
 //            cout << indices[l] << " ";
                 indicesMap[indice]++;
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
```
---
* **makeLower :** This function just makes words into lower case for comparison  

```cpp
string makeLower(string word) {
    for (char &i : word) {
        i = tolower(i);
    }
    return word;
}
```
---
* **stemmedWords :** This function gets the word list as vector and pushes them through the stemmer. Returns a vector of stemmed words.  

```cpp
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
```
---

#Explanation:

We started with looking through data structures to use in the project. We started with the “map” function which works as a Red-Black tree, we quickly realised this wasn’t what we wanted and we switched to unordered_map which works as a Hashmap. Even though at first we tried to write the Stemming on our own it was too complex to solve so we turned to third party libraries. The [stemming library](http://snowball.tartarus.org/algorithms/english/stemmer.html) we could get working was written in C and it throws some errors, yet it does its job more than enough.  

Since execution time is of the essence and the code would always be tested with “the_truman_show_script.txt” we leant towards a hard coded optimization. After writing most of the code with hard coded optimization(see images below). We decided this isn’t the way to do this for us to learn. We restarted from scratch and wrote a “normal” code which now would also work with any given script. The cost of abandoning the hard coded optimization is insignificant compared to the achievement we made.  

```cpp
int qMarks[] = {3749, 3763, 6476, 7584, 7608, 7769, 8086, 8331, 8467, 8507, 8973, 9165, 10072, 11473, 11896, 12180,
                    12792, 12835, 15624, 15669, 16102, 16172, 17416, 19595, 19861, 20051, 20156, 20254, 21437, 21616,
                    21628, 21723, 21760, 22544, 22702, 22716, 23178, 24362, 24507, 24843, 24961, 26792, 27041, 27391,
                    27529, 27586, 28012, 28071, 28115, 33430, 34206, 34851, 35003, 35489, 35660, 36421, 36527, 37678,
                    37939, 38121, 39366, 40815, 41781, 42198, 44578, 44602, 45198, 45223, 45725, 45779, 46074, 48843,
                    49203, 49315, 49351, 49431, 49583, 50641, 50712, 50906, 51627, 52001, 52427, 52512, 52519, 52672,
                    52780, 53234, 53329, 53706, 54145, 54183, 54323, 54622, 54641, 60958, 61427, 61606, 61761, 63446,
                    64066, 64267, 64362, 64445, 65911, 66031, 66465, 66643, 67658, 68741, 69187, 69283, 69317, 74271};
    int eMarks[] = {2353, 3216, 3224, 3230, 3412, 3686, 13916, 13934, 14023, 17603, 17604, 17618, 17619, 21669, 26312,
                    26323, 28749, 28750, 28759, 28760, 30928, 30934, 30935, 31981, 32639, 32654, 33431, 33497, 33508,
                    33522, 33536, 33629, 38007, 38156, 38224, 40698, 40704, 43257, 44563, 50439, 50448, 50464, 50914,
                    50922, 51041, 51054, 52936, 53144, 53184, 53235, 53255, 53275, 53298, 53330, 53354, 53483, 53647,
                    53933, 62917, 62918, 62929, 62930, 64150, 64268, 64446, 64547, 64674, 65220, 65328, 65351, 65912,
                    65969, 66466, 66914, 66952, 67008, 67422, 67636, 68286, 73034, 73056, 73214, 73606, 73620, 75827,
                    75837, 79354, 79403, 80610};
    int tDots[] = {6553, 7364, 7403, 7431, 7467, 7511, 7585, 7609, 8027, 8078, 8087, 8385, 8438, 8450, 8468, 8497, 8515,
                   10043, 11897, 12093, 12234, 12340, 12403, 12942, 12981, 14024, 16712, 21663, 23512, 28751, 28761,
                   30408, 39464, 40094, 40141, 40691, 40705, 40781, 40804, 42265, 42286, 42952, 42971, 43006, 43224,
                   43258, 43308, 43639, 43679, 45652, 45691, 45726, 45803, 48096, 48500, 49961, 50011, 50073, 50484,
                   52570, 54389, 55233, 56183, 61041, 65726, 71246, 71293, 72339, 72468, 73290, 73406, 74718, 74910,
                   74937, 78259, 78635};
    int mEdits[] = {41, 302, 310, 593, 619, 1086, 1108, 2093, 2126, 2571, 2589, 4293, 4330, 5646, 5658, 6799, 6737,
                    7223, 7258, 8566, 8586, 10299, 10310, 10732, 10760, 11547, 11576, 11999, 12018, 13329, 13355,
                    13660, 13679, 14144, 14170, 14611, 14625, 14757, 14763, 14951, 14978, 15178, 15191, 15808,
                    15816, 18073, 18089, 18519, 18535, 19459, 19485, 20259, 20277, 23881, 23890, 24990, 25010,
                    25511, 25544, 26328, 26354, 26504, 26520, 27870, 27906, 28187, 28213, 28491, 28510, 28793,
                    28818, 29406, 29423, 33944, 33955, 36219, 36237, 36973, 37010, 38671, 38696, 39717, 39736,
                    41046, 41063, 42408, 42418, 42737, 42751, 43754, 43766, 44052, 44079, 44674, 44688, 45228,
                    45250, 47599, 47621, 49712, 49734, 54188, 54205, 54460, 54474, 54974, 54996, 55322, 55343,
                    55945, 55980, 56708, 56722, 57534, 57556, 58554, 58569, 58814, 58841, 59143, 59169, 59846,
                    59870, 61100, 61127, 61946, 61968, 62650, 62671, 63344, 63358, 63724, 63750, 65004, 65018,
                    65433, 65459, 68291, 68310, 71095, 71109, 71871, 71885, 72150, 72163, 72689, 72701, 73979,
                    73993, 74427, 74440, 75134, 75146, 76656, 76668};
```

```cpp
std::string storage[1383];
```

