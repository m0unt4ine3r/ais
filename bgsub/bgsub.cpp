#include<iostream>
#include<fstream>
#include<string>
#include<cmath>

using namespace std;

//struct to represent a pixel
struct pixel {
	int r, g, b;

	void print();
	int getrgb();
};

//prints rgb of pixel
void pixel::print()
{
	cout << r << " ";
	cout << g << " ";
	cout << b << "\n";
}

//struct to represent a ppm file
struct ppm {
	string filename, type, comment;
	int size[3], intensity;
	fstream file;
	pixel *set;

	void setppm(string);
	void printrgb();
	int getppm();
	void writeppm();
	ppm copyppm(string);
};

//reads in ppm data and sets relevant variables
void ppm::setppm(string path)
{
	filename = path;
	file.open(filename.c_str(), ios::in);

	getline(file, type);
	getline(file, comment);
	file >> size[0];
	file >> size[1];
	size[2] = size[0] * size[1];
	file >> intensity;

	set = new pixel[size[2]];

	int j = 0;
	for (int i=0; i < size[2]*3; i++) {
		if (i % 3 == 0)
			file >> set[j].r;
		else if (i % 3 == 1)
			file >> set[j].g;
		else if (i % 3 == 2) {
			file >> set[j].b;
			j++;
		}
	}

	file.close();
}

//prints prints all pixels
void ppm::printrgb()
{
	for (int i=0; i < size[2]; i++) {
		cout << "Pixel: " << i << "\n";
		set[i].print();
	}
}

//writes data to ppm
void ppm::writeppm()
{
	file.open(filename.c_str(), ios::out);

	file << type << "\n";
	file << comment << "\n";
	file << size[0] << " " << size[1] << "\n";
	file << intensity << "\n";

	for (int i=0; i < size[2]; i++) {
		file << set[i].r << " ";
		file << set[i].g << " ";
		file << set[i].b << "\n";
	}

	file.close();
}

int main()
{
	int fc = 21;
	string in[fc], out[fc];
	
	string text = "i/mage002.ppm";
	for (int i=0; i < fc; i++) {	//generates input filenames
		if (text[14] > '9') {
			text[13]++;
			text[14] = '0';
		}
		in[i] = text;
		text[14]++;
	}
	
	text = "new002.ppm";		//outfile name
	for (int i=0; i < fc; i++) {	//generates output filenames
		if (text[5] > '9') {
			text[4]++;
			text[5] = '0';
		}
		out[i] = text;
		text[11]++;
	}

	ppm image[fc];
	ppm sub[fc];

	for (int i=0; i < fc; i++)	//gets data from each ppm and reads into an array of ppm structs
		image[i].setppm(in[i]);

	for (int i=0; i < fc; i++) {	//copies base data to another array of ppm structs for processing
		sub[i].filename = out[i];
		sub[i].type = image[i].type;
		sub[i].comment = image[i].comment;
		sub[i].size[0] = image[i].size[0];
		sub[i].size[1] = image[i].size[1];
		sub[i].size[2] = image[i].size[2];
		sub[i].intensity = image[i].intensity;
		sub[i].set = new pixel[sub[i].size[2]];
	}

	double dist, avg, avgint;

	for (int k=0; k < fc; k++) {					//calulates mean difference for each pixel in every picture
		for (int j=0; j < image[0].size[2]; j++) {
			avg = 0;
			for (int i=0; i < fc-1; i++) {
				if (k != i) {
					dist = pow((image[i].set[j].r - image[i+1].set[j].r), 2) + pow((image[i].set[j].g - image[i+1].set[j].g), 2) + pow((image[i].set[j].b - image[i+1].set[j].b), 2);
					avg += dist;
				}
			}

			avg /= (fc-2);
			avgint = (image[k].set[j].r + image[k].set[j].g + image[k].set[j].b) / 3;

			if (avg >= 100 && avgint <= 125) {		//sets pixels in second ppm array based on previously calculated averages against a threshhold
				sub[k].set[j].r = image[k].set[j].r;
				sub[k].set[j].g = image[k].set[j].g;
				sub[k].set[j].b = image[k].set[j].b;
			}
			else {
				sub[k].set[j].r = 255;
				sub[k].set[j].g = 255;
				sub[k].set[j].b = 255;
			}
		}
	}

	for (int i=0; i < fc; i++) //writes second ppm array to new ppm files
		sub[i].writeppm();

	return 0;
}
