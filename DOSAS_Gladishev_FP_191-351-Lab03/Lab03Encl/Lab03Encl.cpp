#include "Lab03Encl_t.h"

#include "sgx_trts.h"
#include <string.h>

const char table[6][41] = {
	"1BAA2126338A42868E4C0859DB78EAEA446A4BC7",
	"98FB6AC0E6BB4B9896BE6653D0FB68BDDE423ACD",
	"55AF7B6333514BFFB433898F897C38E3ED45CA21",
	"D485BE269E2B487681EF4121593736C1EA3167EC",
	"F1C41715CEA0460EA9BD7B6E93F05194021CD412",
	"A466EDCF78ACD5213AEFBC2215612D4D3712ACF7"
};

void foo(char* buf, size_t len, size_t idx) { // buf - указатель на область памяти
	if (idx < 6) {
		const char* data_ptr = data_ptr = table[idx]; // указывает на ячейку памяти, где начинается строка под индексом
		memcpy(buf, data_ptr, strlen(data_ptr)); // memcpy - копирует содержимое из одной области памяти в другую
	}
	else {
		memset(buf, 0, strlen(table[0])); // возврат нулевой строки, если такого индекса не найдено, memset - заполнение массива указанными символами
	}
	return;
}