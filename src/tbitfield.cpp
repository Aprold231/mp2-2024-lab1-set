// ННГУ, ВМК, Курс "Методы программирования-2", С++, ООП
//
// tbitfield.cpp - Copyright (c) Гергель В.П. 07.05.2001
//   Переработано для Microsoft Visual Studio 2008 Сысоевым А.В. (19.04.2015)
//
// Битовое поле

#include "tbitfield.h"

TBitField::TBitField(int len)
{
	if (len <= 0) {
		throw out_of_range("Invalid lenght, use positive numbers only");
	}
	BitLen = len;
	MemLen = (BitLen + sizeof(TELEM) * 8 - 1) / (sizeof(TELEM) * 8);
	pMem = new TELEM[MemLen];
	memset(pMem, 0, MemLen * sizeof(TELEM));
}

TBitField::TBitField(const TBitField& bf) // конструктор копирования
{
	BitLen = bf.BitLen;
	MemLen = bf.MemLen;
	pMem = new TELEM[MemLen];
	for (int i = 0; i < MemLen; i++) {
		pMem[i] = bf.pMem[i];
	}
}

TBitField::~TBitField() // деструктор
{
	delete pMem;
}

int TBitField::GetMemIndex(const int n) const // индекс Мем для бита n
{
	if (n < 0 || n >= BitLen) {
		throw out_of_range("mem index out of range");
	}
	return n / (sizeof(TELEM) * 8);
}

TELEM TBitField::GetMemMask(const int n) const // битовая маска для бита n
{
	if (n < 0 || n >= BitLen) {
		throw out_of_range("bitmask index out of range");
	}
	return (TELEM{ 1 }) << (n % (sizeof(TELEM) * 8));
}

// доступ к битам битового поля

int TBitField::GetLength(void) const // получить длину (к-во битов)
{
	return BitLen;
}

void TBitField::SetBit(const int n) // установить бит
{
	if (n < 0 || n >= BitLen) {
		throw out_of_range("set bit index out of range");
	}
	pMem[GetMemIndex(n)] |= GetMemMask(n);
}

void TBitField::ClrBit(const int n) // очистить бит
{
	if (n < 0 || n >= BitLen) {
		throw out_of_range("clear bit index out of range");
	}
	pMem[GetMemIndex(n)] &= ~GetMemMask(n);
}

int TBitField::GetBit(const int n) const // получить значение бита
{
	if (n < 0 || n >= BitLen) {
		throw out_of_range("get bit index out of range");
	}
	return((pMem[GetMemIndex(n)] & GetMemMask(n)) != 0);
}

// битовые операции

TBitField& TBitField::operator=(const TBitField& bf) // присваивание
{
	if (this != &bf) {
		if (bf.MemLen != MemLen) {
			delete[] pMem;
			pMem = new TELEM[bf.MemLen];
		}
		BitLen = bf.BitLen;
		MemLen = bf.MemLen;
		for (int i = 0; i < MemLen; i++) {
			pMem[i] = bf.pMem[i];
		}
	}
	return *this;
}

int TBitField::operator==(const TBitField& bf) const // сравнение
{
	int MinMemLen = min(MemLen, bf.MemLen);
	int MaxMemLen = max(MemLen, bf.MemLen);
	for (int i = 0; i < MinMemLen;i++) {
		if (pMem[i] != bf.pMem[i]) {
			return 0;
		}
	}
	if (MemLen == MaxMemLen) {
		for (int i = MinMemLen; i < MaxMemLen; i++) {
			if (pMem[i] != (TELEM{ 0 }))
				return 0;
		}
	}
	else {
		for (int i = MinMemLen; i < MaxMemLen; i++) {
			if (bf.pMem[i] != (TELEM{ 0 }))
				return 0;
		}
	}
	return 1;
}

int TBitField::operator!=(const TBitField& bf) const // сравнение
{
	return !(*this == bf);
}

TBitField TBitField::operator|(const TBitField& bf) // операция "или"
{
	TBitField ans(max(BitLen, bf.BitLen));
	int MinMemLen = min(MemLen, bf.MemLen);
	int MaxMemLen = max(MemLen, bf.MemLen);
	for (int i = 0; i < MinMemLen;i++) {
		ans.pMem[i] = pMem[i] | bf.pMem[i];
	}
	if (MemLen == MaxMemLen) {
		for (int i = MinMemLen; i < MaxMemLen; i++) {
			ans.pMem[i] = pMem[i];

		}
	}
	else {
		for (int i = MinMemLen; i < MaxMemLen; i++) {
			ans.pMem[i] = bf.pMem[i];
		}
	}
	return ans;
}

TBitField TBitField::operator&(const TBitField& bf) // операция "и"
{
	TBitField ans(max(BitLen, bf.BitLen));
	int MinMemLen = min(MemLen, bf.MemLen);
	for (int i = 0; i < MinMemLen;i++) {
		ans.pMem[i] = pMem[i] & bf.pMem[i];
	}
	return ans;
}

TBitField TBitField::operator~(void) // отрицание
{
	TBitField ans(BitLen);
	for (int i = 0; i < MemLen; i++) {
		ans.pMem[i] = ~pMem[i];
	}
	if ((BitLen % (sizeof(TELEM) * 8)) != 0) {
		for (int i = BitLen; i < MemLen * sizeof(TELEM) * 8; i++)
			ans.pMem[MemLen - 1] &= ~((TELEM{ 1 }) << (i % (sizeof(TELEM) * 8)));
	}
	return ans;
}

// ввод/вывод

istream& operator>>(istream& istr, TBitField& bf) // ввод
{
	string str;
	istr >> str;
	int len = str.size();

	if (len != bf.BitLen) {
		throw out_of_range("wrong length");
	}
	for (int i = 0; i < len; i++) {
		if (str[i] == '1') {
			bf.SetBit(i);
		}
		else if (str[i] == '0') {
			bf.ClrBit(i);
		}
		else {
			throw exception("only ones or zeros!");
		}
	}
	return istr;
}

ostream& operator<<(ostream& ostr, const TBitField& bf) // вывод
{
	for (int i = 0; i < bf.BitLen; i++) {
		ostr << bf.GetBit(i);
	}
	return ostr;
}
