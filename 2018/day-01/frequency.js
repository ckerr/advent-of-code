const fs = require('fs');
const path = require('path');

function one(lines) {
  let calibration = 0;
  for (const line of lines) calibration += Number.parseInt(line);
  console.log(calibration);
}

function two(lines) {
  let calibration = 0;
  const visited = new Set();
  for (;;) { 
    for (const line of lines) {
      calibration += Number.parseInt(line);
      if (visited.has(calibration)) {
        console.log(`visited ${calibration} twice`);
        return;
      }
      visited.add(calibration);
    }
  }
}

const lines = fs.readFileSync('input.txt', 'utf8').split('\n').filter((line) => !!line);
one(lines);
two(lines);
