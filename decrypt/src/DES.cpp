#include "DES.h"

// Các bảng tra cứu của DES (định nghĩa đầy đủ theo chuẩn)
const int IP[64] = {
    58, 50, 42, 34, 26, 18, 10, 2,
    60, 52, 44, 36, 28, 20, 12, 4,
    62, 54, 46, 38, 30, 22, 14, 6,
    64, 56, 48, 40, 32, 24, 16, 8,
    57, 49, 41, 33, 25, 17, 9, 1,
    59, 51, 43, 35, 27, 19, 11, 3,
    61, 53, 45, 37, 29, 21, 13, 5,
    63, 55, 47, 39, 31, 23, 15, 7
};

const int IP_INV[64] = {
    40, 8, 48, 16, 56, 24, 64, 32,
    39, 7, 47, 15, 55, 23, 63, 31,
    38, 6, 46, 14, 54, 22, 62, 30,
    37, 5, 45, 13, 53, 21, 61, 29,
    36, 4, 44, 12, 52, 20, 60, 28,
    35, 3, 43, 11, 51, 19, 59, 27,
    34, 2, 42, 10, 50, 18, 58, 26,
    33, 1, 41, 9, 49, 17, 57, 25
};

const int PC1[56] = {
    57, 49, 41, 33, 25, 17, 9,
    1, 58, 50, 42, 34, 26, 18,
    10, 2, 59, 51, 43, 35, 27,
    19, 11, 3, 60, 52, 44, 36,
    63, 55, 47, 39, 31, 23, 15,
    7, 62, 54, 46, 38, 30, 22,
    14, 6, 61, 53, 45, 37, 29,
    21, 13, 5, 28, 20, 12, 4
};

const int PC2[48] = {
    14, 17, 11, 24, 1, 5,
    3, 28, 15, 6, 21, 10,
    23, 19, 12, 4, 26, 8,
    16, 7, 27, 20, 13, 2,
    41, 52, 31, 37, 47, 55,
    30, 40, 51, 45, 33, 48,
    44, 49, 39, 56, 34, 53,
    46, 42, 50, 36, 29, 32
};

const int E[48] = {
    32, 1, 2, 3, 4, 5,
    4, 5, 6, 7, 8, 9,
    8, 9, 10, 11, 12, 13,
    12, 13, 14, 15, 16, 17,
    16, 17, 18, 19, 20, 21,
    20, 21, 22, 23, 24, 25,
    24, 25, 26, 27, 28, 29,
    28, 29, 30, 31, 32, 1
};

const int P[32] = {
    16, 7, 20, 21,
    29, 12, 28, 17,
    1, 15, 23, 26,
    5, 18, 31, 10,
    2, 8, 24, 14,
    32, 27, 3, 9,
    19, 13, 30, 6,
    22, 11, 4, 25
};

const int S[8][4][16] = {
    // S1
    {{14, 4, 13, 1, 2, 15, 11, 8, 3, 10, 6, 12, 5, 9, 0, 7},
     {0, 15, 7, 4, 14, 2, 13, 1, 10, 6, 12, 11, 9, 5, 3, 8},
     {4, 1, 14, 8, 13, 6, 2, 11, 15, 12, 9, 7, 3, 10, 5, 0},
     {15, 12, 8, 2, 4, 9, 1, 7, 5, 11, 3, 14, 10, 0, 6, 13}},
    // S2
    {{15, 1, 8, 14, 6, 11, 3, 4, 9, 7, 2, 13, 12, 0, 5, 10},
     {3, 13, 4, 7, 15, 2, 8, 14, 12, 0, 1, 10, 6, 9, 11, 5},
     {0, 14, 7, 11, 10, 4, 13, 1, 5, 8, 12, 6, 9, 3, 2, 15},
     {13, 8, 10, 1, 3, 15, 4, 2, 11, 6, 7, 12, 0, 5, 14, 9}},
    // S3
    {{10, 0, 9, 14, 6, 3, 15, 5, 1, 13, 12, 7, 11, 4, 2, 8},
     {13, 7, 0, 9, 3, 4, 6, 10, 2, 8, 5, 14, 12, 11, 15, 1},
     {13, 6, 4, 9, 8, 15, 3, 0, 11, 1, 2, 12, 5, 10, 14, 7},
     {1, 10, 13, 0, 6, 9, 8, 7, 4, 15, 14, 3, 11, 5, 2, 12}},
    // S4
    {{7, 13, 14, 3, 0, 6, 9, 10, 1, 2, 8, 5, 11, 12, 4, 15},
     {13, 8, 11, 5, 6, 15, 0, 3, 4, 7, 2, 12, 1, 10, 14, 9},
     {10, 6, 9, 0, 12, 11, 7, 13, 15, 1, 3, 14, 5, 2, 8, 4},
     {3, 15, 0, 6, 10, 1, 13, 8, 9, 4, 5, 11, 12, 7, 2, 14}},
    // S5
    {{2, 12, 4, 1, 7, 10, 11, 6, 8, 5, 3, 15, 13, 0, 14, 9},
     {14, 11, 2, 12, 4, 7, 13, 1, 5, 0, 15, 10, 3, 9, 8, 6},
     {4, 2, 1, 11, 10, 13, 7, 8, 15, 9, 12, 5, 6, 3, 0, 14},
     {11, 8, 12, 7, 1, 14, 2, 13, 6, 15, 0, 9, 10, 4, 5, 3}},
    // S6
    {{12, 1, 10, 15, 9, 2, 6, 8, 0, 13, 3, 4, 14, 7, 5, 11},
     {10, 15, 4, 2, 7, 12, 9, 5, 6, 1, 13, 14, 0, 11, 3, 8},
     {9, 14, 15, 5, 2, 8, 12, 3, 7, 0, 4, 10, 1, 13, 11, 6},
     {4, 3, 2, 12, 9, 5, 15, 10, 11, 14, 1, 7, 6, 0, 8, 13}},
    // S7
    {{4, 11, 2, 14, 15, 0, 8, 13, 3, 12, 9, 7, 5, 10, 6, 1},
     {13, 0, 11, 7, 4, 9, 1, 10, 14, 3, 5, 12, 2, 15, 8, 6},
     {1, 4, 11, 13, 12, 3, 7, 14, 10, 15, 6, 8, 0, 5, 9, 2},
     {6, 11, 13, 8, 1, 4, 10, 7, 9, 5, 0, 15, 14, 2, 3, 12}},
    // S8
    {{13, 2, 8, 4, 6, 15, 11, 1, 10, 9, 3, 14, 5, 0, 12, 7},
     {1, 15, 13, 8, 10, 3, 7, 4, 12, 5, 6, 11, 0, 14, 9, 2},
     {7, 11, 4, 1, 9, 12, 14, 2, 0, 6, 10, 13, 15, 3, 5, 8},
     {2, 1, 14, 7, 4, 10, 8, 13, 15, 12, 9, 0, 3, 5, 6, 11}}
};

const int SHIFTS[16] = {1, 1, 2, 2, 2, 2, 2, 2, 1, 2, 2, 2, 2, 2, 2, 1};

// Hàm thao tác bit
void setBit(unsigned char* data, int pos, int value) {
    int bytePos = pos / 8;
    int bitPos = 7 - (pos % 8);
    if (value)
        data[bytePos] |= (1 << bitPos);
    else
        data[bytePos] &= ~(1 << bitPos);
}

int getBit(const unsigned char* data, int pos) {
    int bytePos = pos / 8;
    int bitPos = 7 - (pos % 8);
    return (data[bytePos] >> bitPos) & 1;
}

void permute(const unsigned char* input, unsigned char* output, const int* table, int size) {
    for (int i = 0; i < size; i++) {
        setBit(output, i, getBit(input, table[i] - 1));
    }
}

void leftShift(unsigned char* data, int bits, int len) {
    unsigned char temp[8] = {0};
    for (int i = 0; i < len; i++) {
        setBit(temp, i, getBit(data, (i + bits) % len));
    }
    for (int i = 0; i < len / 8; i++) {
        data[i] = temp[i];
    }
}

// Sinh 16 subkey từ khóa chính
void generateSubkeys(const unsigned char* key, unsigned char subkeys[ROUNDS][6]) {
    unsigned char key56[7] = {0}; // 56 bit sau PC-1
    unsigned char C[4] = {0}, D[4] = {0}; // Hai nửa C và D (28 bit mỗi nửa)

    // Áp dụng PC-1 để giảm từ 64 bit xuống 56 bit
    permute(key, key56, PC1, 56);

    // Chia thành C và D
    for (int i = 0; i < 28; i++) {
        setBit(C, i, getBit(key56, i));
        setBit(D, i, getBit(key56, i + 28));
    }

    // Sinh 16 subkey
    for (int round = 0; round < ROUNDS; round++) {
        // Dịch trái C và D
        leftShift(C, SHIFTS[round], 28);
        leftShift(D, SHIFTS[round], 28);

        // Gộp C và D thành 56 bit
        unsigned char temp[7] = {0};
        for (int i = 0; i < 28; i++) {
            setBit(temp, i, getBit(C, i));
            setBit(temp, i + 28, getBit(D, i));
        }

        // Áp dụng PC-2 để tạo subkey 48 bit
        permute(temp, subkeys[round], PC2, 48);
    }
}

// Hàm f của DES
void desFunction(const unsigned char* R, const unsigned char* subkey, unsigned char* output) {
    unsigned char expanded[6] = {0}; // 48 bit sau khi mở rộng
    unsigned char temp[6] = {0};     // Sau khi XOR với subkey
    unsigned char sboxOut[4] = {0};  // 32 bit sau S-box

    // Mở rộng R từ 32 bit lên 48 bit
    permute(R, expanded, E, 48);

    // XOR với subkey
    for (int i = 0; i < 6; i++) {
        temp[i] = expanded[i] ^ subkey[i];
    }

    // Áp dụng 8 S-box
    for (int i = 0; i < 8; i++) {
        int row = (getBit(temp, i * 6) << 1) | getBit(temp, i * 6 + 5);
        int col = (getBit(temp, i * 6 + 1) << 3) | (getBit(temp, i * 6 + 2) << 2) |
                  (getBit(temp, i * 6 + 3) << 1) | getBit(temp, i * 6 + 4);
        int val = S[i][row][col];
        for (int j = 0; j < 4; j++) {
            setBit(sboxOut, i * 4 + j, (val >> (3 - j)) & 1);
        }
    }

    // Hoán vị P
    permute(sboxOut, output, P, 32);
}

// Mã hóa một block
void desEncryptBlock(const unsigned char* plaintext, const unsigned char (*subkeys)[6], unsigned char* ciphertext) {
    unsigned char permuted[8] = {0};
    unsigned char L[4] = {0}, R[4] = {0};
    unsigned char temp[4] = {0};

    // Hoán vị ban đầu IP
    permute(plaintext, permuted, IP, 64);

    // Chia thành L và R
    for (int i = 0; i < 32; i++) {
        setBit(L, i, getBit(permuted, i));
        setBit(R, i, getBit(permuted, i + 32));
    }

    // 16 vòng lặp
    for (int round = 0; round < ROUNDS; round++) {
        unsigned char fOut[4] = {0};
        desFunction(R, subkeys[round], fOut); // Sửa: subkeys[round] đã là con trỏ tới mảng 6 byte

        // L = R cũ, R = L cũ XOR f(R, subkey)
        for (int i = 0; i < 4; i++) {
            temp[i] = L[i];
            L[i] = R[i];
            R[i] = temp[i] ^ fOut[i];
        }
    }

    // Gộp L và R (đổi chỗ lần cuối)
    unsigned char combined[8] = {0};
    for (int i = 0; i < 32; i++) {
        setBit(combined, i, getBit(R, i));
        setBit(combined, i + 32, getBit(L, i));
    }

    // Hoán vị ngược IP^-1
    permute(combined, ciphertext, IP_INV, 64);
}

// Giải mã một block
void desDecryptBlock(const unsigned char* ciphertext, const unsigned char (*subkeys)[6], unsigned char* plaintext) {
    unsigned char permuted[8] = {0};
    unsigned char L[4] = {0}, R[4] = {0};
    unsigned char temp[4] = {0};

    permute(ciphertext, permuted, IP, 64);

    for (int i = 0; i < 32; i++) {
        setBit(L, i, getBit(permuted, i));
        setBit(R, i, getBit(permuted, i + 32));
    }

    for (int round = ROUNDS - 1; round >= 0; round--) {
        unsigned char fOut[4] = {0};
        desFunction(R, subkeys[round], fOut); // Sửa: subkeys[round] đã là con trỏ tới mảng 6 byte

        for (int i = 0; i < 4; i++) {
            temp[i] = L[i];
            L[i] = R[i];
            R[i] = temp[i] ^ fOut[i];
        }
    }

    unsigned char combined[8] = {0};
    for (int i = 0; i < 32; i++) {
        setBit(combined, i, getBit(R, i));
        setBit(combined, i + 32, getBit(L, i));
    }

    permute(combined, plaintext, IP_INV, 64);
}