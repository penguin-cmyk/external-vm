//
// Created by sigma on 01/08/2025.
//

#ifndef MEM_H
#define MEM_H

#include <iostream>
#include <windows.h>
#include <tlhelp32.h>
#include <vector>

class Memory {
private:
    static uintptr_t getBaseAddress(DWORD processId)
    {
        HANDLE snapshot = CreateToolhelp32Snapshot(TH32CS_SNAPMODULE | TH32CS_SNAPMODULE32, processId);
        if (snapshot == INVALID_HANDLE_VALUE) {
            return 0;
        }

        MODULEENTRY32 entry;
        entry.dwSize = sizeof(MODULEENTRY32);

        if (Module32First(snapshot, &entry) == 0) {
            CloseHandle(snapshot);
            return 0;
        }

        CloseHandle(snapshot);
        return (uintptr_t)(entry.modBaseAddr);
    }

    static DWORD getProcessId(const std::string& processName )
    {
        HANDLE snapshot = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
        if (snapshot == INVALID_HANDLE_VALUE) {
            return 0;
        }

        PROCESSENTRY32 entry;
        entry.dwSize = sizeof(PROCESSENTRY32);

        if (!Process32First(snapshot, &entry)) {
            CloseHandle(snapshot);
            return 0;
        }

        do {
            if ( processName == entry.szExeFile ) {
                CloseHandle(snapshot);
                return entry.th32ProcessID;
            }
        } while (Process32Next(snapshot, &entry));

        CloseHandle(snapshot);
        return 0;
    }


public:
    uintptr_t BaseAddress;
    DWORD     ProcessId;
    HANDLE    ProcessHandle;

    uintptr_t rebase(uintptr_t address) {
        return BaseAddress + address;
    }

    std::vector<char> readBytes(uintptr_t address, size_t size)
    {
        if (address == 0) return std::vector<char>();

        std::vector<char> buffer(size);
        SIZE_T bytesRead;

        if (ReadProcessMemory(ProcessHandle, (LPCVOID)address, buffer.data(), size, &bytesRead)) {
            if (bytesRead == size) {
                return buffer;
            }
        }

        return std::vector<char>();
    }

    template<typename T>
    T readMem(uintptr_t address) {
        if (address == 0) return T{};

        T buffer;
        SIZE_T bytesRead;

        if (ReadProcessMemory(ProcessHandle, (LPCVOID)address, &buffer, sizeof(T), &bytesRead)) {
            if (bytesRead == sizeof(T)) {
                return buffer;
            }
        }

        return T{};
    }

    std::string readString(uintptr_t address)
    {
        if (address == 0) return "";

        size_t length = readMem<size_t>(address + 0x10);
        if (length == 0) return "";

        uintptr_t stringPointer = address;
        if (length >= 15) {
            stringPointer = readMem<uintptr_t>(address);
            if (stringPointer == 0) return "";
        }

        auto buffer = readBytes(stringPointer, length);
        if (buffer.empty()) return "";

        return std::string(buffer.begin(), buffer.end());
    }

    uintptr_t allocate(uintptr_t amount) {
        return (uintptr_t)VirtualAllocEx(ProcessHandle, NULL, amount, MEM_COMMIT | MEM_RESERVE, PAGE_READWRITE);
    }

    void writeString(uintptr_t address, const char* value) {
        if (address == 0) return;

        size_t length = strlen(value);

        writeMem<size_t>(address + 0x10, length);

        size_t bytesWritten;
        if (length >= 15) {
            uintptr_t allocated = allocate(length + 1);
            if (allocated == 0) return;

            WriteProcessMemory(ProcessHandle, (LPVOID)(allocated), value, length  + 1, &bytesWritten);
            writeMem<uintptr_t>(address, allocated);
        } else {
            WriteProcessMemory(ProcessHandle, (LPVOID)(address), value, length  + 1, &bytesWritten);
        }
    }

    template<typename T>
    bool writeMem(uintptr_t address, const T& value)
    {
        if (address == 0) return false;

        SIZE_T bytesWritten;
        return WriteProcessMemory(ProcessHandle, (LPVOID)address, &value, sizeof(T), &bytesWritten)
               && bytesWritten == sizeof(T);
    }

    explicit Memory(const std::string& processName)
    {
        ProcessId = getProcessId(processName);
        BaseAddress = getBaseAddress(ProcessId);
        ProcessHandle = OpenProcess(PROCESS_VM_READ | PROCESS_QUERY_INFORMATION | PROCESS_VM_WRITE | PROCESS_VM_OPERATION, 0, ProcessId);
    }

    bool isValid()
    {
        return ProcessHandle != nullptr && BaseAddress != 0;
    }

    ~Memory() {
        if (ProcessHandle) CloseHandle(ProcessHandle);
    }


};

#endif // MEM_H