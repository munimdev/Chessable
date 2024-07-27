#include "common/pgn_utils.h"

#include <algorithm>

int extractMaximum(const std::string &str)
{
    int num = 0;
    int result = 0;
    for (std::size_t index = 0; index < str.size(); ++index)
    {
        if (str[index] >= '0' && str[index] <= '9')
        {
            num = num * 10 + static_cast<int>(str[index] - '0');
        }
        else if (str[index] == '.')
        {
            result = std::max(result, num);
            num = 0;
        }
        else
        {
            num = 0;
        }
        if (index == (str.size() - 1))
        {
            num = 0;
        }
    }
    return std::max(result, num);
}
