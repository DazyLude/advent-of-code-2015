#include <cmath>
#include <iostream>

int presents(int n, int x = 1)
{
    if (x * x >= n) 
    {
        return x * x == n? x * 10 : 0;
    }
    return n % x == 0? x * 10 + n / x * 10 + presents(n, x + 1) : presents(n, x + 1);
}

int lazy_presents(int n, int x = 1)
{
    if (x * x >= n) 
    {
        return (50 >= x && x * x == n) ? x : 0;
    }
    return n % x == 0 ? ((50 * x >= n) ? x : 0) + ((50 >= x) ? n / x : 0) + lazy_presents(n, x + 1) : lazy_presents(n, x + 1);
}

int main()
{
    int r {0};
    int n {1};
    int puzzle_input {29000000};
    while (r < puzzle_input)
    {
        r = lazy_presents(n) * 11;
        // std::cout << "House " << n << " got " << r << " presents." << std::endl;
        ++n;
    }
    std::cout << "House " << --n << " got " << r << " presents." << std::endl;
    return EXIT_SUCCESS;
}