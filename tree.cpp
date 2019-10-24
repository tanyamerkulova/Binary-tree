#include "tree.hpp"
#include <stdexcept>

#define _T_ template <typename K,typename T>

_T_ tree<K,T>::tree(): root(nullptr), size(0)
{
	//при создании нет узлов, размер = 0
}

_T_ tree<K,T>::tree(const tree& Tree)
{
	size = 0;
	root = nullptr;
	if(Tree.size)
	{
		this->insert(Tree.root->key, Tree.root->data);
		Node* current = root;
		Node* Tcurrent = Tree.root;
		while(size != Tree.size) //пока не добавлены все узлы
		{
			//Если есть левый ребенок и он не добавлен в новое дерево
			if(Tcurrent->l_child != nullptr && current->l_child == nullptr)
			{
				//Добавляем и переходим в эту вершину
				this->insert(Tcurrent->l_child->key, Tcurrent->l_child->data);
				current = current->l_child;
				Tcurrent = Tcurrent->l_child;
			}
			//То же самое для правого
			else if(Tcurrent->r_child != nullptr && current->r_child == nullptr)
			{
				this->insert(Tcurrent->r_child->key, Tcurrent->r_child->data);
				current = current->r_child;
				Tcurrent = Tcurrent->r_child;
			}
			//если все дети текущей вершины есть в новом дереве, идем на узел вверх
			else
			{
				current = current->parent;
				Tcurrent = Tcurrent->parent;
			}
		}
	}
}

_T_ tree<K,T>::~tree()
{
	if(size)
	{
		Node* current = root;

		while(size) //пока в дереве есть элементы
		{
			//ищем листья
			if(current->l_child != nullptr)
				current = current->l_child;
				
			else if(current->r_child != nullptr)
				current = current->r_child;
			
			else //нашли лист
			{
				Node*  tmp = current->parent;
				if(size > 1) //для любых листьев кроме корня
				{
					//для родителя этого листа child = nullptr
					if(tmp->key > current->key)
						tmp->l_child = nullptr;
					else
						tmp->r_child = nullptr;
				}
				delete current; //удаляем лист
				current = tmp; //следующий проверяемый узел - родитель удалённого листа
				size--;
			}	
		}
	}
}

_T_ void tree<K,T>::insert(const K& key, const T& value)
{
	size++;
	if(root != nullptr)//если дерево не пустое
	{
		Node* current = root;
		while(true)
		{
			Node* tmp;
			if(key == current->key) // если узел с таким ключом есть в дереве, меняем значение
			{
				current->data = value;	
				size--;
				return;
			}
				//ищем место для нового узла по его ключу
			else if(key > current->key)
				tmp = current->r_child;
			else
				tmp = current->l_child;
				
			if(tmp == nullptr)//если нашли добавляем и связываем с родителем
			{
				Node *new_node = new Node(current, key, value);
				
				if(key > current->key)
					current->r_child = new_node;
				else
					current->l_child = new_node;
				break;
			}
			else//если не нашли, ищем дальше
				current = tmp;
		}
	}
	
	else//если дерево пустое 
		root = new Node(nullptr, key, value);
}

_T_ bool tree<K,T>::remove(const K& key)
{
	Node* current = root;
	//recursion == true в случае удаления узла current, у которого 
	//два ребенка и у current->right есть current->right->left,
	//т.к. необходимо удалить m->right->left без его поиска
	bool recursion = false;
	while(true)
	{
		if(current == nullptr)//если нет узла с ключом key вернем false
			return false;
			
		//ищем нужный узел 
		if(key > current->key && !recursion)
			current = current->r_child;
			
		else if(key < current->key && !recursion)
			current = current->l_child;
			
		else//нашли
		{
			//если у узла current нет детей
			if(current->l_child == nullptr && current->r_child == nullptr)
			{
				//удалим его, обнулим ссылку на него у его родителя
				if(current->parent != nullptr)
				{
					if(current->key > current->parent->key)
						current->parent->r_child = nullptr;
					else
						current->parent->l_child = nullptr;
				}
				delete current;
				size--;
				break;
			}
			//если у узла current оба ребенка
			else if(current->l_child != nullptr && current->r_child != nullptr)
			{
				Node* tmp;
				//если у current->right нет левого ребёнка
				if(current->r_child->l_child == nullptr)
				{
					//key и value узла current->right приравниваются узлу current
					//связываются друг с другом элементы current и current->right->right
					//удаляется узел current->right
					tmp = current->r_child;
					current->r_child = tmp->r_child;
					if(tmp->r_child != nullptr)
						tmp->r_child->parent = current;
					current->data = tmp->data;
					current->key = tmp->key;
					delete tmp;
					size--;
					break;
				}
				else //если у current->right есть левого ребёнка
				{
					//key и value узла current->right->left приравниваются узлу current
					//"рекурсивно" удаляем узел current->right->left
					current->key = current->r_child->l_child->key;
					current->data = current->r_child->l_child->data;
					current = current->r_child->l_child;
					recursion = true;//current == удаляемый объект => его искать не надо
				}		
			}
			//если у узла current один ребёнок
			else
			{
				//key и value узла current->child приравниваются узлу current
				//связываются друг с другом элементы current и элементы current->child->right,
				//current->child->left, удаляется узел current->child
				Node* tmp;
				if(current->l_child != nullptr)
					tmp = current->l_child;
					
				else
					tmp = current->r_child;
				
				
				current->r_child = tmp->r_child;
				current->l_child = tmp->l_child;
				if(tmp->r_child != nullptr)
					tmp->r_child->parent = current;
				if(tmp->l_child != nullptr)
					tmp->l_child->parent = current;
				current->data = tmp->data;
				current->key = tmp->key;
			
				delete tmp;
				size--;
				current = root;
				break;
			}
		}
	}
	return true;
}

_T_ const T* tree<K,T>::private_find(const K& key) const
{
	if(!size) //если дерево пустое
		return nullptr;
	Node* current = root;
	while(current != nullptr)
	{
		//если нашли нужный узел, возвращаем его
		//если нет, спускаемся от корня, пока не найдем
		if(current->key == key) 
			return &current->data;
		else if(key > current->key)
			current = current->r_child;
		else
			current = current->l_child;
	}
	return nullptr;//если нет узлов с таким ключом
}

_T_ const T& tree<K,T>::find(const K& key) const
{
	try
	{
		auto a = private_find(key);
		if(a == nullptr)
			throw std::out_of_range(
				  "A node you are trying to access, does not exist.\n"
				  "\t   Please, use 'bool exist(const K& key)' first, to " 
				  "find out, whether it exists.");
		else
			return *a;
	}
	catch(std::out_of_range& err) { throw err; }
}

_T_ bool tree<K,T>::exist(const K& key) const
{
	if(private_find(key) != nullptr)
		return true;
	else 
		return false;
}

_T_ int tree<K,T>::get_size() const
{
	
	return size;
}

_T_ const T& tree<K,T>::operator[](const K& key) const
{
	return find(key);
}


_T_ std::ostream& operator<<(std::ostream &out, const tree<K, T>& Tree)
{
	typename tree<K, T>::Node* tmp = Tree.root;
		//Если last равен одному из детей A текущей вершины M
		//значит все поддерево c корнем A уже пройдено.
		//Если last == tmp->left необходимо пройти правое поддерево
		//Если last == tmp->right, значит оба поддерева пройдены, поднимаемся
		//Иначе необходимо пройти левое поддерево
	typename tree<K, T>::Node* last = Tree.root; 
	while(true)
	{
			//Если нет левого поддерева, а правое(если есть) еще не пройдено
			//или если есть левое поддерево и оно уже пройдено
		if(tmp->l_child == nullptr && last != tmp->r_child || last == tmp->l_child)
			out << tmp->data << std::endl;
		
			//если ешё не обошли левое дерево при непройденном правом
		if(tmp->l_child != nullptr && last != tmp->l_child && last != tmp->r_child)
			tmp = tmp->l_child;
			//если не обошли правое
		else if(tmp->r_child != nullptr && last != tmp->r_child)
			tmp = tmp->r_child;
			
			//если обошли оба поддерева, поднимаемся	
		else
		{
			last = tmp; //тк обошли оба поддерева
			if(tmp->parent != nullptr)
				tmp = tmp->parent;
			else
				break;
		}
	}
	return out;
}
