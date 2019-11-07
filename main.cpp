#include <iostream>
#include <utility>
#include <vector>
#include <random>

using namespace std;

static const size_t L = 40;

struct encoder {
    explicit encoder(vector<char> key) : key(move(key)) {
        init_s();
    }

    char encode(char m) {
        return gen_k() ^ m;
    }

private:
    static const size_t BLOCK_SIZE = 256;
    int i = 0;
    int j = 0;
    vector<char> s;
    vector<char> key;

    void init_s() {
        s.resize(BLOCK_SIZE);
        for (size_t cur = 0; cur < BLOCK_SIZE; ++cur) {
            s[cur] = cur;
        }
        size_t cur_pos_for_change = 0;
        for (size_t cur = 0; cur < BLOCK_SIZE; ++cur) {
            cur_pos_for_change = (cur_pos_for_change + s[cur] + key[cur % L]) % BLOCK_SIZE;
            swap(s[cur], s[cur_pos_for_change]);
        }
    }

protected:
    char gen_k() {
        i = (i + 1) % BLOCK_SIZE;
        j = (j + s[i]) % BLOCK_SIZE;
        swap(s[i], s[j]);
        int t = (s[i] + s[j]) % BLOCK_SIZE;
        return s[t];
    }
};

struct decoder : encoder {
    explicit decoder(vector<char> key) : encoder(move(key)) {}

    char decode(char m) {
        return gen_k() ^ m;
    }
};

vector<char> gen_key() {
    vector<char> key(L);
    random_device rd;
    mt19937 mt(rd());
    std::uniform_int_distribution<char> distribution(-128, 127);
    for (int i = 0; i < L; ++i) {
        key[i] = distribution(mt);
    }
    return key;
}

void print_char_as_bits(char c) {
    for (int i = 0; i < 8; ++i) {
        if (((int) c) & (1 << i)) {
            cout << 1;
        } else {
            cout << 0;
        }
    }
}

int main() {
    vector<char> key = gen_key();

    encoder encoder(key);
    decoder decoder(key);

    cout << "Key:" << endl;
    for (auto c : key) {
        print_char_as_bits(c);
    }
    cout << endl;

    cout << "Enter characters for encoding." << endl;

    char m;
    while (cin >> m) {
        char encrypted = encoder.encode(m);
        char decrypted = decoder.decode(encrypted);

        cout << endl << "Read char | Encrypted char | Decrypted char\n";
        cout << "    " << m    << "     |       " << encrypted << "        |       " << decrypted << "       " << endl;
        cout << " ";
        print_char_as_bits(m);
        cout << " |    ";
        print_char_as_bits(encrypted);
        cout << "    |   ";
        print_char_as_bits(decrypted);
        cout << "   " << endl;

        if (decrypted != m) {
            cerr << "-----------------------------------FAILED!!!-----------------------------------";
        }
    }

    return 0;
}