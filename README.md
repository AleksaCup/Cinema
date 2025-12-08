# Cinema

Pokretanje na Windowsu

# 1) Instalacija GLFW preko vcpkg
cd C:\path\to\vcpkg
.\vcpkg install glfw3

# 2) Ulazak u projekat
cd C:\path\to\Cinema

# 3) Generisanje builda preko Cmake
mkdir build
cd build
cmake .. -DCMAKE_TOOLCHAIN_FILE=C:\path\to\vcpkg\scripts\buildsystems\vcpkg.cmake

# 4) Build (Debug konfiguracija)
cmake --build . --config Debug

# 5) Pokretanje
cd Debug
.\Cinema.exe