//
//  main.swift
//  imgutil
//
//  Created by Ramesh on 25/01/25.
//

import liteim

func spngError(_ code: Int32) -> String {
	if let string = spng_strerror(code) {
		return String(cString: string)
	}
	return ""
}

func start() -> Int32 {
	
	let fileName: String
	if CommandLine.argc < 2 {
		// Default file name
		fileName = "/Users/ramesh/Documents/projects/liteim/simple.png"
	} else {
		fileName = CommandLine.arguments[1];
	}
	
	let file = fopen(fileName, "rb");
	if file == nil {
		// Unable to open file
		print("error opening input file \(fileName)")
	}
	
	var ctx = spng_ctx_new(0)
	if (ctx == nil) {
		print("spng_ctx_new() failed")
	}
	let CRC_USE = SPNG_CRC_USE
	spng_set_crc_action(ctx!, Int32(SPNG_CRC_USE.rawValue), Int32(SPNG_CRC_USE.rawValue))
	
	/* Set memory usage limits for storing standard and unknown chunks,
	   this is important when reading untrusted files! */
	let limit = 1024 * 1024 * 64;
	spng_set_chunk_limits(ctx, limit, limit);

	/* Set source PNG */
	spng_set_png_file(ctx, file); /* or _buffer(), _stream() */

	var ihdr : spng_ihdr!
	var ret = spng_get_ihdr(ctx, &ihdr);

	if(ret != 0)
	{
		print("spng_get_ihdr() error: \(spngError(ret))")
		spng_ctx_free(ctx)
		return ret
	}
	
	var color_name = "";

	print("width: \(ihdr.width)\n",
		   "height: \(ihdr.height)\n",
		   "bit depth: \(ihdr.bit_depth)\n",
		   "color type: \(ihdr.color_type) - \(color_name)\n")

	print("compression method: \(ihdr.compression_method)\n",
		   "filter method: \(ihdr.filter_method)\n",
		   "interlace method: \(ihdr.interlace_method)\n")
	
	
	var plte = spng_plte()
	ret = spng_get_plte(ctx, &plte)
//	if(ret != spng_errno.SPNG_ECHUNKAVAIL)
//	{
//		print("spng_get_plte() error: \(spngError(ret))")
//		return ret
//	}

	if ret != 0 {
		print("palette entries: \(plte.n_entries)")
	}
	
	var image_size: size_t, image_width: size_t;
	var fmt = SPNG_FMT_PNG;

	
	return 0
}


/*
=======  MAIN FUNCTION  ==============
--------------------------------------
*/
@main
struct Runner {
	static func main() {
		print("Starting worker ... ")

		// .... Start of the process
		
		
		print("Wroker completed ... ")
	}
}
