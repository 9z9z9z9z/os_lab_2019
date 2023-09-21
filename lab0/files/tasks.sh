cd lab0
mkdir hello
cd hello
touch empty
cp ../src/hello.c hello.c
mv hello.c newhello.c
sh ../../update.sh
gcc newhello.c -o hello_world
./hello_world
git add .
git commit -m "добавлены файлы, созданные во время выполнения лабораторной работы"
git push