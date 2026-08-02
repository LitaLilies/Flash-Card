# Build instructions for Windows

## ✅ MinGW (Recommended for Windows)

### 1. Download & Install MinGW-w64
- Visit: https://www.mingw-w64.org/
- Or use: https://github.com/niXman/mingw-builds-binaries/releases

### 2. Compile
```cmd
cd flashcard-app
g++ -std=c++17 -o flashcard.exe main.cpp flashcard.cpp ui.cpp storage.cpp
```

### 3. Run
```cmd
flashcard.exe
```

---

## ✅ Visual Studio MSVC

### 1. Open Visual Studio Developer Command Prompt

### 2. Compile
```cmd
cd flashcard-app
cl /EHsc /utf-8 /std:c++17 main.cpp flashcard.cpp ui.cpp storage.cpp
```

### 3. Run
```cmd
main.exe
```

---

## ✅ CMake + Visual Studio

### 1. Install CMake from https://cmake.org/download/

### 2. Configure
```cmd
cd flashcard-app
mkdir build
cd build
cmake .. -G "Visual Studio 17 2022"
```

### 3. Build
```cmd
cmake --build . --config Release
```

### 4. Run
```cmd
Release\flashcard.exe
```

---

## ✅ Linux/macOS

```bash
cd flashcard-app
g++ -std=c++17 -o flashcard main.cpp flashcard.cpp ui.cpp storage.cpp
./flashcard
```

---

## 📝 Notes

- Requires **C++17** or later
- All source files use UTF-8 encoding (for Vietnamese support)
- No external dependencies (pure C++ standard library)

## 🐛 Troubleshooting

**Q: "g++: command not found"**
- A: Install MinGW or GCC. See https://www.mingw-w64.org/

**Q: Compilation errors with Unicode**
- A: Add `/utf-8` flag for MSVC or ensure your editor saves as UTF-8

**Q: "cannot find -lstdc++fs"**
- A: Use `-std=c++17` flag (std::filesystem is included)
