# Создание статической библиотеки
gcc -c revert_string.c -o reverse.o
ar rcs libReverseString.a reverse.o
# Создание динамической библиотеки
gcc -shared -o libDynamicReverseString.so reverse.o
# Компиляция статической библиотеки
gcc main.c -o mainStatic -L. -lReverseString
# Компиляция динамической библиотеки 
gcc main.c -o mainDynamic -L. -lDynamicReverseString -Wl,-rpath,.
sudo apt -y install libcunit1 libcunit1-doc libcunit1-dev
