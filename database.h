#pragma once
#include <iostream>
#include <fstream>
#include <cstdio>
#include <cstring>
#include <sys\stat.h>
#include <QDebug>

#ifndef DATABASE_H
#define DATABASE_H

extern const int InstanceFieldCount;
extern const int InstanceCountPosition;
extern const int KeyMaxLength;
extern const int MasterPasswordMaxLength;
extern const char * MainKey;
extern const int MaxNameCardLength;
extern const char * FileNameText;
extern const int MaxStringCard;

enum InstanceFormat { ServiceName, Password, Login, URL };
enum Coding { Encryption, Decryption };

extern const char * SalvatoryFileName;
struct SalvatoryHeader
{
    int  Offset  = sizeof(SalvatoryHeader);
    int  InstanceCount = 0;
    int  FileSize = sizeof(SalvatoryHeader);
    char Key[33];
    char MasterPassword[17];
};
//чтение из текстового файла кол-во записей
 std::fstream ride_string_number(int &n);

//запись в текстовый файл
void write_string(char *NameCard);

//удаление из текстового файла
void txt_delete_card(int numCard);

//изменение карточки в текстовом файле
void TextModify(int numCard, bool FlagModifyNameCard, char* NameCard);

  //чтение названия элементов
  void ride_string(std::fstream TextFile,char *StringName);

  bool GetFlag (std::fstream * SalvatoryFile); //получение флага

  void GetMainPass (std::fstream * SalvatoryFile, char MasterPassword[]);//получение главного пароля

  unsigned char * GetCodeStream(char ** Instance, int & CodeStreamLength, int & CodeStreamLength2);//записываем карточку в двумерный массив

  char * Coder(char ** StringM, int StringCount, int &CodeStreamLength);//записываем карточку в двумерный массив
  char ** Decoder(char * CodeStream, int &StringCount);

  std::fstream * SalvatoryOpen(const char * FileName, SalvatoryHeader * Header);//открываем бинарник

  unsigned char * InstanceRead(std::fstream * SalvatoryFile, int InstanceNumber, int & CodeStreamLength);//чтение карточки

  bool InstanceDelete(std::fstream * SalvatoryFile, int InstanceNumber, SalvatoryHeader & Header);//удаляем карточку

  bool InstanceAdd(std::fstream * SalvatoryFile,  SalvatoryHeader & Header, unsigned char * EncriptedCodeStream, unsigned int EncriptedCodeStreamLength);//записываем карточку в бинарник

  //изменение
  bool InstanceModify(std::fstream * SalvatoryFile, int InstanceNumber, SalvatoryHeader & Header, unsigned char * EncriptedModifiedCodeStream, unsigned int EncriptedModifiedCodeStreamLength);

  //тяжелое шифрование
  //шифрование внутренняя подпрограмма
  void encryptionPassword(unsigned char *RES, const char key[], unsigned char pass[]);
  //основное шифрование
  void encryptionAndCut(const char key[], unsigned char pass[]);
  //расшифровка главная функция
  void decryptionAndGlue(const char key[], unsigned char pass[]);
  //расшифровка
  void decryptionPassword(unsigned char *RES,const char key[], unsigned char pass[]);

  //легкое шифрование
  //шифровка
  bool InstanceEncription(unsigned char * OutputCodeStream, int InputCodeStreamLength);
  //расшифровка
  bool InstanceDecription(unsigned char * OutputCodeStream, int InputCodeStreamLength);


#endif // DATABASE_H
