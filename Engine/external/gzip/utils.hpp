
/*
	Copyright (c) 2017, Mapbox Inc.  
	All rights reserved.

	Redistribution and use in source and binary forms, with or without
	modification, are permitted provided that the following conditions
	are met:

	- Redistributions of source code must retain the above copyright notice,
	  this list of conditions and the following disclaimer.
	- Redistributions in binary form must reproduce the above copyright notice,
	  this list of conditions and the following disclaimer in the documentation
	  and/or other materials provided with the distribution.

	THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
	"AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
	LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
	A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
	HOLDER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
	SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED
	TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR
	PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF
	LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING
	NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
	SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
*/

#include <cstdlib>

namespace gzip {

// These live in gzip.hpp because it doesnt need to use deps.
// Otherwise, they would need to live in impl files if these methods used
// zlib structures or functions like inflate/deflate)
inline bool is_compressed(const char* data, std::size_t size)
{
    return size > 2 &&
           (
               // zlib
               (
                   static_cast<uint8_t>(data[0]) == 0x78 &&
                   (static_cast<uint8_t>(data[1]) == 0x9C ||
                    static_cast<uint8_t>(data[1]) == 0x01 ||
                    static_cast<uint8_t>(data[1]) == 0xDA ||
                    static_cast<uint8_t>(data[1]) == 0x5E)) ||
               // gzip
               (static_cast<uint8_t>(data[0]) == 0x1F && static_cast<uint8_t>(data[1]) == 0x8B));
}
} // namespace gzip
