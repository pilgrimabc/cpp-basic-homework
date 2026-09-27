#ifndef TENCON_CONTRACTION_HPP
#define TENCON_CONTRACTION_HPP

#include <set>
#include <tuple>
#include <vector>

#include "tensor.hpp"

namespace tencon {
    // define struct for contraction cost 
    
    /**
     * @brief estimation of the contraction cost
     * 
     * Calculate contracting costs according to the dimensions of the
     * contracted incides.
     */
    struct ContractionCost {
        double flops; // num of FLOPs
        size_t memory_bytes; // the PEAK memory during contraction in bytes
        Tensor::ShapeType result_shape; // shape of the resulting tensor
    };

    /*******************************
    * constraction functions
    *******************************/

    /**
     * @brief find shared indices between two tensors
     * @param A -- left tensor
     * @param B -- right tensor
     * @return a set of shared indices
     */
    std::set<Tensor::IndexType> shared_indices(const Tensor& A, const Tensor& B);

    /**
     * @brief calculate indices ordering for tensor
     * @param T -- tensor
     * @param contracted -- set of contracted indices
     * @param position -- left or right position in pairwise contraction
     * @return a tuple(indices_ordering, free_indices)
     */
    std::tuple<std::vector<size_t>, std::vector<Tensor::IndexType>> indices_ordering(
        const Tensor& T,
        const std::set<Tensor::IndexType>& contracted,
        const std::string& position
    );

    /**
     * @brief pairwise contraction between two tensors
     * @param A -- left tensor
     * @param B -- right tensor
     * @param name -- name of the resulting tensor, by default it is "A&B"
     * @return new tensor -- a result of the pairwise contraction
     */
    Tensor pairwise_contraction(const Tensor& A, const Tensor& B, const std::string& name = "");

    /**
     * @brief a sequence contraction of a list tensor with respect to their order in a list
     * @param tensors -- a vector of the tensors
     * @return a result of the overall contraction
     */
    Tensor contraction(const std::vector<Tensor>& tensors);
    
    /*******************************
    * cost functions 
    *******************************/

    /**
     * @brief estimates a cost of the contraction (btw without contraction)
     * @param A -- left tensor
     * @param B -- right tensor
     * @return struct ContractionCost with FLOPs and a peak memory
     */
    ContractionCost estimate_contraction_cost(const Tensor& A, const Tensor& B);

    /**
     * @brief calculates resulting shape of the tensor, which is getting from the pairwise contraction
     * @param A -- left tensor
     * @param B -- right tensor
     * @result shape of the resulting tendor
     */
    Tensor::ShapeType compute_result_shape(const Tensor& A, const Tensor& B);

    /**
     * @brief calculates indices of the resulting tensor in pairwise contraction
     * @param A -- left tensor
     * @param B -- right tensor
     * @result vector of indices names of the resulting tensor
     */
    std::vector<Tensor::IndexType> compute_result_indices(const Tensor& A, const Tensor& B);

    /*******************************************
    * overloading the operator* for contraction
    *******************************************/

    /**
     * @brief overloads an operator* from Tensor class
     * @param A -- left tensor
     * @param B -- right tensor
     * @return result of the pairwise contraction
     */
    Tensor operator*(const Tensor& A, const Tensor& B);
}



#endif