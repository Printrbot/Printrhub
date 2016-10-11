var Jimp = require("jimp")
  , Promise = require('bluebird')
  , fs = Promise.promisifyAll(require("fs"))
  , _ = require('underscore')
  , _inJobs = 0;

function toHex(i) {
    return "0x" + i.toString(16).toUpperCase();
}

function RGB565(color) {
  var r = color >> 24 & 0xFF;
  var g = color >> 16 & 0xFF;
  var b = color >> 8 & 0xFF;
  var a = color & 0xFF;

  var rgb565 = ((r & 0xF8) << 8) | ((g & 0xFC) << 3) | (b >> 3);
  return rgb565;
}

var imgToBuf = function(img) {
  return new Promise(function(resolve, reject) {
    Jimp.read("./gui/png/"+img).then(function (image) {
      var fbuf = new Buffer(0);
      image.dither565();
      for (var x=0;x<image.bitmap.width;x++) {
        for (var y=0;y<image.bitmap.height;y++) {
          var b = image.getPixelColor(x,y);
          var rgba = Jimp.intToRGBA(b);
          var _p = rgba.r << 8 | rgba.g << 3 | rgba.b >> 3;
          var buf = new Buffer(2);
          buf.writeUInt16LE(_p);
          fbuf = Buffer.concat([fbuf, buf])
        }
      }
      resolve([fbuf, image.bitmap.width, image.bitmap.height]);
    });
  });
}

var cImgToBuf = function(img) {
  return new Promise(function(resolve, reject) {
    var lastColor = 0;
    var colorCounter = 0;
    console.log(img);
    Jimp.read("./gui/png/"+img).then(function (image) {
      var fbuf = new Buffer(0);
      for (var x=0;x<image.bitmap.width;x++) {
        for (var y=0;y<image.bitmap.height;y++) {
          //Read 32-bit color value from image
          var b = image.getPixelColor(x,y);
          //Compress to RGB565 format
          b = RGB565(b);

          if (colorCounter == 0) {
            lastColor = b;
            colorCounter++;
          } else {
            //Write to buffer if color is different or colorCounter will overflow (as it's only one byte we can only store up to 255 chunks of data)
            if (b != lastColor || colorCounter > 255) {
              //We finished this chunk of data, write to file
              var buf = new Buffer(3);
              buf.writeUInt8(colorCounter);
              buf.writeUInt16LE(lastColor);
              fbuf = Buffer.concat([fbuf, buf])
              colorCounter = 1;
              lastColor = b;
            }
          }
        }
      }

      //Make sure we also store the last chunk of data
      if (colorCounter > 0) {
        var buf = new Buffer(3);
        buf.writeUInt8(colorCounter);
        buf.writeUInt16LE(lastColor);
        fbuf = Buffer.concat([fbuf, buf])
      }
      console.info("RESOLVING ", fbuf.length)
      resolve(fbuf);
    });
  });
}


var promises = []
  , pngs = [];

fs.openAsync('./gui/ui', 'w')
fs.openAsync('./gui/ui.min', 'w')
.then(fs.openAsync('./gui/struct.h', 'w'))
.then(fs.readdirSync('./gui/png').forEach(function(file) {
  if (file[0] != ".")
    pngs.push(file);
}))
.then(function() {
  // reduce here
  return Promise.reduce(pngs, function(offset, png) {
    return createCImg(png);
  }, 0);
})
.then(function() {
  // reduce here
  return Promise.reduce(pngs, function(offset, png) {
    return createImg(png, offset);
  }, 0);
})

var createCImg = function(imgPath) {
  return new Promise(function(resolve, reject) {
    var ps = cImgToBuf(imgPath).then(function(ibuf) {
      console.info(ibuf.length)
      fs.appendFileAsync('./gui/ui.min', ibuf)
      resolve(ibuf.length);
    })
  })
}

var createImg = function(imgPath, offset) {
  return new Promise(function(resolve, reject) {
    var ps = imgToBuf(imgPath).spread(function(ibuf, w, h) {
      fs.appendFileAsync('./gui/ui', ibuf)
      .then(function() {
        var name = imgPath.split(".")[0];
        fs.appendFileAsync('./gui/struct.h', 'UIBitmap ' + name + ' = {' + (offset) + ',' + ibuf.length + ',' + w + ',' + h + '};\n')
        .then(function() {
          setTimeout(function() {
            resolve(offset + ibuf.length);
          }, 100);
        })
      })
    })
  })
}
