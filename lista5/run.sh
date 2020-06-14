#!/bin/bash

echo -e "********************************* ZADANIE 1 *********************************\n"

echo -e "---------------------- dane wejściowe: ----------------------\n"

while IFS= read -r line; do
  echo "$line"
done <comand1.txt

echo -e "--------------------- wyniki działania programu --------------------- \n"

python3 zad1.py <comand1.txt

echo -e "********************************* ZADANIE 2 *********************************\n"

echo -e "---------------------- dane wejściowe: ----------------------\n"

while IFS= read -r line; do
  echo "$line"
done <comand2.txt

echo -e "--------------------- wyniki działania programu --------------------- \n"

python3 zad2.py <comand2.txt

echo -e "********************************* ZADANIE 3 *********************************\n"

echo -e "---------------------- dane wejściowe: ----------------------\n"

while IFS= read -r line; do
  echo "$line"
done <comand3.txt

echo -e "--------------------- wyniki działania programu --------------------- \n"

echo -e "--------------- algorytm Prima ------------------ \n"

python3 zad3.py -p <comand3.txt

echo -e "--------------- algorytm Kruskala ------------------ \n"

python3 zad3.py -k <comand3.txt


echo -e "********************************* ZADANIE 4 *********************************\n"

echo -e "---------------------- dane wejściowe: ----------------------\n"

while IFS= read -r line; do
  echo "$line"
done <comand4.txt

echo -e "--------------------- wyniki działania programu --------------------- \n"

python3 zad4.py <comand4.txt