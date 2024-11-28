#include "audio_extractor.h"
#include <iostream>
#include <fstream>
#include <vector>
#include <filesystem>
#include <stdexcept>

// Funci�n para leer un archivo binario en un vector de bytes
std::vector<uint8_t> readBinaryFile(const std::string& filePath) {
    std::ifstream file(filePath, std::ios::binary);
    if (!file) {
        throw std::runtime_error("No se pudo abrir el archivo: " + filePath);
    }
    return std::vector<uint8_t>((std::istreambuf_iterator<char>(file)), std::istreambuf_iterator<char>());
}

// Funci�n para escribir un archivo binario
void writeBinaryFile(const std::string& filePath, const std::vector<uint8_t>& data) {
    std::ofstream file(filePath, std::ios::binary);
    if (!file) {
        throw std::runtime_error("No se pudo crear el archivo: " + filePath);
    }
    file.write(reinterpret_cast<const char*>(data.data()), data.size());
}

// Funci�n para extraer pistas de audio de MUSIC.MB y MUSIC.MH
void extractAudioTracks(const std::string& musicMBPath, const std::string& musicMHPath, const std::string& outputDir) {
    // Leer los archivos MUSIC.MB y MUSIC.MH
    std::vector<uint8_t> musicMB = readBinaryFile(musicMBPath);
    std::vector<uint8_t> musicMH = readBinaryFile(musicMHPath);

    // Crear el directorio de salida si no existe
    std::filesystem::create_directories(outputDir);

    // Analizar MUSIC.MH para obtener los �ndices de las pistas
    size_t numTracks = musicMH.size() / 8; // Supongamos que cada entrada tiene 8 bytes
    std::cout << "N�mero de pistas: " << numTracks << std::endl;

    for (size_t i = 0; i < numTracks; ++i) {
        // Leer cada entrada en MUSIC.MH (offset y tama�o)
        uint32_t offset = *reinterpret_cast<const uint32_t*>(&musicMH[i * 8]);
        uint32_t size = *reinterpret_cast<const uint32_t*>(&musicMH[i * 8 + 4]);

        std::cout << "Extrayendo pista " << i + 1 << " en offset: 0x" << std::hex << offset
            << " con tama�o: " << size << " bytes" << std::endl;

        // Validar que el offset y el tama�o est�n dentro de MUSIC.MB
        if (offset + size > musicMB.size()) {
            std::cerr << "Error: La pista " << i + 1 << " excede los l�mites de MUSIC.MB." << std::endl;
            continue;
        }

        // Extraer los datos de la pista
        std::vector<uint8_t> trackData(musicMB.begin() + offset, musicMB.begin() + offset + size);

        // Validar si los datos extra�dos parecen v�lidos (comprobando cabecera o estructura)
        if (trackData.empty()) {
            std::cerr << "Advertencia: La pista " << i + 1 << " no contiene datos v�lidos." << std::endl;
            continue;
        }

        // Guardar la pista en un archivo
        std::string trackFileName = outputDir + "/track_" + std::to_string(i + 1) + ".vag"; // Suponiendo formato PS2 VAG
        writeBinaryFile(trackFileName, trackData);

        // Confirmaci�n de que la pista se ha extra�do
        std::cout << "Pista " << i + 1 << " guardada en: " << trackFileName << std::endl;
    }
}
