
<div align="center">

![C++](https://img.shields.io/badge/c++-%2300599C.svg?style=for-the-badge&logo=c%2B%2B&logoColor=white)
![Lua](https://img.shields.io/badge/lua-%232C2D72.svg?style=for-the-badge&logo=lua&logoColor=white)
![CMake](https://img.shields.io/badge/CMake-%23008FBA.svg?style=for-the-badge&logo=cmake&logoColor=white)
[![GitHub](https://img.shields.io/badge/github-%23121011.svg?style=for-the-badge&logo=github&logoColor=white)](https://github.com/penguin-cmyk/external-vm)

*A memory-based recreation of Roblox's Luau environment*

</div>

---

## 📋 Overview

**External VM** is a project that recreates Roblox's Luau environment externally by reading and writing memory directly. While this implementation uses standard Lua instead of Luau, it aims to replicate the core functionality and behavior of Roblox's scripting environment.

## 🛠️ Building the Project

### Prerequisites

- **CMake** (3.15 or higher)
- **Visual Studio** (2019 or later recommended)
- **Git**

### Build Instructions

#### Using CLion

1. Open the project in CLion
2. At the top of the IDE, locate the build configuration dropdown (shows "Release" or "Debug")
3. Click on it and select **"Edit CMake Profiles"**
4. Under **"Toolchain"**, select **"Visual Studio"** (not MinGW)
5. Apply changes and build the project

#### Using Visual Studio

1. Open Visual Studio
2. Select **"Open a local folder"**
3. Navigate to and select your project directory
4. Visual Studio will automatically detect the CMake project
5. Go to **Project → CMake Settings** to configure if needed
6. Build using **Build → Build All**

#### Using Command Line

```bash
# Clone the repository
git clone https://github.com/penguin-cmyk/external-vm.git
cd external-vm

# Configure the project with Visual Studio generator
cmake -G "Visual Studio 17 2022" -A x64 -B build

# Build the project
cmake --build build --config Release