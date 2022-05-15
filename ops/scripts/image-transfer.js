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

// Checks if the colour is in the map already and returns the index if it finds it
const matchColours = (array, colour) => {
  let result = undefined;

  array.forEach((item, index) => {
    if (
      item.r === colour.r &&
      item.g === colour.g &&
      item.b === colour.b
    ) {
      result = index;
    }
  });

  return result;
}

const transferImageFile = async (file) => {
  Jimp.read(`images/raw/${file}`).then(async (image) => {
    console.log(`Transferring ${file}...`);

    const width = image.bitmap.width;
    const height = image.bitmap.height;

    console.log(`w: ${width} h: ${height}`);

    // Image Globals...

    const colourMap = [];
    const imageArray = [];

    for (let y = 0; y < height; y++) {
      for (let x = 0; x < width; x++) {
        const pixelColour = Jimp.intToRGBA(image.getPixelColor(x, y));

        // Add the colour to the colour map if it is not already there...

        let result = matchColours(colourMap, pixelColour);

        if (result === undefined) {
          imageArray.push(colourMap.length);

          colourMap.push(pixelColour);
        } else {
          imageArray.push(result);
        }
      }
    }

    const fixedColourMap = [];

    colourMap.forEach((colour) => {
      const redBits = Math.round(colour.r / 256 * 32);
      const greenBits = Math.round(colour.g / 256 * 32);
      const blueBits = Math.round(colour.b / 256 * 32);

      const hex = parseInt(`${redBits.toString(2)}${greenBits.toString(2)}${blueBits.toString(2)}`, 2).toString(16).toUpperCase();

      fixedColourMap.push(`0x${hex}`);
    });

    const fileName = file.replace('.png', '.c');
    const imageName = file.split('.')[0];

    // Write the file to the processed directory...
    await fs.writeFile(
      `images/processed/${fileName}`,
      `int ${imageName}ColorMap[] = {${fixedColourMap}};
int ${imageName}[] = {${imageArray}};
int ${imageName}Size = ${imageArray.length};
int ${imageName}Width = ${width};\n`
    );
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

