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