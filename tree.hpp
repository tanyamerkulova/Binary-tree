#ifndef TREE_HPP
#define TREE_HPP
#include <iostream>

template <typename K, typename T>
struct tree;
template <typename K, typename T> 
std::ostream& operator<<(std::ostream &out, const tree<K, T>& Tree);

/*
 * Все методы, включая конструктор копирования, деструктор и оператор<<
 * реализованы без использования рекурсивных вызовов.
 * Node* parent необходим для обхода дерева в конструкторе копирования,
 * деструкторе и  операторе<<
 * K - key, T - value(data)
 */

template <typename K, typename T>
struct tree
{
	private:
		struct Node
		{
			Node* parent;
			Node* l_child;
			Node* r_child;
			T data;
			K key;
			
			Node(Node* _parent, K _key, T _data) : 
				parent(_parent), key(_key), data(_data), 
				l_child(nullptr), r_child(nullptr)
			{}
		};
		Node* root;
		int size;
		
		//функция для find(K) и exist(K)
		const T* private_find(const K& key) const;
		void clear();
		void copy(const tree& Tree);
		
	public:
		tree();
		tree(const tree& Tree);
		~tree();
			
		void insert(const K& key, const T& value);
		bool remove(const K& key);
			//если нет узла с таким ключом, выбрасывает исключение out_of_range
		const T& find(const K& key) const;
		bool exist(const K& key) const;
		int get_size() const;
		
		const tree& operator=(const tree& Tree);
			//оператор возвращает find(key)
		const T& operator[](const K& key) const;
			//вывод всех узлов от меньшего к большему
		friend std::ostream& operator<< <>(std::ostream &out, const tree<K, T>& Tree);
};

#endif
