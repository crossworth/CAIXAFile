/*
* @Author: Pedro Henrique
* @Date:   2016-06-11 19:04:34
* @Last Modified by:   Pedro Henrique
* @Last Modified time: 2016-09-17 10:06:19
*/

#include <stdio.h>
#include "CaixaFile.h"

int main() {


	caixa_file my_caixa_file = {};

	caixa_create_header(&my_caixa_file.header, "Internal name of the caixa file", "The Description of the caixa file");

	// caixa_open_file(&my_caixa_file, "teste.caixa");

	printf("Size of description %lu\n", sizeof(my_caixa_file.header.description));

	// printf("Caixa Header Name: %s\n", my_caixa_file.header.name);
	// printf("Caixa Header Description: %s\n", my_caixa_file.header.description);
	// printf("Caixa Header Version: %s\n", my_caixa_file.header.version);
	// printf("Caixa Header Number Files: %u\n", my_caixa_file.header.number_files);
	// printf("Caixa Header Created on: %llu\n", my_caixa_file.header.created_on);
	// printf("Caixa Header Updated on: %llu\n", my_caixa_file.header.updated_on);

	// printf("Caixa Header Sizeof: %lu bytes\n", sizeof(my_caixa_file.header));


	// printf("Caixa File Sizeof: %lu\n", sizeof(my_caixa_file));

	caixa_entry my_entry_file = {};
	caixa_crete_entry_from_file(&my_entry_file, "test.txt");
	caixa_add_entry(&my_caixa_file, &my_entry_file);

	// caixa_entry my_entry_file2 = {};
	// caixa_crete_entry_from_file(&my_entry_file2, "test2.txt");
	// caixa_add_entry(&my_caixa_file, &my_entry_file2);

	// for(uint32_t index = 0; index < my_caixa_file.header.number_files; index++) {
	// 	printf("Caixa Entry File Name: %s\n", my_caixa_file.files[index]->name);
	// 	printf("Caixa Entry File Size: %llu\n", my_caixa_file.files[index]->size);
	// 	printf("Caixa Entry Sizeof: %lu\n", sizeof(my_caixa_file.files[index]));
	

	// 	printf("Caixa Entry Data: ");

	// 	for(int i = 0; i <= my_caixa_file.files[index]->size; i++) {
	// 		printf("%c", my_caixa_file.files[index]->data[i]);	
	// 	}
		
	// 	printf("\n");
	// }



	// byte *test_file = caixa_get_file(&my_caixa_file, "test.txt");
	// printf("Test File: %s\n", test_file);


	// caixa_remove_file(&my_caixa_file, "test.txt");

	// byte *test_file1 = caixa_get_file(&my_caixa_file, "test.txt");

	// printf("Test File after removed: %s\n", test_file1);
	
	// printf("Caixa Entry Data: ");

	// for(int i = 0; i <= my_entry_file.size; i++) {
	// 	printf("%c %d ", my_entry_file.data[i], my_entry_file.data[i]);	
	// }
	
	// printf("\n");

	// 

	caixa_write_to_file(&my_caixa_file, "teste.caixa");

	caixa_free(&my_caixa_file);

    return(0);
}