// #ifndef DES_H
// #define DES_H

// // Kích thước block và khóa
// #define BLOCK_SIZE 64  // 64 bits cho mỗi block
// #define KEY_SIZE 64    // 64 bits cho khóa
// #define SUBKEY_SIZE 48 // 48 bits cho mỗi subkey
// #define ROUNDS 16      // 16 vòng lặp

// // Khai báo các bảng tra cứu của DES
// extern const int IP[64];        // Initial Permutation (Hoán vị ban đầu)
// extern const int IP_INV[64];    // Inverse Initial Permutation (Hoán vị ngược)
// extern const int PC1[56];       // Permuted Choice 1 (Chọn khóa 56 bit)
// extern const int PC2[48];       // Permuted Choice 2 (Chọn subkey 48 bit)
// extern const int E[48];         // Expansion table (Mở rộng R từ 32 lên 48 bit)
// extern const int P[32];         // Permutation (Hoán vị trong hàm f)
// extern const int S[8][4][16];   // 8 S-boxes (Mỗi S-box ánh xạ 6 bit thành 4 bit)
// extern const int SHIFTS[16];    // Số lần dịch bit cho mỗi vòng

// // Hàm tiện ích thao tác bit
// void setBit(unsigned char* data, int pos, int value);
// int getBit(const unsigned char* data, int pos);
// void permute(const unsigned char* input, unsigned char* output, const int* table, int size);
// void leftShift(unsigned char* data, int bits, int len);

// // Hàm chính của DES
// void generateSubkeys(const unsigned char* key, unsigned char subkeys[ROUNDS][6]);
// void desFunction(const unsigned char* R, const unsigned char* subkey, unsigned char* output);
// void desEncryptBlock(const unsigned char* plaintext, const unsigned char* subkeys, unsigned char* ciphertext);
// void desDecryptBlock(const unsigned char* ciphertext, const unsigned char* subkeys, unsigned char* plaintext);

// #endif

#ifndef DES_H
#define DES_H

#define ROUNDS 16

extern const int PC1[56];
extern const int PC2[48];
extern const int IP[64];
extern const int IP_INV[64];
extern const int E[48];
extern const int P[32];
extern const int S[8][4][16];
extern const int SHIFTS[16];

void setBit(unsigned char* data, int pos, int value);
int getBit(const unsigned char* data, int pos);
void permute(const unsigned char* input, unsigned char* output, const int* table, int size);
void leftShift(unsigned char* data, int bits, int len);
void generateSubkeys(const unsigned char* key, unsigned char subkeys[ROUNDS][6]);
void desFunction(const unsigned char* R, const unsigned char* subkey, unsigned char* output);
void desEncryptBlock(const unsigned char* plaintext, const unsigned char (*subkeys)[6], unsigned char* ciphertext);
void desDecryptBlock(const unsigned char* ciphertext, const unsigned char (*subkeys)[6], unsigned char* plaintext);

#endif