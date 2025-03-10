// ННГУ, ВМК, Курс "Методы программирования-2", С++, ООП
//
// tset.cpp - Copyright (c) Гергель В.П. 04.10.2001
//   Переработано для Microsoft Visual Studio 2008 Сысоевым А.В. (19.04.2015)
//
// Множество - реализация через битовые поля

#include "tset.h"
#include <string>

TSet::TSet(int mp) : BitField(mp)
{
    MaxPower = mp;
}

// конструктор копирования
TSet::TSet(const TSet& s) : BitField(s.BitField)
{
    MaxPower = s.MaxPower;
}

// конструктор преобразования типа
TSet::TSet(const TBitField& bf) : BitField(bf)
{
    MaxPower = bf.GetLength();
}

TSet::operator TBitField()
{
    return BitField;
}

int TSet::GetMaxPower(void) const // получить макс. к-во эл-тов
{
    return MaxPower;
}

int TSet::IsMember(const int Elem) const // элемент множества?
{
    if (Elem > (MaxPower - 1) || Elem < 0) {
        throw out_of_range("Element is not in Universe");
    }
    return BitField.GetBit(Elem);
}

void TSet::InsElem(const int Elem) // включение элемента множества
{
    if (Elem > (MaxPower - 1) || Elem < 0) {
        throw out_of_range("Element is not in Universe");
    }
    BitField.SetBit(Elem);
}

void TSet::DelElem(const int Elem) // исключение элемента множества
{
    if (Elem > (MaxPower - 1) || Elem < 0) {
        throw out_of_range("Element is not in Universe");
    }
    BitField.ClrBit(Elem);
}

// теоретико-множественные операции

TSet& TSet::operator=(const TSet& s) // присваивание
{
    if (this != &s) {
        MaxPower = s.MaxPower;
        BitField = s.BitField;
    }
    return *this;
}

int TSet::operator==(const TSet& s) const // сравнение
{
    if (MaxPower != s.MaxPower || BitField != s.BitField) {
        return 0;
    }
    return 1;
}

int TSet::operator!=(const TSet& s) const // сравнение
{
    return !(*this == s);
}

TSet TSet::operator+(const TSet& s) // объединение
{
    TSet ans(BitField | s.BitField);
    return ans;
}

TSet TSet::operator+(const int Elem) // объединение с элементом
{
    TSet ans = *this;
    ans.InsElem(Elem);
    return ans;
}

TSet TSet::operator-(const int Elem) // разность с элементом
{
    TSet ans = *this;
    ans.DelElem(Elem);
    return ans;
}

TSet TSet::operator*(const TSet& s) // пересечение
{
    TSet ans(BitField & s.BitField);
    return ans;
}

TSet TSet::operator~(void) // дополнение
{
    TSet ans(~BitField);
    return ans;
}

// перегрузка ввода/вывода

istream& operator>>(istream& istr, TSet& s) // ввод
{
    int sz;
    istr >> sz;
    int Elem;
    for (int i = 0; i < s.MaxPower; i++) {
        s.DelElem(i);
    }
    for (int i = 0; i < sz; i++) {
        istr >> Elem;
        s.InsElem(Elem);
    }
    return istr;
}

ostream& operator<<(ostream& ostr, const TSet& s) // вывод
{
    string output = "";
    int counter = 0;
    for (int i = 0; i < s.MaxPower; i++) {
        if (s.IsMember(i)) {
            counter++;
            output += std::to_string(i) + " ";
        }
    }
    output = std::to_string(counter) + " " + output;
    ostr << output;
    return ostr;
}
