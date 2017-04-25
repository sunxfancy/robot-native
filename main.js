const addon = require('./build/Release/robot-native');

const fn = addon();
console.log(fn());