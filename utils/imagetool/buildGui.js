var Jimp = require("jimp")
  , Promise = require('bluebird')
  , fs = Promise.promisifyAll(require("fs"))
  , _ = require('underscore')
  , _inJobs = 0;

var imgToBuf = function(img) {
  return new Promise(function(resolve, reject) {
    var c = 0;
    Jimp.read("./gui/png/"+img).then(function (image) {
      var buf = new Buffer((image.bitmap.width*image.bitmap.height)*2);
      image.dither565();
      for (var x=0;x<image.bitmap.width;x++) {
        for (var y=0;y<image.bitmap.height;y++) {
          var b = image.getPixelColor(x,y);
          var rgba = Jimp.intToRGBA(b);
          var _p = rgba.r << 8 | rgba.g << 3 | rgba.b >> 3;
          buf.writeUInt16LE(_p, c, 2);
          c+=2;
        }
      }
      resolve([buf, image.bitmap.width, image.bitmap.height]);
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
  Promise.reduce(pngs, function(offset, png) {
    return createImg(png, offset);
  }, 0);


});

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

/*
_.each(pngs, function(p) {
  var ps = imgToBuf(p).spread(function(ibuf, w, h) {
    fs.appendFileAsync('./gui/ui', ibuf)
    .then(function() {
      var name = p.split(".")[0];
      offset += ibuf.length;
      return fs.appendFileAsync('./gui/struct.h', 'UIBitmap ' + name + ' = {' + (offset-ibuf.length) + ',' + ibuf.length + ',' + w + ',' + h + '};\n');
    })
  })
  promises.push(ps);
});

Promise.all(promises).then(function(promises) {
  console.info('all done');
})
*/
