#include "tree.hpp"
#include <fstream>
#include <string>
#include <cstdlib>
#include <ctime>

using std::cout;
using std::string;

const int min = 100;
const int max = 1000;
const int n = 50;

template <typename T>
void add(tree<T, T>& Tree, const T& value)
{
	Tree.insert(value, value);
}

void integers()
{
	tree<int, int> Tree;
	
	//заполняем дерево n случайными числами от min до max
	srand(time(0));
	for(int i = 0; i < n; i++)
		add(Tree, min + rand() % (max - min + 1));
	
	//уаляем все чётные числа
	int i = max;
	while(i-- != min - 1)
	{
		if(Tree.exist(i) && Tree[i] % 2 == 0)
			Tree.remove(i);
	}
	
	//печатаем дерево на экран
	cout << "Tree contains" << Tree.get_size() << " numbers:\n" << Tree;
}

void strings()
{
	tree<string, string> Tree;
	
	//заполняем дерево n случайными буквенными строками длиной от 1 до 5
	srand(time(0));
	for(int i = 0; i < n; i++)
	{
		int string_length = 1 + rand() % 5;
		string a;
		
		for(int j = 0; j < string_length; j++)
			a += (char)('a' + rand() % ('z' - 'a' + 1));
			
		add(Tree, a);
	}
	
	//Удаляем все двухбуквенные строки
	for(int i = 'a'; i <= 'z';i++)
		for(int j = 'a'; j <= 'z'; j++)
		{
			string tmp;
			tmp += char(i);
			tmp += char(j);
			if(Tree.exist(tmp))
				Tree.remove(tmp);
		}
	
	//печатаем дерево на экран
	cout << "Tree contains " << Tree.get_size() << " strings:\n" << Tree;
}

int main()
{
	integers();
	strings();
}
