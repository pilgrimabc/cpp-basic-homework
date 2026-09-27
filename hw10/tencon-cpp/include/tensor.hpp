#ifndef TENCON_TENSOR_HPP
#define TENCON_TENSOR_HPP

#include <fstream>
#include <iostream>
#include <string>
#include <vector>
#include <unordered_map>
#include <unordered_set>

#include <xtensor/containers/xarray.hpp>
#include <xtensor/views/xview.hpp>
#include <xtensor/misc/xmanipulation.hpp>

namespace tencon {

/*
* @brief Tensor class for basic manipulations with tensor for tensor contractions. 
*
* Example of usage:
* @code
* xt::xarray<double> data = {{1.0, 2.0}, {3.0,4.0}};
* Tensor A("A", {"i","j"}, data);
* A.transpose({"j","i"});
* @endcode
*/
class Tensor {
public:
    // types for convience
    using IndexType = std::string;
    using ShapeType = std::vector<size_t>;
    using DataType = xt::xarray<double>; // harcoded type double for NumPy-like array
    using IndicesMap = std::unordered_map<IndexType, size_t>;

    /*******************************
    * public members
    *******************************/
   std::string name; //tensor name
   std::vector<IndexType> indices; // indices names
   DataType data; // data of the tensor

   /*******************************
    * constructors
    *******************************/
   Tensor() = default;

   /**
    * @brief main Tensor constructor
    * @param name -- name of the Tensor
    * @param indices -- names of indices in the axes order in data
    * @param data -- Tensor data xt::array 
    */
   Tensor(const std::string& name, const std::vector<IndexType>& indices, const DataType& data);

   /*******************************
    * getters
    *******************************/

    /**
     * @brief get shape of the Tensor
     * @return copy of the shape
     */
    ShapeType shape() const;

    /**
     * @brief get Tensor's rank (num of axes/indices)
     * @return dimension of the tensor
     */
    size_t rank() const;

    /**
     * @brief get total number of elements
     * @return data dimension
     */
    size_t size() const;

    /**
     * @brief emptyness check
     * @return true if there is no data
     */
    // bool empty() const;

    /**
     * @brief index existence check 
     * @param indx -- index name
     * @return true if there is such index
     */
    bool has_index(const IndexType& indx) const;

    /**
     * @brief get a dimension of the index
     * @param indx -- index name
     * @return index dimension
     */
    size_t dim(const IndexType& indx) const;

    /**
     * @brief get a local position/index of the global IndexType
     * @param indx -- index name (global index)
     * @return local index
     */
    size_t axis(const IndexType& indx) const;

    /**
     * @brief access to an element by indices
     * @param indices -- indices of the element
     * @return link to the element
     */
    template<typename... Args>
    double& operator() (Args... indices);

    /**
     * @brief access to an element by indices
     * @param indices -- indices of the element
     * @return copy to the element
     */
    template<typename... Args>
    double operator() (Args... indices) const;

    /*******************************
    * setters
    *******************************/

    /**
     * @brief transpose Tensor axes
     * @param axes -- new order of the indices by names 
     * 
     */
    void transpose(const std::vector<IndexType>& axes = {});

    /**
     * @brief change a shape of the Tensor
     * @param new_shape -- new shape
     */
    void reshape(const ShapeType& new_shape = {});

    /**
     * @brief rename of the indices
     * @param name_dict -- dict with form {new_indx: old_indx}
     */
    void rename_indices(const std::unordered_map<IndexType, IndexType>& name_dict);

    /*******************************
    * other
    *******************************/

    /**
     * @brief operator of the tensor contraction (like @ in NumPy)
     * @param other -- another Tensor
     * @return result of the pairwise contraction
     * 
     * NOTE: here we implement the realization of the contraction.hpp
     */
    //Tensor operator*(const Tensor& other) const;

    // std::cout 
    friend std::ostream& operator<<(std::ostream& os, const Tensor& t);

    friend class TensorNetwork;
    friend class ContractionTree;

private:
    // dict with correspondence between global and local indices
    IndicesMap global_local_indices;

    // for tensors in tensor network: each index is connected with another 
    std::unordered_map<IndexType, std::string> contraction_indices_tensors;

    // free indices, which are not taking part in the contraction
    std::vector<IndexType> free_indices;
};

// streaming 
std::ostream& operator<<(std::ostream& os, const Tensor& t);

//templates
template<typename... Args>
double& Tensor::operator()(Args... indices) {
    return data(indices...);
}

template<typename... Args>
double Tensor::operator()(Args... indices) const {
    return data(indices...);
}

// is mentioned in contraction.hpp
Tensor operator*(const Tensor& A, const Tensor& B);

}

#endif