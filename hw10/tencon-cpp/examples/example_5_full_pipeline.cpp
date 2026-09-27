/**
 * @example example_05_full_pipeline.cpp
 * Full pipeline: Tensor → Network → Optimizer → Contract.
 */

 #include <iostream>
 #include "tensor.hpp"
 #include "tensor_network.hpp"
 #include "optimizer.hpp"
 
 using namespace tencon;
 
 int main() {
     std::cout << "###### Full Tensor Network Pipeline ######" << std::endl;
     
     // Step 1: Create tensors for quantum circuit simulation
     // Gate matrix: G_ij (2x2)
     xt::xarray<double> G_data = {{1.0, 0.0}, {0.0, 1.0}};
     Tensor G("Gate", {"i", "j"}, G_data);
     
     // Input state: psi_i (2)
     xt::xarray<double> psi_data = {1.0, 0.0};
     Tensor psi("Psi", {"i"}, psi_data);
     
     // Step 2: Create network
     TensorNetwork network("quantum_circuit", {G, psi});
     std::cout << "\n1. Network created: " << network << std::endl;
     
     // Step 3: Validate
     if (network.validate()) {
         std::cout << "2. Network validation: PASSED" << std::endl;
     }
     
     // Step 4: Optimize contraction order
     GreedyOptimizer optimizer(GreedyOptimizer::Metric::FLOPS);
     ContractionTree tree = optimizer.optimize(network);
     std::cout << "\n3. Contraction tree:" << std::endl;
     std::cout << tree << std::endl;
     
     // Step 5: Contract using optimized tree
     std::unordered_map<std::string, Tensor> tensor_map = {
         {"Gate", G}, {"Psi", psi}
     };
     Tensor result = tree.contract(tensor_map);
     
     std::cout << "\n4. Result: " << result << std::endl;
     std::cout << "5. Result shape: [" << result.shape()[0] << "]" << std::endl;
     
     // Step 6: Compare with naive contraction
     Tensor naive = network.contract();
     std::cout << "\n6. Naive contraction: " << naive << std::endl;
     std::cout << "7. Results match: " << (result.shape() == naive.shape() ? "YES" : "NO") << std::endl;
     
     return 0;
 }