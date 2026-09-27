#include "optimizer.hpp"
#include <algorithm>
#include <sstream>
#include <limits>
#include <cmath>

namespace tencon {

/*******************************
* ContractionTree
*******************************/

ContractionTree::ContractionTree(std::shared_ptr<ContractionTreeNode> root,
                                  const std::vector<Tensor::IndexType>& output_indices)
    : root_{root}, output_indices_{output_indices}
{
}

Tensor ContractionTree::contract(const std::unordered_map<std::string, Tensor>& tensors) const {
    // call a recursive  contraction of the tree
    Tensor result = contract_node(root_, tensors);
    
    // if there is output_indices check that they are in correct order
    if (!output_indices_.empty() && result.indices != output_indices_) {
        // if optimizer fail with output order fix it
        result.transpose(output_indices_);
    }
    
    return result;
}

Tensor ContractionTree::contract_node(std::shared_ptr<ContractionTreeNode> node,
    const std::unordered_map<std::string, Tensor>& tensors) const 
{
    // basic case -- leaf
    if (node->is_leaf()) {
        auto it = tensors.find(node->name);
        if (it == tensors.end()) {
            throw std::runtime_error("Tensor '" + node->name + "' not found");
        }
        return it->second;
    }
    
    // recursive contraction of the children
    Tensor left_tensor = contract_node(node->left, tensors);
    Tensor right_tensor = contract_node(node->right, tensors);
    
    // pairwise contraction
    Tensor result = left_tensor * right_tensor;
    
    return result;
}

double ContractionTree::total_flops() const {
    double total = 0.0;
    auto nodes = get_internal_nodes();
    for (const auto& node : nodes) {
        total += node->cost.flops;
    }
    return total;
}

size_t ContractionTree::max_memory() const {
    size_t max_mem = 0;
    auto nodes = get_internal_nodes();
    for (const auto& node : nodes) {
        max_mem = std::max(max_mem, node->cost.memory_bytes);
    }
    return max_mem;
}

std::vector<std::shared_ptr<ContractionTreeNode>> ContractionTree::get_leaves() const {
    std::vector<std::shared_ptr<ContractionTreeNode>> leaves;
    collect_leaves(root_, leaves);
    return leaves;
}

void ContractionTree::collect_leaves(
    std::shared_ptr<ContractionTreeNode> node,
    std::vector<std::shared_ptr<ContractionTreeNode>>& leaves) const 
{
    if (node->is_leaf()) {
        leaves.push_back(node);
    } else {
        collect_leaves(node->left, leaves);
        collect_leaves(node->right, leaves);
    }
}

std::vector<std::shared_ptr<ContractionTreeNode>> ContractionTree::get_internal_nodes() const {
    std::vector<std::shared_ptr<ContractionTreeNode>> nodes;
    collect_internal(root_, nodes);
    return nodes;
}

void ContractionTree::collect_internal(
    std::shared_ptr<ContractionTreeNode> node,
    std::vector<std::shared_ptr<ContractionTreeNode>>& nodes) const 
{
    if (!node->is_leaf()) {
        nodes.push_back(node);
        collect_internal(node->left, nodes);
        collect_internal(node->right, nodes);
    }
}

std::string ContractionTree::draw() const {
    std::vector<std::string> lines;
    draw_node(root_, "", true, lines);
    
    std::ostringstream oss;
    for (const auto& line : lines) {
        oss << line << "\n";
    }
    return oss.str();
}

void ContractionTree::draw_node(
    std::shared_ptr<ContractionTreeNode> node,
    const std::string& prefix,
    bool is_tail,
    std::vector<std::string>& lines) const 
{
    std::string node_repr = node->name;
    if (!node->is_leaf()) {
        std::ostringstream cost_ss;
        cost_ss << " (FLOPs=" << static_cast<long long>(node->cost.flops)
                << ", MEM=" << node->cost.memory_bytes << ")";
        node_repr += cost_ss.str();
    }
    
    lines.push_back(prefix + (is_tail ? "└── " : "├── ") + node_repr);
    
    if (!node->is_leaf()) {
        std::string new_prefix = prefix + (is_tail ? "    " : "│   ");
        draw_node(node->left, new_prefix, false, lines);
        draw_node(node->right, new_prefix, true, lines);
    }
}

std::ostream& operator<<(std::ostream& os, const ContractionTree& tree) {
    os << tree.draw();
    return os;
}

// =====================================================================
// GreedyOptimizer
// =====================================================================

GreedyOptimizer::GreedyOptimizer(Metric metric)
    : metric_(metric)
{
}

ContractionTree GreedyOptimizer::optimize(const TensorNetwork& tn) const {
    // Получить список тензоров
    std::vector<Tensor> tensors;
    for (const auto& tensor : tn.tensors_list) {
        tensors.push_back(tensor);
    }
    
    return optimize(tensors);
}

ContractionTree GreedyOptimizer::optimize(const std::vector<Tensor>& tensors) const {
    if (tensors.empty()) {
        throw std::invalid_argument("Cannot optimize empty tensor list");
    }
    
    if (tensors.size() == 1) {
        auto leaf = std::make_shared<ContractionTreeNode>(tensors[0].name);
        return ContractionTree(leaf, tensors[0].indices);
    }
    
    // copy od tensor for modification
    std::vector<Tensor> current_tensors = tensors;
    
    // make a dict {tensor name : node}
    std::unordered_map<std::string, std::shared_ptr<ContractionTreeNode>> nodes;
    for (const auto& t : tensors) {
        nodes[t.name] = std::make_shared<ContractionTreeNode>(t.name);
    }
    
    // VERY GREEEEEDY ITERATIONSSS
    while (current_tensors.size() > 1) {
        // find best edge to contract
        auto [best_i, best_j] = find_best_pair(current_tensors);
        
        const Tensor& A = current_tensors[best_i];
        const Tensor& B = current_tensors[best_j];
        
        // estimate costs
        ContractionCost cost = estimate_contraction_cost(A, B);
        
        // result name
        std::string result_name = A.name + "&" + B.name;
        
        // create a node 
        auto new_node = std::make_shared<ContractionTreeNode>(
            result_name,
            nodes[A.name],
            nodes[B.name],
            cost
        );
        nodes[result_name] = new_node;
        
        // do pairwise contraction
        Tensor result = A * B;
        result.name = result_name;
        
        // update: delete A and B, add result
        std::vector<Tensor> next_tensors;
        next_tensors.reserve(current_tensors.size() - 1);
        
        for (size_t k = 0; k < current_tensors.size(); ++k) {
            if (k != best_i && k != best_j) {
                next_tensors.push_back(current_tensors[k]);
            }
        }
        next_tensors.push_back(result);
        
        current_tensors = std::move(next_tensors);
    }
    
    // last tensor is a root
    std::string root_name = current_tensors[0].name;
    return ContractionTree(nodes[root_name], current_tensors[0].indices);
}

std::pair<size_t, size_t> 
GreedyOptimizer::find_best_pair(const std::vector<Tensor>& tensors) const {
    size_t best_i = 0;
    size_t best_j = 1;
    double best_cost = std::numeric_limits<double>::max();
    
    for (size_t i = 0; i < tensors.size(); ++i) {
        for (size_t j = i + 1; j < tensors.size(); ++j) {
            double cost = get_cost(tensors[i], tensors[j]);
            if (cost < best_cost) {
                best_cost = cost;
                best_i = i;
                best_j = j;
            }
        }
    }
    
    return {best_i, best_j};
}

double GreedyOptimizer::get_cost(const Tensor& A, const Tensor& B) const {
    ContractionCost cost = estimate_contraction_cost(A, B);
    
    if (metric_ == Metric::FLOPS) {
        return cost.flops;
    } else {  // MEMORY
        return static_cast<double>(cost.memory_bytes);
    }
}

/*******************************
* helpers
*******************************/

double estimate_cost_by_shapes(const std::vector<size_t>& shape_A,
                                const std::vector<size_t>& shape_B,
                                const std::vector<Tensor::IndexType>& indices_A,
                                const std::vector<Tensor::IndexType>& indices_B,
                                const std::string& metric) 
{
    xt::xarray<double> data_A(shape_A);
    xt::xarray<double> data_B(shape_B);
    
    Tensor A("temp_A", indices_A, data_A);
    Tensor B("temp_B", indices_B, data_B);
    
    ContractionCost cost = estimate_contraction_cost(A, B);
    
    if (metric == "flops") {
        return cost.flops;
    } else {
        return static_cast<double>(cost.memory_bytes);
    }
}

std::pair<std::vector<size_t>, std::vector<Tensor::IndexType>>
compute_result_shape_and_indices(const std::vector<size_t>& shape_A,
                                  const std::vector<size_t>& shape_B,
                                  const std::vector<Tensor::IndexType>& indices_A,
                                  const std::vector<Tensor::IndexType>& indices_B) 
{
    
    xt::xarray<double> data_A(shape_A);
    xt::xarray<double> data_B(shape_B);
    
    Tensor A("temp_A", indices_A, data_A);
    Tensor B("temp_B", indices_B, data_B);
    
    auto result_shape = compute_result_shape(A, B);
    auto result_indices = compute_result_indices(A, B);
    
    return {result_shape, result_indices};
}

} 