#include <iostream>
#include <fstream>
#include <cstdlib>
#include <ctime>
#include <cstdint>
#include <chrono>

#include "merge_sort.h"


int main() {
	std::srand(std::time(nullptr));

	std::fstream file1;
	file1.open("C:\\test\\test1.txt", std::ios::in | std::ios::binary | std::ios::out);
	std::fstream file2;
	file2.open("C:\\test\\test2.txt", std::ios::in | std::ios::binary | std::ios::out);
	std::fstream file3;
	file3.open("C:\\test\\test3.txt", std::ios::in | std::ios::binary | std::ios::out);
	std::fstream file4;
	file4.open("C:\\test\\test4.txt", std::ios::in | std::ios::binary | std::ios::out);

	//количество корзин
	const int backets = 3000;

	//размер файла
	int N = 300000;

	//генерация файла
	generate_file(file1, N);
	generate_file(file2, N);
	generate_file(file3, N);
	generate_file(file4, N);

	//замер времени сортировки файлов методом слияния
	std::chrono::time_point<std::chrono::high_resolution_clock> start, end;
	
	start = std::chrono::high_resolution_clock::now();
	merge_sort_file(file1, 0, N-1);
	end = std::chrono::high_resolution_clock::now();
	std::cout << "Merge sort , N = " << N << ", time:" << std::chrono::duration_cast<std::chrono::milliseconds>
		(end - start).count() << " ms;" << std::endl;
	std::cout << "-----------------------------------" << std::endl;
	
	//замер времени сортировки файла методом слияния, части меньше 130 сортируются методом быстрая сортировка 
	start = std::chrono::high_resolution_clock::now();
	merge_quick_sort_file(file2, 0, N - 1);
	end = std::chrono::high_resolution_clock::now();
	std::cout << "Merge_quick sort , N = " << N << ", time:" << std::chrono::duration_cast<std::chrono::milliseconds>
		(end - start).count() << " ms;" << std::endl;
	std::cout << "-----------------------------------" << std::endl;
	
	file1.close();
	file2.close();
	file3.close();
	file4.close();
	
	//замер времени сортировки файла корзинами
	start = std::chrono::high_resolution_clock::now();
	backet_sort("C:\\test\\test3.txt", 65355, backets, N);
	end = std::chrono::high_resolution_clock::now();
	std::cout << "Backet sort , N = " << N << ", time:" << std::chrono::duration_cast<std::chrono::milliseconds>
		(end - start).count() << " ms;" << std::endl;
	std::cout << "-----------------------------------" << std::endl;
	
	//замер времени сортировки файла подсчетом
	start = std::chrono::high_resolution_clock::now();
	count_sort("C:\\test\\test4.txt", 65355, N);
	end = std::chrono::high_resolution_clock::now();
	std::cout << "Count sort , N = " << N << ", time:" << std::chrono::duration_cast<std::chrono::milliseconds>
		(end - start).count() << " ms;" << std::endl;
	std::cout << "-----------------------------------" << std::endl;

	return 0;
}
