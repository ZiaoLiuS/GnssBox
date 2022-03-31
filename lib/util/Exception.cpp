
/**
 * @file Exception.cpp
 */

#include <sstream>
#include "Exception.hpp"

using std::ostream;
using std::ostringstream;
using std::streambuf;
using std::string;
using std::endl;

namespace utilSpace
{

   void ExceptionLocation::dump(ostream& s) const
   {
      s << getFileName() << ":"
#ifdef __FUNCTION__
        << getFunctionName() << ":"
#endif
        << getLineNumber();
   }

   Exception::Exception()
   {
   }

   Exception::Exception(const string& errorText,
                        const unsigned long& errId,
                        const Severity& sever)
   {
      text.push_back(errorText);
      errorId = errId;
      severity = sever;
   }

   Exception::Exception(const char* errorText,
      const unsigned long& errId,
      const Severity& sever)
   {
      text.push_back(string(errorText));
      errorId = errId;
      severity = sever;
   }

   Exception::Exception(const Exception& e)
         : errorId(e.errorId),
           locations(e.locations),
           severity(e.severity),
           text(e.text),
           streamBuffer(e.streamBuffer)
   {}

   Exception& Exception::operator=(const Exception& e)
   {
      errorId = e.errorId;
      locations = e.locations;
      severity = e.severity;
      text = e.text;
         // reuse existing stream objects, no matter.
         //streambuf(), ostream((streambuf*)this),
      streamBuffer = e.streamBuffer;

      return *this;
   }

   Exception& Exception::addLocation(
      const ExceptionLocation& location)
   {
      locations.push_back(location);
      return *this;
   }

   const ExceptionLocation Exception::getLocation(
      const size_t& index) const
   {
      if (index>=getLocationCount())
      {
         return ExceptionLocation();
      }
      else
      {
         return locations[index];
      }
   }

   size_t Exception::getLocationCount() const
   {
      return locations.size();
   }

   Exception& Exception::addText(const string& errorText)
   {
      text.push_back(errorText);
      return *this;
   }

   string Exception::getText(const size_t& index) const
   {
      if (index>=getTextCount())
      {
         string tmp;
         return tmp;
      }
      else
      {
         return text[index];
      }
   }

   size_t Exception::getTextCount() const
   {
      return text.size();
   }

   void Exception::dump(ostream& s) const
   {
      size_t i;
      for (i=0; i<getTextCount(); i++)
      {
         s << "text " << i << ":" << this->getText(i) << endl;
      }
      for (i=0; i<getLocationCount(); i++)
      {
         s << "location " << i << ":" << getLocation(i).what() << endl;
      }
   }

   int Exception::overflow(int c)
   {
      if (c == '\n' || !c)
      {
         if (streamBuffer.length() == 0)
         {
            return c;
         }
         addText(streamBuffer);
         streamBuffer = "";
         return c;
      }
      streamBuffer.append(1, (char)c);
      return c;
   }

   string ExceptionLocation::what() const
   {
      ostringstream oss;
      this->dump(oss);
      return oss.str();
   }

   string Exception::what() const
   {
      ostringstream oss;
      this->dump(oss);
      return oss.str();
   }

    ostream& operator<<( ostream& s,
                         const Exception& e )
    {
       e.dump(s);
       return s;
    }

    ostream& operator<<( ostream& s,
                         const ExceptionLocation& e )
    {
       e.dump(s);
       return s;
    }

} // namespace utilSpace
