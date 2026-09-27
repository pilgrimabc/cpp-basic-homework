/**
 * @example example_01_tensor.cpp
 * Basic Tensor operations: creation, transpose, reshape, rename.
 */

 #include <iostream>
 #include "tensor.hpp"
 
 using namespace tencon;
 
 int main() {
     // Create 2x2 tensor: A_ij
     xt::xarray<double> data = {{1.0, 2.0}, {3.0, 4.0}};
     Tensor A("A", {"i", "j"}, data);
     
     std::cout << "Original: " << A << std::endl;
     
     // Transpose: A_ji
     A.transpose({"j", "i"});
     std::cout << "After transpose: " << A << std::endl;
     
     // Access element
     std::cout << "A(0,1) = " << A(0, 1) << std::endl;
     
     return 0;
 }