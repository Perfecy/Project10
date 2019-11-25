/*
MIT License

Copyright (c) 2019 МГТУ им. Н.Э. Баумана, кафедра ИУ-6, Михаил Фетисов,

Программа-заготовка для домашнего задания
*/

#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <cassert>

#include <sstream>

#include "Header.h"

using namespace std;

const size_t MAX_NAME_LENGTH = 50;


class CourierService
{   
	uint16_t _id;
	string   _cour_name;
	string   _order_name;
	string   _address;
	float _price;

protected:
	bool invariant() const
	{
		return _id >0 && !_address.empty() &&  _price > 10.0
			&& !_cour_name.empty() && _cour_name.size() <= MAX_NAME_LENGTH
			&& !_order_name.empty() && _order_name.size() <= MAX_NAME_LENGTH;
	}

public:
	CourierService() = delete;

	CourierService(uint16_t id, const string & cour_name, const string &  order_name, const string & address, float price)
		: _id(id), _cour_name(cour_name), _order_name(order_name), _address(address), _price(price)
	{
		assert(invariant());
	}

	const string & getCourName() const { return _cour_name; }
	const string & getOrderName() const { return _order_name; }
	const string & getAddress() const { return _address; }
	uint16_t       getId() const { return _id; }
	float          getPrice() const { return _price;  }

	static bool    write(const CourierService &p, ostream& os)
	{
		writeNumber(os, p.getId());
		writeString(os, p.getCourName());
		writeString(os, p.getOrderName());
		writeString(os, p.getAddress());
		writeNumber(os, p.getPrice());

		return os.good();
	}

	static CourierService  read(istream& is)
	{
		uint16_t id = readNumber<uint16_t>(is);
		string   cour_name = readString(is, MAX_NAME_LENGTH);
		string   order_name = readString(is, MAX_NAME_LENGTH);
		string   address = readString(is, MAX_NAME_LENGTH * 10);
		float price = readNumber<float>(is);

		return CourierService(id, cour_name, order_name, address, price);
	}
};

bool performCommand(const vector<string> & args, Collector<CourierService> & col)
{
	if (args.empty())
		return false;

	if (args[0] == "l" || args[0] == "load")
	{
		string filename = (args.size() == 1) ? "hw.data" : args[1];

		if (!col.loadCollection(filename))
		{
			cerr << "Ошибка при загрузке файла '" << filename << "'" << endl;
			return false;
		}

		return true;
	}

	if (args[0] == "s" || args[0] == "save")
	{
		string filename = (args.size() == 1) ? "hw.data" : args[1];

		if (!col.saveCollection(filename))
		{
			cerr << "Ошибка при сохранении файла '" << filename << "'" << endl;
			return false;
		}

		return true;
	}

	if (args[0] == "c" || args[0] == "clean")
	{
		if (args.size() != 1)
		{
			cerr << "Некорректное количество аргументов команды clean" << endl;
			return false;
		}

		col.clean();

		return true;
	}

	if (args[0] == "a" || args[0] == "add")
	{
		if (args.size() != 4)
		{
			cerr << "Некорректное количество аргументов команды add" << endl;
			return false;
		}

		col.addItem(CourierService(stoul(args[1]),args[2].c_str(), args[3].c_str(), args[4].c_str(), stoul(args[5])));
		return true;
	}

	if (args[0] == "u" || args[0] == "update")
	{
		if (args.size() != 5)
		{
			cerr << "Некорректное количество аргументов команды update" << endl;
			return false;
		}

		col.updateItem(stoul(args[1]), CourierService(stoul(args[2]), args[3].c_str(), args[4].c_str(), args[5].c_str(), stoul(args[6])));
		return true;
	}

	if (args[0] == "r" || args[0] == "remove")
	{
		if (args.size() != 2)
		{
			cerr << "Некорректное количество аргументов команды remove" << endl;
			return false;
		}

		col.removeItem(stoul(args[1]));
		return true;
	}

	if (args[0] == "v" || args[0] == "view")
	{
		if (args.size() != 1)
		{
			cerr << "Некорректное количество аргументов команды view" << endl;
			return false;
		}

		size_t count = 0;
		for (size_t i = 0; i < col.getSize(); ++i)
		{
			CourierService item = col.getItem(i);

			if (!col.isRemoved(i))
			{
				cout << "[" << i << "] "
					<< item.getId() << " "
					<< item.getCourName() << " "
					<< item.getOrderName() << " "
					<< item.getAddress() << " "
					<< item.getPrice() << endl;
				count++;
			}
		}

		cout << "Количество элементов в коллекции: " << count << endl;

		return true;
	}

	cerr << "Недопустимая команда '" << args[0] << "'" << endl;
	return false;
}

int main(int, char **)
{
	Collector<CourierService> col;

	for (string line; getline(cin, line); )
	{
		if (line.empty())
			break;

		istringstream  iss(line);
		vector<string> args;

		for (string str; iss.good();)
		{
			iss >> str;
			args.emplace_back(str);
		}

		if (!performCommand(args, col))
			return 1;
	}

	cout << "Выполнение завершено успешно" << endl;
	return 0;
}
