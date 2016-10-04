var AWS = require('aws-sdk')
  , fs = require('fs')
  , Promise = require('bluebird');

AWS.config.update({accessKeyId: '_CHANGE_ME_', secretAccessKey: '_CHANGE_ME_'});

function upload(filename) {
  return new Promise(function(resolve, reject) {
    fs.readFile("files/"+filename, function (err, data) {
      if (err) {
        cb(err, null);
      } else {
        var params = {
          Key: "firmware/simple/"+filename,
          Body: data,
          ACL: 'public-read'
        }
        var s3bucket = new AWS.S3({params: { Bucket: 'static.printrbot.cloud' }});
        s3bucket.upload(params, function(err, sres) {
          if (err) {
            reject(err);
          } else {
            resolve();
          }
        });
      }
    });
  });
}

var files = ['firmware.json', 'mk20.bin', 'esp.bin'];

Promise.reduce(files, function(a, b) {
  console.info("uploading: ",b);
  return upload(b);
}, 0)
