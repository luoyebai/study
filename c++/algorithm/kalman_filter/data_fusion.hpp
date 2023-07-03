#ifndef DATA_FUSION_HPP_
#define DATA_FUSION_HPP_

#include <cmath>
#include <vector>
/**
 * @brief Information of ZData
 *  data and variance
 *
 * @tparam T    data type
 */
template <typename T>
struct ZData
{
    T data;
    T sigma;
};

/**
 * @brief   Multi-group data fusion
 *  to get the best data
 *
 * @tparam T    data type
 * @param datas all data
 * @return best data---result
 */
template <typename T>
ZData<T> DataFusion(std::vector<ZData<T>> datas)
{

    double k;
    ZData<T> result = ZData<T>();
    ZData<T> pre_data = datas[0];
    bool is_frist = true;
    for (auto &i : datas)
    {
        if (is_frist)
        {
            is_frist = false;
            continue;
        }

        k = pow(pre_data.sigma, 2) / (pow(pre_data.sigma, 2) + pow(i.sigma, 2));

        result.data = pre_data.data + k * (i.data - pre_data.data);

        result.sigma =
            sqrt(pow((1.0 - k) * pre_data.sigma, 2) + pow(k * i.sigma, 2));

        pre_data = result;
    }

    return result;
}

#endif // DATA_FUSION_HPP_
