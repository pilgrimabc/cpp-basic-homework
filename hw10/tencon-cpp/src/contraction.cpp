#include <algorithm>
#include <numeric>
#include <stdexcept>

#include "contraction.hpp"

namespace tencon {
    /*******************************
    * helpers
    *******************************/

    namespace detail {

        /**
         * @brief muliplication of the container's elements
         */
        template<typename Container>
        size_t prod(const Container& c) {
            // a prod for empty set is equal to 1
            if (c.empty()) {
                return 1;
            }
            // accumulate the final res
            return std::accumulate(c.begin(), c.end(), size_t{1}, std::multiplies<size_t>());
        }

        /**
         * @brief a diff between sets (by the elements)
         */
        template<typename T>
        std::vector<T> set_difference(
            const std::vector<T>& elements, // vector of elements
            const std::set<T>& exclude // a set for exclude
        ) {
            std::vector<T> result;
            // make an enumeration fo the elements
            for (const auto& element : elements) {
                // check that element is in exclude set
                if (exclude.find(element) == exclude.end()) {
                    result.push_back(element);
                }
            }

            return result;

        }

        /**
         * @brief tensor product of two xt::xarrays 
         */
        inline xt::xarray<double> outer_product(
            const xt::xarray<double>& A,
            const xt::xarray<double>& B
        ) {
            // get shapes 
            auto A_shape = A.shape();
            auto B_shape = B.shape();

            // resulting shape 
            std::vector<size_t> result_shape(A_shape.begin(), A_shape.end());
            result_shape.insert(result_shape.end(), B_shape.begin(), B_shape.end());

            // create result
            xt::xarray<double> result(result_shape);

            // fill it by broadcasting
            auto A_reshaped = xt::reshape_view(A, result_shape);
            auto B_reshaped = xt::reshape_view(B, result_shape);

            // for simplity use cycles
            for (auto& element : result) {
                element = 0.0;
            }

            auto it_result = result.begin();
            for (auto it_A = A.begin(); it_A != A.end(); ++it_A) {
                for (auto it_B = B.begin(); it_B != B.end(); ++it_B) {
                    *it_result = (*it_A) * (*it_B);
                    ++it_result;
                }
            }

            return result;
        }
    }

    /*******************************
    * constraction functions
    *******************************/
    
    std::set<Tensor::IndexType> shared_indices(const Tensor& A, const Tensor& B) {
        // initialize a set of the shared indices
        std::set<Tensor::IndexType> result;

        // create a set of tensor A indices for fast search
        std::set<Tensor::IndexType> A_indices(A.indices.begin(), A.indices.end());

        // find shared indices
        for (const auto& indx : B.indices) {
            // check if indx is in A_indices
            if (A_indices.find(indx) != A_indices.end()) {
                result.insert(indx);
            }
        }

        return result;
    }

    std::tuple<std::vector<size_t>, std::vector<Tensor::IndexType>> indices_ordering(
        const Tensor& T,
        const std::set<Tensor::IndexType>& contracted,
        const std::string& position
    ) {
        // divide indices on FREE and CONTACTED/SHARED
        std::vector<Tensor::IndexType> free_indices = detail::set_difference(T.indices, contracted);

        std::vector<Tensor::IndexType> contracted_indices(contracted.begin(), contracted.end());

        // now we define the order of the indices, there are two possible cases
        // (1) left tensor -- free + contracted
        // (2) right tensor -- contracted + free
        std::vector<Tensor::IndexType> ordered_indices;
        // according to position of the tensor
        if (position == "left") {
            // it is a left tensor -> free + contracted
            ordered_indices = free_indices;
            ordered_indices.insert(ordered_indices.end(), contracted_indices.begin(), contracted_indices.end());
        } else if (position == "right") {
            // it is a right tensor -> contracted + free
            ordered_indices = contracted_indices;
            ordered_indices.insert(ordered_indices.end(), free_indices.begin(), free_indices.end());
        } else {
            throw std::invalid_argument("Position arg must be 'left' or 'right'");
        }

        // transformate indices names (global indices) into local indices (numbers for transpose)
        std::vector<size_t> axes;
        axes.reserve(ordered_indices.size());

        for (const auto& indx : ordered_indices) {
            axes.push_back(T.axis(indx));
        }

        return {axes, free_indices};
    }

    Tensor pairwise_contraction(const Tensor& A, const Tensor& B, const std::string& name) {
        // find shared/contracted indices 
        auto contracted = shared_indices(A, B);

        // if there are no indices, then we use tensor (or outer) product
        if (contracted.empty()) {
            // get data
            auto result_data = detail::outer_product(A.data, B.data);

            /* auto result_data = xt::xarray<double>(
                A.data.shape().begin(), A.data.shape().end(),
                B.data.shape().begin(), B.data.shape().end()
            );
            */

            // get indices
            std::vector<Tensor::IndexType> result_indices = A.indices;
            result_indices.insert(result_indices.end(), B.indices.begin(), B.indices.end());

            // get name
            std::string result_name = name.empty() ? A.name + "&" + B.name : name;

            return Tensor(result_name, result_indices, result_data);
        }

        // get orders of the axes to transpose
        auto [A_axes, A_free] = indices_ordering(A, contracted, "left");
        auto [B_axes, B_free] = indices_ordering(B, contracted, "right");

        // transpose tensors
        auto A_transposed = xt::transpose(A.data, A_axes);
        auto B_transposed = xt::transpose(B.data, B_axes);

        // calculate sizes for reshape
        size_t num_contracted = contracted.size();
        size_t num_A_free = A_free.size();
        size_t num_B_free = B_free.size();

        // shapes of the free axes
        std::vector<size_t> A_free_shape(A_transposed.shape().begin(), A_transposed.shape().begin() + num_A_free);
        std::vector<size_t> B_free_shape(B_transposed.shape().begin() + num_contracted, B_transposed.shape().end());

        // size of the contracted axes
        // they are same for the both tensors
        std::vector<size_t> contracted_shape(A_transposed.shape().begin() + num_A_free, A_transposed.shape().end());

        size_t I = detail::prod(A_free_shape);
        size_t J = detail::prod(contracted_shape);
        size_t K = detail::prod(B_free_shape);

        // reshape to matrices
        auto A_matrix = xt::reshape_view(A_transposed, {I, J});
        auto B_matrix = xt::reshape_view(B_transposed, {J, K});

        // matrix multiplication
        // in x-tensor there is no matrix multiplication -> make it on my own
        auto result_matrix = xt::xarray<double>(std::vector<size_t>{I, K});

        for (size_t i=0; i < I; i++) {
            for (size_t k=0; k < K; k++) {
                double sum = 0.0;
                for (size_t j=0; j < J; j++) {
                    sum += A_matrix(i, j) * B_matrix(j, k);
                }
                result_matrix(i,k) = sum;
            }
        }

        // auto result_matrix = xt::linalg::dot(A_matrix, B_matrix);

        // reshape back to tensor
        std::vector<size_t> result_shape = A_free_shape;
        result_shape.insert(result_shape.end(), B_free_shape.begin(), B_free_shape.end());

        auto result_data = xt::reshape_view(result_matrix, result_shape);

        // get back to IndexType global indices
        std::vector<Tensor::IndexType> result_indices = A_free;
        result_indices.insert(result_indices.end(), B_free.begin(), B_free.end());

        // get name of the resulting tensor 
        std::string result_name = name.empty() ? A.name + "&" + B.name : name;

        return Tensor(result_name, result_indices, result_data);
    }

    Tensor contraction(const std::vector<Tensor>& tensors) {
        // consider some edge cases
        if (tensors.empty()) {
            throw std::invalid_argument("Cannot contract empty tensor list"); 
        }

        if (tensors.size() == 1) {
            return tensors[0];
        }

        // make a sequence contraction ((A @ B) @ C) @ D etc.
        Tensor result = pairwise_contraction(tensors[0], tensors[1]);

        for (size_t i = 2; i < tensors.size(); i++) {
            result = pairwise_contraction(result, tensors[i]);
        }

        return result;
    }

    /*******************************
    * cost functions 
    *******************************/

    ContractionCost estimate_contraction_cost(const Tensor& A, const Tensor& B) {
        // initialize result
        ContractionCost cost; 

        auto contracted = shared_indices(A, B);

        // if there are no indices to contract, then we have tensor (or outer) product
        if (contracted.empty()) {
            // get flops
            cost.flops = static_cast<double>(A.size()) * static_cast<double>(B.size());

            // get a result shape
            std::vector<size_t> result_shape = A.shape();
            result_shape.insert(result_shape.end(), B.shape().begin(), B.shape().end());
            cost.result_shape = result_shape;

            // get PEAK memory 
            cost.memory_bytes = detail::prod(result_shape) * sizeof(double);

            return cost;
        }

        // size of the contracted indices
        size_t contracted_dim = 1;
        for (const auto& indx : contracted) {
            contracted_dim *= A.dim(indx);
        }

        // size of the free indices
        auto A_free = detail::set_difference(A.indices, contracted);
        auto B_free = detail::set_difference(B.indices, contracted);
        
        size_t A_free_dim = 1;
        for (const auto& indx : A_free) {
            A_free_dim *= A.dim(indx);
        }

        size_t B_free_dim = 1;
        for (const auto& indx : B_free) {
            B_free_dim *= B.dim(indx);
        }

        // FLOPS = 2 * contrcted * A_free * B_free
        cost.flops = 2 * static_cast<double>(contracted_dim) * static_cast<double>(A_free_dim) * static_cast<double>(B_free_dim);

        // shape of the result 
        std::vector<size_t> result_shape;
        result_shape.reserve(A_free.size() + B_free.size());
        
        for (const auto& indx : A_free) {
            result_shape.push_back(A.dim(indx));
        }

        for (const auto& indx : B_free) {
            result_shape.push_back(B.dim(indx));
        }
        cost.result_shape = result_shape;

        // memory = result_size * sizof(double)
        cost.memory_bytes = detail::prod(result_shape) * sizeof(double);

        return cost;
    }

    Tensor::ShapeType compute_result_shape(const Tensor& A, const Tensor& B) {
        auto cost = estimate_contraction_cost(A, B);
        
        return cost.result_shape;
    }

    std::vector<Tensor::IndexType> compute_result_indices(const Tensor& A, const Tensor& B) {
        // get contracted indices
        auto contracted = shared_indices(A, B);

        // get free indices
        auto A_free = detail::set_difference(A.indices, contracted);
        auto B_free = detail::set_difference(B.indices, contracted);

        // get resulting shape
        std::vector<Tensor::IndexType> result_indices = A_free;
        result_indices.insert(result_indices.end(), B_free.begin(), B_free.end());

        return result_indices;
    }

     /*******************************************
    * overloading the operator* for contraction
    *******************************************/
    
    Tensor operator*(const Tensor& A, const Tensor& B) {
        return pairwise_contraction(A, B);
    }
    


}