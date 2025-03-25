#include "DES.h"
#include <stdio.h>
#include <stdlib.h>

#define BLOCK_SIZE 8 // Kích thước block của DES (8 byte)

// Mã hóa file và lưu kích thước dữ liệu gốc
bool encryptFile(const char* inputFile, const char* outputFile, unsigned char subkeys[ROUNDS][6]) {
    FILE* inFile = fopen(inputFile, "rb");
    if (!inFile) {
        printf("Khong the mo file %s\n", inputFile);
        return false;
    }

    FILE* outFile = fopen(outputFile, "wb");
    if (!outFile) {
        printf("Khong the mo file %s\n", outputFile);
        fclose(inFile);
        return false;
    }

    // Lấy kích thước file gốc
    fseek(inFile, 0, SEEK_END);
    long originalSize = ftell(inFile);
    fseek(inFile, 0, SEEK_SET);

    // Ghi kích thước gốc (4 byte) vào đầu file mã hóa
    fwrite(&originalSize, sizeof(long), 1, outFile);

    unsigned char buffer[BLOCK_SIZE];
    unsigned char output[BLOCK_SIZE];
    size_t bytesRead;

    // Đọc và mã hóa từng block
    while ((bytesRead = fread(buffer, 1, BLOCK_SIZE, inFile)) > 0) {
        if (bytesRead < BLOCK_SIZE) {
            int padding = BLOCK_SIZE - bytesRead;
            for (size_t i = bytesRead; i < BLOCK_SIZE; i++) {
                buffer[i] = (unsigned char)padding; // Padding PKCS5/PKCS7
            }
            bytesRead = BLOCK_SIZE;
        }
        desEncryptBlock(buffer, subkeys, output);
        fwrite(output, 1, BLOCK_SIZE, outFile);
    }

    // Thêm block padding nếu file chia hết cho 8
    if (originalSize % BLOCK_SIZE == 0) {
        for (int i = 0; i < BLOCK_SIZE; i++) {
            buffer[i] = BLOCK_SIZE;
        }
        desEncryptBlock(buffer, subkeys, output);
        fwrite(output, 1, BLOCK_SIZE, outFile);
    }

    fclose(inFile);
    fclose(outFile);
    return true;
}

// Giải mã file, dùng kích thước gốc để bỏ padding
bool decryptFile(const char* inputFile, const char* outputFile, unsigned char subkeys[ROUNDS][6]) {
    FILE* inFile = fopen(inputFile, "rb");
    if (!inFile) {
        printf("Khong the mo file %s\n", inputFile);
        return false;
    }

    FILE* outFile = fopen(outputFile, "wb");
    if (!outFile) {
        printf("Khong the mo file %s\n", outputFile);
        fclose(inFile);
        return false;
    }

    // Đọc kích thước gốc từ 4 byte đầu
    long originalSize;
    fread(&originalSize, sizeof(long), 1, inFile);

    unsigned char buffer[BLOCK_SIZE];
    unsigned char output[BLOCK_SIZE];
    size_t bytesRead;
    long bytesProcessed = 0;

    // Đọc và giải mã từng block
    while ((bytesRead = fread(buffer, 1, BLOCK_SIZE, inFile)) == BLOCK_SIZE) {
        desDecryptBlock(buffer, subkeys, output);
        bytesProcessed += BLOCK_SIZE;

        // Chỉ ghi phần dữ liệu gốc, bỏ padding
        if (bytesProcessed <= originalSize) {
            fwrite(output, 1, BLOCK_SIZE, outFile);
        } else {
            long remaining = originalSize - (bytesProcessed - BLOCK_SIZE);
            if (remaining > 0) {
                fwrite(output, 1, remaining, outFile);
            }
        }
    }

    fclose(inFile);
    fclose(outFile);
    return true;
}

// In nội dung file gốc
void printFileContent(const char* filename) {
    FILE* file = fopen(filename, "rb");
    if (!file) {
        printf("Khong the mo file %s\n", filename);
        return;
    }

    fseek(file, 0, SEEK_END);
    long size = ftell(file);
    fseek(file, 0, SEEK_SET);

    unsigned char* buffer = (unsigned char*)malloc(size);
    if (!buffer) {
        printf("Khong the cap phat bo nho\n");
        fclose(file);
        return;
    }

    fread(buffer, 1, size, file);
    fclose(file);

    for (int i = 0; i < size; i++) {
        if (buffer[i] >= 32 && buffer[i] <= 126) {
            putchar(buffer[i]);
        } else {
            putchar('.');
        }
    }
    free(buffer);
}

// In nội dung file giải mã, bỏ 4 byte đầu (kích thước)
void printDecryptedContent(const char* filename) {
    FILE* file = fopen(filename, "rb");
    if (!file) {
        printf("Khong the mo file %s\n", filename);
        return;
    }

    // Bỏ qua 4 byte đầu (kích thước gốc)
    fseek(file, sizeof(long), SEEK_SET);
    long size = ftell(file);
    fseek(file, sizeof(long), SEEK_SET);

    unsigned char* buffer = (unsigned char*)malloc(size);
    if (!buffer) {
        printf("Khong the cap phat bo nho\n");
        fclose(file);
        return;
    }

    size_t bytesRead = fread(buffer, 1, size, file);
    fclose(file);

    for (size_t i = 0; i < bytesRead; i++) {
        if (buffer[i] >= 32 && buffer[i] <= 126) {
            putchar(buffer[i]);
        } else {
            putchar('.');
        }
    }
    free(buffer);
}

int main() {
    unsigned char key[8] = {0x13, 0x34, 0x57, 0x79, 0x9B, 0xBC, 0xDF, 0xF1};
    unsigned char subkeys[ROUNDS][6];
    generateSubkeys(key, subkeys);

    const char* inputFile = "D:/Nam_4/ky2/dot1/CoSoAnToan/DES_project/input/sample.txt";
    const char* encryptedFile = "D:/Nam_4/ky2/dot1/CoSoAnToan/DES_project/output/encrypted.txt";
    const char* decryptedFile = "D:/Nam_4/ky2/dot1/CoSoAnToan/DES_project/output/decrypted.txt";

    printf("Du lieu goc: ");
    printFileContent(inputFile);
    putchar('\n');

    if (encryptFile(inputFile, encryptedFile, subkeys)) {
        printf("Du lieu ma hoa da duoc ghi vao encrypted.txt\n");
    } else {
        printf("Ma hoa that bai\n");
        return 1;
    }

    if (decryptFile(encryptedFile, decryptedFile, subkeys)) {
        printf("Du lieu giai ma da duoc ghi vao decrypted.txt\n");
    } else {
        printf("Giai ma that bai\n");
        return 1;
    }

    printf("Du lieu giai ma: ");
    printFileContent(decryptedFile);
    putchar('\n');

    getchar();
    return 0;
}