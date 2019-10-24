#include "tree.hpp"
#include "tree.cpp"

#include <string>
using std::string;

//объявление шаблонов классов и оператора-нечлена с необходимыми параметрами 
//для их компиляции 
template struct tree<int, int>;
template struct tree<string, string>;

template std::ostream& operator<< <int, int>(std::ostream &out, const tree<int, int>& Tree);
template std::ostream& operator<< <string, string>(std::ostream &out, const tree<string, string>& Tree);
