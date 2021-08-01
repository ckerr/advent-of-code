const fs = require('fs');

function getFrequencies(line) {
	const frequencies = new Map();
	for (const ch of line) {
		frequencies.set(ch, (frequencies.get(ch) || 0) + 1);
	}
  return frequencies;
}

function getChecksum(line) {
	const frequencies = getFrequencies(line);
	const ret = {};
	const counts = [...frequencies.values()];
	if (counts.includes(2)) ret.two = true;
	if (counts.includes(3)) ret.three = true;
	return ret;
}

function one(lines) {
	let two = 0;
	let three = 0;
  for (const line of lines) {
		const check = getChecksum(line);
		if (check.two) ++two;
		if (check.three) ++three;
	}

	console.log(two * three);
}

function getDiffIndices(a, b) {
	let indices = [];
	for (let i = 0; i < a.length; ++i) {
		if (a[i] != b[i]) indices.push(i);
	}
	return indices;
}

function two(lines) {
	for (a of lines) {
		for (b of lines) {
			if (a === b) continue;
			const indices = getDiffIndices(a, b);
			if (indices.length === 1) {
				const result = [...a];
				indices.reverse();
				for (const idx of indices) result.splice(idx, 1);
				console.log(result.join(''));
				return;
			}
		}
	}
}

const filename = 'input.txt';
// const filename = 'test.1.txt';
// const filename = 'test.2.txt';
const lines = fs.readFileSync(filename, 'utf8').split('\n').filter((line) => !!line);
// one(lines);
two(lines);
