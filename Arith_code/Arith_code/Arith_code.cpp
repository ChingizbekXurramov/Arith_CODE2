#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <numeric>
using namespace std;

const int MAX_VALUE = 1 << 30;

void encode(const string& inputFileName, const string& outputFileName) {
    ifstream inputFile(inputFileName);
    if (!inputFile.is_open()) {
        cerr << "Ошибка: файл " << inputFileName << " не найден." << endl;
        return;
    }

    string text((istreambuf_iterator<char>(inputFile)), istreambuf_iterator<char>());
    inputFile.close();

    int low = 0, high = MAX_VALUE - 1;
    vector<int> freq(256, 1);
    int totalSymbols = text.size();


    for (char c : text) {
        freq[static_cast<unsigned char>(c)]++;
    }

    for (char c : text) {
        int range = high - low + 1;
        high = low + (range * accumulate(freq.begin(), freq.begin() + static_cast<unsigned char>(c) + 1, 0) / totalSymbols) - 1;
        low = low + (range * accumulate(freq.begin(), freq.begin() + static_cast<unsigned char>(c), 0) / totalSymbols);


        while (true) {
            if (high < (MAX_VALUE / 2)) {
                low *= 2;
                high = 2 * high + 1;
            }
            else if (low >= (MAX_VALUE / 2)) {
                low = 2 * (low - (MAX_VALUE / 2));
                high = 2 * (high - (MAX_VALUE / 2)) + 1;
            }
            else {
                break;
            }
        }
    }


    ofstream outputFile(outputFileName);
    if (!outputFile.is_open()) {
        cerr << "Ошибка записи в файл " << outputFileName << "." << endl;
        return;
    }

    outputFile << low;
    outputFile.close();
}

void decode(const string& inputFileName, const string& outputFileName) {
    ifstream inputFile(inputFileName);
    if (!inputFile.is_open()) {
        cerr << "Ошибка: файл " << inputFileName << " не найден." << endl;
        return;
    }

    int code;
    inputFile >> code;
    inputFile.close();

    int low = 0, high = MAX_VALUE - 1;
    vector<int> freq(256, 1);
    string decodedText;


    while (decodedText.size() < 100) {
        int range = high - low + 1;
        int totalSymbols = std::accumulate(freq.begin(), freq.end(), 0);


        int cumFreq = 0;
        for (int i = 0; i < 256; ++i) {
            cumFreq += freq[i];
            if (low + range * cumFreq / totalSymbols > code) {
                decodedText += static_cast<char>(i);
                freq[i]++;
                break;
            }
        }


        while (true) {
            if (high < (MAX_VALUE / 2)) {
                low *= 2;
                high = 2 * high + 1;
            }
            else if (low >= (MAX_VALUE / 2)) {
                low = 2 * (low - (MAX_VALUE / 2));
                high = 2 * (high - (MAX_VALUE / 2)) + 1;
            }
            else {
                break;
            }
        }
    }


    ofstream outputFile(outputFileName);
    if (!outputFile.is_open()) {
        cerr << "Ошибка записи в файл " << outputFileName << "." << endl;
        return;
    }

    outputFile << decodedText;
    outputFile.close();
}

int main()
{
    string inputFileName, outputFileName;


    cout << "Введите имя файла для кодирования: ";
    cin >> inputFileName;

    cout << "Введите имя файла для сохранения закодированного текста: ";
    cin >> outputFileName;
    encode(inputFileName, outputFileName);


    cout << "Введите имя файла для декодирования: ";
    cin >> inputFileName;

    cout << "Введите имя файла для сохранения декодированного текста: ";
    cin >> outputFileName;

    decode(inputFileName, outputFileName);

    return 0;
}