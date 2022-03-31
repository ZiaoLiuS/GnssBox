
//////////////////////////////////////////
// 
// EpochXvStore is used to store xyz for kinematic vehicle,
// such as moving autos, or LEOs
// 
// shjzhang. 2020/03/20
//                                        
//////////////////////////////////////////

#ifndef EpochXvStore_HPP
#define EpochXvStore_HPP

#include <iostream>
#include <cmath>
#include <vector>
#include <map>

#include "CommonTime.hpp"
#include "Xv.hpp"

using namespace std;
using namespace timeSpace;
using namespace coordSpace;

namespace gnssSpace
{
   class EpochXvStore
   {
   public:
      
      EpochXvStore()
      {}
      
      virtual ~EpochXvStore()
      {}
      
      
      virtual void loadFile(std::string filename);
      
      virtual void loadFile(std::string filename,
                            const CommonTime& bTime,
                            const CommonTime& eTime);

      virtual Triple getPos(string& satName, CommonTime& epoch);
      virtual Triple getVel(CommonTime& epoch);
      
      
   private:
   
      string markerName;
      std::map< CommonTime, Xv > epochXvData;
      
   }; // END of "class EpochXvStore"
   
} // END of "namespace gnssSpace"

#endif
