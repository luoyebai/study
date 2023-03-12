#include "myfmt.hpp"
#include <cmath>
#include <cstddef>
#include <initializer_list>
#include <iostream>
#include <vector>

using namespace my;

template <typename T> struct ZData {
    T data;
    T sigma;
};

template <typename T> ZData<T> DataFusion(std::vector<ZData<T>> datas) {

    double k;
    ZData<T> result = ZData<T>();
    ZData<T> pre_data = datas[0];

    for (auto &i : datas) {

        k = pow(pre_data.sigma, 2) / (pow(pre_data.sigma, 2) + pow(i.sigma, 2));

        result.data = pre_data.data + k * (i.data - pre_data.data);

        result.sigma =
            sqrt(pow((1.0 - k) * pre_data.sigma, 2) + pow(k * i.sigma, 2));

        pre_data = i;
    }

    return result;
}

int main() {

    std::vector<ZData<double>> datas;
    "Enter the data you need to merge\n"_f();
    for (size_t i = 0;; ++i) {

        double data, sigma;

        "group{}\ndata:"_f(i + 1);

        std::cin >> data;

        if (std::cin.fail()) {
            "Don't input data blindly, and do something normal"_f();
            break;
        }
        if (data == 114514) {
            break;
        }

        "sigma:"_f();

        std::cin >> sigma;

        if (std::cin.fail()) {
            "Don't input data blindly, and do something normal"_f();
            break;
        }
        datas.emplace(datas.end(), ZData<double>{data, sigma});
    }

    auto out = DataFusion<double>(datas);
    "result:\ndata---{}\nsigma---{}\n"_f(out.data, out.sigma);
    return 0;
}
