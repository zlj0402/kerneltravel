#include "Algo.h"
#include "AlgoImpl.h"  // PRIVATE 依赖：只有 Algo 自己能看到 AlgoImpl
#include "Compute.h"
#include <iostream>

void algo_run() {
    std::cout << "Algo: running..." << std::endl;
    algo_impl_do_something();  // 调用内部实现
    compute_process();          // 调用 PUBLIC 依赖
}
