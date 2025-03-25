#include "DES.h"
#include <stdio.h>
#include <stdlib.h>
#include <winsock2.h>

#pragma comment(lib, "Ws2_32.lib")
#define BLOCK_SIZE 8
#define PORT 12345

bool decryptData(unsigned char* encryptedData, long encryptedSize, const char* outputFile, unsigned char subkeys[ROUNDS][6]) {
    FILE* outFile = fopen(outputFile, "wb");
    if (!outFile) {
        printf("Khong the mo file %s\n", outputFile);
        return false;
    }

    long originalSize;
    memcpy(&originalSize, encryptedData, sizeof(long));
    unsigned char* dataPtr = encryptedData + sizeof(long);

    unsigned char buffer[BLOCK_SIZE];
    unsigned char output[BLOCK_SIZE];
    long bytesProcessed = 0;

    while (bytesProcessed + BLOCK_SIZE <= encryptedSize - sizeof(long)) {
        memcpy(buffer, dataPtr, BLOCK_SIZE);
        desDecryptBlock(buffer, subkeys, output);
        bytesProcessed += BLOCK_SIZE;
        dataPtr += BLOCK_SIZE;

        if (bytesProcessed <= originalSize) {
            fwrite(output, 1, BLOCK_SIZE, outFile);
        } else {
            long remaining = originalSize - (bytesProcessed - BLOCK_SIZE);
            if (remaining > 0) {
                fwrite(output, 1, remaining, outFile);
            }
        }
    }

    fclose(outFile);
    return true;
}

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

int main() {
    WSADATA wsaData;
    WSAStartup(MAKEWORD(2, 2), &wsaData);

    unsigned char key[8] = {0x13, 0x34, 0x57, 0x79, 0x9B, 0xBC, 0xDF, 0xF1};
    unsigned char subkeys[ROUNDS][6];
    generateSubkeys(key, subkeys);

    SOCKET sock = socket(AF_INET, SOCK_STREAM, 0);
    struct sockaddr_in server;
    server.sin_family = AF_INET;
    server.sin_port = htons(PORT);
    server.sin_addr.s_addr = inet_addr("192.168.1.100"); // Thay bằng IP của Laptop A

    printf("Dang ket noi den Laptop A...\n");
    if (connect(sock, (struct sockaddr*)&server, sizeof(server)) < 0) {
        printf("Ket noi that bai\n");
        return 1;
    }

    long encryptedSize;
    recv(sock, (char*)&encryptedSize, sizeof(long), 0);
    unsigned char* encryptedData = (unsigned char*)malloc(encryptedSize);
    recv(sock, (char*)encryptedData, encryptedSize, 0);

    const char* decryptedFile = "D:/DES_project/output/decrypted.txt";
    if (decryptData(encryptedData, encryptedSize, decryptedFile, subkeys)) {
        printf("Da nhan va giai ma vao %s\n", decryptedFile);
        printf("Du lieu giai ma: ");
        printFileContent(decryptedFile);
        putchar('\n');
    } else {
        printf("Giai ma that bai\n");
    }

    free(encryptedData);
    closesocket(sock);
    WSACleanup();

    getchar();
    return 0;
}