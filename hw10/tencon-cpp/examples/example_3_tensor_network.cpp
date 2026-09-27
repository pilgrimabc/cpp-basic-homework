/**
 * @example example_03_network.cpp
 * TensorNetwork: automatic contraction of multiple tensors.
 */

 #include <iostream>
 #include "tensor.hpp"
 #include "tensor_network.hpp"
 
 using namespace tencon;
 
 int main() {
     // Create tensors: A_ij * B_jk
     xt::xarray<double> A_data = {{1.0, 2.0}, {3.0, 4.0}};
     Tensor A("A", {"i", "j"}, A_data);
     
     xt::xarray<double> B_data = {{5.0, 6.0}, {7.0, 8.0}};
     Tensor B("B", {"j", "k"}, B_data);
     
     // Create network (automatically finds contracted index 'j')
     TensorNetwork network("AB_network", {A, B});
     
     std::cout << "Network: " << network << std::endl;
     std::cout << "Free indices: ";
     for (const auto& idx : network.free_indices) {
         std::cout << idx << " ";
     }
     std::cout << std::endl;
     
     // Contract entire network
     Tensor result = network.contract();
     std::cout << "Result: " << result << std::endl;
     
     return 0;
 }