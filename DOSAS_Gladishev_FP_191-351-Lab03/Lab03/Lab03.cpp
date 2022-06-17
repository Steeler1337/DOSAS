#include <iostream>
#include <tchar.h>
#include <string.h>

#include "sgx_urts.h" // специфические заголовки для использования анклава
#include "sgx_tseal.h"
#include "Lab03Encl_u.h"
#define ENCLAVE_FILE _T("Lab03Encl.signed.dll") // явно указываем с каким образом анклава нужно работать

#define BUF_LEN 100 // длина буфера обмена между анклавом и небезопасным приложением

int main() {
	char buffer[BUF_LEN] = { 0 };

	sgx_enclave_id_t eid;
	sgx_status_t ret = SGX_SUCCESS;
	sgx_launch_token_t token = { 0 };
	int updated = 0;

	ret = sgx_create_enclave(ENCLAVE_FILE, SGX_DEBUG_FLAG, &token, &updated, &eid, NULL); // создание и инициализация анклава из библиотеки
	if (ret != SGX_SUCCESS) { // проверка анклава на ошибки
		printf("App: error %#x, failed to create enclave.\n", ret);
		return -1;
	}

	while (true) { // основной код приложения, который итеративно запрашивает у пользователя индекс из таблицы и печатает содержимое
		printf("Input index to retrieve, or -1 to exit: \t");
		int idx = -1;
		scanf_s("%d", &idx); // считывается число, которое вводится в консоль
		if (idx < 0) {
			return 0;
		}
		foo(eid, buffer, BUF_LEN, idx); // eid - id анклава, при вызове функции важно указать из какого анклава она вызывается
		printf("%s\n=======================================\n\n", buffer);
	}
	if (SGX_SUCCESS != sgx_destroy_enclave(eid)) {
		return -1; // выгрузка анклава
	}
	return 0;
}

