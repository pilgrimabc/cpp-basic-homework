#include <algorithm>
#include <sstream>
#include <stdexcept>

#include "tensor.hpp"

namespace tencon {
    /*******************************
    * constructors
    *******************************/

    Tensor::Tensor(const std::string& name, const std::vector<IndexType>& indices, const DataType& data)
    : name{name}, indices{indices}, data{data} {
        // check that num of indices is equal to the dimension
        if (indices.size() != data.dimension()) {
            throw std::invalid_argument(
                "Number of indices (" + std::to_string(indices.size()) + 
                + ") must be equal to the tensor dimension (" + std::to_string(data.dimension()) + ")"
            );
        }

        // make a dict with global <--> local indices correspondence
        for (size_t i=0; i < indices.size(); i++) {
            global_local_indices[indices[i]] = i;
        }
    }

    /*******************************
    * getters
    *******************************/

    Tensor::ShapeType Tensor::shape() const {
        return ShapeType(data.shape().begin(), data.shape().end());
    }

    size_t Tensor::rank() const {
        return indices.size();
    }

    size_t Tensor::size() const {
        return data.size(); 
    }

    /*
    bool Tensor::empty() const {
        return data.
    }
    */

    bool Tensor::has_index(const IndexType& indx) const {
        // get iterator and if we do not have such index return false
        return global_local_indices.find(indx) != global_local_indices.end();
    }

    size_t Tensor::dim(const IndexType& indx) const {
        // same as in Tensor::has_index
        auto it = global_local_indices.find(indx); 
        if (it == global_local_indices.end()) {
            throw std::out_of_range("Index " + indx + " not found");
        }
        // if there is such index return value from xt:array shape vector
        return data.shape()[it->second];
    }

    size_t Tensor::axis(const IndexType& indx) const {
        // get iterator
        auto it = global_local_indices.find(indx);
        if (it == global_local_indices.end()) {
            throw std::out_of_range("Index " + indx + " not found");
        }
        // get value of the dict
        return it->second;
    }

    /*******************************
    * setters
    *******************************/

    void Tensor::transpose(const std::vector<IndexType>& axes) {
        // check number of axes
        if (axes.size() != indices.size()) {
            throw std::invalid_argument(
                "Number of axes (" + std::to_string(axes.size()) + 
                ") must match tensor rank (" + std::to_string(indices.size()) + ")"
            );
        }

        // get new local indices
        std::vector<size_t> new_local_indices;
        // reserve num of axes
        new_local_indices.reserve(axes.size());

        for (const auto& global_indx : axes) {
            // get local index iter
            auto it = global_local_indices.find(global_indx);
            if (it == global_local_indices.end()) {
                throw std::invalid_argument("Index " + global_indx + " not found in tensor");
            }
            // add value to the vector of local indices
            new_local_indices.push_back(it->second);
        }

        // transpose data
        data = xt::transpose(data, new_local_indices);

        // update indices
        indices = axes; 

        // update dict with {global_indx:local_indx}
        global_local_indices.clear();
        for (size_t i=0; i < indices.size(); i++) {
            global_local_indices[indices[i]] = i;
        }
    }

    void Tensor::reshape(const ShapeType& new_shape) {
        // IMPORTANT CHECK
        // check that num of elemnts is const after reshaping
        size_t old_size = data.size();
        size_t new_size = 1;
        
        for (auto n : new_shape) {
            new_size *= n;
        }
        // check num of elements
        if (old_size != new_size) {
            throw std::invalid_argument(
                "Reshape mismatch (old num elements doesn't equal to new): " + 
                std::to_string(old_size) + " != " + std::to_string(new_size)
            );
        }

        // change shape by xtensor
        data.reshape(new_shape);
    }

    void Tensor::rename_indices(const std::unordered_map<IndexType, IndexType>& name_dict) {
        // work with name_dict in the format {new_index:old_index}
        // (this is global indices)
        for (const auto& pair : name_dict) {
            // unpack pair from dict
            const auto& new_indx = pair.first;
            const auto& old_indx = pair.second;
            
            // get iter of old_indx
            auto it = global_local_indices.find(old_indx);

            if (it != global_local_indices.end()) {
                // get local index
                size_t local_indx = it->second;

                // update global_local_indices dict
                global_local_indices.erase(it);
                global_local_indices[new_indx] = local_indx;

                // update the indices 
                for (auto& indx : indices) {
                    if (indx == old_indx) {
                        indx = new_indx;
                        break;
                    }
                }
            }
        }
    }

    // contraction will be taken from the contraction.hpp and is not emplemented now
    // Tensor operator*(const Tensor& other) const {};

    std::ostream& operator<<(std::ostream& os, const Tensor& t) {
        os << "Tensor(" << t.name << ", indices=[";
        for (size_t i=0; i < t.indices.size();i++) {
            if (i > 0) {
                os << ", ";
            }
            os << t.indices[i];
        }
        os << "], shape=[";
        for (size_t i=0; i < t.data.dimension(); i++) {
            if (i > 0) {
                os << ", ";
            }
            os << t.data.shape()[i];
        }
        os << "])";
        return os;
    }

}