/*
* @Author: Pedro Henrique
* @Date:   2016-06-11 18:36:22
* @Last Modified by:   Pedro Henrique
* @Last Modified time: 2016-06-14 11:03:28
*/

'use strict';

var fs =  require('fs')

const VERSION = "1.0.0";
const MAX_FILE_NAME = 255;
const MAX_DESCRIPTION_SIZE = 2048;


Buffer.prototype.writeUInt64LE = function(data) {
    this.writeInt32LE(data & -1, 0)
    this.writeUInt32LE(Math.floor(data / 0x100000000), 4)
};

var caixa_entry = {
	name: new Buffer(MAX_FILE_NAME).fill(0),
	size: new Buffer(8).fill(0),
	data: "", 
    create: function(name, size, data) {
        this.name.write(name);
        this.size.writeUInt32LE(size);
        this.data = data;

        return this;
    }, 
    getBuffer: function() {
        return this.name + this.size + this.data;
    }
};

var caixa_header = {
	name: new Buffer(MAX_FILE_NAME).fill(0),
	version: new Buffer(10).fill(0),
	description: new Buffer(MAX_DESCRIPTION_SIZE).fill(0),
	number_files: new Buffer(4).fill(0),
	created_on: new Buffer(8).fill(0),
	updated_on: new Buffer(8).fill(0),
    getBuffer: function() {
        return this.name + this.version + this.description + this.number_files 
                + this.created_on + this.updated_on;
    }
};

var caixa_file = {
	header: caixa_header,
	files: [],
    getBuffer: function() {
        var files= "";

        for(var index = 0; index < this.files.length; index++) {
            files = files + this.files[index].getBuffer();
        }

        return this.header.getBuffer() +  files;
    }
};


function time() {
	return (Date.now() / 1000);
}



caixa_header.name.write("Internal name of the caixa file");
caixa_header.version.write(VERSION);
caixa_header.description.write("The Description of the caixa file", 0, MAX_DESCRIPTION_SIZE);
caixa_header.number_files.writeUInt32LE(1);
caixa_header.created_on.writeUInt64LE(time());
caixa_header.updated_on.writeUInt64LE(time());


var test = fs.readFileSync("test.txt");

var fileEntry = caixa_entry.create("test.txt", test.length, test);
caixa_file.files.push(fileEntry);


var output = caixa_file.getBuffer();

fs.open("teste2.caixa", 'w', function(err, fd) {
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
