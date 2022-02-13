#ifndef _title_h__
#define _title_h__

#include "movie.h"
#include <cstring>


class TitleComparator
{
  public:
    bool operator( ) (const Movie::SecondaryKey & key1, 
                      const Movie::SecondaryKey & key2) const
    { 
      int titleOneLength = key1.getTitle().size();
      int titleTwoLength = key2.getTitle().size();
      
      std::string titleOne = key1.getTitle();
      std::string titleTwo = key2.getTitle();
      int i = 0;
      for(i = 0;i < titleOneLength;i++){
          titleOne[i] = tolower(titleOne[i]);
      }
      
      for(i = 0;i<titleTwoLength;i++){
          titleTwo[i] = tolower(titleTwo[i]);
      }
      
      if(titleOne < titleTwo){
          return true;
      }
      else if(titleOne == titleTwo){
          int directorOneLength = key1.getDirector().size();
          int directorTwoLength = key2.getDirector().size();
          
          std::string directorOne = key1.getDirector();
          std::string directorTwo = key2.getDirector();
          
          for(i = 0;i < directorOneLength;i++){
              directorOne[i] = tolower(directorOne[i]);
          
              
          }
          
          for(i = 0;i < directorTwoLength; i++){
              directorTwo[i] = tolower(directorTwo[i]);
          
          }
          
          if(directorOne < directorTwo){
              return true;
              
          }
          else{
              return false;
          }
      }
      else {
         return false;         
      }
    }
};

#endif
