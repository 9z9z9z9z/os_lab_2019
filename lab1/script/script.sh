../src/background.sh &
cd ../src/
touch ../src/file.txt
cat > ../src/file.txt # abcdefg
cat ../src/file.txt
# clear
wc -c ../src/file.txt
touch with_cake.txt | grep "cake" cake_rhymes.txt > with_cake.txt
touch 1.txt | rm 1.txt >/dev/null
chmod +x hello.sh
touch bin.sh | chmod +x bin.sh
touch average.sh | chmod +x average.sh
touch numbers.txt | shuf -i 1-1000 -n 150 > numbers.txt