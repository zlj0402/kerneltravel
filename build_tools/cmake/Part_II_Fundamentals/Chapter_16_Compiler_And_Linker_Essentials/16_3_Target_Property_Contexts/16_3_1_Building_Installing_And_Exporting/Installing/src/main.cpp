#include "Algo.h"
#include "Compute.h"  // 显式包含：因为 Compute 是 Algo 的 PUBLIC 依赖，MyApp 可以访问
// #include "AlgoImpl.h"  // 错误！AlgoImpl 是 Algo 的 PRIVATE 依赖，MyApp 看不到
#include <iostream>

int main() {
    std::cout << "MyApp: starting..." << std::endl;

    algo_run();  // 使用 Algo

    // 因为 Compute 是 Algo 的 PUBLIC 依赖，MyApp 也能直接使用 Compute
    compute_process();

    // 如果尝试使用 AlgoImpl，会链接失败（找不到符号）
    // algo_impl_do_something();  // 错误！

    std::cout << "MyApp: done" << std::endl;
    return 0;
}
