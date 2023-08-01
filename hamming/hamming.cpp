// Referencia Algoritmos
// https://html.rincondelvago.com/codigo-de-hamming.html
// https://chovis2.tripod.com/CODIGO_HAMMING.HTM
// https://www.youtube.com/watch?v=ay9LRC4IdpM
// https://www.youtube.com/watch?v=3Td-aqtD14M
// https://www.geeksforgeeks.org/hamming-code-in-computer-network/

// Comando para ejecutar:
// g++ hamming/hamming.cpp -o hamming/hamming
// ./hamming/hamming

#include <iostream>
#include <bits/stdc++.h>

void hammingCode(std::string bits);
bool verifyPow(int numVerify, int amountNums);

// 2^m >= m + r + 1
void hammingCode(std::string bits)
{
    int m = bits.length();
    int r = 0;

    // 2^r >= m + r + 1
    while(pow(2, r) < (m + r + 1)) r++;

    std::cout << "m: " << m << std::endl;
    std::cout << "r: " << r << std::endl;

    int totalLong = m + r;

    std::cout << "Total Long: " << totalLong << std::endl;

    int nums[totalLong];

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

    for(int t = 0; t < r; t++)
    {
        int exp = pow(2, t);
        std::cout << "=> " << t << " exp: " << exp << std::endl;
        for(int i = 1; i < totalLong; i++)
        {
            if (((i >> t) & 1) == 1)
            {
                if (exp != i) nums[exp] = nums[exp] ^ nums[i];
            }
            std::cout << i << std::endl;
            // std::cout << i << " : " << nums[i] << std::endl;
        }
    }

    for(int i = 0; i < totalLong; i++)
    {
        std::cout << i << " : " << nums[i] << std::endl;
    }
    
    return;
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

int main()
{
    // verifyPow(9, 4);
    // verifyPow(8, 4);
    // 1001 = 4
    hammingCode("1011001");
    // hammingCode("0111011");
    // hammingCode("1001");

    return 1;
}