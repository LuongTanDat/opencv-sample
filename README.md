# opencv-sample

- Makefile

```bash
make -j$(nproc)
```

- CMakeList

```bash
mkdir build
cd build
cmake ..
make -j$(nproc)
```

- Command Line

```bash
g++ main.cpp -o app -std=c++11 `pkg-config --cflags --libs opencv`
```