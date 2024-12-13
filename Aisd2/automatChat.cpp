//eksperyment pokazujacy co zrobil chat po kilkunastu podpowiedziach
//poleceniem bylo zeby napisal kod zgodnie z dobrymi praktykami c++ (jakies stl, rozdzielenie logiki obslugi napisow i logiki automatu itp.)
//zdaje sobie sprawe ze na tym przedmiocie ten moj c++ wygladal bardziej jak c z klasami
//ale jednak mam takie wrazenie ze o to chodzi w tym przedmiocie zeby troche podzialac w tej kwestii rowniez

#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <stdexcept>
#include <bitset>

class Automat {
public:
    Automat(const std::string& pattern);
    void nextState(char c);
    int getState() const;
    void resetState();

private:
    int state;
    int patternLength;
    std::bitset<64> alphabetMask;
    std::vector<char> alphabetTable;
    std::vector<std::vector<int>> stateMachine;

    void buildAlphabet(const std::string& pattern);
    void buildStateMachine(const std::string& pattern);
    int findCharacterIndex(char c) const;
};

Automat::Automat(const std::string& pattern) : state(0), patternLength(pattern.size()) {
    if (pattern.empty()) {
        throw std::invalid_argument("Pattern cannot be empty");
    }
    buildAlphabet(pattern);
    buildStateMachine(pattern);
}

void Automat::buildAlphabet(const std::string& pattern) {
    for (char c : pattern) {
        if (c >= 'a' && c <= 'z') {
            alphabetMask.set(static_cast<size_t>(c - 'a'));
        }
        else if (c >= 'A' && c <= 'Z') {
            alphabetMask.set(static_cast<size_t>(c - 'A'));
        }
        else if (c >= '0' && c <= '9') {
            alphabetMask.set(static_cast<size_t>(c - '0' + 26)); // Cyfry zajmuj¹ bity 26-35
        }
        else {
            throw std::invalid_argument("Pattern contains unsupported character: " + std::string(1, c));
        }
    }
    for (size_t i = 0; i < alphabetMask.size(); ++i) {
        if (alphabetMask.test(i)) {
            if (i < 26) {
                alphabetTable.push_back('a' + i);
            }
            else {
                alphabetTable.push_back('0' + (i - 26));
            }
        }
    }
}

void Automat::buildStateMachine(const std::string& pattern) {
    stateMachine.resize(patternLength + 1, std::vector<int>(alphabetTable.size(), 0));
    for (int y = 0; y <= patternLength; ++y) {
        for (size_t x = 0; x < alphabetTable.size(); ++x) {
            std::string temp = pattern.substr(0, y) + alphabetTable[x];
            for (int len = y + 1; len > 0; --len) {
                if (temp.substr(y + 1 - len, len) == pattern.substr(0, len)) {
                    stateMachine[y][x] = len;
                    break;
                }
            }
        }
    }
}

void Automat::nextState(char c) {
    int index = findCharacterIndex(c);
    if (index != -1) {
        state = stateMachine[state][index];
    }
    else {
        state = 0;
    }
}

int Automat::findCharacterIndex(char c) const {
    if (c >= 'A' && c <= 'Z') {
        c = c - 'A' + 'a'; // Konwersja na ma³e litery
    }
    auto it = std::find(alphabetTable.begin(), alphabetTable.end(), c);
    return (it != alphabetTable.end()) ? std::distance(alphabetTable.begin(), it) : -1;
}

int Automat::getState() const {
    return state;
}

void Automat::resetState() {
    state = 0;
}

void searchPatternInFile(const std::string& filePath, const std::string& pattern, bool verbose) {
    std::ifstream file(filePath);
    if (!file.is_open()) {
        throw std::runtime_error("Unable to open file: " + filePath);
    }

    Automat automat(pattern);
    int occurrences = 0;
    int position = 0;
    char c;

    while (file.get(c)) {
        automat.nextState(c);
        if (automat.getState() == pattern.size()) {
            ++occurrences;
            if (verbose) {
                std::cout << "Pattern found at position: " << position - pattern.size() + 1 << std::endl;
            }
            automat.resetState();
        }
        ++position;
    }

    std::cout << "Total occurrences: " << occurrences << std::endl;
}

int main(int argc, char* argv[]) {
    if (argc < 3) {
        std::cerr << "Usage: " << argv[0] << " <file_path> <pattern> [verbose]" << std::endl;
        return 1;
    }

    std::string filePath = argv[1];
    std::string pattern = argv[2];
    bool verbose = (argc > 3) && (std::string(argv[3]) == "1");

    try {
        searchPatternInFile(filePath, pattern, verbose);
    }
    catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << std::endl;
        return 1;
    }

    return 0;
}
