#include <iostream>
#include <string>
#include <cmath>
#include <bitset>

using namespace std;

string Bin_to_Str(string s);
string Str_to_Bin(string s);
string Dec_to_Bin(int n);

class DES_Encryption
{

    // constants for the keys

    const int pc1[56] = {57, 49, 41, 33, 25, 17, 9,
                         1, 58, 50, 42, 34, 26, 18,
                         10, 2, 59, 51, 43, 35, 27,
                         19, 11, 3, 60, 52, 44, 36,
                         63, 55, 47, 39, 31, 23, 15,
                         7, 62, 54, 46, 38, 30, 22,
                         14, 6, 61, 53, 45, 37, 29,
                         21, 13, 5, 28, 20, 12, 4};

    const int pc_2[48] = {14, 17, 11, 24, 1, 5,
                          3, 28, 15, 6, 21, 10,
                          23, 19, 12, 4, 26, 8,
                          16, 7, 27, 20, 13, 2,
                          41, 52, 31, 37, 47, 55,
                          30, 40, 51, 45, 33, 48,
                          44, 49, 39, 56, 34, 53,
                          46, 42, 50, 36, 29, 32};

    // constants for the message

    const int IP_t[64] = {58, 50, 42, 34, 26, 18, 10, 2, // intital permutation table
                          60, 52, 44, 36, 28, 20, 12, 4,
                          62, 54, 46, 38, 30, 22, 14, 6,
                          64, 56, 48, 40, 32, 24, 16, 8,
                          57, 49, 41, 33, 25, 17, 9, 1,
                          59, 51, 43, 35, 27, 19, 11, 3,
                          61, 53, 45, 37, 29, 21, 13, 5,
                          63, 55, 47, 39, 31, 23, 15, 7};

    const int E_t[48] = {32, 1, 2, 3, 4, 5,
                         4, 5, 6, 7, 8, 9,
                         8, 9, 10, 11, 12, 13,
                         12, 13, 14, 15, 16, 17,
                         16, 17, 18, 19, 20, 21,
                         20, 21, 22, 23, 24, 25,
                         24, 25, 26, 27, 28, 29,
                         28, 29, 30, 31, 32, 1};

    int S[4][16] = { // S-box
        {14, 4, 13, 1, 2, 15, 11, 8, 3, 10, 6, 12, 5, 9, 0, 7},
        {0, 15, 7, 4, 14, 2, 13, 1, 10, 6, 12, 11, 9, 5, 3, 8},
        {4, 1, 14, 8, 13, 6, 2, 11, 15, 12, 9, 7, 3, 10, 5, 0},
        {15, 12, 8, 2, 4, 9, 1, 7, 5, 11, 3, 14, 10, 0, 6, 13}};

    const int P[32] = {16, 7, 20, 21,
                       29, 12, 28, 17,
                       1, 15, 23, 26,
                       5, 18, 31, 10,
                       2, 8, 24, 14,
                       32, 27, 3, 9,
                       19, 13, 30, 6,
                       22, 11, 4, 25};

    const int P_1[64] = {40, 8, 48, 16, 56, 24, 64, 32,
                         39, 7, 47, 15, 55, 23, 63, 31,
                         38, 6, 46, 14, 54, 22, 62, 30,
                         37, 5, 45, 13, 53, 21, 61, 29,
                         36, 4, 44, 12, 52, 20, 60, 28,
                         35, 3, 43, 11, 51, 19, 59, 27,
                         34, 2, 42, 10, 50, 18, 58, 26,
                         33, 1, 41, 9, 49, 17, 57, 25};

    // some functions

    void expand_R(string r, string r32) // expanding according to expantion table E_t
    {
        r = "";
        for (int j = 0; j < 48; j++)
        {
            r += r32[E_t[j] - 1];
        }
    }

    string xor_add(string s1, string s2)
    {
        string result = "";
        for (int j = 0; j < s1.size(); j++)
        {
            if (s1[j] != s2[j])
                result += '1';
            else
                result += '0';
        }
        return result;
    }
    string shift_bit(string s)
    {
        char firstChar = s[0];
        for (size_t i = 1; i < s.size(); i++)
        {
            s[i - 1] = s[i];
        }
        s[s.size() - 1] = firstChar;

        return s;
    }
    string sbox(string s, int k)
    {
        int d1 = (s[0] - '0') * 2 + (s[5] - '0');
        int d2 = 0;

        for (int i = s.size() - 2, pwr = 0; i >= 1; i--, pwr++)
        {
            d2 += (s[i] - '0') * static_cast<int>(pow(2, pwr));
        }
        return Dec_to_Bin(S[d1][d2]);
    }

public:
    void encrypt(const string &plain_txt, const string &key)
    {

        string R_32;
        string R_xor_K;
        string R_48;
        string s[8];
        string s_1;
        string P_R[16];
        string key_64 = Str_to_Bin(key);

        string key_56 = "";
        string key_firstHalf = "", key_secondHalf = "";
        int half = 0;
        for (int i = 0; i < 56; i++)
        {
            key_56 += key_64[pc1[i] - 1];
            if (i == 28)
                half = 1;
            if (!half)
            {
                key_firstHalf += key_56[i];
            }
            else
            {
                key_secondHalf += key_56[i];
            }
        }
        string L_key, R_key;

        L_key = shift_bit(key_firstHalf);
        R_key = shift_bit(key_secondHalf);

        string key_48, keys_56;

        keys_56 = L_key + R_key;

        key_48 = "";
        for (int j = 0; j < 48; j++)
            key_48 += keys_56[pc_2[j] - 1]; // PC-2

        // Text

        string plain_txt_64 = Str_to_Bin(plain_txt);

        string IP = ""; // permuted key

        for (int i = 0; i < 64; i++)
            IP += plain_txt_64[IP_t[i] - 1];

        string L = "", R = "";

        for (int i = 0; i < 32; i++)
        {
            L += IP[i];
        }
        for (int i = 32; i < 64; i++)
        {
            R += IP[i];
        }

        R_48 = "";
        for (int j = 0; j < 48; j++)
        {
            R_48 += R[E_t[j] - 1];
        }
        R_xor_K = xor_add(R_48, key_48);

        for (int j = 0; j < 48; j += 6)
        { // Converting to an 8x6
            for (int k = j; k < j + 6; k++)
            {
                s[j / 6] += R_xor_K[k];
            }
        }
        s_1 = "";
        for (int j = 0; j < 8; j++)
        {
            s_1 += sbox(s[j], j);
        }

        for (int j = 0; j < 32; j++)
        {
            P_R[0] += s_1[P[j] - 1];
        }

        R_32 = "";
        R_32 = xor_add(P_R[0], L);

        cout << R_32 << endl;
    }
};

int main()
{
    DES_Encryption DES;
    string plain_txt, key;

    cout << "Enter PLAIN TEXT : ";

    cin >> plain_txt;

    cout << "Enter KEY : ";
    cin >> key;

    DES.encrypt(plain_txt, key);
    return 0;
}

string Dec_to_Bin(int n)
{
    string bin = "";
    while (n > 0)
    {
        bin = (char)(n % 2 + '0') + bin;
        n /= 2;
    }
    while (bin.size() < 4)
        bin = '0' + bin;
    return bin;
}

string Str_to_Bin(string input)
{
    string binaryString = "";

    for (char c : input)
    {
        // Convert each character to its ASCII value and then to a binary string.
        bitset<8> binaryChar(static_cast<unsigned char>(c));
        binaryString += binaryChar.to_string();
    }

    return binaryString;
}
