#ifndef _AQUARIUM_H_
#define _AQUARIUM_H_


#include <iostream>
#include <CImg.h>
#include <list>

using namespace std;
using namespace cimg_library;


class Medium;


class Aquarium : public CImgDisplay
{

private :
   Medium       * aqua;

   int            delay;

   void user_interaction(unsigned char key);

public :
   Aquarium( int width, int height, int _delay );
   ~Aquarium( void );

   Medium & getMedium( void ) { return *aqua; }

   void run( void );

};


#endif
