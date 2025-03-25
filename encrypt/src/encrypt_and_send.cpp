#include "DES.h"
#include <stdio.h>
#include <stdlib.h>
#include <winsock2.h>

#pragma comment(lib, "Ws2_32.lib")
#define BLOCK_SIZE 8
#define PORT 12345

bool encryptFile(const char* inputFile, unsigned char** encryptedData, long* encryptedSize, unsigned char subkeys[ROUNDS][6]) {
    FILE* inFile = fopen(inputFile, "rb");
    if (!inFile) {
        printf("Khong the mo file %s\n", inputFile);
        return false;
    }

    fseek(inFile, 0, SEEK_END);
    long originalSize = ftell(inFile);
    fseek(inFile, 0, SEEK_SET);

    int numBlocks = (originalSize + BLOCK_SIZE - 1) / BLOCK_SIZE + (originalSize % BLOCK_SIZE == 0 ? 1 : 0);
    *encryptedSize = sizeof(long) + numBlocks * BLOCK_SIZE;
    *encryptedData = (unsigned char*)malloc(*encryptedSize);

    memcpy(*encryptedData, &originalSize, sizeof(long));
    unsigned char* dataPtr = *encryptedData + sizeof(long);

    unsigned char buffer[BLOCK_SIZE];
    unsigned char output[BLOCK_SIZE];
    size_t bytesRead;

    while ((bytesRead = fread(buffer, 1, BLOCK_SIZE, inFile)) > 0) {
        if (bytesRead < BLOCK_SIZE) {
            int padding = BLOCK_SIZE - bytesRead;
            for (size_t i = bytesRead; i < BLOCK_SIZE; i++) {
                buffer[i] = (unsigned char)padding;
            }
            bytesRead = BLOCK_SIZE;
        }
        desEncryptBlock(buffer, subkeys, dataPtr);
        dataPtr += BLOCK_SIZE;
    }

    if (originalSize % BLOCK_SIZE == 0) {
        for (int i = 0; i < BLOCK_SIZE; i++) {
            buffer[i] = BLOCK_SIZE;
        }
        desEncryptBlock(buffer, subkeys, dataPtr);
    }

    fclose(inFile);
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

    const char* inputFile = "D:/Nam_4/ky2/dot1/CoSoAnToan/DES_project/input/sample.txt";
    unsigned char* encryptedData;
    long encryptedSize;

    printf("Du lieu goc: ");
    printFileContent(inputFile);
    putchar('\n');

    if (!encryptFile(inputFile, &encryptedData, &encryptedSize, subkeys)) {
        return 1;
    }

    SOCKET server_fd = socket(AF_INET, SOCK_STREAM, 0);
    struct sockaddr_in address;
    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = htons(PORT);

    bind(server_fd, (struct sockaddr*)&address, sizeof(address));
    listen(server_fd, 3);

    printf("Dang cho ket noi tu Laptop B...\n");
    SOCKET client_fd = accept(server_fd, NULL, NULL);

    send(client_fd, (char*)&encryptedSize, sizeof(long), 0);
    send(client_fd, (char*)encryptedData, encryptedSize, 0);

    printf("Da gui file ma hoa sang Laptop B\n");

    free(encryptedData);
    closesocket(client_fd);
    closesocket(server_fd);
    WSACleanup();

    getchar();
    return 0;
}