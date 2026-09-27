/**
 * @example example_02_contraction.cpp
 * Tensor contraction: matrix-vector and matrix-matrix multiplication.
 */

 #include <iostream>
 #include "tensor.hpp"
 #include "contraction.hpp"
 
 using namespace tencon;
 
 int main() {
     // Matrix 2x3: M_ij
     xt::xarray<double> M_data = {{1.0, 2.0, 3.0}, {4.0, 5.0, 6.0}};
     Tensor M("M", {"i", "j"}, M_data);
     
     // Vector 3: v_j
     xt::xarray<double> v_data = {1.0, 2.0, 3.0};
     Tensor v("v", {"j"}, v_data);
     
     // Contract: (M * v)_i = sum_j(M_ij * v_j)
     Tensor result = M * v;
     
     std::cout << "Matrix M: " << M << std::endl;
     std::cout << "Vector v: " << v << std::endl;
     std::cout << "M * v = " << result << std::endl;
     std::cout << "Result[0] = " << result(0) << " (expected: 14)" << std::endl;
     std::cout << "Result[1] = " << result(1) << " (expected: 32)" << std::endl;
     
     return 0;
 }