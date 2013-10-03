// float2bmp2.hpp
//
// Copyright 2010 Michael Thomas Greer
// Distributed under the Boost Software License, Version 1.0.
// (See http://www.boost.org/LICENSE_1_0.txt )
//

#ifndef _BMP_H
#define _BMP_H

#include <fstream>
#include <iostream>
#include <algorithm>
namespace cfdtd
  {

  //--------------------------------------------------------------------------
  // This little helper is to write little-endian values to file.
  //
  struct lwrite
    {
    unsigned long value;
    unsigned      size;
    lwrite( unsigned long value, unsigned size ):
      value( value ), size( size )
      { }
    };

  //--------------------------------------------------------------------------
  inline std::ostream& operator << ( std::ostream& outs, const lwrite& v )
    {
    unsigned long value = v.value;
    for (unsigned cntr = 0; cntr < v.size; cntr++, value >>= 8)
      outs.put( static_cast <char> (value & 0xFF) );
    return outs;
    }

  //--------------------------------------------------------------------------
  template <typename Iterator>
  bool
  floats_to_bitmap_file(
    const char*   filename,
    Iterator      begin,
    size_t        rows,
    size_t        columns,
    unsigned char default_value = 0
    ) {
    std::ofstream f( filename, std::ios::out | std::ios::trunc | std::ios::binary );
    if (!f) return false;

    // Some basic
    unsigned long headers_size    = 14  // sizeof( BITMAPFILEHEADER )
                                  + 40; // sizeof( BITMAPINFOHEADER )
    unsigned long padding_size    = (4 - ((columns * 3) % 4)) % 4;
    unsigned long pixel_data_size = rows * ((columns * 3) + padding_size);

    // Write the BITMAPFILEHEADER
    f.put( 'B' ).put( 'M' );                           // bfType
    f << lwrite( headers_size + pixel_data_size, 4 );  // bfSize
    f << lwrite( 0,                              2 );  // bfReserved1
    f << lwrite( 0,                              2 );  // bfReserved2
    f << lwrite( headers_size,                   4 );  // bfOffBits

    // Write the BITMAPINFOHEADER
    f << lwrite( 40,                             4 );  // biSize
    f << lwrite( columns,                        4 );  // biWidth
    f << lwrite( rows,                           4 );  // biHeight
    f << lwrite( 1,                              2 );  // biPlanes
    f << lwrite( 24,                             2 );  // biBitCount
    f << lwrite( 0,                              4 );  // biCompression=BI_RGB
    f << lwrite( pixel_data_size,                4 );  // biSizeImage
    f << lwrite( 0,                              4 );  // biXPelsPerMeter
    f << lwrite( 0,                              4 );  // biYPelsPerMeter
    f << lwrite( 0,                              4 );  // biClrUsed
    f << lwrite( 0,                              4 );  // biClrImportant
    //we will be trying to find the max value for red and min value for blue
    Iterator mi=std::min_element(begin,begin+columns*rows);
    Iterator ma=std::max_element(begin,begin+columns*rows);
    // Write the pixel data
    unsigned int row;
    for (row = rows; row>0; row--)           // bottom-to-top
      {
      for (unsigned col = 0; col < columns; col++)  // left-to-right
        {
			double        d = 0.0;


			d=*(begin+row*columns+col);
			if(d>0.){
			f.put( static_cast <char> (255-255.*d / *ma) )
			 .put( static_cast <char> (255-255.*d / *ma) )
			 .put( static_cast <char> (255) );
			}
			else{
				f.put( static_cast <char> (255) )
				 .put( static_cast <char> (255+255.*d / *mi) )
				 .put( static_cast <char> (255+255.*d / *mi) );
			}
        }

      if (padding_size) f << lwrite( 0, padding_size );
      }

    // All done!
    return f.good();
    }

  } 

#endif

