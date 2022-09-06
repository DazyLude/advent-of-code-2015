#include <vector>
#include <iostream>

long long int calc_index(int x, int y)
{
    return (x + y - 2) * (x + y - 1) / 2 + y - 1;
}

long long int calc_next(long long int prev)
{
    return (prev * 252533) % 33554393;
}

int main()
{
    long long int prev {20151125};
    int index = calc_index(3010, 3019);
    for (int i = 0; i < index; ++i)
    {
        prev = calc_next(prev);
    }
    std::cout << prev;
}