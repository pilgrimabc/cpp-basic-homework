#ifndef TENCON_OPTIMIZER_HPP
#define TENCON_OPTIMIZER_HPP

#include "tensor.hpp"
#include "contraction.hpp"
#include "tensor_network.hpp"

#include <string>
#include <vector>
#include <memory>
#include <unordered_map>

namespace tencon {

    /*******************************
    * ContractionTree Node
    *******************************/

    /**
     * @brief node of the contraction tree
     * 
     * leafs = start tensors
     * internal nodes = intermediate results of contraction
     */
    struct ContractionTreeNode {
        std::string name;                    // name of the node
        std::shared_ptr<ContractionTreeNode> left;   // left child
        std::shared_ptr<ContractionTreeNode> right;  // right child
        ContractionCost cost;                // contraction cost (only for internal nodes)
        
        // LEAF constructor
        ContractionTreeNode(const std::string& name) : name{name}, left{nullptr}, right{nullptr} {}
        
        // INTERNAL NODE constructor
        ContractionTreeNode(const std::string& name, std::shared_ptr<ContractionTreeNode> left,
                            std::shared_ptr<ContractionTreeNode> right, const ContractionCost& cost)
            : name{name}, left{left}, right{right}, cost{cost} {}
        
        // check is node a leaf 
        bool is_leaf() const {
            return left == nullptr && right == nullptr;
        }
    };
    
    /*******************************
    * ContractionTree
    *******************************/
    /**
     * @brief contraction tree for tensor network
     * 
     * Defines contraction order by flops or pear memory optimizstion
     */
    class ContractionTree {
    public:

        /*******************************
        * constructors
        *******************************/
        
        /**
         * @brief 
         * @param root -- root of the tree
         * @param output_indices -- output = free indices
         */
        ContractionTree(std::shared_ptr<ContractionTreeNode> root, const std::vector<Tensor::IndexType>& output_indices = {});
        
        /*******************************
        * members
        *******************************/

        /**
         * @brief contract tree according to the order
         * @param tensors -- dic twith {tensor name : tensor} 
         * @return contraction result
         */
        Tensor contract(const std::unordered_map<std::string, Tensor>& tensors) const;

        /*******************************
        * getters
        *******************************/
        
        /**
         * @brief get total cost in flops
         * @return sum of contracting all internal nodes in flops
         */
        double total_flops() const;
        
        /**
         * @brief get peak memory
         * @return peak memory for BIGGEST contraction (not for all!!!)
         */
        size_t max_memory() const;
        
        /**
         * @brief get root
         * @return shared_ptr on root
         */
        std::shared_ptr<ContractionTreeNode> get_root() const { return root_; }
        
        /**
         * @brief get all leafs (initial tensors)
         * @return vector of leafs
         */
        std::vector<std::shared_ptr<ContractionTreeNode>> get_leaves() const;
        
        /**
         * @brief get all internal nodes
         * @return vector of internal nodes
         */
        std::vector<std::shared_ptr<ContractionTreeNode>> get_internal_nodes() const;
        
        /**
         * @brief string representation of the tree in ASCII format
         * @return ASCII graph for tree
         */
        std::string draw() const;
        
        /**
         * @brief std::cout
         */
        friend std::ostream& operator<<(std::ostream& os, const ContractionTree& tree);
        
    private:
        std::shared_ptr<ContractionTreeNode> root_;
        std::vector<Tensor::IndexType> output_indices_;
        
        // recursive traversal along the graph for collect_leaves
        void collect_leaves(std::shared_ptr<ContractionTreeNode> node,
                            std::vector<std::shared_ptr<ContractionTreeNode>>& leaves) const;
        
        // recursive traversal along the graph for collect_internal
        void collect_internal(std::shared_ptr<ContractionTreeNode> node,
                              std::vector<std::shared_ptr<ContractionTreeNode>>& nodes) const;
        
        // recursive contraction
        Tensor contract_node(std::shared_ptr<ContractionTreeNode> node,
                             const std::unordered_map<std::string, Tensor>& tensors) const;
        
        // recursive printing
        void draw_node(std::shared_ptr<ContractionTreeNode> node,
                       const std::string& prefix,
                       bool is_tail,
                       std::vector<std::string>& lines) const;
    };
    
   /*******************************
    * GreedyOptimizer 
    *******************************/
    
    /**
     * @brief greedy optimizer for tensor network contraction
     * 
     * It works simple: at each it peaks a node and contract it. 
     * After that it updates graph and go futher UNTIL THE WIN.
     * 
     */
    class GreedyOptimizer {
    public:
        /**
         * @brief enums with optimization metrics
         */
        enum class Metric {
            FLOPS,     // minimize by flops
            MEMORY     // minimize by peak mem
        };
        
        /*******************************
        * constructors
        *******************************/

        /**
         * @brief constructor
         * @param metric -- optimization metric, by default = flops
         */
        explicit GreedyOptimizer(Metric metric = Metric::FLOPS);
        
        /*******************************
        * methods
        *******************************/

        /**
         * @brief optimize contraction order
         * @param tn -- tensor network
         * @return contraction tree with optimal contraction path
         */
        ContractionTree optimize(const TensorNetwork& tn) const;
        
        /**
         * @brief optimize contraction order by list of tensors
         * @param tensors -- list of tensors
         * @return contraction tree with optimal contraction path
         */
        ContractionTree optimize(const std::vector<Tensor>& tensors) const;

        /*******************************
        * getters
        *******************************/
        
        /**
         * @brief get optimization metric
         */
        Metric get_metric() const { return metric_; }
        
    private:
        Metric metric_;
        
        /**
         * @brief find best pair/edge for contraction
         * @param tensors -- current tensors
         * @return indices of the best pair
         */
        std::pair<size_t, size_t> 
        find_best_pair(const std::vector<Tensor>& tensors) const;
        
        /**
         * @brief estiomate contraction cost of the pair
         * @param A -- left tensor
         * @param B -- right tensor
         * @return contraction cost with respect to metric
         */
        double get_cost(const Tensor& A, const Tensor& B) const;
    };
    
    /*******************************
    * helpers
    *******************************/
    
    /**
     * @brief estimate cost by shapes
     * @param shape_A -- left tensor shape
     * @param shape_B -- right tensor shape
     * @param indices_A -- left tensor indices
     * @param indices_B -- right tensor indices
     * @param metric -- metirc: 'flops' or 'memory'
     * @return contraction cost
     */
    double estimate_cost_by_shapes(const std::vector<size_t>& shape_A,
                                   const std::vector<size_t>& shape_B,
                                   const std::vector<Tensor::IndexType>& indices_A,
                                   const std::vector<Tensor::IndexType>& indices_B,
                                   const std::string& metric = "flops");
    
    /**
     * @brief estimate shape without contraction 
     * @param shape_A -- left tensor shape
     * @param shape_B -- right tensor shape
     * @param indices_A -- left tensor indices
     * @param indices_B -- right tensor indices
     * @return (result_shape, result_indices)
     */
    std::pair<std::vector<size_t>, std::vector<Tensor::IndexType>>
    compute_result_shape_and_indices(const std::vector<size_t>& shape_A,
                                      const std::vector<size_t>& shape_B,
                                      const std::vector<Tensor::IndexType>& indices_A,
                                      const std::vector<Tensor::IndexType>& indices_B);
    
}

#endif