// -----------------------------------------------------------------------
//
//
//	Joelis Jonas Šereika ir Domantas Masolas
//	Operacinių Sistemų II-ojo atsiskaitymo realizacija
//
//
// -----------------------------------------------------------------------
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

// C boolean apibrėžimas
#define TRUE 1
#define FALSE 0

// Darbo pabaigos tekstas
#define END_OF_WORK "\n\nDarbas baigtas..."

// Klaidų tekstai
#define FILE_FORMAT_ERROR_LENGTH "Klaida! Failo ilgis neatitinka reikalavimu!"
#define FILE_FORMAT_ERROR_BEGINNING "Klaida! Nerasta DATA zyme!"
#define FILE_FORMAT_ERROR_MIDDLE "Klaida! Nerasta CODE zyme!"
#define FILE_FORMAT_ERROR_ENDING "Klaida! Nerasta HALT zyme!"
#define FILE_FORMAT_ERROR_INCORRECT_LINES_LENGTH "Klaida! Programos maksimalus spausdinamų eiluciu kiekis - neteisingas!"
#define FILE_FORMAT_ERROR_INCORRECT_NAME "Klaida! Programos pavadinimas nenurodytas!"
#define INCORRECT_PARAMETERS "Klaida! Neteisingi parametrai!"
#define FILE_DOES_NOT_EXIST "Klaida! Failas neegzistuoja!"
#define MEMORY_ERROR_DATA_FAILURE "Klaida! Nepavyko irasyti i atminti duotuju duomenu"
#define MEMORY_ERROR_CODE_FAILURE "Klaida! Nepavyko irasyti kodo eiluciu i atminti!"
#define INCORRECT_COMMAND_READ_FROM_MEMORY "Klaida! I atminti isivele neatpazistama komanda!"

// Atminties spausdinimo tekstai
#define MEMORY_STATUS_TEXT_FIRST_LINE "\n\nAtminties bukle:\n"
#define MEMORY_STATUS_TEXT_WORD_LINE "-as zodis"
#define PAGE_TABLE_TEXT_FIRST_LINE "\n\nPuslapiavimo lentele\nVM :: RM\n"
#define REGISTER_VALUES_FIRST_LINE "Baigta vykdyti (vartotojo rezime)"
#define REGISTER_VALUES_PTR "PTR"
#define REGISTER_VALUES_MODE "MODE"
#define REGISTER_VALUES_PI "PI"
#define REGISTER_VALUES_SI "SI"
#define REGISTER_VALUES_TI "TI"
#define REGISTER_VALUES_CH1 "CH1"
#define REGISTER_VALUES_CH2 "CH2"
#define REGISTER_VALUES_IOI "IOI"
#define REGISTER_VALUES_CHPRTS "CHPRTS"
#define REGISTER_VALUES_CHPRTN "CHPRTN"
#define REGISTER_VALUES_CHREAD "CHREAD"
#define REGISTER_VALUES_R "R (virtualus)"
#define REGISTER_VALUES_D "D (virtualus)"
#define REGISTER_VALUES_IC "IC (virtualus)"
#define REGISTER_VALUES_C "C (virtualus)"
#define HEADLINE_CHANNEL_READ "Pradeda veikti kanalų įrenginys - vykdo READ"
#define HEADLINE_CHANNEL_PRTS "Pradeda veikti kanalų įrenginys - vykdo PRTS"
#define HEADLINE_CHANNEL_PRTN "Pradeda veikti kanalų įrenginys - vykdo PRTN"

// Programos failų formato reikalavimai
#define FILE_FORMAT_WORD_LENGTH 4
#define FILE_FORMAT_MAX_LENGTH 444
#define FILE_FORMAT_BEGINNING "DATA"
#define FILE_FORMAT_MIDDLE "CODE"
#define FILE_FORMAT_ENDING "HALT"

// OS Atminties specifikacija
#define OS_DESIGN_BLOCKS_FOR_VM 16
#define OS_DESIGN_WORDS_AMOUNT 4096
#define OS_DESIGN_BLOCKS_AMOUNT 256
#define OS_DESIGN_WORDS_IN_BLOCK 16
#define OS_DESIGN_BYTES_PER_WORD 4
#define OS_DESIGN_EMPTY_WORD_BYTE_SYMBOL '*'
#define OS_DESIGN_RESERVED_WORD_SYMBOL '-'
#define OS_DESIGN_DATA_BLOCK_FROM 0
#define OS_DESIGN_DATA_BLOCK_TO 6
#define OS_DESIGN_CODE_BLOCK_FROM 7
#define OS_DESIGN_CODE_BLOCK_TO 13
#define OS_DESIGN_STACK_BLOCK_FROM 14
#define OS_DESGIN_STACK_BLOCK_TO 15
#define OS_DESIGN_DEFAULT_R_VALUE 0
#define OS_DESIGN_DEFAULT_D_VALUE 0
#define OS_DESIGN_DEFAULT_IC_VALUE 112
#define OS_DESIGN_DEFAULT_C_VALUE 0
#define OS_DESIGN_MODE_USER 0
#define OS_DESIGN_MODE_SUPERVISOR 1
#define OS_DESIGN_DEFAULT_PI_VALUE 0
#define OS_DESIGN_DEFAULT_PI_INVALID_ADDRESS 1
#define OS_DESIGN_DEFAULT_PI_INVALID_OPERATION 2
#define OS_DESIGN_DEFAULT_PI_INVALID_ASSIGNMENT 3
#define OS_DESIGN_DEFAULT_SI_VALUE 0
#define OS_DESIGN_DEFAULT_SI_STOP 1
#define OS_DESIGN_DEFAULT_SI_READ 2
#define OS_DESIGN_DEFAULT_SI_PRTS 3
#define OS_DESIGN_DEFAULT_SI_PRTN 4
#define OS_DESIGN_DEFAULT_TI_VALUE 100
#define OS_DESIGN_CHANNEL_FREE 0
#define OS_DESIGN_CHANNEL_IN_USE 1
#define OS_DESIGN_IOI_DEFAULT 0
#define OS_DESIGN_IOI_DONE_READ 1
#define OS_DESIGN_IOI_DONE_PRTN 2
#define OS_DESIGN_IOI_DONE_PRTS 3
#define OS_DESIGN_CH_CMD_DO 1
#define OS_DESIGN_CH_CMD_DEFAULT 0

// Atminties išvesties failo duomenys
#define MEMORY_STATUS_FILE_NAME "memory_status.txt"

// OS funkcijų string pavadinimai
#define AD "AD" // ADxy
#define DA "DA" // DAxy
#define SB "SB" // SBxy
#define BS "BS" // BSxy
#define CR "CR" // CRxy
#define CD "CD" // CDxy
#define LD "LD" // LDxy
#define LR "LR" // LRxy
#define SD "SD" // SDxy
#define SR "SR" // SRxy
#define GD "GD" // GDxy
#define PD "PD" // PDxy
#define PS "PS" // PSxy
#define HALT "HALT" // HALT

// KLAUSIMAI
// Ar DATA skaicius saugoti kaip sesioliktainius ar kaip ir simbolius? // iki ffff
// Ar tiesiog reikia atskiro atspausdinimo eilutei ir skaiciui? // isvesti iki simbolio arba registras nurodo kiek isvesti
// Ar ne per daug gauti ir skaityti 16 zodziu? Gal geriau viena zodi?

// rezime pazingsniui atskirai leisti atspausdinti realius ir virtualius registrus
// atminti vaizduoti kvadratine martica (virtualia)
// realia atminti i eilute po bloka (16 zodziu)

// Atmintis
char memory[OS_DESIGN_WORDS_AMOUNT][OS_DESIGN_BYTES_PER_WORD];

// Realios mašinos registrai
char ptr[2];
int mode;
int pi;
int si;
int ti;
int ch1;
int ch2;
int ioi;

// Kanalų įrenginio registrai
int chPrtn;
int chPrts;
int chRead;

// Tarpiniai kintamieji
int chToPrtn;
char *chToPrts;
char *chToRead;
int temp;

// Virtualios mašinos registrai
int r, d;
int ic;
int c;

/**
 * Užpildo atminties masyvą simboliais, reiškiančiais tuščią baitą.
 */
void initialize_memory_to_empty_bytes() {
  for (int i=0 ; i<OS_DESIGN_WORDS_AMOUNT ; i++) {
    for (int j=0 ; j<OS_DESIGN_BYTES_PER_WORD ; j++) {
	  memory[i][j] = OS_DESIGN_EMPTY_WORD_BYTE_SYMBOL;
	}
  }
}

void initialize_registers() {
  initialize_memory_to_empty_bytes();

  ch1 = OS_DESIGN_CHANNEL_FREE;
  ch2 = OS_DESIGN_CHANNEL_FREE;
  mode = OS_DESIGN_MODE_USER;
  pi = OS_DESIGN_DEFAULT_PI_VALUE;
  si = OS_DESIGN_DEFAULT_SI_VALUE;
  ti = OS_DESIGN_DEFAULT_TI_VALUE;
  r = OS_DESIGN_DEFAULT_R_VALUE;
  d = OS_DESIGN_DEFAULT_D_VALUE;
  ic = OS_DESIGN_DEFAULT_IC_VALUE;
  c = OS_DESIGN_DEFAULT_C_VALUE;
  ioi = OS_DESIGN_IOI_DEFAULT;
  chPrtn = OS_DESIGN_CH_CMD_DEFAULT;
  chPrts = OS_DESIGN_CH_CMD_DEFAULT;
  chRead = OS_DESIGN_CH_CMD_DEFAULT;

  initialize_ptr();
  initialize_page_table();
}

/**
 * Spausdina registrų reikšmes į failą.
 */
void print_register_values() {
  FILE *file;
  file = fopen(MEMORY_STATUS_FILE_NAME, "a");

  fprintf(file, "%s: %s\n", REGISTER_VALUES_PTR, ptr);
  fprintf(file, "%s: %d\n", REGISTER_VALUES_MODE, mode);
  fprintf(file, "%s: %d\n", REGISTER_VALUES_PI, pi);
  fprintf(file, "%s: %d\n", REGISTER_VALUES_SI, si);
  fprintf(file, "%s: %d\n", REGISTER_VALUES_TI, ti);
  fprintf(file, "%s: %d\n", REGISTER_VALUES_CH1, ch1);
  fprintf(file, "%s: %d\n", REGISTER_VALUES_CH2, ch2);
  fprintf(file, "%s: %d\n", REGISTER_VALUES_IOI, ioi);
  fprintf(file, "%s: %d\n", REGISTER_VALUES_CHPRTN, chPrtn);
  fprintf(file, "%s: %d\n", REGISTER_VALUES_CHPRTS, chPrts);
  fprintf(file, "%s: %d\n", REGISTER_VALUES_CHREAD, chRead);
  fprintf(file, "%s: %d\n", REGISTER_VALUES_R, r);
  fprintf(file, "%s: %d\n", REGISTER_VALUES_D, d);
  fprintf(file, "%s: %d\n", REGISTER_VALUES_IC, ic);
  fprintf(file, "%s: %d\n", REGISTER_VALUES_C, c);

  fclose(file);
}

/**
 * Spausdina registrų reikšmes į failą ir nurodo komandą,
 * po kurios spausdinta.
 */
void print_register_values_after_command(char *commandName) {
  FILE *file;
  file = fopen(MEMORY_STATUS_FILE_NAME, "a");
  fprintf(file, "\n%s: %s\n", REGISTER_VALUES_FIRST_LINE, commandName);
  fclose(file);

  print_register_values();
}

/**
 * Spausdina registrų reikšmes į failą ir nurodo eilutė virš
 * registrų.
 */
void print_register_values_with_headline(char *headline) {
  FILE *file;
  file = fopen(MEMORY_STATUS_FILE_NAME, "a");
  fprintf(file, "\n%s\n", headline);
  fclose(file);

  print_register_values();
}

/**
 * Išspausdina atminties būklę su nurodyta priežastimi į failą.
 */
void print_memory_status() {
  FILE *file;
  file = fopen(MEMORY_STATUS_FILE_NAME, "a");

  fprintf(file, "%s", MEMORY_STATUS_TEXT_FIRST_LINE);
  for (int i=0 ; i<OS_DESIGN_WORDS_AMOUNT ; i++) {
	for (int j=0 ; j<OS_DESIGN_BYTES_PER_WORD ; j++) {
	  fprintf(file, "%c", memory[i][j]);
	}
	fprintf(file, ": %d%s\n", (i+1), MEMORY_STATUS_TEXT_WORD_LINE);
  }

  fclose(file);
}

// Išspausdina puslapiavimo lentelę į failą
void print_page_table() {
  FILE *file;
  file = fopen(MEMORY_STATUS_FILE_NAME, "a");

  int pageTableBlockIndex = atoi(ptr);
  int pageTableFirstWordIndex = pageTableBlockIndex * OS_DESIGN_WORDS_IN_BLOCK;

  fprintf(file, "%s", PAGE_TABLE_TEXT_FIRST_LINE);
  for (int i=0 ; i<OS_DESIGN_BLOCKS_FOR_VM ; i++) {
    fprintf(file, "%d :: %s\n", i, memory[pageTableFirstWordIndex + i]);
  }
}

// Paverčia skaičių kaip simbolį. Galioja tik rėžiuose [0 ; 9].
char int_to_char_symbol(int number) {
  return (char)((int)'0' + number);
}

// Paverčia simbolį į skaičių. Galioja tik rėžiuose [0 ; 9].
int char_to_int_symbol(char number) {
  return (int)(number - '0');
}

// Rezervuoja bloką. Techniškai, pakeičia kiekvieną žodžio baitą iš OS_DESIGN_EMPTY_WORD_BYTE_SYMBOL į OS_DESIGN_RESERVED_WORD_SYMBOL.
void reserve_block(int blockIndex) {
  int wordsFrom = blockIndex * OS_DESIGN_WORDS_IN_BLOCK;
  int wordsTo = blockIndex * OS_DESIGN_WORDS_IN_BLOCK + OS_DESIGN_WORDS_IN_BLOCK;

  for (int i=wordsFrom ; i<wordsTo ; i++) {
    for (int j=0 ; j<OS_DESIGN_BYTES_PER_WORD ; j++) {
	  memory[i][j] = OS_DESIGN_RESERVED_WORD_SYMBOL;
	}
  }
}

// Įdiegia PTR registrą su reikšme, kur saugoma puslapiavimo lentelė.
void initialize_ptr() {
  // Įdiegiame atsitikinių skaičių generatorių pagal laiką nuo 1970.01.01
  srand(time(NULL));

  // Parenkame atsitiktinį bloką puslapiavimo lentelei
  int pageTableBlockIndex = rand() % OS_DESIGN_BLOCKS_AMOUNT;
  int a1 = pageTableBlockIndex / 10;
  int a2 = pageTableBlockIndex % 10;

  // Įrašome reikšmę į PTR
  ptr[0] = int_to_char_symbol(a1);
  ptr[1] = int_to_char_symbol(a2);

  // Rezervuojame bloką puslapiavimo lentelei
  reserve_block(pageTableBlockIndex);
}

// Tikrina, ar realios atminties blokas yra tuščias.
int is_block_empty(int blockIndex) {
  int wordsFrom = blockIndex * OS_DESIGN_WORDS_IN_BLOCK;
  int wordsTo = blockIndex * OS_DESIGN_WORDS_IN_BLOCK + OS_DESIGN_WORDS_IN_BLOCK;

  for (int i=wordsFrom ; i<wordsTo ; i++) {
    for (int j=0 ; j<OS_DESIGN_BYTES_PER_WORD ; j++) {
	  if (memory[i][j] != OS_DESIGN_EMPTY_WORD_BYTE_SYMBOL) {
	    return FALSE;
	  }
	}
  }

  return TRUE;
}

// Įrašo bloko atitikmenį į puslapiavimo lentelę.
void write_block_adress_to_page_table(int virtualIndex, int realIndex) {
  int pageTableBlockIndex = atoi(ptr);
  int pageTableFirstWordIndex = pageTableBlockIndex * OS_DESIGN_WORDS_IN_BLOCK;
  sprintf(memory[pageTableFirstWordIndex + virtualIndex], "%d", realIndex);
}

// Inicializuoja puslapiavimo lentelę.
void initialize_page_table() {
  // Įdiegiame atsitikinių skaičių generatorių pagal laiką nuo 1970.01.01
  srand(time(NULL));

  // Iteruojame per nuo 0 iki tiek blokų kiek reikia VM
  for (int i=0 ; i<OS_DESIGN_BLOCKS_FOR_VM ; i++) {
    int randomBlockIndex = -1;
	int isBlockEmpty = FALSE;

	// Generuojame atsitiktinį bloko numerį kol randame tuščią
	do {
	  randomBlockIndex = rand() % OS_DESIGN_BLOCKS_AMOUNT;
	  isBlockEmpty = is_block_empty(randomBlockIndex);
	} while (isBlockEmpty == FALSE);

	// Rezervuojame bloką
	reserve_block(randomBlockIndex);

	// Įrašome į puslapiavimo lentelę adresą
	write_block_adress_to_page_table(i, randomBlockIndex);
  }
}

// Gražina žodžio virtualų adresą.
int get_real_word_address(int virtualWordAddress) {
  return (OS_DESIGN_BLOCKS_FOR_VM * atoi(memory[OS_DESIGN_BLOCKS_FOR_VM * atoi(ptr) + virtualWordAddress / OS_DESIGN_BLOCKS_FOR_VM])) +
    (virtualWordAddress % OS_DESIGN_BLOCKS_FOR_VM);
}

// Laukia vartotojo įvesties.
void wait_for_user_interaction() {
  printf("%s\n", END_OF_WORK);
  getchar();
}

// Tikrina, ar failas atitinka reikalaujamą formatą.
int is_file_of_required_format(char *fileByteArray) {
  // Tikriname masyvo ilgį
  if (sizeof(fileByteArray) > FILE_FORMAT_MAX_LENGTH) {
	printf("%s\n", FILE_FORMAT_ERROR_LENGTH);
	return FALSE;
  }

  // Tikriname DATA žymę
  int actualFormatBeginningLength = strlen(FILE_FORMAT_BEGINNING);
  if (strncmp(fileByteArray, FILE_FORMAT_BEGINNING, actualFormatBeginningLength) != 0) {
    printf("%s\n", FILE_FORMAT_ERROR_BEGINNING);
	return FALSE;
  }

  // Tikriname CODE zyme
  if (!strstr(fileByteArray, FILE_FORMAT_MIDDLE)) {
    printf("%s\n", FILE_FORMAT_ERROR_MIDDLE);
	return FALSE;
  }

  // Tikriname HALT žymę
  int actualFormatEndingLength = strlen(FILE_FORMAT_ENDING);
  char *lastSymbols = (char *)calloc(actualFormatEndingLength, sizeof(char));
  for (int i=0,j=(strlen(fileByteArray) - actualFormatEndingLength) ; i<actualFormatEndingLength ; i++,j++) {
	lastSymbols[i] = fileByteArray[j];
  }

  if (strcmp(lastSymbols, FILE_FORMAT_ENDING) != 0) {
	printf("%s\n", FILE_FORMAT_ERROR_ENDING);
	free(lastSymbols);
	return FALSE;
  }
  free(lastSymbols);

  // Jeigu viskas gerai, gražiname TRUE
  return TRUE;
}

// Nuskaito failą ir gražina simbolių masyvą.
char *init_char_array_from_file(const char *fileName) {
  // Atidarome failą
  FILE *osFile;
  osFile = fopen(fileName, "r");
  if (osFile == NULL) {
	printf("%s\n", FILE_DOES_NOT_EXIST);
    return NULL;
  }

  // Einame į failo pabaigą, paskaičiuojame ilgį
  fseek(osFile, 0L, SEEK_END);
  int elements = ftell(osFile);

  // Sukuriame char masyvą tolygų failo dydžiui
  char *dataToReturn = (char *)calloc(elements + 1, sizeof(char));

  // Grįžtame į failo pradžią
  fseek(osFile, 0L, SEEK_SET);

  // Nuskaitome visus char'us iš failo į masyvą
  int charCounter = 0;
  char readChar;
  while ((readChar = getc(osFile)) != EOF) {
    dataToReturn[charCounter++] = readChar;
  }

  // Uždarome failą
  fclose(osFile);

  // Pridedame pabaigos simbolį prie programos duomenų eilutės
  dataToReturn[elements + 1] = '\0';

  // Tikriname, ar failas atitinka formatą
  if (is_file_of_required_format(dataToReturn) == FALSE) {
	return NULL;
  }

  // Gražiname rezultatą
  return dataToReturn;
}

// Gražina tekstinę eilutę nurodytuose rėžiuose.
char *substring_from_to(const char *dataLine, const int from, const int to) {
  char *arrayToReturn = (char *)calloc(to - from + 2, sizeof(char));

  for (int i=from,j=0 ; i<=to ; i++,j++) {
	arrayToReturn[j] = dataLine[i];

	if (i == to) {
	  arrayToReturn[j+1] = '\0';
	}
  }

  return arrayToReturn;
}

// Gražina tekstinę eilutę nuo nurodyto rėžio iki nurodyto simbolio.
char *substring_from_until_symbol(const char *dataLine, const int from, const char endSymbol, const int maxLength) {
  char *arrayToReturn = (char *)calloc(maxLength, sizeof(char));
  int dataArrayIteratorIndex = from;
  int newArrayIteratorIndex = 0;

  do {
    char symbol = dataLine[dataArrayIteratorIndex];
	if (symbol == endSymbol) {
	  break;
	}

	arrayToReturn[newArrayIteratorIndex++] = symbol;
	dataArrayIteratorIndex++;

  } while (dataArrayIteratorIndex > 0 && dataArrayIteratorIndex <= maxLength);
  arrayToReturn[newArrayIteratorIndex] = '\0';

  arrayToReturn = (char *)realloc(arrayToReturn, newArrayIteratorIndex);
  return arrayToReturn;
}


// Įrašo duomenis i VM duomenu bloka. Nuo kur prasideda blokas nurodo: OS_DESIGN_DATA_BLOCK_FROM.
int initialize_program_data_to_memory(const char *data, int length) {
  for (int i=0,j=0 ; i<length ; i=i+OS_DESIGN_BYTES_PER_WORD,j++) {
	int virtualWordIndex = OS_DESIGN_DATA_BLOCK_FROM * 16 + j;

    for (int k=0 ; k<OS_DESIGN_BYTES_PER_WORD ; k++) {
	  memory[get_real_word_address(virtualWordIndex)][k] = data[i+k];
	}
  }

  return TRUE;
}

// Į bloką skirta programos kodui surašo visas komandas iš failo.
int initialize_program_code_to_memory(char programCodeLines[][FILE_FORMAT_WORD_LENGTH + 1], int linesLength) {
  for (int i=0 ; i<linesLength ; i++) {
    int virtualWordIndex = OS_DESIGN_CODE_BLOCK_FROM * 16 + i;

	for (int j=0 ; j<OS_DESIGN_BYTES_PER_WORD ; j++) {
	  memory[get_real_word_address(virtualWordIndex)][j] = programCodeLines[i][j];
	}
  }

  return TRUE;
}

// Pagal įvesties duomenis, suvedame visus duomenis į atmintį
int initialize_given_program_to_memory(const char *fileByteArray) {

  // apdorojame duomenis
  int dataCount = 0;
  char data[256] = { 0 };
  int index = strlen(FILE_FORMAT_BEGINNING) + 1;
  while(TRUE) {
    if (fileByteArray[index] == 'C' && fileByteArray[index+1] == 'O'
        && fileByteArray[index+2] == 'D' && fileByteArray[index+3] == 'E') {
        break;
    } else if (fileByteArray[index] == 'D' && fileByteArray[index+1] == 'W') {
      char * num = substring_from_until_symbol(fileByteArray, index + 3, '\n', 256);
      int numInt = string_to_decimal_format(num);

      data[dataCount] = (numInt & 0xff000000) >> 24;
      data[dataCount+1] = (numInt & 0xff0000) >> 16;
      data[dataCount+2] = (numInt & 0xff00) >> 8;
      data[dataCount+3] = numInt & 0xff;
      dataCount += 4;

      index += strchr(fileByteArray + index, '\n') - (fileByteArray + index) + 1;
    } else if (fileByteArray[index] == 'D' && fileByteArray[index+1] == 'B') {
      data[dataCount] = fileByteArray[index + 3];
      data[dataCount+1] = fileByteArray[index + 4];
      data[dataCount+2] = fileByteArray[index + 5];
      data[dataCount+3] = fileByteArray[index + 6];
      dataCount += 4;

      index += strchr(fileByteArray + index, '\n') - (fileByteArray + index) + 1;
    }
  }
  index += 5; // Perzengiame 'CODE\n'

  // Įrašome į atmintį
  int initializedData = initialize_program_data_to_memory(data, dataCount);
  if (initializedData == FALSE) {
	printf("%s\n", MEMORY_ERROR_DATA_FAILURE);
    return FALSE;
  }

  // Išgauname pačią programą į simbolių masyvą
  int programCodeLength = strlen(fileByteArray) - index - 1;

  const char* programCode = substring_from_to(fileByteArray, index, index + programCodeLength);
  remove_char(programCode, '\n');
  programCodeLength = strlen(programCode);

  // Iš programos simbolių masyvo, sukuriame programos žodžių 2d masyvą
  const int wordsAmount = programCodeLength / FILE_FORMAT_WORD_LENGTH;
  char programCodeLines[wordsAmount][FILE_FORMAT_WORD_LENGTH + 1];

  int lineNo = 0;
  for (int i=0 ; i<strlen(programCode) ; i=i+FILE_FORMAT_WORD_LENGTH) {
    for (int j=0 ; j<FILE_FORMAT_WORD_LENGTH ; j++) {
	  programCodeLines[lineNo][j] = programCode[i+j];
	}

	programCodeLines[lineNo][FILE_FORMAT_WORD_LENGTH] = '\0';
	lineNo++;
  }

  // Į atmintį įrašome visas kodų komandas
  int initializedCode = initialize_program_code_to_memory(programCodeLines, lineNo);
  if (initializedCode == FALSE) {
    printf("%s\n", MEMORY_ERROR_CODE_FAILURE);
	return FALSE;
  }

  // Jeigu neįvyko problemų - gražiname sėkmės reikšmę
  return TRUE;
}

// Pasalina nurodytus simbolius is teksto eilutes
void remove_char(char *str, char c) {
    int len = strlen(str);
  for(int i = 0; i < len; i++)
	{
		if(str[i] == c)
		{
			for(int j = i; j < len; j++)
			{
				str[j] = str[j + 1];
			}
			len--;
			i--;
		}
  }
}

// Iš atminties išgeneruoja žodį.
char *parse_word_from_memory(int address) {
  char *word = (char *)calloc(OS_DESIGN_BYTES_PER_WORD, sizeof(char));

  for (int i=0 ; i<OS_DESIGN_BYTES_PER_WORD ; i++) {
    word[i] = memory[address][i];
  }

  return word;
}

// Tekstinę eilutę paverčia į Skaičių.
int string_to_decimal_format(char * decimal) {
  int returnable;
  sscanf(decimal, "%d", &returnable);
  return returnable;
}

// Skaičių paverčia į tekstinę eilutę.
char *decimal_to_string_format(int decimal) {
  char *returnable = (char *)calloc(OS_DESIGN_BYTES_PER_WORD, sizeof(char));
  sprintf(returnable, "%d", decimal);
  return returnable;
}

// Į nurodytą realų adresą įrašomas žodis.
void write_word_to_memory_at(int realAddress, char *word) {
  for (int i=0 ; i<OS_DESIGN_BYTES_PER_WORD ; i++) {
    memory[realAddress][i] = word[i];
  }
}

// Konvertuoja du simbolius, is eiles sudarancius sesioliktaini skaiciu i desimatini ir grazina jo reiksme.
int two_hex_symbols_to_decimal(char x, char y) {
  char hexString[3] = {x, y, '\0'};
  return strtol(hexString, NULL, 16);
}


// Apjungia keturis char'us, kurie is tikro yra int'as
int four_char_to_decimal(char * x) {
  int number = (unsigned char)x[0] << 24;
  number += (unsigned char)x[1] << 16;
  number += (unsigned char)x[2] << 8;
  number += (unsigned char)x[3];
  return number;
}

// Tikrina, ar pateiktas adresas patenka į atmintį ir pagal tai gražina loginę reikšmę.
int is_address_within_boundaries(int realWordAddress) {
  if (realWordAddress < 0 || realWordAddress > OS_DESIGN_WORDS_AMOUNT) {
    return FALSE;
  }

  return TRUE;
}

void os_command_ad(char x1, char x2) {
  int wordNumber = 16 * char_to_int_symbol(x1) + char_to_int_symbol(x2);
  if (is_address_within_boundaries(get_real_word_address(wordNumber)) == FALSE) {
    pi = OS_DESIGN_DEFAULT_PI_INVALID_ADDRESS;
    return;
  }

  char * numStr = memory[get_real_word_address(wordNumber)];
  r += four_char_to_decimal(numStr);
}

void os_command_da(char x1, char x2) {
  r += d;
}

void os_command_sb(char x1, char x2) {
  r -= d;
}

void os_command_bs(char x1, char x2) {
  int wordNumber = 16 * char_to_int_symbol(x1) + char_to_int_symbol(x2);
  if (is_address_within_boundaries(get_real_word_address(wordNumber)) == FALSE) {
    pi = OS_DESIGN_DEFAULT_PI_INVALID_ADDRESS;
    return;
  }

  char * numStr = memory[get_real_word_address(wordNumber)];
  r -= four_char_to_decimal(numStr);
}

void os_command_cr(char x1, char x2) {
  c = r > d ? TRUE : FALSE;
}

void os_command_cd(char x1, char x2) {
  c = r == d ? TRUE : FALSE;
}

void os_command_ld(char x1, char x2) {
  int wordNumber = 16 * char_to_int_symbol(x1) + char_to_int_symbol(x2);
  if (is_address_within_boundaries(get_real_word_address(wordNumber)) == FALSE) {
    pi = OS_DESIGN_DEFAULT_PI_INVALID_ADDRESS;
    return;
  }

  char * numStr = memory[get_real_word_address(wordNumber)];
  d = four_char_to_decimal(numStr);
}

void os_command_lr(char x1, char x2) {
  int wordNumber = 16 * char_to_int_symbol(x1) + char_to_int_symbol(x2);
  if (is_address_within_boundaries(get_real_word_address(wordNumber)) == FALSE) {
    pi = OS_DESIGN_DEFAULT_PI_INVALID_ADDRESS;
    return;
  }

  char * numStr = memory[get_real_word_address(wordNumber)];
  r = four_char_to_decimal(numStr);
}

void os_command_sr(char x1, char x2) {
  int wordNumber = 16 * char_to_int_symbol(x1) + char_to_int_symbol(x2);
  if (is_address_within_boundaries(get_real_word_address(wordNumber)) == FALSE) {
    pi = OS_DESIGN_DEFAULT_PI_INVALID_ADDRESS;
    return;
  }

  for (int i=0 ; i<OS_DESIGN_BYTES_PER_WORD ; i++) {
    memory[get_real_word_address(wordNumber)][i] = r << (8 * (4 - i));
  }
}

void os_command_gd(char x1, char x2) {
  temp = 16 * char_to_int_symbol(x1) + char_to_int_symbol(x2);
  chRead = OS_DESIGN_CH_CMD_DO;
  si = OS_DESIGN_DEFAULT_SI_READ;
}

void os_command_pd(char x1, char x2) {
  chPrts = OS_DESIGN_CH_CMD_DO;
  si = OS_DESIGN_DEFAULT_SI_PRTS;
  temp = 16 * char_to_int_symbol(x1) + char_to_int_symbol(x2);
}

void os_command_ps(char x1, char x2) {
  chPrtn = OS_DESIGN_CH_CMD_DO;
  si = OS_DESIGN_DEFAULT_SI_PRTN;
  temp = 16 * char_to_int_symbol(x1) + char_to_int_symbol(x2);
}

// Nuskaito komandą, kokia buvo nurodyta atmintyje ir ją vykdo.
void parse_command_and_launch_it(char *command) {

  if (strncmp(AD, command, 2) == 0) {
    os_command_ad(command[2], command[3]);
  } else if (strncmp(DA, command, 2) == 0) {
    os_command_da(command[2], command[3]);
  } else if (strncmp(SB, command, 2) == 0) {
    os_command_sb(command[2], command[3]);
  } else if (strncmp(BS, command, 2) == 0) {
    os_command_bs(command[2], command[3]);
  } else if (strncmp(CR, command, 2) == 0) {
    os_command_cr(command[2], command[3]);
  } else if (strncmp(CD, command, 2) == 0) {
    os_command_cd(command[2], command[3]);
  } else if (strncmp(LD, command, 2) == 0) {
    os_command_ld(command[2], command[3]);
  } else if (strncmp(LR, command, 2) == 0) {
    os_command_lr(command[2], command[3]);
  } else if (strncmp(SR, command, 2) == 0) {
    os_command_sr(command[2], command[3]);
  } else if (strncmp(GD, command, 2) == 0) {
    os_command_gd(command[2], command[3]);
  } else if (strncmp(PD, command, 2) == 0) {
    os_command_pd(command[2], command[3]);
  } else if (strncmp(PS, command, 2) == 0) {
    os_command_ps(command[2], command[3]);
  }

  // Padidiname komandos skaitliuką
  ic++;

  // Spausdiname registrų reikšmes
  print_register_values_after_command(command);
}

// Vykdomas nuskaitymas iš kanalų įrenginio.
void channel_read() {
  int numberTyped = 0;
  scanf("%d", &numberTyped);

  if (numberTyped < -999 || numberTyped > 9999) {
    pi = OS_DESIGN_DEFAULT_PI_INVALID_ASSIGNMENT;
  }

  chToRead = decimal_to_string_format(numberTyped);
}

// Vykdomas skaičiaus spausdinimas iš kanalų įrenginio.
void channel_prtn() {
  char *numberToPrintStr = (char *)calloc(OS_DESIGN_BYTES_PER_WORD, sizeof(char));

  for (int i=0 ; i<OS_DESIGN_BYTES_PER_WORD ; i++) {
    numberToPrintStr[i] = memory[get_real_word_address(temp)][i];
  }

  chToPrtn = atoi(numberToPrintStr);
}

// Vykdomas tekstinės eilutės spausdinimas iš kanalų įrenginio.
void channel_prts() {
  chToPrts = memory[get_real_word_address(temp)];
}

// Įjungia kanalų įrenginį ir tikrina kanalų registrus.
void enable_channel_mechanism() {
  // Vykdome skaičiaus nuskaitymą jeigu laisvas 1 kanalas
  if (chRead == OS_DESIGN_CH_CMD_DO && ch1 == OS_DESIGN_CHANNEL_FREE) {
    char headline[] = HEADLINE_CHANNEL_READ;
    print_register_values_with_headline(headline);
    channel_read();
    chRead = OS_DESIGN_CH_CMD_DEFAULT;

	ioi = OS_DESIGN_IOI_DONE_READ;

  // Vykdome eilutės spausdinimą jeigu laisvas 2 kanalas
  } else if (chPrts == OS_DESIGN_CH_CMD_DO && ch2 == OS_DESIGN_CHANNEL_FREE) {
	char headline[] = HEADLINE_CHANNEL_PRTS;
    print_register_values_with_headline(headline);
    channel_prts();
    chPrts = OS_DESIGN_CH_CMD_DEFAULT;

	ioi = OS_DESIGN_IOI_DONE_PRTS;

  // Vykdome skaičiaus spausdinimą jeigu laisvas 2 kanalas
  } else if (chPrtn == OS_DESIGN_CH_CMD_DO && ch2 == OS_DESIGN_CHANNEL_FREE) {
    char headline[] = HEADLINE_CHANNEL_PRTN;
	print_register_values_with_headline(headline);
    channel_prtn();
    chPrtn = OS_DESIGN_CH_CMD_DEFAULT;

	ioi = OS_DESIGN_IOI_DONE_PRTN;
  }
}

// Baigia darbą bei atspausdina atminties išraišką ir puslapiavimo lentelę
void perform_interrupt_quit(int errorCode) {
  char stopName[] = HALT;
  print_register_values_after_command(stopName);
  print_memory_status();
  print_page_table();
  exit(errorCode);
}

// Tikrina pertraukimus ir juos vykdo.
void check_interrupts() {
  // Pereiname į supervizoriaus rėžimą, jeigu vykdomas pertraukimas
  if ((pi + si) > 0 || ti == 0) {
    mode = OS_DESIGN_MODE_SUPERVISOR;
  }

  // Jeigu PI reikšme didesnė už 0, vadinasi ivyko klaida
  if (pi > 0) {
    perform_interrupt_quit(EXIT_FAILURE);

  // Jeigu taimeris tapo 0, tiketina, kad:
  //  - patekome į infinite loop'ą
  //  - programoje esti daugiau komandų nei leidžiama
  } else if (ti == 0) {
    perform_interrupt_quit(EXIT_FAILURE);

  // Jeigu SI tapo 1, vadinasi vykdoma komandą STOP
  } else if (si == OS_DESIGN_DEFAULT_SI_STOP) {
    perform_interrupt_quit(EXIT_SUCCESS);

  // Jeigu SI tapo 2, vadinasi vykdoma komanda READ
  } else if (si == OS_DESIGN_DEFAULT_SI_READ) {
    ch1 = OS_DESIGN_CHANNEL_IN_USE;
    write_word_to_memory_at(get_real_word_address(temp), chToRead);

  // Jeigu SI tapo 3, vadinasi vykdoma komanda PRTS
  } else if (si == OS_DESIGN_DEFAULT_SI_PRTS) {
    ch2 = OS_DESIGN_CHANNEL_IN_USE;
	for (int i=0 ; i<OS_DESIGN_BYTES_PER_WORD ; i++) {
      printf("%c", chToPrts[i]);
    }
    printf("\n");

  // Jeigu SI tapo 4, vadinasi vykdoma komanda PRTN
  } else if (si == OS_DESIGN_DEFAULT_SI_PRTN) {
    ch2 = OS_DESIGN_CHANNEL_IN_USE;
    printf("%d\n", chToPrtn);
  }

  // Atnaujiname SI ir PI
  si = OS_DESIGN_DEFAULT_SI_VALUE;
  pi = OS_DESIGN_DEFAULT_PI_VALUE;

  // Jeigu IOI rodo, kad baige spausdinimo darba - atlaisviname antraji kanala
  if (ioi == OS_DESIGN_IOI_DONE_PRTN
      || ioi == OS_DESIGN_IOI_DONE_PRTN) {
    ch2 = OS_DESIGN_CHANNEL_FREE;
    ioi = OS_DESIGN_IOI_DEFAULT;

  // Jeigu IOI rodo, kad baige skaitymo darba - atlaisviname pirmaji kanala
  } else if (ioi == OS_DESIGN_IOI_DONE_READ) {
    ch1 = OS_DESIGN_CHANNEL_FREE;
    ioi = OS_DESIGN_IOI_DEFAULT;
  }

  // Grįžtame į vartotojo rėžimą
  mode = OS_DESIGN_MODE_USER;
}

// Vykdo visą OS algoritmą iš atminties.
void load_os() {
  while (TRUE) {
	// Dekoduojame komandą
	char *command = parse_word_from_memory(get_real_word_address(ic));

	// Vykdome komandą
	parse_command_and_launch_it(command);

	// Sumažiname taimerio reikšmė
	ti--;

	// Įjungiame kanalų įrenginį
	enable_channel_mechanism();

	// Tikriname pertraukimus
	check_interrupts();
  }
}

// Pradeda darbą.
int main(int argc, const char *argv[]) {
  char filename[256];
  printf("Iveskite programos faila: ");
  scanf("%s", &filename);

  // Nuskaitome failo duomenis į baitų masyvą
  const char *fileByteArray = init_char_array_from_file(filename);
  if (fileByteArray == NULL) {
    wait_for_user_interaction();
	return EXIT_FAILURE;
  }

  // Išvalome failą, skirtą atminties būklei stebėti,
  // jeigu toks egzistuoja
  remove(MEMORY_STATUS_FILE_NAME);

  // Įdiegiame registrų pradines reikšmes
  initialize_registers();

  // Pakrauname programą į atmintį
  int flowSuccess = initialize_given_program_to_memory(fileByteArray);
  if (flowSuccess == FALSE) {
	wait_for_user_interaction();
	return EXIT_FAILURE;
  }

  // Vykdome pačią programą
  load_os();

  // Jeigu programa pasiekia šį tašką, tai ji išjungiama ne su OS komanda
  return EXIT_FAILURE;
}
