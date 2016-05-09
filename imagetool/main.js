var Jimp = require("jimp");
var img = "startup";
var fs = require('fs');


var c = 0;
//var out = fs.createWriteStream('./raw/'+img+'.dat');


Jimp.read("./png/"+img+'.png').then(function (image) {

  image.dither565();
  var buf = new Buffer((image.bitmap.width * image.bitmap.height)*2, 'utf16le');


  for (var x=0;x<image.bitmap.width;x++) {
	  for (var y=0;y<image.bitmap.height;y++) {
      var b = image.getPixelColor(x,y);

      //out.write(b);
      //console.info(new Buffer(b));
      //console.info(b);
      buf.write(b.toString(8), c, 2);
      //out.write();

      c+=2;
    }
  }

  fs.writeFile('./raw/'+img+'.dat', buf);

  //out.write(buf);
  //out.end();

  console.info(c);

/*
  console.info(image.bitmap.data);
  console.info(image.bitmap.width);
  console.info(image.bitmap.height);
*/
}).catch(function (err) {
  console.error(err);
});
