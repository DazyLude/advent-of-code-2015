#include <vector>
#include <array>
#include <iostream>
#include <istream>
#include <fstream>
#include <chrono>
#include <thread>
#include "src/parsers.h"

using std::vector;

class Field 
{
    vector<vector<int>> field;
    int size {100};
public:
    Field(int s) 
    {
        size = s;
        vector<int> row(size, 0);
        field = vector<vector<int>>(size, row);
    }
    Field() : Field(100) {}
    ~Field() {}

    int check_cell(int x, int y)
    {
        if (x < 0 || y < 0 || x >= size || y >= size)
        {
            return 0;
        }
        return field[y][x];
    }

    int check_neighbours(int x, int y)
    {
        int count {0};
        for (int dx = -1; dx <= 1; ++dx)
        {
            for (int dy = -1; dy <= 1; ++dy)
            {
                count += check_cell(x+dx, y+dy);
            }
        }
        return count - check_cell(x, y);
    }

    void next_step()
    {
        vector<vector<int>> temp_vector{};
        for (int iy {0}; iy < size; ++iy)
        {
            temp_vector.push_back({});
            for (int ix {0}; ix < size; ++ix)
            {
                int new_val;
                if (field[iy][ix] == 0) 
                {
                    new_val = check_neighbours(ix, iy) == 3 ? 1 : 0;
                } 
                else
                {
                    auto neighb = check_neighbours(ix, iy);
                    new_val = (neighb == 3 || neighb == 2) ? 1 : 0;
                }
                temp_vector[iy].push_back(new_val);
            }
        }
        field = temp_vector;
    }

    void turn_on(int x, int y)
    {
        field.at(y).at(x) = 1;
    }

    void break_corners()
    {
        turn_on(0, 0);
        turn_on(0, size - 1);
        turn_on(size - 1, 0);
        turn_on(size - 1, size - 1);
    }

    void next_step_broken()
    {
        next_step();
        break_corners();
    }

    int count_lights()
    {
        auto count {0};
        for (int ix {0}; ix < size; ++ix)
        {
            for (int iy {0}; iy < size; ++iy)
            {
                count += field[iy][ix];
            }
        }
        return count;
    }

    friend std::istream& operator>> (std::istream& is, Field& f);
    friend std::ostream& operator<< (std::ostream& os, const Field& f);
};

std::istream& operator>> (std::istream& is, Field& f)
{
    int y_count {0};
    for (auto i {0}; i < f.size; ++i) 
    {
        vector<char> line(100, ' ');
        is.getline(&line[0], f.size + 1, '\n');
        if (line.size() > f.size) std::cerr << "line larger than the field\n";
        int x_count {0};
        
        for (auto light : line)
        {
            f.field[y_count][x_count] = (light == '#' ? 1 : 0);
            ++x_count;
        }
        ++y_count;
    }
    return is;
}

std::ostream& operator<< (std::ostream& os, const Field& f)
{
    for (auto row : f.field)
    {
        for (auto light : row)
        {
            os << (light == 0 ? '.' : '#');
        }
        os << std::endl;
    }
    return os;
}



int main()
{
    std::ifstream ifs;
    FileParsers::open_input(ifs);
    Field backyard {};
    ifs >> backyard;
    backyard.break_corners();
    for (auto i {0}; i < 100; ++i)
    {
        std::cout << backyard << std::endl << std::endl << std::endl;
        backyard.next_step_broken();
        std::this_thread::sleep_for(std::chrono::seconds(1));
    }
    std::cout << backyard;
    std::cout << backyard.count_lights() << std::endl;
}