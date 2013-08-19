#pragma once

#include <Tree.h>

#include <vector>
#include <unordered_map>
#include <utility>
#include <functional>

#include <Signal.hpp>
#include <ConnectionMap.h>

class LinearizedTree
{
public:
	enum Algorithm
	{
		None,
		DepthFirst,
		BreadthFirst
	};

	LinearizedTree();
	LinearizedTree(const Tree* tree, Algorithm strategy);

	void linearize();
	void clear();

	const Tree* tree() const;
	void setTree(const Tree* tree);

	void setAlgorithm(Algorithm strategy);

	unsigned size() const;
	int maxId() const;
	const Node* root() const;

	int indexOf(const Node* node) const;
    int indexOf(int id) const;
	const Node* at(int index) const;
	const Node* operator[](int index) const;
    const Node* getNode(int id) const;

	std::vector<const Node*>::const_iterator begin() const;
	std::vector<const Node*>::const_iterator end() const;

	void treeLayerRangesDo(std::function<void(int, int)> callback) const;

	signal::Signal<> linearized;
protected:
	const Tree* _tree;
	Algorithm _strategy;
	int _nextIndex;

	std::vector<const Node*> _nodes;
	std::unordered_map<const Node*, int> _indices;
	std::vector<int> _treeDepthTresholds;

	void linearizeDepthFirst();
	void linearizeBreadthFirst();

	void add(const Node* node);
};
