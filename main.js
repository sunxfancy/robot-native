const {createFunction} = require('./build/Release/robot-native');

const fn = createFunction();
console.log(fn());