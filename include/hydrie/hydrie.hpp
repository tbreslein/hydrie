#include <Eigen/Dense>
#include <iostream>
#include <print>

using Eigen::MatrixXd;

namespace hydrie {

auto run() -> int {
    std::println("Hello from Hydrie");
    MatrixXd m(2, 2);
    m(0, 0) = 3;
    m(1, 0) = 2.5;
    m(0, 1) = -1;
    m(1, 1) = m(1, 0) + m(0, 1);
    std::cout << m << std::endl;
    return 0;
}

} // namespace hydrie
