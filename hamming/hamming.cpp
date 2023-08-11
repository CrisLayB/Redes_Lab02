/*************************************************************
 * Universidad del Valle de Guatemala
 * Redes - Sección 10
 * Laboratorio 2
 * 
 * C++ (Emisor)
 * 
 * Autores: [
 *      Cristian Laynez,
 *      Mario de León
 * ]
************************************************************/

#include <iostream>
#include <bits/stdc++.h>

#include <netdb.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <pthread.h>
#include <ifaddrs.h>
#include <unistd.h>

#define HOST "127.0.0.1"
#define PORT 65432

std::string hammingCode(std::string bits);
bool verifyPow(int numVerify, int amountNums);
std::string printCode(int* arr, int totalLong);

std::string hammingCode(std::string bits)
{
    int m = bits.length();
    int r = 0;

    std::cout << "Input: " << bits << std::endl;

    // 2^r >= m + r + 1
    while(pow(2, r) < (m + r + 1)) r++;

    int totalLong = m + r;
    int nums[totalLong];

    // Define bits in the arrayCode
    int counter = 0;
    for(int i = totalLong - 1; i > -1; i--)
    {
        if(verifyPow(i + 1, r)) nums[i] = -1;

        else if(counter < m) 
        {
            int num = (int)bits[counter];
            if(num == 49) num = 1;
            else if(num == 48) num = 0;
            nums[i] = num;
            counter++;
        }
    }

    // Calculate Parity
    for(int i = 0; i < r; i++)
    {
        int exp = pow(2, i);
        int controller = 0, countOnes = 0;
        bool isOne = false;

        for(int j = 0; j < totalLong; j++) // check amount ones
        {
            controller++;
            if(controller >= exp){ // Make Swap
                isOne = !isOne;
                controller = 0;
            }

            if(nums[j] == 1 && isOne) countOnes++;
        }

        nums[exp - 1] = (countOnes % 2 == 0) ? 0 : 1;
    }

    // Flip values
    for (int i = 0; i < totalLong / 2; ++i) 
    {
        int temp = nums[i];
        nums[i] = nums[totalLong - 1 - i];
        nums[totalLong - 1 - i] = temp;
    }

    std::cout << "Output: ";
    std::string result = printCode(nums, totalLong);
    std::cout << "r: " << r << std::endl;
    return result;
}

bool verifyPow(int numVerify, int amountNums)
{
    int n = amountNums - 1;
    while(n >= 0)
    {
        if(pow(2, n) == numVerify) return true;
        n--;
    }

    return false;
}

std::string printCode(int* arr, int totalLong)
{
    std::string str = "";

    for(int i = 0; i < totalLong; i++)
    {
        std::cout << arr[i];
        str += std::to_string(arr[i]);
    }

    std::cout << std::endl;
    return str;
}

int main(int argc, char *argv[])
{
    if(argc != 2)
    {
        std::cout << "No ingresaste el trama de bits esperado" << std::endl;
        std::cout << "\nEjemplo de input esperado:" << std::endl;
        std::cout << "./hamming/hamming 1011001" << std::endl;
        return 0;
    }

    int status, valread, client_fd;
    struct sockaddr_in serv_addr;    
    char buffer[1024] = { 0 };

    if ((client_fd = socket(AF_INET, SOCK_STREAM, 0)) < 0)
    {
        std::cout << "Error en la creacion del socket" << std::endl;
        return -1;
    }

    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(PORT);

    // Convert IPv4 and IPv6 addresses from text to binary
    // form
    if (inet_pton(AF_INET, HOST, &serv_addr.sin_addr) <= 0) 
    {
        std::cout << "Invalid address/ Address not supported" << std::endl;
        return -1;
    }
  
    if ((status = connect(client_fd, (struct sockaddr*)&serv_addr, sizeof(serv_addr))) < 0) 
    {
        std::cout << "Connection Failed" << std::endl;
        return -1;
    }
    
    char *plotBits = argv[1];
    std::string plot = hammingCode(plotBits);
    plot += "\n";

    // Put noise in the binary plot

    const char* ptrPlot = plot.c_str();
    send(client_fd, ptrPlot, strlen(ptrPlot), 0);
    printf("\n");
    std::cout << "Binary Plot sent" << std::endl;
    valread = read(client_fd, buffer, 1024);

    std::cout << "\nRecived plot from server: " << buffer << std::endl;

    // See if is the correct binary plot
    std::string result = (plot == buffer) ? "CORRECTO" : "INCORRECTO";

    std::cout << result << std::endl;
  
    // closing the connected socket
    close(client_fd);
    return 1;
}