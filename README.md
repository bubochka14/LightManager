# LightManager
LightManager - разработан на Qt версии 6.5 и используя C++17.  Данная программа предназначена для управления источником света в двух режимах: ручном, с помощью циферблата и автоматическом, с пмощью сенсора.
## Сборка
```
git submodule update --init --recursive
mkdir build && cd build
cmake --build . --config Release --target app
```
