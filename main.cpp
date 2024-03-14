#include <iostream>
#include <memory> //библиотека для работы с умными указателями

#include <string>
#include <vector>

// auto_ptr - c++17 - deprecated, c++20 - removed.
// unique_ptr - гарантирует существование единственной ссылки  на объект в памяти. Является владеющим указателем.
// shared_ptr - ведет учет ссылок на один и тот же объект в памяти, которым управляет несколько модулей в программе. Является владеющим указателем
// weak_ptr - служит только хранениемадреса внутри себя. Не служит ни для чего кроме передачи адресов между частями приложения. НЕвладеющий указатель.

//1 
//если не передавать по ссылке, то объект на который указывает автоматический указатель будет уничтожен, в отличии от unique_ptr auto_ptr
//не препятствует созданию своих копий   ТОШ
void F00(std::auto_ptr<std::string> &obj) 
{
	std::cout << *obj;
	return;
} //позволяет портить данные
void F01(std::unique_ptr<std::string> &obj) {} //не позволяет портить данные

//2
void F02(std::shared_ptr<std::string> obj) 
{
	if (obj.use_count() != 10)F02(obj); //выдал одновременно счетчик хранящихся в рекурсии адреса
	std::cout << "\n" << obj.use_count() << " - счетчик ссылок ";
}

//3
class Trait 
{
public:
	Trait(std::string name) :name_(name) {}
	std::string GetName()const
	{
		return name_;
	}
private:
	std::string name_;
};

class Color :public Trait 
{
public:
	Color(std::string name) :Trait(name) {}
};

class Material :public Trait 
{
public:
	Material(std::string name) :Trait(name) {}
};

class Toy 
{
public:
	Toy(std::string name, std::shared_ptr<Trait> color, std::shared_ptr<Trait> material)
		:name_(name), color_(color), material_(material)
	{
		
	}

	std::string GetName()const
	{
		return name_;
	}
	std::string GetColor()const
	{
		return color_->GetName();
	}
	std::string GetMaterial()const
	{
		return material_->GetName();
	}

private:
	std::string name_;
	std::shared_ptr<Trait> color_;
	std::shared_ptr<Trait> material_;
};

std::vector<std::shared_ptr<Color>> g_colors
{
	std::make_shared<Color>("Red"),
	std::make_shared<Color>("Green"),
	std::make_shared<Color>("Blue")
};
std::vector<std::shared_ptr<Material>> g_materials
{
	std::make_shared<Material>("Glass"),
	std::make_shared<Material>("Stone"),
	std::make_shared<Material>("Wood"),
};


int main() {
	setlocale(LC_ALL, "Russian");
	int n;

	auto tmp = new std::string("qwerty"); 
	//пример как не стоит делать хоть и будет работать. Может примевсти к тому, что два уник поинтера указывают на один и тот же адрес в памяти
	//std::unique_ptr<std::string> p_str (tmp); //уники не умеют контролировать какой в них адрес запихивают, поэтому нужно делать этот самим
	//правильный вариант
	auto u_ptr = std::make_unique<std::string>("qwerty");
					//маке выдают только новые указатели, и лучше делать только через них

	std::auto_ptr<std::string> a_ptr(new std::string("qwerty"));

	//1 unique and auto
	F00(a_ptr);
	F01(u_ptr); //добавили ссылку в аргументы функции
	//F02(u_ptr);

	delete tmp;

	//2 shared
	std::shared_ptr<std::string> sh_ptr = std::make_shared<std::string>("asdfg");
	F02(sh_ptr);

	//3
	std::vector<Toy> toys;
	toys.reserve(9);
	int q{ 1 };
	for (int i = 0; i < g_colors.size(); i++)//раздали цыета и материалы девяти игрушкам
	{
		for (int j = 0; j < g_materials.size(); j++) 
		{
			toys.emplace_back(std::string("Toy") + std::to_string(q),
				g_colors[i],
				g_materials[j]	);
		}
	}

	return 0;
}