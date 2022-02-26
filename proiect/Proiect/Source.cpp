#include <iostream>
#include <stdio.h>
#include <chrono>
#include "pbPlots.hpp"
#include "supportLib.hpp"
#include <stdio.h>


short cache;
long long* lvlSize;
int timeN;
int percent = -1;

std::vector<double> numberGen(long long n, long long lim) {
	std::vector<double> numb;
	for (int i = 0;i < n;i++)
		numb.push_back(rand() % lim);
	return numb;

}

void makeGraph(std::vector<double> x, std::vector<double> y, char name[]) {
	RGBABitmapImageReference* imageRef = CreateRGBABitmapImageReference();
	DrawScatterPlot(imageRef, 1290, 700, &x, &y);

	std::vector<double>* pngData = ConvertToPNG(imageRef->image);
	char* c = (char*)malloc(strlen(name) + 4 * sizeof(char));
	c = strcat(name, ".png");
	WriteToFile(pngData, c);
}


void fixNumbers(int* numbers) {

	for (int i = 0;i < 5;i++) {
		for (int j = 0;j < 4 - i;j++)
			if (numbers[j] > numbers[j + 1]) {
				int aux = numbers[j];
				numbers[j] = numbers[j + 1];
				numbers[j + 1] = aux;
			}
	}
	int temp = numbers[4];
	int k = 0;
	while (temp) {
		temp /= 10;
		k++;
	}
	timeN = 5;
	int* frecv = (int*)calloc(k + 1, sizeof(int));
	for (int i = 0;i < 5;i++) {
		temp = numbers[i];
		k = 0;
		while (temp) {
			temp /= 10;
			k++;
		}

		frecv[k]++;
	}
	int maj = frecv[1], frecvMaj = 1;
	for (int i = 2;i < k + 1;i++)
		if (frecv[i] > maj) {
			maj = frecv[i];
			frecvMaj = i;
		}
	int dif = 1;
	while (frecvMaj) {
		dif *= 10;
		frecvMaj--;
	}

	int i = 0;
	while (i < timeN) {
		if (numbers[i] < dif / 10 || numbers[i] >= dif) {
			for (int j = i;j < timeN - 1;j++)
				numbers[j] = numbers[j + 1];
			numbers = (int*)realloc(numbers, (timeN - 1) * sizeof(int));
			timeN--;
		}
		else
			i++;
	}
}

void testHitMiss() {
	std::vector<double> x, y;
	int totalMemory = 0, limit = 100, nrHit = 0, nrMiss = 0, inc = 0;
	int buffer, timeMiss = 0, hitTime = 0;
	FILE* f = fopen("HitMiss.txt", "w");
	for (int i = 0;i < cache;i++) {
		nrHit = 0;
		nrMiss = 0;
		timeMiss = 0;
		totalMemory += (lvlSize[i] / 8);
		int* tester = (int*)calloc(totalMemory + 100, sizeof(int));
		for (int j = 0;j < limit;j++) {

			int loc = rand() % (totalMemory + 100);
			auto begin = std::chrono::high_resolution_clock::now();
			tester[loc]++;
			auto end = std::chrono::high_resolution_clock::now();
			auto elapsed = std::chrono::duration_cast<std::chrono::nanoseconds>(end - begin);
			int time = elapsed.count();
			if (time > 100) {
				nrMiss++;
				timeMiss += time - 100;
			}
			else {
				hitTime += time;
				nrHit++;
			}

		}

		float hitRatio, missRatio, averageMemoryAccesTime;
		hitRatio = (float)nrHit / limit;
		missRatio = 1 - hitRatio;
		fprintf(f, "%d %d %.2f %.2f", nrHit, nrMiss, hitRatio * 100, missRatio * 100);
		hitTime /= nrHit;
		if (timeMiss) {

			averageMemoryAccesTime = hitTime + missRatio * ((float)timeMiss / nrMiss);
			fprintf(f, " %.2f %.2f\n", (float)timeMiss / nrMiss, averageMemoryAccesTime);
		}
		else {
			averageMemoryAccesTime = hitTime;
			fprintf(f, " %.2f\n", averageMemoryAccesTime);
		}
		limit *= 10;
	}
	fclose(f);
}

void testReadingCache() {
	std::vector<double> x, y;
	long long totalMemory = 0;
	int buffer;
	for (int i = 0;i < cache;i++)
		totalMemory += lvlSize[i];
	totalMemory /= 8;
	double previousAverage = -1;

	for (int size = 5000;size <= totalMemory;size += 10000) {

		double average = 0;

		x.push_back(size);
		std::vector<double> numbers = numberGen(size, size / 4);
		int* times = (int*)malloc(5 * sizeof(int));
		FILE* f = fopen("forReading.txt", "w+");
		for (int i = 0;i < size;i++)
			fprintf(f, "%.0f ", numbers[i]);
		for (int j = 0;j < 5;j++) {
			auto begin = std::chrono::high_resolution_clock::now();
			for (int i = 0;i < size;i++)
				fscanf(f, "%d", &buffer);
			fclose(f);
			auto end = std::chrono::high_resolution_clock::now();
			auto elapsed = std::chrono::duration_cast<std::chrono::microseconds>(end - begin);
			times[j] = elapsed.count();
		}
		fixNumbers(times);
		for (int i = 0;i < timeN;i++)
			average += times[i];
		average /= timeN;
		y.push_back(average);
	}
	char* name = (char*)malloc(40 * sizeof(char));
	sprintf(name, "MicrosecondsAveragePerSizeReading");
	makeGraph(x, y, name);

}

void testWritingCache() {
	std::vector<double> x, y;
	int totalMemory = 0, buffer;
	for (int i = 0;i < cache;i++)
		totalMemory += lvlSize[i];
	totalMemory /= 8;
	double previousAverage = -1;

	for (int size = 5000;size <= totalMemory;size += 5000) {

		double average = 0;
		std::vector<double> numbers = numberGen(size, size / 4);
		int* times = (int*)malloc(5 * sizeof(int));
		for (int j = 0;j < 5;j++) {
			auto begin = std::chrono::high_resolution_clock::now();
			for (long long i = 0;i < size;i++)
				numbers[i] += 1;
			auto end = std::chrono::high_resolution_clock::now();
			auto elapsed = std::chrono::duration_cast<std::chrono::microseconds>(end - begin);
			times[j] = elapsed.count();
		}

		fixNumbers(times);
		for (int i = 0;i < timeN;i++)
			average += times[i];
		average /= timeN;
		if (previousAverage == -1 || abs(previousAverage - average) * 100 / previousAverage < percent) {
			previousAverage = average;
			int area = 0, temp = 10, lim[] = { 50,250,400,1000,1400 };
			while (temp < previousAverage) {
				temp += 10;
				if (temp >= lim[area])
					area++;
			}
			switch (area)
			{
			case 0: {percent = 300; break;}
			case 1: {percent = 100; break;}
			case 2: {percent = 50; break;}
			case 3: {percent = 25; break;}
			case 4: {percent = 15; break;}
			case 5: {percent = 6; break;}
			}
			y.push_back(average);
			x.push_back(size);
		}
		else
			size -= 5000;
	}
	char* name = (char*)malloc(40 * sizeof(char));
	sprintf(name, "MicrosecondsAveragePerSizeWriting");
	makeGraph(x, y, name);


}

void getPCInfo() {
	FILE* f = fopen("PCinfo.txt", "w");
	char vendor[13];
	char processor[50];
	int a[4];
	__asm {
		mov eax, 0x0
		cpuid
		mov a[0], ebx
		mov a[4], edx
		mov a[8], ecx
	}
	short poz = 0;
	for (int i = 0;i < 3;i++) {
		memcpy(vendor + poz, &a[i], 4);
		poz += 4;
	}
	vendor[12] = '\0';
	printf("Processor Brand:%s\n", vendor);
	fprintf(f, "Processor Brand:%s\n", vendor);
	unsigned int op = 0x80000002;
	poz = 0;
	for (op = 0x80000002;op <= 0x80000004;op += 1) {
		__asm {
			mov eax, op
			cpuid
			mov a[0], eax
			mov a[4], ebx
			mov a[8], ecx
			mov a[12], edx
		}
		for (int i = 0;i < 4;i++) {
			memcpy(processor + poz, &a[i], 4);
			poz += 4;
		}
	}
	processor[49] = '\0';
	printf("Processor:%s\n", processor);
	fprintf(f, "Processor:%s\n", processor);
	op = 0x1;
	long long lvl[4];
	printf("Cache Memory:\n");
	fputs("Cache Memory:\n", f);
	do {
		__asm {
			mov eax, 0x4
			mov ecx, op
			cpuid
			mov a[0], ebx
			mov a[4], ecx
		}

		if (a[0] != 0) {
			printf("L%d cache:", op);
			fprintf(f, "L%d cache:", op);
			int line_size = a[0] & 0xFFF, partitions = (a[0] >>= 12) & 0x3FF, ways = (a[0] >>= 10) & 0x3FF, sets = a[1];
			int total = (ways + 1) * (partitions + 1) * (line_size + 1) * (sets + 1);
			printf("%d line size, %d partitions, %d ways, %d sets, %d bytes total\n", line_size, partitions, ways, sets, total);
			fprintf(f, "%d line size, %d partitions, %d ways, %d sets, %d bytes total\n", line_size, partitions, ways, sets, total);
			lvl[cache] = total;
		}
		else
			break;
		cache = op;
		op += 1;

	} while (a[1] != 0);
	fclose(f);
	lvlSize = (long long*)calloc(cache, sizeof(long long));
	for (short i = 0;i < cache;i++)
		lvlSize[i] = lvl[i];
}



int main(int argc,char** argv)
{
	srand(time(0));
	int choice = atoi(argv[1]);
	getPCInfo();
	switch (choice) {
		case 0: {testWritingCache(); break;}
		case 1: {testReadingCache(); break;}
		case 3: {testHitMiss(); break;}
		default:;
	}
	return 0;
}