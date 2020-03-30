#ifndef FILE_IO_H
#define FILE_IO_H

//#include <iostream>
#include <fstream>
#include <vector>

#include <cassert>

class FileIO {
public:
    //    FileSystem();
    //    template<class T>
    //    static void read(T& val, std::ifstream & file) {
    //        file.read(reinterpret_cast<char*>(&val), sizeof(val));
    //    }

    //    template<class T>
    //    static void write(const T& val, std::ofstream & file) {
    //        file.write(reinterpret_cast<const char*>(&val), sizeof(val));
    //    }

    // ------------------------------- READ
    static void read(size_t& val, std::ifstream& file)
    {
        file.read(reinterpret_cast<char*>(&val), sizeof(size_t));
    }
    static void read(int& val, std::ifstream& file)
    {
        file.read(reinterpret_cast<char*>(&val), sizeof(int));
    }
    static void read(float& val, std::ifstream& file)
    {
        file.read(reinterpret_cast<char*>(&val), sizeof(float));
    }
    // static void read(uint & val, std::ifstream & file) {
    //     file.read(reinterpret_cast<char*>(&val), sizeof(int));
    // }
    static void read(float* val, size_t len, std::ifstream& file)
    {
        file.read(reinterpret_cast<char*>(val), len * sizeof(float));
    }
    static void read(uint8_t* val, size_t len, std::ifstream& file)
    {
        file.read(reinterpret_cast<char*>(val), len * sizeof(uint8_t));
    }

    template <class T>
    static void read(std::vector<T>& val, std::ifstream& file)
    {
        assert(val.empty());
        size_t size;
        read(size, file);
        val.resize(size);
        auto* tab = val.data();
        file.read(reinterpret_cast<char*>(tab), sizeof(T) * size);
    }

    static void read(std::string& val, std::ifstream& file)
    {
        assert(val.empty());
        size_t size;
        read(size, file);
        val.resize(size);
        file.read(const_cast<char*>(val.data()), size * sizeof(char));
    }

    // ---------------------------------- WRITE
    static void write(size_t val, std::ofstream& file)
    {
        file.write(reinterpret_cast<const char*>(&val), sizeof(size_t));
    }
    static void write(int val, std::ofstream& file)
    {
        file.write(reinterpret_cast<const char*>(&val), sizeof(int));
    }
    static void write(float val, std::ofstream& file)
    {
        file.write(reinterpret_cast<const char*>(&val), sizeof(float));
    }
    // static void write(uint  val, std::ofstream & file) {
    //     file.write(reinterpret_cast<const char*>(&val), sizeof(int));
    // }
    static void write(const float* val, size_t len, std::ofstream& file)
    {
        file.write(reinterpret_cast<const char*>(val), len * sizeof(float));
    }
    static void write(const uint8_t* val, size_t len, std::ofstream& file)
    {
        file.write(reinterpret_cast<const char*>(val), len * sizeof(uint8_t));
    }

    template <class T>
    static void write(const std::vector<T>& val, std::ofstream& file)
    {
        size_t size = val.size();
        write(size, file);

        auto* tab = val.data();
        file.write(reinterpret_cast<const char*>(tab), sizeof(T) * size);
    }

    static void write(const std::string& val, std::ofstream& file)
    {
        size_t size = val.size();
        write(size, file);
        //        val.resize(size);
        file.write(const_cast<const char*>(val.data()), size * sizeof(char));
    }

    //    static void read(int & val, std::ifstream & file) {

    //    }
};

#endif // FILE_IO_H
