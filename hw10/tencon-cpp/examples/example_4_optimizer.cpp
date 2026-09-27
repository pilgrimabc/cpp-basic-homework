/**
 * @example example_04_optimizer.cpp
 * GreedyOptimizer: find optimal contraction order.
 */

 #include <iostream>
 #include "tensor.hpp"
 #include "optimizer.hpp"
 
 using namespace tencon;
 
 int main() {
     // Create chain: A_ij * B_jk * C_kl
     xt::xarray<double> A_data = xt::ones<double>({10, 20});
     Tensor A("A", {"i", "j"}, A_data);
     
     xt::xarray<double> B_data = xt::ones<double>({20, 30});
     Tensor B("B", {"j", "k"}, B_data);
     
     xt::xarray<double> C_data = xt::ones<double>({30, 5});
     Tensor C("C", {"k", "l"}, C_data);
     
     // Optimize contraction order (minimize FLOPs)
     GreedyOptimizer optimizer(GreedyOptimizer::Metric::FLOPS);
     ContractionTree tree = optimizer.optimize({A, B, C});
     
     std::cout << "Contraction tree:" << std::endl;
     std::cout << tree << std::endl;
     std::cout << "Total FLOPs: " << tree.total_flops() << std::endl;
     std::cout << "Max memory: " << tree.max_memory() << " bytes" << std::endl;
     
     return 0;
 }