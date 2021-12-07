#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <cmath>
#include <iomanip>
#include <complex>

using namespace std;

const double pi = 3.141592653589793238;

typedef vector<complex<double>> ComplVector;
typedef vector<vector<complex<double>>> matrix;

matrix createF(int N) {
	matrix F(N, vector<complex<double>>(N));
	for(int k = 0; k < N; k++) {
		for(int n = 0; n < N; n++) {
			F[k][n] = complex<double>(cos(2*pi * k * n / N), sin(2*pi * k * n / N));
		}
	}
	return F;
}

vector<complex<double>> product(vector<complex<double>> samplingSignal, matrix F){
	int N = samplingSignal.size();
	vector<complex<double>> restoredSamplingSignal(N);
	for(int k = 0; k < N; k++) {
		complex<double> sum = 0;
		for(int n = 0; n < N; n++) {
			sum += (F[n][k] * samplingSignal[n]);
		}
		restoredSamplingSignal[k] = sum;
	}
	return restoredSamplingSignal;
}

void swap(int& a, int& b) {
	int tmp = a;
	a = b;
	b = tmp;
}

int main() {
	int blockSize = 800;

	int blockCount = 10;

	ofstream fout;
	fout << fixed << setprecision(22);

	system("python3 ./readWAV.py 14.wav melody.txt");
	ifstream fin;
	fin.open("melody.txt");
	ComplVector melody;
	for(int i = 0; fin >> i;) {
		melody.emplace_back(i, 0);
	}
	fin.close();
	system("python3 ./plot.py melody.txt &");

	for (int i = 0; i < blockCount; i++) {
		ComplVector block;
		for (int j = 0; j <= blockSize; j++) {
			block.emplace_back(melody[i*2*blockSize + j]);
		}
		ComplVector spectr = product(block, createF(block.size()));
	
		fout.open("block.txt");
		for(complex<double> i : spectr) fout << abs(i.imag()) << '\n';
		fout.close();

		int maxPOS1 = 0, maxPOS2 = 0;
		int maxNUM1 = 0, maxNUM2 = 0;

		for(int i = 9; i < spectr.size()/2; i++) {
			if(spectr[i].imag() > maxNUM1) {
				maxNUM2 = maxNUM1;
				maxNUM1 = spectr[i].imag();
				
				maxPOS2 = maxPOS1;
				maxPOS1 = i;		
			} else if( spectr[i].imag() > maxNUM2) {
				maxNUM2 = spectr[i].imag();
				maxPOS2 = i;
			}
		}
		if(maxPOS2 > maxPOS1 ){
			swap(maxPOS2, maxPOS1);
			swap(maxNUM2, maxNUM1);
		}
		cout << maxPOS2 << "/" << maxPOS1 << '\n';
		system("python3 ./plot.py block.txt");
	}
}