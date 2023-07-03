/**
 * @file data_fusion.cpp
 * @brief one-dimensional kalman filter
 * data fusion
 * @author luoyebai(2112216825@qq.com)
 * @version 1.0
 * @date 2023-03-13
 */
#include "./data_fusion.hpp"
#include "../../myhead_file/myfmt.hpp"
#include <cmath>
#include <iostream>
#include <vector>

using namespace my;
/**
 *
 * @brief
 * @param argc
 * @param argv
 * @return int
 */
int main()
{
    std::vector<ZData<double>> datas;
    "Enter the data you need to merge\n"_f();
    for (size_t i = 0;; ++i)
    {

        double data, sigma;

        "group{}\ndata:"_f(i + 1);

        std::cin >> data;

        if (std::cin.fail())
        {
            "Don't input data blindly, and do something normal"_f();
            break;
        }
        if (data == 114514)
        {
            break;
        }

        "sigma:"_f();

        std::cin >> sigma;

        if (std::cin.fail())
        {
            "Don't input data blindly, and do something normal"_f();
            break;
        }
        if (sigma == 0)
        {
            "result:\ndata---{}\nsigma---{}\n"_f(data, sigma);
            // If sigma is 0, then no fusion is needed,
            // then the data is accurate, no fusion is needed.
            // If sigma is 0, then the weight of this data is infinite,
            return 0;
        }
        datas.emplace(datas.end(), ZData<double>{data, sigma});
    }

    auto out = DataFusion<double>(datas);

    "result:\ndata---{}\nsigma---{}\n"_f(out.data, out.sigma);
    return 0;
}
