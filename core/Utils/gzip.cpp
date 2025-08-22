#include "Utils/gzip.h"

#include "Utils/Util.h"

#include <zlib.h>
#include <stdarg.h>

namespace MC 
{
	namespace Utils 
	{
		unsigned char* decompress(const void* in, size_t in_size, size_t* out)
		{
			z_stream inflate_s;
			size_t max_size = 1024 * 1024 * 1024;
			int window_bits = 15 + 32;
			size_t out_size = in_size * 2;
			size_t total = 0;
			int ret = 0;
			unsigned char* output = NULL;
			
			/* check valid size */
			if (in_size == 0) 
				return NULL; /* invalid size */

			/* init zlib struct */
			inflate_s.zalloc = Z_NULL;
			inflate_s.zfree = Z_NULL;
			inflate_s.opaque = Z_NULL;
			inflate_s.next_in = (Bytef*)in;
			inflate_s.avail_in = (unsigned int)in_size;

			/* init zlib */
			if (inflateInit2(&inflate_s, window_bits) != Z_OK)
				return NULL; /* zlib init error */
			
			/* check max decompression size */
			mc_assert(out_size < max_size, "decompression size too big");
			
			/* allocate output buffer */
			output = (unsigned char*)malloc(out_size);
			if (!output) {
				inflateEnd(&inflate_s);
				return NULL; /* out of memory */
			}
			
			/* decompress process */
			do {
				if (total == out_size) {
					size_t new_size = out_size * 2;
					mc_assert(new_size < max_size, "decompression size too big");
				
					/* realloc buffer */
					unsigned char* tmp = (unsigned char*)realloc(output, new_size);
					if (!tmp) {
						inflateEnd(&inflate_s);
						free(output);
						return NULL; /* out of memory */
					}
					
					output = tmp;
					out_size = new_size;
				}
				
				inflate_s.next_out = output + total;
				inflate_s.avail_out = (unsigned int)(out_size - total);
				
				/* decompres buffer */
				ret = inflate(&inflate_s, Z_FINISH);
				if (ret != Z_STREAM_END && ret != Z_OK && ret != Z_BUF_ERROR) {
					inflateEnd(&inflate_s);
					free(output);
					return NULL; /* zlib error */
				}
				
				/* recalculate total */
				total = out_size - inflate_s.avail_out;		
			} while (ret != Z_STREAM_END);
				
			inflateEnd(&inflate_s);
			output = (unsigned char*)realloc(output, total);
			if (!output)
				return NULL; /* out of memory */
			
			*out = total;
			return output;
		}
		
		unsigned char* compress(const void* input, size_t in_size, size_t* out)
		{
			z_stream deflate_s;
			int window_bits = 15 + 16;
			int mem_level = 8;
			size_t out_size = in_size * 2;
			unsigned char* output = NULL;
			size_t total = 0;
			int ret = 0;
			
			/* check valid size */
			if (in_size == 0) 
				return NULL; /* invalid size */
			
			/* init zlib struct */
			deflate_s.zalloc = Z_NULL;
			deflate_s.zfree = Z_NULL;
			deflate_s.opaque = Z_NULL;
			deflate_s.next_in = (Bytef*)input;
			deflate_s.avail_in = (unsigned int)in_size;
			
			/* init zlib */
			if (deflateInit2(&deflate_s, Z_DEFAULT_COMPRESSION, Z_DEFLATED, window_bits, mem_level, Z_DEFAULT_STRATEGY) != Z_OK)
				return NULL; /* zlib init error */
			
			/* allocate output buffer */
			output = (unsigned char*)malloc(out_size);
			if (!output) {
				deflateEnd(&deflate_s);
				return NULL; /* out of memory */
			}
			
			do {
				if (total == out_size) {
					size_t new_size = out_size * 2;
					
					/* realloc buffer */
					unsigned char* tmp = (unsigned char*)realloc(output, new_size);
					if (!tmp) {
						deflateEnd(&deflate_s);
						free(output);
						return NULL; /* out of memory */
					}
					
					out_size = new_size;
					output = tmp;
				}
				
				deflate_s.next_out = output + total;
				deflate_s.avail_out = (unsigned int)(out_size - total);

				ret = deflate(&deflate_s, Z_FINISH);
				if (ret != Z_STREAM_END && ret != Z_OK) {
					deflateEnd(&deflate_s);
					free(output);
					return NULL; /* zlib error */
				}
				
				/* recalculate total */
				total = out_size - deflate_s.avail_out;
				
			} while (ret != Z_STREAM_END);
			
			
			deflateEnd(&deflate_s);
			
			output = (unsigned char*)realloc(output, total);
			if (!output)
				return NULL; /* out of memory */
			
			*out = total;
			return output;
		}
	}
}


