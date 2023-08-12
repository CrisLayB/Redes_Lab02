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

#include <random>

#define HOST "127.0.0.1"
#define PORT 65432

// Define Methods
std::string hammingCode(std::string bits);
bool verifyPow(int numVerify, int amountNums);
std::string printCode(int* arr, int totalLong);
int numberRandomInRange(int min, int max);
std::string generateMessage(int lenght);
std::string strToBinaryPlot(std::string text);
std::string simulateNoise(std::string message, float probability);
std::string makeProcess(std::string plot);

std::string hammingCode(std::string bits)
{
    int m = bits.length();
    int r = 0;

    std::cout << "\nInput: " << bits << std::endl;

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

    std::cout << "\nOutput: ";
    std::string result = printCode(nums, totalLong);
    // std::cout << "r: " << r << std::endl;
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

int numberRandomInRange(int min, int max)
{
    return (rand() % (max - min + 1)) + min;
}

std::string generateMessage(int length)
{
    std::string message;
    static const char charset[] = "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789";

    std::mt19937 generator(time(0));
    std::uniform_int_distribution<> distribution(0, sizeof(charset) - 2);

    for (int i = 0; i < length; ++i) {
        message += charset[distribution(generator)];
    }

    return message;
}

std::string strToBinaryPlot(std::string text)
{
    std::string binaryPlot;
    
    for (char c : text) 
    {
        std::bitset<8> binaryChar(c);
        binaryPlot += binaryChar.to_string();
    }
    
    return binaryPlot;
}

std::string simulateNoise(std::string text, float probability)
{
    std::string binaryPlotWithNoise = "";
    std::random_device rd;
    std::mt19937 generator(rd());

    float minValue = 0.0;
    float maxValue = 1.0;
    std::uniform_real_distribution<float> distribution(minValue, maxValue);

    for (char c : text)
    {
        float randomFloat = distribution(generator);
        if(randomFloat < probability)
        {
            binaryPlotWithNoise += (c == '0') ? '1' : '0';
            continue;
        }

        binaryPlotWithNoise += c;
    }
    
    return binaryPlotWithNoise;
}

std::string makeProcess(std::string plot)
{
    plot += "\n";

    int status, valread, client_fd;
    struct sockaddr_in serv_addr;    
    char buffer[1024] = { 0 };

    if ((client_fd = socket(AF_INET, SOCK_STREAM, 0)) < 0)
    {
        std::cout << "Error en la creacion del socket" << std::endl;
        close(client_fd);
        return "";
    }

    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(PORT);

    // Convert IPv4 and IPv6 addresses from text to binary form
    if (inet_pton(AF_INET, HOST, &serv_addr.sin_addr) <= 0) 
    {
        std::cout << "Invalid address/ Address not supported" << std::endl;
        close(client_fd);
        return "";
    }
  
    if ((status = connect(client_fd, (struct sockaddr*)&serv_addr, sizeof(serv_addr))) < 0) 
    {
        std::cout << "Connection Failed" << std::endl;
        close(client_fd);
        return "";
    }
        
    // Send the message
    const char* ptrPlot = plot.c_str();
    send(client_fd, ptrPlot, strlen(ptrPlot), 0);
    printf("\n");
    std::cout << "Binary Plot sent" << std::endl;
    valread = read(client_fd, buffer, 1024);

    std::cout << "\nRecived plot from server: " << buffer << std::endl;
    
    // closing the connected socket
    close(client_fd);

    std::string result = buffer;
    result.erase(result.size() - 1);
    return result;
}

int main(int argc, char *argv[])
{   
    int numMessages = 10000;
    int amountMinChars = 10;
    int amountMaxChars = 100;
    std::vector<int> data;

    for(int i = 0; i < numMessages; i++)
    {
        int sizeMessague = numberRandomInRange(amountMinChars, amountMaxChars);
        std::string message = generateMessage(sizeMessague);

        std::cout << "\n==============================================" << std::endl;
        std::cout << "Message: " << message << std::endl;

        std::string binaryPlot = strToBinaryPlot(message);

        std::string plot = hammingCode(binaryPlot);

        std::string plotWithNoise = simulateNoise(plot, 0.01);

        std::cout << "\nOutput: " << plotWithNoise << std::endl;
        
        std::string plotProcessed = makeProcess(plotWithNoise);
        
        std::string result = (plot == plotProcessed)
            ? "CORRECTO"
            : "INCORRECTO";

        std::cout << result << std::endl;

        int num = (plot == plotProcessed) ? 1 : 0;
        data.push_back(num);
    }

    // Save data
    const std::string filename = "data/hammingResults.csv";
    std::ofstream file(filename);
    if (file.is_open()) 
    {
        for(int num : data)
        {
            file << num;
            file << "\n";
        }
        file.close();
        std::cout << "Data saved to " << filename << std::endl;
    } else {
        std::cerr << "Unable to open file: " << filename << std::endl;
    }
    
    return 1;
}