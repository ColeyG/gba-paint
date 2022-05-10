import Jimp from 'jimp';
import { promises as fs } from 'fs';
import path from 'path';

// Configuration
const fileTypes = ['.png', '.jpg', '.jpeg'];

const includesAny = (string, array) => {
  for (const item of array) {
    if (string.includes(item)) {
      return true;
    }
  }

  return false;
}

const transferImageFile = (file) => {
  Jimp.read(`images/raw/${file}`).then((image) => {
    console.log(`Transferring ${file}...`);

    const width = image.bitmap.width;
    const height = image.bitmap.height;

    console.log(`w: ${width} h: ${height}`);

    for (let y = 0; y < height; y++) {
      for (let x = 0; x < width; x++) {
        // console.log(`x: ${x} y: ${y}`);
        console.log(Jimp.intToRGBA(image.getPixelColor(x, y)));

        // Add the colour to the colour map if it is not already there...

        // Add an index to the image array bound to the colour map...

        // Make each rgb value 5 bit instead of 8 bit integers...
        // Divide by 256 and multiply by 32 and encode as hex...

        // Write the file to the processed directory...
      }
    }
  });
}

// Read images and put any allowed images through the transformer
await fs.readdir('images/raw').then((files) => {
  files.forEach((file) => {
    if (includesAny(file, fileTypes)) {
      transferImageFile(file);
    }
  });
});

