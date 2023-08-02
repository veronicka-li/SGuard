#include "database.h"
const int InstanceFieldCount = 3;
const int InstanceCountPosition = 4;
const int KeyMaxLength = 33;
const int MasterPasswordMaxLength = 17;
const char * MainKey = "0123456789ABCDEFFEDCBA987654321\0";
const char * SalvatoryFileName = "Salvatory.bin";
const char * FileNameText = "Text.txt";
const int MaxNameCardLength = 30;
const int MaxStringCard = 150;

//чтение из текстового файла кол-во записей
 std::fstream ride_string_number(int &n)
{
    std::fstream TextFile(FileNameText, std::ios::in);
        TextFile >> n;
  return TextFile;
}

 //удаление карточки
 void txt_delete_card(int numCard)
 {
     int flagDelete = 0;
     std::fstream TextFile(FileNameText, std::ios::out | std::ios::in);
     if (!TextFile.fail())
     {
         int n;
         TextFile >> n;
         char **Buffer = new char*[n+1];
         for (int i = 0; i < n+1; i++)
              Buffer[i] = new char[MaxNameCardLength];
         for (int i = 0; i < n+1; i++)
         {
             if(i-1 != numCard)
             {
            //     Buffer[i-flagDelete] = new char[MaxNameCardLength];
                 TextFile.getline(Buffer[i-flagDelete], MaxNameCardLength);
             }
             else
             {
            //     Buffer[n+1] = new char[MaxNameCardLength];
                 TextFile.getline(Buffer[n], MaxNameCardLength);
                 flagDelete=1;
             }
         }
         n--;
         TextFile.seekp(0, std::ios::beg);
         TextFile << n << std::endl;
         for (int i = 1; i < n+1; i++) {
             TextFile << Buffer[i] << std::endl;
         }
         TextFile << std::endl;
         for (int i = 0; i < n+2; i++)
             delete[] Buffer[i];
         delete[] Buffer;
         TextFile.close();
     }
 }
//запись в текстовый файл
void write_string (char *NameCard)
{
    int NameCardLength = strlen(NameCard);
    struct stat statBuff;

    if (stat(FileNameText, &statBuff) < 0)
    {
        std::fstream TextFile(FileNameText, std::ios::out);
        if (errno == ENOENT)
        {
            if (!TextFile.fail())
            {
                int n = 1;
                TextFile << n << std::endl;
                for (int i = 0; i < NameCardLength; i++)
                {
                    TextFile << NameCard[i];
                }
                TextFile << std::endl;
                TextFile.close();
            }
        }
    }
    else {
        std::fstream TextFile(FileNameText, std::ios::out | std::ios::in);
        if (!TextFile.fail())
        {
            int n;
            TextFile >> n;
            char **Buffer = new char*[n+1];
            for (int i = 0; i < n+1; i++)
            {
                Buffer[i] = new char[MaxNameCardLength];
                TextFile.getline(Buffer[i], MaxNameCardLength);
            }
            n++;
            TextFile.seekp(0, std::ios::beg);
            TextFile << n << std::endl;
            for (int i = 1; i < n; i++) {
                TextFile << Buffer[i] << std::endl;
            }
            for (int i = 0; i < NameCardLength; i++)
            {
                TextFile << NameCard[i];
            }
            TextFile << std::endl;
            for (int i = 0; i < n; i++)
                delete[] Buffer[i];
            delete[] Buffer;
            TextFile.close();
        }
    }
}
//изменение карточки
void TextModify(int numCard, bool FlagModifyNameCard, char* NameCard)
{
    int flagDelete = 0;
    std::fstream TextFile(FileNameText, std::ios::out | std::ios::in);
    if (!TextFile.fail())
    {
        int n;
        TextFile >> n;
        char **Buffer = new char*[n+1];
        for (int i = 0; i < n+1; i++)
        {
            if(i-1 != numCard)
            {
                Buffer[i-flagDelete] = new char[MaxNameCardLength];
                TextFile.getline(Buffer[i-flagDelete], MaxNameCardLength);
            }
            else
            {
                Buffer[n] = new char[MaxNameCardLength];
                TextFile.getline(Buffer[n], MaxNameCardLength);
                flagDelete=1;
            }
        }
        TextFile.seekp(0, std::ios::beg);
        TextFile << n << std::endl;
        for (int i = 1; i < n+1; i++) {
            if ((i==n)&&FlagModifyNameCard)
                TextFile << NameCard << std::endl;
        else
                TextFile << Buffer[i] << std::endl;
        }
        TextFile << std::endl;
        for (int i = 0; i < n+1; i++)
            delete[] Buffer[i];
        delete[] Buffer;
        TextFile.close();
    }
}


void GetMainPass (std::fstream * SalvatoryFile, char MasterPassword[])
{
    SalvatoryHeader CurrentHeader;
    SalvatoryFile->seekg(0, std::ios::beg);
    SalvatoryFile->read((char *)&CurrentHeader, CurrentHeader.Offset);
    int MasterPasswordLength = strlen(CurrentHeader.MasterPassword);
    for (int i = 0; i <MasterPasswordMaxLength; i++)
    {
        if (i < MasterPasswordLength) MasterPassword[i]=CurrentHeader.MasterPassword[i];
        else MasterPassword[i] = 0;
    }
}

unsigned char * GetCodeStream(char ** Instance, int & CodeStreamLength, int & CodeStreamLength2) //записываем карточку в двумерный массив
{
    char * CodeStream = 0;
    CodeStreamLength = 0;

    int * InstanceFieldLengths = new int[InstanceFieldCount];
    InstanceFieldLengths[0] = 0;

    for (int i = 0; i < InstanceFieldCount; i++)
    {
        InstanceFieldLengths[i] = strlen(Instance[i]);
        CodeStreamLength += InstanceFieldLengths[i];
    }
    CodeStreamLength2=CodeStreamLength+3;
    CodeStreamLength +=  InstanceFieldCount * sizeof(unsigned int);
    CodeStream = new char[CodeStreamLength + 1];
    strncat_s(CodeStream, CodeStreamLength, (char *)(InstanceFieldLengths), sizeof(unsigned int));
    for (int i = 0; i <  InstanceFieldCount; i++)
    {
        strncat_s(CodeStream, CodeStreamLength, (char *)(InstanceFieldLengths+i), sizeof(unsigned int));
        strcat_s(CodeStream,  CodeStreamLength, Instance[i]);
    }

    delete[] InstanceFieldLengths;

    return (unsigned char *)CodeStream;
}

char * Coder(char ** StringM, int StringCount, int &CodeStreamLength)
{
    CodeStreamLength = sizeof(StringCount);
    int * StringMLength = 0;
    if (StringCount > 0) StringMLength = new int[StringCount];

    for (int i = 0; i < StringCount; i++)
    {
        StringMLength[i] = strlen(StringM[i]);
        CodeStreamLength += StringMLength[i];
    }

    CodeStreamLength += StringCount * sizeof(int);

    char * CodeStream = new char[CodeStreamLength];
    int * CodeStreamIntCurrentPTR = (int *)CodeStream;
    *CodeStreamIntCurrentPTR = StringCount;
    CodeStreamIntCurrentPTR++;
    int CodeStremRestLendth = CodeStreamLength - sizeof(StringCount);

    for (int i = 0; i < StringCount; i++)
    {
        *CodeStreamIntCurrentPTR = StringMLength[i];
        CodeStreamIntCurrentPTR++;
        CodeStremRestLendth -= sizeof(*CodeStreamIntCurrentPTR);

        memcpy_s(CodeStreamIntCurrentPTR, CodeStremRestLendth, StringM[i], StringMLength[i]);

        CodeStreamIntCurrentPTR = (int *)(((char *)CodeStreamIntCurrentPTR) + StringMLength[i]);

        CodeStremRestLendth -= StringMLength[i];
    }

    if (StringMLength != 0) delete[] StringMLength;

    return CodeStream;
}

char ** Decoder(char * CodeStream, int &StringCount)
    {
            int * CodeStreamIntCurrentPTR = (int *)CodeStream;
            StringCount = *CodeStreamIntCurrentPTR;
            CodeStreamIntCurrentPTR++;

            char ** StringM = 0;
            if (StringCount > 0)
            {
                StringM = new char *[StringCount];
                int StringLength;

                for (int i = 0; i < StringCount; i++)
                {
                    StringLength = *CodeStreamIntCurrentPTR;
                    CodeStreamIntCurrentPTR++;

                    StringM[i] = new char[StringLength + 1];
                    memcpy_s(StringM[i], StringLength + 1, CodeStreamIntCurrentPTR, StringLength);
                    StringM[i][StringLength] = 0;
                    CodeStreamIntCurrentPTR = (int *)(((char *)CodeStreamIntCurrentPTR) + StringLength);
                }
            }

            return StringM;
        }

std::fstream * SalvatoryOpen(const char * FileName,  SalvatoryHeader * Header)//открываем бинарник
{
    std::fstream * SalvatoryFile = 0;

    struct stat statBuff;

    if (stat(FileName, &statBuff) < 0)
    {
        if (errno == ENOENT)
        {
            SalvatoryFile = new std::fstream(FileName, std::ios::out | std::ios::binary);

            if (!SalvatoryFile->fail())
            {
                SalvatoryFile->write((char *)Header, Header->Offset);
                SalvatoryFile->seekg(Header->Offset, std::ios::beg);
                SalvatoryFile->flush();
            }
            else SalvatoryFile = 0;
        }
    }
    else
    {
        SalvatoryFile = new std::fstream(FileName, std::ios::binary | std::ios::out | std::ios::in);

        if (!SalvatoryFile->fail())
        {
            SalvatoryFile->read((char *)Header, sizeof( SalvatoryHeader));
            SalvatoryFile->seekp(Header->Offset, std::ios::beg);
        }
        else SalvatoryFile = 0;
    }

    return SalvatoryFile;
}


bool InstanceAdd(std::fstream * SalvatoryFile,  SalvatoryHeader & Header, unsigned char * EncriptedCodeStream, unsigned int EncriptedCodeStreamLength)//записываем карточку в бинарник
{
    bool Rezult = false;

    if (!SalvatoryFile->fail())
    {
        SalvatoryHeader CurrentHeader;
        SalvatoryFile->seekg(0, std::ios::beg);
        SalvatoryFile->read((char *)&CurrentHeader,  CurrentHeader.Offset);

        SalvatoryFile->seekp(CurrentHeader.FileSize, std::ios::beg);

        SalvatoryFile->write((char *)&EncriptedCodeStreamLength, sizeof(EncriptedCodeStreamLength));
        SalvatoryFile->write((char *)EncriptedCodeStream, EncriptedCodeStreamLength);

        SalvatoryFile->seekp(0, std::ios::beg);
        CurrentHeader.InstanceCount++;
        CurrentHeader.FileSize += sizeof(EncriptedCodeStreamLength) + EncriptedCodeStreamLength;
        SalvatoryFile->write((char *)&CurrentHeader, CurrentHeader.Offset);

        Header = CurrentHeader;

        SalvatoryFile->flush();

        Rezult = true;
    }

    return Rezult;
}


unsigned char * InstanceRead(std::fstream * SalvatoryFile, int InstanceNumber, int & CodeStreamLength)//чтение карточки
{
    unsigned char * CodeStream = 0;
    CodeStreamLength = 0;

    if (!SalvatoryFile->fail())
    {
        SalvatoryHeader CurrentHeader;
        SalvatoryFile->seekg(0, std::ios::beg);
        SalvatoryFile->read((char *)&CurrentHeader, CurrentHeader.Offset);

        if (0 <= InstanceNumber && InstanceNumber < CurrentHeader.InstanceCount)
        {
            int Shift = 0;

            for (int i = 0; i < InstanceNumber; i++)
            {
                SalvatoryFile->read((char *)&Shift, sizeof(Shift));
                SalvatoryFile->seekg(Shift, std::ios::cur);
            }

            SalvatoryFile->read((char *)&CodeStreamLength, sizeof(CodeStreamLength));
            CodeStream = new unsigned char[CodeStreamLength];
            SalvatoryFile->read((char *)CodeStream, CodeStreamLength);
        }
    }

    return CodeStream;
}


bool InstanceDelete(std::fstream * SalvatoryFile, int InstanceNumber,  SalvatoryHeader & Header)//удаляем карточку
{
    bool Rezult = false;

    if (!SalvatoryFile->fail())
    {
        SalvatoryHeader CurrentHeader;
        SalvatoryFile->seekg(0, std::ios::beg);
        SalvatoryFile->read((char *)&CurrentHeader, CurrentHeader.Offset);

        if (0 <= InstanceNumber && InstanceNumber < CurrentHeader.InstanceCount)
        {
            int Shift = 0;

            for (int i = 0; i < InstanceNumber; i++)
            {
                SalvatoryFile->read((char *)&Shift, sizeof(Shift));
                SalvatoryFile->seekg(Shift, std::ios::cur);
            }

            int CurrentPosition = SalvatoryFile->tellg();
            int InstanceSize = 0;

            SalvatoryFile->read((char *)&InstanceSize, sizeof(InstanceSize));
            SalvatoryFile->seekg(InstanceSize, std::ios::cur);
            int BufferSize = CurrentHeader.FileSize - (CurrentPosition + InstanceSize + sizeof(InstanceSize));
            unsigned char * Buffer = new unsigned char[BufferSize];
            SalvatoryFile->read((char *)Buffer, BufferSize);
            SalvatoryFile->seekp(CurrentPosition, std::ios::beg);
            SalvatoryFile->write((char *)Buffer, BufferSize);
            delete[] Buffer;

            CurrentHeader.InstanceCount--;
            CurrentHeader.FileSize -= sizeof(InstanceSize) + InstanceSize;
            SalvatoryFile->seekp(0, std::ios::beg);
            SalvatoryFile->write((char *)&CurrentHeader, CurrentHeader.Offset);

            Header = CurrentHeader;

            SalvatoryFile->flush();

            Rezult = true;
        }
    }

    return Rezult;
}
//изменение
bool InstanceModify(std::fstream * SalvatoryFile, int InstanceNumber,  SalvatoryHeader & Header, unsigned char * EncriptedModifiedCodeStream, unsigned int EncriptedModifiedCodeStreamLength)
{
    bool Rezult = false;

    if (!SalvatoryFile->fail())
    {
        SalvatoryHeader CurrentHeader;
        SalvatoryFile->seekg(0, std::ios::beg);
        SalvatoryFile->read((char *)&CurrentHeader, CurrentHeader.Offset);

        if (0 <= InstanceNumber && InstanceNumber < CurrentHeader.InstanceCount)
        {
            if (InstanceDelete(SalvatoryFile, InstanceNumber, CurrentHeader))
            {
                InstanceAdd(SalvatoryFile, CurrentHeader, EncriptedModifiedCodeStream, EncriptedModifiedCodeStreamLength);

                Header = CurrentHeader;

                SalvatoryFile->flush();

                Rezult = true;
            }
        }
    }

    return Rezult;
}
//тяж. расшифровка главная функция
void decryptionAndGlue(const char key[], unsigned char pass[])
{
    unsigned char passRES[17];							// результирующий пароль
    unsigned char passRESenc[9];						// Выходящий пароль из функции шифрования

    int NumberOfSymbol = 0;
    for (int i = 0; i < 16; ++i)						// Считаю количество символов в пароле
            if (pass[i] != 0) NumberOfSymbol++;

        if (NumberOfSymbol == 8)
        {
            unsigned char *RES;								// Указатель на строку из функции шифрования
            RES = passRESenc;
            decryptionPassword(RES, key, pass);

            if (passRESenc[7] == 197)						// Есть ли в пароле дописанные кресты
            {
                int NumberOfCross = 0;
                for (int i = 7; passRESenc[i] == 197; --i)	// Сколько крестов дописано
                    NumberOfCross++;
                passRESenc[8 - NumberOfCross] = '\0';		// Обрезаем кресты
                passRES[8 - NumberOfCross] = '\0';

                for (int i = 0; i < 8 - NumberOfCross; ++i) // Результат функции шифрования записываю в результирующую строку
                {
                    passRES[i] = passRESenc[i];
                }
            }
            else
            {
                for (int i = 0; i < 8; ++i)					// Результат функции шифрования записываю в результирующую строку
                {
                    passRES[i] = passRESenc[i];
                }
                passRES[8] = '\0';
            }
        }
        else
            if (NumberOfSymbol == 16)
            {
                unsigned char pass1Part[9];				// 1 часть пароля
                unsigned char pass2Part[9];				// 2 часть пароля

                for (int i = 0, j = 8; i < 8; ++i, ++j)	// разделываю пароль на 2 части
                    pass2Part[i] = pass[j];
                pass[8] = '\0';

                unsigned char *RES;						// Указатель на строку из функции шифрования
                RES = passRESenc;
                decryptionPassword(RES, key, pass);
                for (int i = 0; i < 8; ++i)				// Результат функции шифрования записываю в результирующую строку
                {
                    passRES[i] = passRESenc[i];
                }

                for (int i = 0; i < 8; ++i)				// 2 часть пароля записываю в строку, идущую в дешифрование
                    pass[i] = pass2Part[i];
                decryptionPassword(RES, key, pass);
                if (passRESenc[7] == 197)				// Есть ли в пароле дописанные кресты
                {
                    int NumberOfCross = 0;
                    for (int i = 7; passRESenc[i] == 197; --i)	// Сколько крестов дописано
                        NumberOfCross++;
                    passRESenc[8 - NumberOfCross] = '\0';		// Обрезаем кресты

                    for (int i = 0, j = 8; i < 8 - NumberOfCross; ++i, ++j) // Результат функции шифрования записываю в результирующую строку
                    {
                        passRES[j] = passRESenc[i];
                    }
                     passRES[16 - NumberOfCross] = '\0';
                }
                else
                {
                    for (int i = 0, j = 8; i < 9; ++i, ++j) // Результат функции шифрования записываю в результирующую строку
                    {
                        passRES[j] = passRESenc[i];
                    }
                }
            }
        strcpy((char *)pass,(char *)passRES);
}
//Тяжел. расшифровка доп.
void decryptionPassword(unsigned char *RES, const char key[], unsigned char pass[])
{
    unsigned char passR[5];			// Правая часть пароля
    unsigned char passL[5];			// Левая часть пароля
    unsigned int numkey[33];		// Собранный 256битный ключ в числа
    unsigned char passkey[5];		// Сумма ключа и правой части
    unsigned char passkeyFrag[9];	// Разбитая на 4битные фрагменты правая часть пароля
    unsigned char keyFrag[5];		// Нужные 4 байта ключа для сложения с правой частью

    //разбиение пароля на 2 блока
    for (int i = 0; i < 4; ++i)						 // Записываем первые 4 байта пароля в левый кусок
        passL[i] = pass[i];

    for (int i = 0, pasi = 4; pasi < 8; ++i, ++pasi) // Записываем вторые 4 байта пароля в правый кусок и его копию
        passR[i] = pass[pasi];

    for (int i = 0; i < 32; ++i)					 // i - счетчик раундов с 0 по 31 (с 1 по 32)
    {
        ////////////// НАЧАЛО ФУНКЦИИ F

        int numRound; // Служит для определения раунда
        if ((i == 0) || (i == 15) || (i == 23) || (i == 31)) // Узнаем номер раунда при дешифровывании
            numRound = 0;
        if ((i == 1) || (i == 14) || (i == 22) || (i == 30))
            numRound = 5;
        if ((i == 2) || (i == 13) || (i == 21) || (i == 29))
            numRound = 9;
        if ((i == 3) || (i == 12) || (i == 20) || (i == 28))
            numRound = 13;
        if ((i == 4) || (i == 11) || (i == 19) || (i == 27))
            numRound = 17;
        if ((i == 5) || (i == 10) || (i == 18) || (i == 26))
            numRound = 21;
        if ((i == 6) || (i == 9) || (i == 17) || (i == 25))
            numRound = 25;
        if ((i == 7) || (i == 8) || (i == 16) || (i == 24))
            numRound = 29;

        for (int j = 0; j < 4; ++j, ++numRound)		// 4 нужные байта в строку
        {
            keyFrag[j] = key[numRound];
        }

        unsigned long keyNum;		// Склеенные 4 байта ключа части в одно число
        keyNum = keyFrag[0] * 16777216 + keyFrag[1] * 65536 + keyFrag[2] * 256 + keyFrag[3];	// Склеивание 4 байтов ключа в одно число

        unsigned long passRNum;		// Склеенные 4 байта правой части в одно число
        passRNum = passR[0] * 16777216 + passR[1] * 65536 + passR[2] * 256 + passR[3];			// Склеивание 4 байтов правой части в одно число

        unsigned long passkeyNum;	// Склеенные 4 байта правой части + ключа в одно число
        if (keyNum + passRNum < 4294967296)
            passkeyNum = keyNum + passRNum;
        else
            if (keyNum + passRNum >= 4294967296)
                passkeyNum = keyNum + passRNum - 4294967296;

        passkey[3] = passkeyNum & 0xFF; // Разложение числа на 4 байта
        passkey[2] = (passkeyNum & 0xFF00) >> 8;
        passkey[1] = (passkeyNum & 0xFF0000) >> 16;
        passkey[0] = (passkeyNum & 0xFF000000) >> 24;

        for (int j = 0, oneHalfJ = 0, twoHalfJ = 1; j < 4; ++j, oneHalfJ += 2, twoHalfJ += 2)	// Разбиение правой части пароля на фрагменты
        {
            passkeyFrag[oneHalfJ] = (passkey[j] & 0xF0) >> 4;									// 1 часть байта в четный элемент строки
            passkeyFrag[twoHalfJ] = passkey[j] & 0x0F;											// 2 часть байта в НЕчетный элемент строки
        }

        /////// НАЧАЛО ФУНКЦИИ S
        unsigned int P[8][16] = // Подстановки
        {
            { 12,4,6,2,10,5,11,9,14,8,13,7,0,3,15,1 }, // П0
            { 6,8,2,3,9,10,5,12,1,14,4,7,11,13,0,15 }, // П1
            { 11,3,5,8,2,15,10,13,14,1,7,4,12,9,6,0 }, // П2
            { 12,8,2,1,13,4,15,6,7,0,10,5,3,14,9,11 }, // П3
            { 7,15,5,10,8,1,6,13,0,9,3,14,11,4,2,12 }, // П4
            { 5,13,15,6,9,2,12,10,11,7,8,1,4,3,14,0 }, // П5
            { 8,14,2,5,8,9,1,12,15,4,11,0,13,10,3,7 }, // П6
            { 1,7,14,13,0,5,8,3,4,15,10,6,9,12,11,2 }  // П7
        };

        for (int j = 0, numP = 7; j < 8; ++j, --numP) // Цикл подстановок
            passkeyFrag[j] = P[numP][passkeyFrag[j]];
        /////// КОНЕЦ ФУНКЦИИ S

        for (int j = 0, oneHalfJ = 0, twoHalfJ = 1; j < 4; ++j, oneHalfJ += 2, twoHalfJ += 2) // Cклеивание 4битных фрагментов
            passkey[j] = (passkeyFrag[oneHalfJ] << 4) + passkeyFrag[twoHalfJ];

        // Сдвиг на 11 битов влево
        //passkeyNum - Склеенные 4 байта passkey в одно число
        passkeyNum = passkey[0] * 16777216 + passkey[1] * 65536 + passkey[2] * 256 + passkey[3];	// Склеивание 4 байтов в одно число

        unsigned long passkey11bit = passkeyNum & 0xFFE00000 >> 21;									// 11 начальных битов в отдельной переменной
        passkeyNum = passkeyNum << 11;																// Сдвиг начального числа на 11 битов
        passkeyNum = passkeyNum + passkey11bit;														// Результат циклического сдвига

        passkey[3] = passkeyNum & 0xFF; // Разложение числа на 4 байта
        passkey[2] = (passkeyNum & 0xFF00) >> 8;
        passkey[1] = (passkeyNum & 0xFF0000) >> 16;
        passkey[0] = (passkeyNum & 0xFF000000) >> 24;

        ////////////// КОНЕЦ ФУНКЦИИ F

        for (int j = 0; j < 4; ++j) // XOR левой части с 4 байтами из функции
            passL[j] = passL[j] ^ passkey[j];

        if (i < 31)					// Обмениваем местами левую и правую части на 1-31 раундах: passL[j] и passR[i]
        {
            unsigned char boof;
            for (int j = 0; j < 4; ++j)
            {
                boof = passL[j];
                passL[j] = passR[j];
                passR[j] = boof;
            }
        }
    } // Конец раунда

      // Склеивание пароля из 2 блоков в единую 8байтную строку
    for (int i = 0; i < 4; ++i)						 // Записываем левый блок в результирующий пароль
        RES[i] = passL[i];

    for (int i = 0, pasi = 4; pasi < 8; ++i, ++pasi) // Записываем левый блок в результирующий пароль
        RES[pasi] = passR[i];
}

//тяж. шифровка дополнительная

void encryptionPassword(unsigned char *RES, const char key[], unsigned char pass[])
{
    unsigned char passR[5];			// Правая часть пароля
    unsigned char passL[5];			// Левая часть пароля
    unsigned int numkey[33];		// Собранный 256битный ключ в числа
    unsigned char passkey[5];		// Сумма ключа и правой части
    unsigned char passkeyFrag[9];	// Разбитая на 4битные фрагменты правая часть пароля
    unsigned char keyFrag[5];		// Нужные 4 байта ключа для сложения с правой частью

    //разбиение пароля на 2 блока
    for (int i = 0; i < 4; ++i)						 // Записываем первые 4 байта пароля в левый кусок
        passL[i] = pass[i];

    for (int i = 0, pasi = 4; pasi < 8; ++i, ++pasi) // Записываем вторые 4 байта пароля в правый кусок и его копию
        passR[i] = pass[pasi];

    for (int i = 0; i < 32; ++i)					 // i - счетчик раундов с 0 по 31 (с 1 по 32)
    {

        ////////////// НАЧАЛО ФУНКЦИИ F

        int numRound; // Служит для определения раунда

        if ((i == 0) || (i == 8) || (i == 16) || (i == 31)) // Узнаем номер раунда при зашифровывании
            numRound = 0;
        if ((i == 1) || (i == 9) || (i == 17) || (i == 30))
            numRound = 5;
        if ((i == 2) || (i == 10) || (i == 18) || (i == 29))
            numRound = 9;
        if ((i == 3) || (i == 11) || (i == 19) || (i == 28))
            numRound = 13;
        if ((i == 4) || (i == 12) || (i == 20) || (i == 27))
            numRound = 17;
        if ((i == 5) || (i == 13) || (i == 21) || (i == 26))
            numRound = 21;
        if ((i == 6) || (i == 14) || (i == 22) || (i == 25))
            numRound = 25;
        if ((i == 7) || (i == 15) || (i == 23) || (i == 24))
            numRound = 29;

        for (int j = 0; j < 4; ++j, ++numRound)		// 4 нужные байта в строку
        {
            keyFrag[j] = key[numRound];
        }

        unsigned long keyNum;		// Склеенные 4 байта ключа части в одно число
        keyNum = keyFrag[0] * 16777216 + keyFrag[1] * 65536 + keyFrag[2] * 256 + keyFrag[3];	// Склеивание 4 байтов ключа в одно число

        unsigned long passRNum;		// Склеенные 4 байта правой части в одно число
        passRNum = passR[0] * 16777216 + passR[1] * 65536 + passR[2] * 256 + passR[3];			// Склеивание 4 байтов правой части в одно число

        unsigned long passkeyNum;	// Склеенные 4 байта правой части + ключа в одно число
        if (keyNum + passRNum < 4294967296)
            passkeyNum = keyNum + passRNum;
        else
            if (keyNum + passRNum >= 4294967296)
                passkeyNum = keyNum + passRNum - 4294967296;

        passkey[3] = passkeyNum & 0xFF; // Разложение числа на 4 байта
        passkey[2] = (passkeyNum & 0xFF00) >> 8;
        passkey[1] = (passkeyNum & 0xFF0000) >> 16;
        passkey[0] = (passkeyNum & 0xFF000000) >> 24;

        for (int j = 0, oneHalfJ = 0, twoHalfJ = 1; j < 4; ++j, oneHalfJ += 2, twoHalfJ += 2) // Разбиение правой части пароля на фрагменты
        {
            passkeyFrag[oneHalfJ] = (passkey[j] & 0xF0) >> 4;			// 1 часть байта в четный элемент строки
            passkeyFrag[twoHalfJ] = passkey[j] & 0x0F;					// 2 часть байта в НЕчетный элемент строки
        }

        /////// НАЧАЛО ФУНКЦИИ S
        unsigned int P[8][16] = // Подстановки
        {
            { 12,4,6,2,10,5,11,9,14,8,13,7,0,3,15,1 }, // П0
            { 6,8,2,3,9,10,5,12,1,14,4,7,11,13,0,15 }, // П1
            { 11,3,5,8,2,15,10,13,14,1,7,4,12,9,6,0 }, // П2
            { 12,8,2,1,13,4,15,6,7,0,10,5,3,14,9,11 }, // П3
            { 7,15,5,10,8,1,6,13,0,9,3,14,11,4,2,12 }, // П4
            { 5,13,15,6,9,2,12,10,11,7,8,1,4,3,14,0 }, // П5
            { 8,14,2,5,8,9,1,12,15,4,11,0,13,10,3,7 }, // П6
            { 1,7,14,13,0,5,8,3,4,15,10,6,9,12,11,2 }  // П7
        };

        for (int j = 0, numP = 7; j < 8; ++j, --numP) // Цикл подстановок
            passkeyFrag[j] = P[numP][passkeyFrag[j]];
        /////// КОНЕЦ ФУНКЦИИ S

        for (int j = 0, oneHalfJ = 0, twoHalfJ = 1; j < 4; ++j, oneHalfJ += 2, twoHalfJ += 2)		// Cклеивание 4битных фрагментов
            passkey[j] = (passkeyFrag[oneHalfJ] << 4) + passkeyFrag[twoHalfJ];

        // Сдвиг на 11 битов влево
        // passkeyNum - Склеенные 4 байта passkey в одно число
        passkeyNum = passkey[0] * 16777216 + passkey[1] * 65536 + passkey[2] * 256 + passkey[3];	// Склеивание 4 байтов в одно число

        unsigned long passkey11bit = passkeyNum & 0xFFE00000 >> 21;									// 11 начальных битов в отдельной переменной
        passkeyNum = passkeyNum << 11;																// Сдвиг начального числа на 11 битов
        passkeyNum = passkeyNum + passkey11bit;														// Результат циклического сдвига

        passkey[3] = passkeyNum & 0xFF; // Разложение числа на 4 байта
        passkey[2] = (passkeyNum & 0xFF00) >> 8;
        passkey[1] = (passkeyNum & 0xFF0000) >> 16;
        passkey[0] = (passkeyNum & 0xFF000000) >> 24;

        ////////////// КОНЕЦ ФУНКЦИИ F

        for (int j = 0; j < 4; ++j) // XOR левой части с 4 байтами из функции
            passL[j] = passL[j] ^ passkey[j];

        if (i < 31)					// Обмениваем местами левую и правую части на 1-31 раундах: passL[j] и passR[i]
        {
            unsigned char boof;
            for (int j = 0; j < 4; ++j)
            {
                boof = passL[j];
                passL[j] = passR[j];
                passR[j] = boof;
            }
        }
    } // Конец раунда

      // Склеивание пароля из 2 блоков в единую 8байтную строку
    for (int i = 0; i < 4; ++i)						 // Записываем левый блок в результирующий пароль
        RES[i] = passL[i];

    for (int i = 0, pasi = 4; pasi < 8; ++i, ++pasi) // Записываем левый блок в результирующий пароль
        RES[pasi] = passR[i];
}
//тяж. шифровка главная
void encryptionAndCut(const char key[], unsigned char pass[])
{
        unsigned char passRES[17];							// результирующий пароль
        unsigned char passRESenc[9];						// Выходящий пароль из функции шифрования

        int NumberOfSymbol = 0;
        for (int i = 0; i < 16; ++i)	 // Считаю количество символов в пароле
            if (pass[i] != 0) NumberOfSymbol++;

        if (NumberOfSymbol == 8)
        {
            unsigned char *RES;			// Указатель на строку из функции шифрования
            RES = passRESenc;
            pass[8] = '\0';
            encryptionPassword(RES, key, pass);

            for (int i = 0; i < 8; ++i) // Результат функции шифрования записываю в результирующую строку
            {
                passRES[i] = passRESenc[i];
            }
            passRES[8] = '\0';
        }
        else
            if (NumberOfSymbol == 16)
            {
                unsigned char pass2Part[9];				// 2 часть пароля

                for (int i = 0, j = 8; i < 8; ++i, ++j)	// разделываю пароль на 2 части
                    pass2Part[i] = pass[j];

                unsigned char *RES;						// Указатель на строку из функции шифрования
                RES = passRESenc;

                pass[8] = '\0';
                encryptionPassword(RES, key, pass);
                for (int i = 0; i < 8; ++i)				// Результат функции шифрования записываю в результирующую строку
                {
                    passRES[i] = passRESenc[i];
                }

                for (int i = 0; i < 8; ++i)				// 2 часть пароля записываю в строку, идущую в шифрование
                    pass[i] = pass2Part[i];
                encryptionPassword(RES, key, pass);
                for (int i = 0, j = 8; i < 8; ++i, ++j)	// Результат функции шифрования записываю в результирующую строку
                {
                    passRES[j] = passRESenc[i];
                }
            }
            else
                if ((NumberOfSymbol < 8) && (NumberOfSymbol > 3))
                {
                    unsigned char *RES;										// Указатель на строку из функции шифрования
                    RES = passRESenc;

                    pass[8] = '\0';
                    for(int i = 0, j = 7; i < 8 - NumberOfSymbol; ++i, j--) // Заполняю пустые элементы строки крестами
                        pass[j] = 197;

                    encryptionPassword(RES, key, pass);

                    for (int i = 0; i < 8; ++i)								// Результат функции шифрования записываю в результирующую строку
                    {
                        passRES[i] = passRESenc[i];
                    }
                    passRES[8] = '\0';
                }
                else
                    if (NumberOfSymbol < 16)
                    {
                        unsigned char pass1Part[9];				// 1 часть пароля
                        unsigned char pass2Part[9];				// 2 часть пароля

                        for (int i = 0, j = 8; i < 8; ++i, ++j)	// разделываю пароль на 2 части
                        {
                            pass1Part[i] = pass[i];
                            pass2Part[i] = pass[j];
                        }

                        unsigned char *RES;						// Указатель на строку из функции шифрования
                        RES = passRESenc;

                        pass[8] = '\0';
                        for (int i = 0; i < 8; ++i)				// 1 часть пароля записываю в строку, идущую в шифрование
                            pass[i] = pass1Part[i];
                        encryptionPassword(RES, key, pass);
                        for (int i = 0; i < 8; ++i)				// Результат функции шифрования записываю в результирующую строку
                        {
                            passRES[i] = passRESenc[i];
                        }
                        for (int i = 0; i < 8; ++i)				// 2 часть пароля записываю в строку, идущую в шифрование
                            pass[i] = pass2Part[i];
                        NumberOfSymbol = NumberOfSymbol - 8;
                        for (int i = 0, j = 7; i < 8 - NumberOfSymbol; ++i, j--) // Заполняю пустые элементы строки крестами
                            pass[j] = 197;
                        encryptionPassword(RES, key, pass);
                        for (int i = 0, j = 8; i < 8; ++i, ++j)		// Результат функции шифрования записываю в результирующую строку
                        {
                            passRES[j] = passRESenc[i];
                        }
                    }
                 strcpy((char *)pass,(char *)passRES);
}
//шифровка
bool InstanceEncription(unsigned char * OutputCodeStream, int InputCodeStreamLength)
{
    for (int i = 0; i < InputCodeStreamLength - 1; ++i)			// инверсия строки
        OutputCodeStream[i] = ~OutputCodeStream[i];

    for (int i = 0; i < 11; ++i)								// циклический сдвиг влево на 11
    {
        char boof = OutputCodeStream[0];

        for (int j = 0, p = 1; j < InputCodeStreamLength - 2; ++j, ++p)
            OutputCodeStream[p - 1] = OutputCodeStream[p];
        OutputCodeStream[InputCodeStreamLength - 2] = boof;
    }

    int triplets = InputCodeStreamLength / 3;
    for (int i = 0, trip = 0; i < triplets + 1; ++i, trip+=3)	// перетасовка строки по тройкам (1-3 2-1 3-1)
    {
        char boof;
        unsigned char boofMAS[4];
        if (i != triplets)
        {
            boofMAS[0] = OutputCodeStream[trip];
            boofMAS[1] = OutputCodeStream[trip + 1];
            boofMAS[2] = OutputCodeStream[trip + 2];

            OutputCodeStream[trip + 2] = boofMAS[0];
                OutputCodeStream[trip] = boofMAS[1];
            OutputCodeStream[trip + 1] = boofMAS[2];
        }
    }

    return 0;
}

//расшифровка
bool InstanceDecription( unsigned char * OutputCodeStream, int InputCodeStreamLength)
{

    int triplets = InputCodeStreamLength / 3;
    for (int i = 0, trip = 0; i < triplets + 1; ++i, trip += 3)	// обратная перетасовка строки по тройкам (1-3 2-1 3-1)
    {
        int boof;
        unsigned char boofMAS[4];
        if (i != triplets)
        {
            boofMAS[0] = OutputCodeStream[trip];
            boofMAS[1] = OutputCodeStream[trip + 1];
            boofMAS[2] = OutputCodeStream[trip + 2];

            OutputCodeStream[trip + 1] = boofMAS[0];
            OutputCodeStream[trip + 2] = boofMAS[1];
                OutputCodeStream[trip] = boofMAS[2];
        }
    }

    for (int i = 0; i < 11; ++i)								// циклический сдвиг вправо на 11
    {
        char boof = OutputCodeStream[InputCodeStreamLength - 2];

        for (int j = 0, p = InputCodeStreamLength - 3; j < InputCodeStreamLength - 2; ++j, --p)
            OutputCodeStream[p + 1] = OutputCodeStream[p];
        OutputCodeStream[0] = boof;
    }

    for (int i = 0; i < InputCodeStreamLength - 1; ++i)			// инверсия строки
        OutputCodeStream[i] = ~OutputCodeStream[i];

    return 0;
}
