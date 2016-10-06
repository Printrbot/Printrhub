var Jimp = require("jimp")
  , fs = require('fs')
  , _ = require('underscore')
  , Promise = require('bluebird')
  , _inJobs = 0;


var project = {
  "name":"Pop Buddha",
  "idx": "1abcb111",
  "image": "4.png",
  "total_items": 3,
  "items": [
    {
      "name":"Bender",
      "image": "2.png",
      "idx": "1823bu34",
      "url": "http://printrapp.s3-us-west-1.amazonaws.com/users/projects/1526cae4477c387ffc07bd6cad001614/361b507f425363124da98c1ce9129417/55254ca1f6ae25b371a7b082fed120c1.gcode"
    },
    {
      "name":"Batman",
      "image": "5.png",
      "idx": "1823b111",
      "url": "http://printrapp.s3-us-west-1.amazonaws.com/users/projects/1526cae4477c387ffc07bd6cad001614/d1cea8d34ccb26cce3773fc95689f6a5/73876ab4dcdabd5da5dc803294c57984.gcode"
    },
    {
      "name":"Yoda",
      "image": "6.png",
      "idx": "2823b111",
      "url": "http://printrapp.s3-us-west-1.amazonaws.com/users/projects/1526cae4477c387ffc07bd6cad001614/d1cea8d34ccb26cce3773fc95689f6a5/73876ab4dcdabd5da5dc803294c57984.gcode"
    }
  ]
};

/*

Project File structure

= 32 byte file type id: BCAF406DFF674B11870F0E74B4D44FD4
= 8 byte project idx
= 32 byte project name
= 1 byte total files
= 129600 byte project thumbnail (RGB565) (270x240)*2)
------------------------------------------------------- = 129673
= 8 byte job idx
= 32 byte job name
= 128 byte url
= 129600 byte job thumbnail (RGB565)
-------------------  ...  ----------------------------- = 129768

*/


var imgToBuf = function(img) {
  return new Promise(function(resolve, reject) {
    var buf = new Buffer((240*270)*2);
    var c = 0;
    Jimp.read("./png/"+img).then(function (image) {
      image.dither565();

      for (var x=0;x<image.bitmap.width;x++) {
        for (var y=0;y<image.bitmap.height;y++) {

          var b = image.getPixelColor(x,y);
          var rgba = Jimp.intToRGBA(b);
          var _p = rgba.r << 8 | rgba.g << 3 | rgba.b >> 3;
          buf.writeUInt16LE(_p, c, 2);

          c+=2;
          //console.info('...');
        }
      }
      resolve(buf);
    });
  });
}

function insertJobBuffer(buf) {
  finalBuffer = Buffer.concat([finalBuffer, buf]);
  _inJobs++;
  if (_inJobs == project.total_items) {
    // done, write the buffer to file

    fs.writeFile('./idx/'+project.idx, finalBuffer);
  }
}

var finalBuffer = new Buffer(0);

var pb = new Buffer(73);
pb.write('BCAF406DFF674B11870F0E74B4D44FD4', 0, 32);

//project name - 256 bits
pb.write(project.idx, 32, 8);
pb.write(project.name, 40, 32);
pb.writeInt8(project.total_items, 72);


imgToBuf(project.image)
.then(function(imgb) {


  console.info(imgb.length);
  console.info(pb.length);

  finalBuffer = Buffer.concat([pb, imgb]);

  console.info(finalBuffer.length);

  _.each(project.items, function(i, k) {
    var bs = 8+32+128 // idx + name + url
      , ji = new Buffer(bs);

    ji.write(i.idx, 0, 8);
    ji.write(i.name, 8, 32);
    ji.write(i.url, 40, 128);

    imgToBuf(i.image)
    .then(function(jib) {
      var tb = Buffer.concat([ji, jib]);
      insertJobBuffer(tb);
      //finalBuffer = tb;
      //console.info(finalBuffer.length);
    });
  })




})




/*
imgToBuf(project.image, function(b) {
  console.info('here');
  console.info(b);
  //fs.writeFile('./idx/'+project.idx, pb);
  fs.writeFile('./idx/'+project.idx, Buffer.concat([pb, b]));
});
*/


/*

fs.readdirSync('./png').forEach(function(file) {
  var _e = file.split(".");
  if (_e[1] == 'png') {

    var c = 0;
    var img = _e[0];

    Jimp.read("./png/"+img+'.png').then(function (image) {
      image.dither565();
      var buf = new Buffer((image.bitmap.width * image.bitmap.height)*2);
      var str = "";
      for (var x=0;x<image.bitmap.width;x++) {
    	  for (var y=0;y<image.bitmap.height;y++) {
          var b = image.getPixelColor(x,y);
          var rgba = Jimp.intToRGBA(b);

          var _p = rgba.r << 8 | rgba.g << 3 | rgba.b >> 3;

          if (x>0 || y>0) str += ",";
          str += _p;

          buf.writeUInt16LE(_p, c, 2);
          c+=2;
        }
      }
      fs.writeFile('./raw/'+img+'.dat', buf);
      fs.writeFile('./str/'+img+'.txt', str);
    }).catch(function (err) {
      console.error(err);
    });
  }
});
*/
