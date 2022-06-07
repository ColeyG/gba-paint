/**
 * This exists because the GBA encodes its colours as 5 bit bgr hex values
 * 
 * Takes one arg of an rgb "29, 43, 83"
 * 
 * Invoked like `npm run rgb-to-hex 29, 43, 83`
 */

const args = process.argv.slice(2);

const rgbArray = args.join("").split(",");

const minBitSpace = (binaryString) => {
  if (binaryString.length === 5) {
    return binaryString;
  } else {
    for (let i = binaryString.length; i < 5; i++) {
      binaryString = `0${binaryString}`
    }

    return binaryString;
  }
}

if (rgbArray.length === 3) {
  console.log(rgbArray);
  let r = rgbArray[0];
  let g = rgbArray[1];
  let b = rgbArray[2];

  // Turn each value into a binary 5 bit representation
  r = Math.floor((r / 256) * 32).toString(2);
  g = Math.floor((g / 256) * 32).toString(2);
  b = Math.floor((b / 256) * 32).toString(2);

  r = minBitSpace(r);
  g = minBitSpace(g);
  b = minBitSpace(b);

  console.log(`r: ${r} g: ${g} b: ${b}`);

  console.log(`0x${parseInt(`${b}${g}${r}`, 2).toString(16).toUpperCase()}`);
} else {
  console.log("That value seems malformed");
}
