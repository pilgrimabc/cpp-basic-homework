#include "tensor_network.hpp"

#include <algorithm>
#include <sstream>
#include <stdexcept>

namespace tencon {

    /*******************************
    * constructors
    *******************************/

    TensorNetwork::TensorNetwork(const std::string& name, const std::vector<Tensor>& tensors_list) : name{name}, tensors_list{tensors_list} {
        // add all tensors to the map 
        for (const auto& tensor : tensors_list) {
            tensors[tensor.name] = tensor;
        }
        
        // collect all links between tensors
        _unpack();
    }

    /*******************************
    * getters
    *******************************/

    const Tensor& TensorNetwork::get_tensor(const std::string& name) const {
        // check map with tensor names and tensors
        auto it = tensors.find(name);
        if (it == tensors.end()) {
            throw std::out_of_range("Tensor '" + name + "' not found in the network");
        }

        return it->second;
    }

    size_t TensorNetwork::num_tensors() const {
        return tensors.size();
    }

    bool TensorNetwork::has_tensor(const std::string& name) const {
        return tensors.find(name) != tensors.end();
    }

    std::vector<std::string> TensorNetwork::get_tensor_names() const {
        // init result
        std::vector<std::string> names;
        names.reserve(tensors.size());

        // add to result
        for (const auto& [name, tensor] : tensors) {
            names.push_back(name);
        }

        return names;
    }

    /*******************************
    * private methods
    *******************************/

    void TensorNetwork::_unpack() {
        // clear previous data
        contracted_indices.clear();
        free_indices.clear();
        index_connections.clear();

        // collect shared indices between all pairs of tensors
        for (size_t i=0; i < tensors_list.size(); i++) {
            // get first tensor
            const auto& tensor_i = tensors_list[i];

            for (size_t j=i+1; j < tensors_list.size(); j++) {
                // get second tensor
                const auto& tensor_j = tensors_list[j];
                
                //collect shared indices
                std::set<IndexType> shared = shared_indices(tensor_i, tensor_j);

                // update contracted_indices
                contracted_indices.insert(shared.begin(), shared.end());

                // remember all links for each tensor
                for (const auto& indx : shared) {
                    index_connections[indx].push_back(tensor_i.name);
                    index_connections[indx].push_back(tensor_j.name);
                }
            }
        }

        // calculate free indices 
        // it appears only one times, therefore we count indices
        std::unordered_map<IndexType, int> index_count;
        for (const auto& tensor : tensors_list) {
            for (const auto& indx : tensor.indices) {
                index_count[indx]++;
            }
        }

        for (const auto& [indx, count] : index_count) {
            if (count == 1) {
                // add index
                free_indices.insert(indx);
            }
        }

        // update information about each tensor
        for (auto& tensor : tensors_list) {
            // clear previous info
            tensor.contraction_indices_tensors.clear();

            // add connections
            for (const auto& indx : tensor.indices) {
                if (contracted_indices.find(indx) != contracted_indices.end()) {
                    // find connected tensor
                    const auto& connections = index_connections[indx];
                    for (const auto& other_name : connections) {
                        if (other_name != tensor.name) {
                            tensor.contraction_indices_tensors[indx] = other_name;
                            break;
                        }
                    }
                }
            }

            // update free indices
            tensor.free_indices.clear();
            for (const auto& indx : tensor.indices) {
                if (free_indices.find(indx) != free_indices.end()) {
                    tensor.free_indices.push_back(indx);
                }
            }
        }
    }

    /*******************************
    * public methods
    *******************************/

    Tensor TensorNetwork::contract() const {
        // check that there are tensors
        if (tensors_list.empty()) {
            throw std::runtime_error("Cannot contract empty tensor network");
        }
        // if there is a one tensor
        if (tensors_list.size() == 1) {
            return tensors_list[0];
        }

        // inm other cases just use a contraction from contraction.hpp
        return ::tencon::contraction(tensors_list);
    }

    void TensorNetwork::add_tensor(const Tensor& tensor) {
        // add tensor in list and dict
        tensors_list.push_back(tensor);
        tensors[tensor.name] = tensor;

        // update all connections in the network
        _unpack();
    }

    bool TensorNetwork::validate() const {
        // check that all indices with the same name have the same dimension
        std::unordered_map<IndexType, size_t> index_dims;

        for (const auto& tensor : tensors_list) {
            for (const auto& indx : tensor.indices) {
                // get dim of the index
                size_t dim = tensor.dim(indx);

                // if in the previous steps we met the index, than check it
                auto it = index_dims.find(indx);
                if (it != index_dims.end()) {
                    // check dim!!
                    if (it->second != dim) {
                        std::cerr << "WRNING: index '" << indx << "' has mismatched dimensions: '"
                                  << it->second << " vs " << dim << std::endl; 
                    }
                } else {
                    index_dims[indx] = dim;
                }
            }
        }

        return true;
    }

    std::string TensorNetwork::repr() const {
        std::ostringstream oss;
        oss << "TensorNetwork(" << name 
            << ", " << num_tensors() << " tensors"
            << ", free_indices=[";
        
        bool first = true;
        for (const auto& indx : free_indices) {
            if (!first) oss << ", ";
            oss << indx;
            first = false;
        }
        
        oss << "], contracted_indices=[";
        first = true;
        for (const auto& indx : contracted_indices) {
            if (!first) oss << ", ";
            oss << indx;
            first = false;
        }
        
        oss << "])";
        return oss.str();
    }

    std::ostream& operator<<(std::ostream& os, const TensorNetwork& tn) {
        os << tn.repr();
        return os;
    }    
}