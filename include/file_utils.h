#pragma once

#include <string>
#include <vector>

// Funci�n para leer un archivo binario en un vector de bytes
std::vector<uint8_t> readBinaryFile(const std::string& filePath);

// Funci�n para escribir un archivo binario
void writeBinaryFile(const std::string& filePath, const std::vector<uint8_t>& data);
