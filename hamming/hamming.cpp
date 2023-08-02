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

void hammingCode(std::string bits);
bool verifyPow(int numVerify, int amountNums);
void printCode(int* arr, int totalLong);

void hammingCode(std::string bits)
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
    for (int i = 0; i < totalLong / 2; ++i) {
        int temp = nums[i];
        nums[i] = nums[totalLong - 1 - i];
        nums[totalLong - 1 - i] = temp;
    }

    std::cout << "Output: ";
    printCode(nums, totalLong);
    std::cout << "r: " << r << std::endl;
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

void printCode(int* arr, int totalLong)
{
    for(int i = 0; i < totalLong; i++)
    {
        std::cout << arr[i];
    }

    std::cout << std::endl;
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

    // 1001 // 0111011 // 1001
    char *plotBits = argv[1];
    hammingCode(plotBits);
    return 1;
}