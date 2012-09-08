﻿#include <iostream>
#include <fstream>
#include <pe_factory.h>
#include "lib.h"

//Пример, показывающий, как считать и получить информацию о статическом TLS (Thread Local Storage, локальная память потока)
//PE или PE+ файла
int main(int argc, char* argv[])
{
	if(argc != 2)
	{
		std::cout << "Usage: tls_reader.exe PE_FILE" << std::endl;
		return 0;
	}

	//Открываем файл
	std::ifstream pe_file(argv[1], std::ios::in | std::ios::binary);
	if(!pe_file)
	{
		std::cout << "Cannot open " << argv[1] << std::endl;
		return -1;
	}

	try
	{
		//Создаем экземпляр PE или PE+ класса с помощью фабрики
		std::auto_ptr<pe_base> image = pe_factory::create_pe(pe_file);

		std::cout << "Reading PE TLS info..." << std::hex << std::showbase << std::endl << std::endl;
		
		//Получаем информацию о TLS
		const pe_base::tls_info info = image->get_tls_info();

		//Выводим информацию о TLS
		std::cout << "Callbacks RVA: " << info.get_callbacks_rva() << std::endl
			<< "Index RVA: " << info.get_index_rva() << std::endl
			<< "Raw data start RVA: " << info.get_raw_data_start_rva() << std::endl
			<< "Raw data end RVA: " << info.get_raw_data_end_rva() << std::endl
			<< "Size of zero fill: " << info.get_size_of_zero_fill() << std::endl;

		//Выведем TLS-коллбеки:
		const pe_base::tls_info::tls_callback_list& tls_callbacks = info.get_tls_callbacks();
		for(pe_base::tls_info::tls_callback_list::const_iterator it = tls_callbacks.begin(); it != tls_callbacks.end(); ++it)
			std::cout << "Callback: " << (*it) << std::endl;
	}
	catch(const pe_exception& e)
	{
		//Если возникла ошибка
		std::cout << "Error: " << e.what() << std::endl;
		return -1;
	}

	return 0;
}
