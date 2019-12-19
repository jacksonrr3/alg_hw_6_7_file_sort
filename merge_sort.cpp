#include <iostream>
#include <fstream>
#include <cstdlib>
#include <ctime>
#include <cstdint>
#include <vector>
#include <utility>
#include <set>
#include <map>
#include <array>
#include "merge_sort.h"

unsigned short int temp;


//вспомогательная функция для генерации файла
void generate_file(std::fstream& file, size_t n) {
	file.seekp(0, std::ios::beg);

	//int temp_i;
	for (size_t i = 0; i < n; i++) {
		temp = std::rand() % 65534;
		file.write(reinterpret_cast<char*>(&temp), sizeof(temp));
	}
}

//вспомогательная функция для печати файла
void print_file(std::fstream& file, size_t n) {
	file.seekg(0, std::ios::beg);
	//int temp;
	for (size_t i = 0; i < n; i++) {
		file.read(reinterpret_cast<char*>(&temp), sizeof(temp));
		std::cout << temp << std::endl;
	}
}

//вспомогательная функция для печати файла, перегрузка
void print_file(const char * c, size_t n) {
	std::ifstream file;
	file.open(c, std::ios::in | std::ios::binary);
	//int temp;
	for (size_t i = 0; i < n; i++) {
		file.read(reinterpret_cast<char*>(&temp), sizeof(temp));
		std::cout << temp << std::endl;
	}
	file.close();
}


//вспомогательная функция для считывания значения из файла 
short int get_from_pos(std::fstream& file, size_t pos) {
	unsigned short int temp1;
	size_t temp_pos_r = pos * sizeof(unsigned short int);
	//file.clear();
	file.seekg(temp_pos_r, std::ios::beg);
	file.read((char*)(&temp1), sizeof(temp1));
	return temp1;
}

//вспомогательная функция для записи значения из файла 
void write_to_pos(std::fstream& file, size_t pos, unsigned short int value) {
	//size_t temp_pos = pos * sizeof(unsigned short);
	temp = value;
	size_t temp_pos_w = pos * sizeof(unsigned short int);
	file.clear();
	file.seekp(temp_pos_w, std::ios::beg);
	file.write(reinterpret_cast<const char*>(&temp), sizeof(temp));
}


//реализация функции разделения элементов для быстрой сортировки файлов 
size_t partition(std::fstream& file, size_t left, size_t right) {
	size_t i = left - 1;
	unsigned short int pivot = get_from_pos(file, right);

	for (size_t j = left; j <= right; j++) {
		unsigned short int temp2 = get_from_pos(file, j);
		if (temp2 <= pivot) {
			++i;
			unsigned short int temp1 = get_from_pos(file, i);
			write_to_pos(std::forward<std::fstream&>(file), i, temp2);
			write_to_pos(std::forward<std::fstream&>(file), j, temp1);
		}
	}
	return i;
}

//реализация функции быстрой сортировки файлов 
void quick_sort_file(std::fstream& file, size_t left, size_t right) {
	if (left >= right) { return; }
	int center = partition(file, left, right);
	quick_sort_file(std::forward<std::fstream&>(file), left, center - 1);
	quick_sort_file(std::forward<std::fstream&>(file), center + 1, right);
}


//реализация функции merge для сортировки файлов
void merge_file(std::fstream& file, size_t left, size_t center, size_t right) {
	std::fstream temp_file;
	temp_file.open("C:\\test\\temp.txt", std::ios::in | std::ios::binary | std::ios::out);
	temp_file.seekp(0, std::ios::beg);
	size_t a = left;
	size_t b = center + 1;
	size_t r = 0;

	unsigned short s_a = get_from_pos(file, a);
	unsigned short s_b = get_from_pos(file, b);

	do {
		if (s_a < s_b) {
			temp_file.write(reinterpret_cast<char*>(&s_a), sizeof(s_a));
			++a;
			s_a = get_from_pos(file, a);
		}
		else {
			temp_file.write(reinterpret_cast<char*>(&s_b), sizeof(s_b));
			++b;
			s_b = get_from_pos(file, b);
		}
	} while (a <= center && b <= right);

	while (a <= center) {
		temp_file.write(reinterpret_cast<char*>(&s_a), sizeof(s_a));
		++a;
		s_a = get_from_pos(file, a);
	}

	while (b <= right) {
		temp_file.write(reinterpret_cast<char*>(&s_b), sizeof(s_b));
		++b;
		s_b = get_from_pos(file, b);
	}

	int pos = left * sizeof(unsigned short);
	file.clear();
	file.seekp(pos, std::ios::beg);
	temp_file.seekg(0, std::ios::beg);
	for (size_t j = left; j <= right; j++) {
		//short int temp;
		temp_file.read(reinterpret_cast<char*>(&temp), sizeof temp);
		file.write(reinterpret_cast<char*>(&temp), sizeof temp);
	}
	temp_file.close();
}


//реализация функции сортировки файла методом слияния, но небольште участки данных (до 130) 
//сортируются массивом методом быстрой сортировкой
void merge_quick_sort_file(std::fstream& file, size_t left, size_t right) {
	if (left >= right) { return; }
	if (right - left < 130) {
		quick_sort_file(std::forward<std::fstream&>(file), left, right);
	}
	else {
		size_t center = left + (right - left) / 2;
		merge_quick_sort_file(std::forward<std::fstream&>(file), left, center);
		merge_quick_sort_file(std::forward<std::fstream&>(file), center + 1, right);
		merge_file(std::forward<std::fstream&>(file), left, center, right);
	}
}


//реализация функции сортировки фалйа методом слияния
void merge_sort_file(std::fstream& file, size_t left, size_t right) {
	if (left >= right) { return; }
	size_t center = left + (right - left) / 2;
	merge_sort_file(std::forward<std::fstream&>(file), left, center);
	merge_sort_file(std::forward<std::fstream&>(file), center + 1, right);
	merge_file(std::forward<std::fstream&>(file), left, center, right);
}


//реализация функции сортировки файла корзинами
void backet_sort(const char * c, int max, int N, size_t n) {
	std::fstream file;
	file.open(c, std::ios::in | std::ios::binary | std::ios::out);
	std::vector<std::multiset<unsigned short int>> arr(N);
	file.seekg(0, std::ios::beg);
	while (n > 0){
		file.read(reinterpret_cast<char*>(&temp), sizeof(temp));
		arr[N * temp / max].insert(temp);
		--n;
	}

	file.seekp(0, std::ios::beg);
	unsigned short int temp_2;
	for (auto& a : arr) {
		if (a.empty()) { continue; }
		else {
			for (auto& b : a) {
				temp_2 = b;
				file.write(reinterpret_cast<char*>(&temp_2), sizeof(temp));
				//std::cout << temp_2 << std::endl;
			}
		}
	}

	file.close();
}


//реализация функции сортировки файла методом подсчета
void count_sort(const char* c, int max, size_t n) {
	std::fstream file;
	file.open(c, std::ios::in | std::ios::binary | std::ios::out);
	std::map<unsigned short int, int> arr;
	file.seekg(0, std::ios::beg);
	while (n > 0) {
		file.read(reinterpret_cast<char*>(&temp), sizeof(temp));
		arr[temp] += 1;
		--n;
	}

	file.clear();
	file.seekp(0, std::ios::beg);
	unsigned short int temp_2;
	for (auto & a : arr) {
		for (int j = 0; j < a.second; j++) {
			temp_2 = a.first;
				file.write(reinterpret_cast<char*>(&temp_2), sizeof(temp_2));
				//std::cout << temp_2 << std::endl;
		}
	}
	file.close();
}
