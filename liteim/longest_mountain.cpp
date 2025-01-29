//
//  ex1.cpp
//  LiteImage
//
//  Created by Ramesh on 28/01/25.
//

#include <iostream>
#include <vector>

#include "include/liteim.h"

using namespace std;

int longestMountain(vector<int>& arr) {
	
	int longest = 0; // store the length of longest peak
	int status = 0;
	int increasing = 0;
	int decreasing = 0;
	int flipped_count = 0;
	
	for (int i = 1; i < arr.size(); i++) {
		if ( arr[i] > arr[i-1] ) {
			increasing++;
			status = status | 1;
		}
		if ( arr[i] < arr[i-1] ) {
			decreasing++;
			status = status & 2;
		}
		
	}

	// result
	return longest ;
	
}
