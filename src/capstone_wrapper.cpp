#include "capstone_wrapper.h"
#include <iostream>
#include <capstone/capstone.h>

void disassembleCode(const std::vector<uint8_t>& binaryCode) {
    // Inicializar Capstone para la arquitectura x86 (ajusta seg�n sea necesario)
    csh handle;
    cs_err err = cs_open(CS_ARCH_X86, CS_MODE_32, &handle); // Cambia a CS_MODE_64 si es arquitectura de 64 bits
    if (err != CS_ERR_OK) {
        std::cerr << "Error al inicializar Capstone: " << cs_strerror(err) << std::endl;
        return;
    }

    // Desensamblar el c�digo binario
    cs_insn *insn;
    size_t count = cs_disasm(handle, binaryCode.data(), binaryCode.size(), 0x1000, 0, &insn);
    if (count > 0) {
        for (size_t i = 0; i < count; i++) {
            // Imprimir la instrucci�n desensamblada
            std::cout << "0x" << std::hex << insn[i].address << ":\t" << insn[i].mnemonic << "\t" << insn[i].op_str << std::endl;
        }
        cs_free(insn, count);
    } else {
        std::cerr << "No se pudo desensamblar el c�digo." << std::endl;
    }

    // Cerrar Capstone
    cs_close(&handle);
}
