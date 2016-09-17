# CaixaFile - Caixa File Format
---

Create packages (non-compressed .caixa files), the format has support for simple informations like **name**, **version**, **description**, **number of files**, **date of creation** and **date of the last update**.


## How to use
You can use the C header or the use the NodeJS example for handling Caixa Files.

**C**

```
	caixa_file my_caixa_file = {};
	caixa_create_header(&my_caixa_file.header, "Name", "Descripition");

	// to open a file
	caixa_open_file(&my_caixa_file, "test.caixa");

	// get header data
	printf("Caixa Header Name: %s\n", my_caixa_file.header.name);
	printf("Caixa Header Description: %s\n", my_caixa_file.header.description);
	printf("Caixa Header Version: %s\n", my_caixa_file.header.version);
	printf("Caixa Header Number Files: %u\n", my_caixa_file.header.number_files);
	printf("Caixa Header Created on: %llu\n", my_caixa_file.header.created_on);
	printf("Caixa Header Updated on: %llu\n", my_caixa_file.header.updated_on);

	// get the entry data
	for(uint32_t index = 0; index < my_caixa_file.header.number_files; index++) {

	    printf("Caixa Entry File Name: %s\n", my_caixa_file.files[index]->name);
	    printf("Caixa Entry File Size: %llu\n", my_caixa_file.files[index]->size);
	    printf("Caixa Entry Sizeof: %lu\n", sizeof(my_caixa_file.files[index]));
	
	    printf("Caixa Entry Data: ");

	    for(int i = 0; i <= my_caixa_file.files[index]->size; i++) {
	    	printf("%c", my_caixa_file.files[index]->data[i]);	
	    }
		
	    printf("\n");
	}

	// get file based on name (byte is a simple array of char's)
	byte *test_file = caixa_get_file(&my_caixa_file, "test.txt");
	printf("Test File: %s\n", test_file);


	// create an new entry on the caixa file
	caixa_entry my_entry_file = {};

	caixa_crete_entry_from_file(&my_entry_file, "test.txt");
	caixa_add_entry(&my_caixa_file, &my_entry_file);

	// remove a entry based on name
	caixa_remove_file(&my_caixa_file, "test.txt");

	// write changes to the file
	caixa_write_to_file(&my_caixa_file, "test.caixa");

	// release the file
	caixa_free(&my_caixa_file);

```



**NodeJS** - You have to change the `nodejs_example.js` file since the NodeJS is not ready.

```
// setting header information
caixa_header.name.write("Name");
caixa_header.version.write(VERSION);
caixa_header.description.write("Description", 0, MAX_DESCRIPTION_SIZE);
caixa_header.number_files.writeUInt32LE(1);
caixa_header.created_on.writeUInt64LE(time());
caixa_header.updated_on.writeUInt64LE(time());


var test = fs.readFileSync("test.txt");

// creating a new entry
var fileEntry = caixa_entry.create("test.txt", test.length, test);
caixa_file.files.push(fileEntry);

// getting the buffer for writing
var output = caixa_file.getBuffer();

// writing the output
fs.open("ouput.caixa", 'w', function(err, fd) {
    if (err) {
        throw 'error opening file: ' + err;
    }

    fs.write(fd, output, 0, output.length, null, function(err) {
        if (err) throw 'error writing file: ' + err;
        fs.close(fd, function() {
            console.log('file written');
        })
    });

});

```