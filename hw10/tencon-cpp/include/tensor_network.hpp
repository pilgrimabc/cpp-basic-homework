#ifndef TENCON_TENSOR_NETWORK_HPP
#define TENCON_TENSOR_NETWORK_HPP

#include "tensor.hpp"
#include "contraction.hpp"

#include <iostream>
#include <string>
#include <vector>
#include <unordered_map>
#include <unordered_set>
#include <set>

namespace tencon {

/**
 * @brief a container for Tensor Network
 * 
 * Automatically defines contracted indices between Tensors by their global indices
 * and free indices
 * 
 * Example:
 * @code
 * Tensor A("A", {"i", "j"}, A_data);
 * Tensor B("B", {"j", "k"}, B_data);
 * 
 * TensorNetwork tnetwork("example_network", {A, B});
 * 
 * Tensor result = tnetwork.contract();
 * @endcode
 */
class TensorNetwork {
public: 
    // types for convience
    using TensorMap = std::unordered_map<std::string, Tensor>;
    using IndexType = Tensor::IndexType;
    using IndexSet = std::set<IndexType>;

    /*******************************
    * public members
    *******************************/

    std::string name; // name of the tensor network
    std::vector<Tensor> tensors_list; // list of tensors to contract
    TensorMap tensors; // dict with {Tensor.name : Tensor}
    IndexSet free_indices; // set with free indices
    IndexSet contracted_indices; // set with contracted indices
    
    /*******************************
    * constructors
    *******************************/

    TensorNetwork() = default;

    /**
     * @brief main constructor
     * @param name -- name of the network
     * @param tensor_list -- list of tensors
     */
    TensorNetwork(const std::string& name, const std::vector<Tensor>& tensors_list); 

    /*******************************
    * getters
    *******************************/

    /**
     * @brief get tensor by name
     * @param name -- name of the tensor
     * @return link to the tensor
     */
    const Tensor& get_tensor(const std::string& name) const;

    /**
     * @brief get number os tensors in the network
     * @return number of tensors
     */
    size_t num_tensors() const;

    /**
     * @brief tensor existence check
     * @param name -- name of the tensor
     * @return true if tensor exists 
     */
    bool has_tensor(const std::string& name) const;

    /**
     * @brief get all tensor names
     * @return vector with names
     */
    std::vector<std::string> get_tensor_names() const;

    /*******************************
    * methods
    *******************************/

    /**
     * @brief contract the whole network
     * @return resulting tensor
     */
    Tensor contract() const;

    /**
     * @brief add tensor to the tensor network
     * @param tensor -- tensor for adding
     */
    void add_tensor(const Tensor& tensor);

    /**
     * @brief tests that network is correct
     * 
     * Checks: 
     * (1) indices have the proper dimension;
     * (2) there is no isolate indices;
     * 
     * @return true if ok
     */
    bool validate() const;

    /** 
     * @brief get string representation
     * @return description of the network
     */
    std::string repr() const;

    /*******************************
    * other
    *******************************/

    //std::cout
    friend std::ostream& operator<<(std::ostream& os, const TensorNetwork& tn);

private:

    /*******************************
    * methods
    *******************************/

    /**
     * @brief collect connections between tensors
     * 
     * In particular it fills: 
     * (1) contracted indices;
     * (2) contraction_indices_tensors for each tensor
     * (3) free indices;
     */
    void _unpack();

    // index connection dict with format {index : [tensor1, tensor2, ...]}
    std::unordered_map<IndexType, std::vector<std::string>> index_connections; 
};

// std::cout 
std::ostream& operator<<(std::ostream& os, const TensorNetwork& tn);
}

#endif