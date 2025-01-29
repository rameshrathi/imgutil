//
//  TestExample.swift
//  ImageUtil
//

import Testing

func longest_mountain(arr: [Int]) ->Int {
	
	// find longest subarr
	var status = 0
	var largest = 0
	
	var incr = 1, decr = 0

	for idx in 1..<arr.count {
		if arr[idx] > arr[idx-1] {
			incr += 1
			status = 1
		}
		if arr[idx] < arr[idx-1] && status & 1 == 1 {
			decr += 1
			status = status & 1 == 1 ? 3 : 0
		}
		if status == 3 && (incr+decr) > 2 {
			let len = incr+decr;
			largest = max(largest, len)
		}
		if ( arr[idx] == arr[idx-1] || (decr > 0 && status != 3)) {
			incr = 1
			decr = 1
			status = 0
		}
	}
	
	return largest
}

@Test func test_ex1() async throws {
	let res = longest_mountain(arr: [1,1,0,0,1,0])
	#expect(res == 0)
}
