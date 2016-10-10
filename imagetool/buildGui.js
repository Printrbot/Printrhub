var Jimp = require("jimp")
  , Promise = require('bluebird')
  , fs = Promise.promisifyAll(require("fs"))
  , _ = require('underscore')
  , _inJobs = 0;

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
    var currentColor = 0;
    var colorCounter = 0;
    Jimp.read("./gui/png/"+img).then(function (image) {
      var fbuf = new Buffer(0);
      image.dither565();
      for (var x=0;x<image.bitmap.width;x++) {
        for (var y=0;y<image.bitmap.height;y++) {
          var b = image.getPixelColor(x,y);
          if (x == 0 && y == 0) {
            currentColor = b;
            colorCounter += 1;
          } else {
            if (b == currentColor) {
              colorCounter += 1;
            } else {
              // write previous color
              var rgba = Jimp.intToRGBA(currentColor);
              var _p = rgba.r << 8 | rgba.g << 3 | rgba.b >> 3;
              var buf = new Buffer(3);
              buf.writeUInt8(colorCounter);
              buf.writeUInt16LE(_p, 1);
              fbuf = Buffer.concat([fbuf, buf]);
              currentColor = b;
              colorCounter = 1;
            }
          }
        }
      }
      console.info("RESOLVING ", fbuf.length)
      resolve(fbuf);
    });
  });
}


var promises = []
  , pngs = [];

fs.openAsync('./gui/ui', 'w')
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
