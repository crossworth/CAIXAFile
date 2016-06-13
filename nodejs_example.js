/*
* @Author: Pedro Henrique
* @Date:   2016-06-11 18:36:22
* @Last Modified by:   pedro
* @Last Modified time: 2016-06-12 15:18:29
*/

'use strict';

var fs =  require('fs')

const VERSION = "1.0.0";
const MAX_FILE_NAME = 255;
const MAX_DESCRIPTION_SIZE = 2048;


var caixa_entry = {
	name: new Buffer(MAX_FILE_NAME).fill(0),
	size: new Buffer(64).fill(0),
	data: new Buffer(1).fill(0)
};

var caixa_header = {
	name: new Buffer(MAX_FILE_NAME).fill(0),
	version: new Buffer(10).fill(0),
	description: new Buffer(MAX_DESCRIPTION_SIZE).fill(0),
	number_files: new Buffer(32).fill(0),
	created_on: new Buffer(64).fill(0),
	updated_on: new Buffer(64).fill(0)
};


var caixa_file = {
	header: caixa_header,
	files: [ caixa_entry ]
};


function time() {
	return (Date.now() / 1000);
}


const MAX_UINT32 = 0xFFFFFFFF






// b.writeUInt32BE(big, 0)  // 00 00 01 53 00 00 00 00
// b.writeUInt32BE(low, 4)  // 00 00 01 53 36 9a 06 58


caixa_header.name.write("Internal name of the caixa file");
caixa_header.version.write(VERSION);
caixa_header.description.write("The Description of the caixa file");
caixa_header.number_files.writeUInt32BE(1);


var big = ~~(time() / MAX_UINT32)
var low = (time() % MAX_UINT32) - big
caixa_header.created_on.writeUInt32BE(big, 0);
caixa_header.created_on.writeUInt32BE(low, 4);

var big = ~~(time() / MAX_UINT32)
var low = (time() % MAX_UINT32) - big
caixa_header.updated_on.writeUInt32BE(big, 0);
caixa_header.updated_on.writeUInt32BE(low, 4);


var output = caixa_header.name + caixa_header.version +
caixa_header.description + caixa_header.number_files + 
caixa_header.created_on + caixa_header.updated_on;

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

    // pos_offset += caixa_header.name.length;

    // fs.write(fd, caixa_header.version, pos_offset, caixa_header.version.length, null, function(err) {
    //     if (err) throw 'error writing file: ' + err;
    //     fs.close(fd, function() {
    //         console.log('file written');
    //     })
    // });

    // pos_offset += caixa_header.version.length;

    // fs.write(fd, caixa_header.description, pos_offset, caixa_header.description.length, null, function(err) {
    //     if (err) throw 'error writing file: ' + err;
    //     fs.close(fd, function() {
    //         console.log('file written');
    //     })
    // });

    // pos_offset += caixa_header.description.length;

    // fs.write(fd, caixa_header.number_files, pos_offset, caixa_header.number_files.length, null, function(err) {
    //     if (err) throw 'error writing file: ' + err;
    //     fs.close(fd, function() {
    //         console.log('file written');
    //     })
    // });

    // pos_offset += caixa_header.number_files.length;

    // fs.write(fd, caixa_header.created_on, pos_offset, caixa_header.created_on.length, null, function(err) {
    //     if (err) throw 'error writing file: ' + err;
    //     fs.close(fd, function() {
    //         console.log('file written');
    //     })
    // });

    // pos_offset += caixa_header.created_on.length;

    // fs.write(fd, caixa_header.updated_on, pos_offset, caixa_header.updated_on.length, null, function(err) {
    //     if (err) throw 'error writing file: ' + err;
    //     fs.close(fd, function() {
    //         console.log('file written');
    //     })
    // });

    // pos_offset += caixa_header.updated_on.length;

});
