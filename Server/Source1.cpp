//memory leaks 


/*

#include<iostream>
#include<string>
class Person
{
public:
int age;
char gender;

};

int main() {


std::string aq;
std::cin >> aq;

std::cout <<aq[0];

int aaa = std::stoi(aq);
std::cout << aaa;

//std::cout << (int)a;


return 0;

char c;
std::string  cc;
std::cin >> c;
int type;
cc = std::to_string(c);

type = std::stoi(cc);
std::cout << type<<std::endl;
return 0;


Person *p = new Person();
(*p).age = 80;
(*p).gender = 'F';

p->age = 60;
p->gender = 'M';
std::cout << (*p).age << p->age << std::endl;
delete[] p;
getchar();

return 1;

int *b = new int(5);
std::cout << "*b" << *b << "b" << b << std::endl;
*b = 8;
std::cout << "*b" << *b << "b" << b << std::endl;
//	delete[] b;
std::cout << "*b" << *b << std::endl;
return 0;


int *a = new int[5];
for (int i = 0; i < 5; i++){
std::cout << &a[i] << std::endl;
std::cout <<"sec" << a++ << std::endl;




}
//std::cout << &i[0] << &i[1] <<&i[2]<<&i[3] << std::endl;
/*int *iii = new int;
*iii = 8;
int *ii = new int(5);
std::cout << *i << std::endl;
std::cout << *iii << std::endl;
std::cout <<"Before deleting "<< *ii << std::endl;

delete ii;
std::cout << *ii << std::endl;
*/

return 0;
}
*/