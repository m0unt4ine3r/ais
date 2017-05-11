#include<iostream>
#include<fstream>

using namespace std;

int main()
{
	int height, width, red, blue, green;
	ofstream ppm;

	cout << "Enter width: ";
	cin >> width;
	
	cout << "Enter height: ";
	cin >> height;
	
	cout << "Enter red: ";
	cin >> red;
	
	cout << "Enter blue: ";
	cin >> blue;
	
	cout << "Enter green: ";
	cin >> green;
	
	ppm.open("pic.ppm");

	ppm << "P3\n" << width << " " << height << "\n255\n";

	for (int i=0; i < height*width; i++)
		ppm << red << " " << blue << " " << green << "\n";

	ppm.close();

	return 0;
}
