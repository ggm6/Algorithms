// Garrett McDonnell
// Algorithms Project 3 - Seam Carving

#include <iostream>
#include <vector>
#include <fstream>
#include <sstream>
#include <string>
#include <algorithm>
#include <cmath>
#include <unistd.h>

using namespace std;

int readFile(vector <vector<int> >& vec, const string& imageFile);
void writeFile(const vector <vector<int> >& newPixels, const string& inFile, const int& maxIntensity);
void computeEnergies(vector <vector<int> >& energyVec, const vector <vector<int> >& imageVec);
void cumulativeVerticalEnergies(const vector <vector<int> >& energyVec, vector <vector<int> >& newEnergies);
void cumulativeHorizontalEnergies(const vector <vector<int> >& energyVec, vector <vector<int> >& newEnergies);
void findVerticalSeam(vector <pair<int,int> >& path, const vector <vector<int> >& energyVec);
void findHorizontalSeam(vector <pair<int,int> >& path, const vector <vector<int> >& energyVec);
void removeVerticalSeam(const vector <pair<int,int> >& path, vector <vector<int> >& imageVec);
void removeHorizontalSeam(const vector <pair<int,int> >& path, vector <vector<int> >& imageVec);
void displayEnergyPath(const vector <pair<int,int> >& path, const vector <vector<int> >& energies);
void displayPath(const vector <pair<int,int> >& path);
void copyPixels(const vector <vector<int> >& imageVec, vector <vector<int> >& newImageVec);
void comPair(const vector <pair<int,int> >& temp, vector <pair<int,int> >& path, const vector <vector<int> >& energyVec);




int main(int argc, char* argv[])
{
	const string imageFile = argv[1];
	string vertSeamsStr = argv[2];
	int vertSeams;
	istringstream(vertSeamsStr) >> vertSeams;

	string horizSeamsStr = argv[3];
	int horizSeams;
	istringstream(horizSeamsStr) >> horizSeams;

	vector <vector<int> > pixels;
	const int maxIntensity = readFile(pixels,imageFile);

	vector <vector<int> > newPixels;
	copyPixels(pixels,newPixels);


	for (int i=0; i<vertSeams; ++i) {
		vector <vector<int> > energies;
		computeEnergies(energies,newPixels);

		vector <vector<int> > cumEnergies;
		cumulativeVerticalEnergies(energies,cumEnergies);

		vector <pair<int,int> > path;
		findVerticalSeam(path,cumEnergies);
		removeVerticalSeam(path,newPixels);
	}

	for (int j=0; j<horizSeams; ++j) {
		vector <vector<int> > energies;
		computeEnergies(energies,newPixels);

		vector <vector<int> > cumEnergies;
		cumulativeHorizontalEnergies(energies,cumEnergies);

		vector <pair<int,int> > path;
		findHorizontalSeam(path,cumEnergies);
		removeHorizontalSeam(path,newPixels);
	}

	writeFile(newPixels,imageFile,maxIntensity);

	return 0;
}




int readFile(vector <vector<int> >& vec, const string& imageFile) {
	ifstream file;
	file.open(imageFile);

	int count=3;
	vector<int> v;
	string init;
	int maxIntens;
	while(count>0  && getline(file,init)) {

		init.erase(remove(init.begin(),init.end(),'\r'),init.end());
		init.erase(remove(init.begin(),init.end(),'\n'),init.end());

		if (init.find('#')<init.length())
			++count;
		else if (init.find('#')>init.length() && init.find('P')>init.length()) {
			if (init.find(' ')<init.length()) {
				size_t pos=init.find(' ');
				string xStr=init.substr(0,pos);
				string yStr=init.substr(pos+1,init.length());
				int xInt,yInt;
				istringstream(xStr) >> xInt;
				istringstream(yStr) >> yInt;
				v.push_back(xInt),v.push_back(yInt);
			}
			else {
				istringstream(init) >> maxIntens;
			}
		}
		--count;
	}

	vector<int> tempVec;
	int token;
	while (file >> token)
		tempVec.push_back(token);

	file.close();

	vec.resize(v[1]);
	for (int i=0; i<v[1]; ++i) {
		vec[i].resize(v[0]);
	}

	auto it=tempVec.begin();
	for (int i=0; i<vec.size(); ++i) {
		int count=0;
		for (int j=0; j<vec[i].size(); ++j) {
			vec[i][j]=*it;
			++it;
		}
	}

	return maxIntens;
}

void computeEnergies(vector <vector<int> >& energyVec, const vector <vector<int> >& imageVec) {
	energyVec.resize(imageVec.size());
	for (int i=0; i<imageVec.size(); ++i)
		energyVec[i].resize(imageVec[i].size());

	for (int i=0; i<imageVec.size(); ++i) {
		for (int j=0; j<imageVec[i].size(); ++j) {

			int deltaX=0;
			int leftX=imageVec[i][j],rightX=imageVec[i][j];
			if (j+1<imageVec[i].size())
				rightX = imageVec[i][j+1];
			if (j-1>=0)
				leftX = imageVec[i][j-1];
			deltaX = abs(imageVec[i][j]-leftX) + abs(imageVec[i][j]-rightX);

			int deltaY=0;
			int downY=imageVec[i][j],upY=imageVec[i][j];
			if (i+1<imageVec.size())
				downY = imageVec[i+1][j];
			if (i-1>=0)
				upY = imageVec[i-1][j];
			deltaY = abs(imageVec[i][j]-upY) + abs(imageVec[i][j]-downY);

			energyVec[i][j]=deltaX + deltaY;
		}
	}
}

void removeVerticalSeam(const vector <pair<int,int> >& path, vector <vector<int> >& imageVec) {
	for (int i=0; i<path.size(); ++i) {
		auto it=imageVec[path[i].first].begin();

		for (int j=0; j<path[i].second; ++j) {
			++it;
		}

		imageVec[path[i].first].erase(it);
	}
}

void removeHorizontalSeam(const vector <pair<int,int> >& path, vector <vector<int> >& imageVec) {
	vector <vector<int> > temp;
	temp.resize(imageVec[0].size());
	for (int i=0; i<imageVec[0].size(); ++i)
		temp[i].resize(imageVec.size());

	for (int i=0; i<temp.size(); ++i) {
		for (int j=0; j<temp[i].size(); ++j) {
			temp[i][j]=imageVec[j][i];
		}
	}

	for (int i=0; i<path.size(); ++i) {
		auto it=temp[path[i].second].begin();

		for (int j=0; j<path[i].first; ++j) {
			++it;
		}

		temp[path[i].second].erase(it);
	}

	imageVec.erase(imageVec.end()-1,imageVec.end());

	for (int i=0; i<imageVec.size(); ++i) {
		for (int j=0; j<imageVec[i].size(); ++j) {
			imageVec[i][j]=temp[j][i];
		}
	}
}

void displayEnergyPath(const vector <pair<int,int> >& path, const vector <vector<int> >& energies) {
	for (int i=0; i<energies.size(); ++i) {
		for (int j=0; j<energies[i].size(); ++j) {
			bool flag=false;
			for (int k=0; k<path.size(); ++k) {
				if (path[k].first == i && path[k].second == j) {
					flag=true;
					break;
				}
			}
			if (flag)
				cout << '#' << ' ';
			else
				cout << '|' << ' ';
		}
		cout << endl;
	}
}

void displayPath(const vector <pair<int,int> >& path) {
	for (int i=0; i<path.size(); ++i)
		cout << "(" << path[i].first << "," << path[i].second << ")" << endl;
}

void copyPixels(const vector <vector<int> >& imageVec, vector <vector<int> >& newImageVec) {
	newImageVec.resize(imageVec.size());
	for (int i=0; i<imageVec.size(); ++i)
		newImageVec[i].resize(imageVec[i].size());

	for (int i=0; i<imageVec.size(); ++i) {
		for (int j=0; j<imageVec[i].size(); ++j) {
			newImageVec[i][j]=imageVec[i][j];
		}
	}
}

void comPair(const vector <pair<int,int> >& temp, vector <pair<int,int> >& path, const vector <vector<int> >& energyVec) {
	int tempIntens=0;
	for (int i=0; i<temp.size(); ++i) {
		tempIntens += energyVec[temp[i].first][temp[i].second];
	}

	int pathIntens=0;
	for (int i=0; i<path.size(); ++i) {
		pathIntens += energyVec[path[i].first][path[i].second];
	}

	if (tempIntens > pathIntens) {
		for (int i=0; i<path.size(); ++i) {
			path[i] = make_pair(temp[i].first,temp[i].second);
		}
	}
}

void writeFile(const vector <vector<int> >& newPixels, const string& inFile, const int& maxIntensity) {
	ofstream file;
	string temp = inFile;
	temp = temp.substr(0,temp.find('.'));
	file.open(temp + "_image_processed.pgm");
	file << "P2" << endl;
	file << "# Created by GStyleMcDon" << endl;
	file << newPixels[0].size() << ' ' << newPixels.size() << endl;
	file << maxIntensity << endl;
	for (int i=0; i<newPixels.size(); ++i) {
		for (int j=0; j<newPixels[i].size(); ++j) {
			if (j!=newPixels[i].size()-1)
				file << newPixels[i][j] << ' ';
			else
				file << newPixels[i][j];
		}
		if (i!=newPixels.size()-1)
			file << endl;
	}
}

void cumulativeVerticalEnergies(const vector <vector<int> >& energyVec, vector <vector<int> >& newEnergies) {
	newEnergies.resize(energyVec.size());
	for (int i=0; i<newEnergies.size(); ++i)
		newEnergies[i].resize(energyVec[i].size());

	for (int i=0; i<newEnergies.size(); ++i) {
		for (int j=0; j<newEnergies[i].size(); ++j)
			newEnergies[i][j]=energyVec[i][j];
	}

	for (int i=1; i<energyVec.size(); ++i) {
		for (int j=0; j<energyVec[i].size(); ++j) {
			int min=2147483647;
			if (j-1>=0 && newEnergies[i-1][j-1]<min)
				min=newEnergies[i-1][j-1];
			if (newEnergies[i-1][j] <= min)
				min=newEnergies[i-1][j];
			if (j+1<newEnergies[i].size() && newEnergies[i-1][j+1]<min)
				min=newEnergies[i-1][j+1];
			newEnergies[i][j] += min;
		}
	}
}

void cumulativeHorizontalEnergies(const vector <vector<int> >& energyVec, vector <vector<int> >& newEnergies) {
	newEnergies.resize(energyVec.size());
	for (int i=0; i<newEnergies.size(); ++i)
		newEnergies[i].resize(energyVec[i].size());

	for (int i=0; i<newEnergies.size(); ++i) {
		for (int j=0; j<newEnergies[i].size(); ++j)
			newEnergies[i][j]=energyVec[i][j];
	}

	for (int j=1; j<energyVec[0].size(); ++j) {
		for (int i=0; i<energyVec.size(); ++i) {
			int min=2147483647;
			if (i-1>=0 && newEnergies[i-1][j-1]<min)
				min=newEnergies[i-1][j-1];
			if (newEnergies[i][j-1] <= min)
				min=newEnergies[i][j-1];
			if (i+1<newEnergies.size() && newEnergies[i+1][j-1]<min)
				min=newEnergies[i+1][j-1];
			newEnergies[i][j] += min;
		}
	}
}

void findVerticalSeam(vector <pair<int,int> >& path, const vector <vector<int> >& energyVec) {

	int min=2147483647, pos=0;
	for (int j=0; j<energyVec[energyVec.size()-1].size(); ++j) {
		if (energyVec[energyVec.size()-1][j] < min)
			min = energyVec[energyVec.size()-1][j],pos=j;
	}

	path.push_back(make_pair(energyVec.size()-1,pos));


	for (int i=energyVec.size()-2; i>=0; --i) {

		min = 2147483647;

		int move=0;                                   // move used to indicate whether to move down+left,down,down+right after all 3 checks, down in event of tie
		if ( pos-1 >= 0 && energyVec[i][pos-1]<min ) { // check lower left point for min energy
			min=energyVec[i][pos-1];
			move=-1;
		}
		if (energyVec[i][pos]<=min) {                    // check point directly beneath
			min=energyVec[i][pos];
			move=0;
		}
		if ( pos+1 < energyVec[i].size() && energyVec[i][pos+1]<min) {  // check lower right point
			min=energyVec[i][pos+1];
			move=1;
		}

		if (move==1)
			++pos;
		else if (move==-1)
			--pos;


		path.push_back(make_pair(i,pos));

	}
}

void findHorizontalSeam(vector <pair<int,int> >& path, const vector <vector<int> >& energyVec) {

	int min=2147483647, pos=0;
	for (int i=0; i<energyVec.size(); ++i) {
		if (energyVec[i][energyVec[i].size()-1] < min)
			min = energyVec[i][energyVec[i].size()-1],pos=i;
	}

	path.push_back(make_pair(pos,energyVec[pos].size()-1));


	for (int j=energyVec[pos].size()-2; j>=0; --j) {

		min = 2147483647;

		int move=0;                                   // move used to indicate whether to move down+left,down,down+right after all 3 checks, down in event of tie
		if ( pos-1 >= 0 && energyVec[pos-1][j]<min )  // check lower left point for min energy
			min=energyVec[pos-1][j],move=-1;
		if (energyVec[pos][j]<=min)                    // check point directly beneath
			min=energyVec[pos][j],move=0;
		if ( pos+1 < energyVec.size() && energyVec[pos+1][j]<min)  // check lower right point
			min=energyVec[pos+1][j],move=1;

		if (move==1)
			++pos;
		else if (move==-1)
			--pos;

		path.push_back(make_pair(pos,j));

	}
}