#pragma once
#include <fstream>

void generate_file(std::fstream& file, size_t n);
void print_file(std::fstream& file, size_t n);
void print_file(const char* c, size_t n);

void merge_file(std::fstream& file, size_t left, size_t center, size_t right);
void merge_sort_file(std::fstream& file, size_t left, size_t right);
void merge_quick_sort_file(std::fstream& file, size_t left, size_t right);
void backet_sort(const char* c, int max, int N, size_t n);
void count_sort(const char* c, int max, size_t n);
