#ifndef _MAKE_MSGC_filename_hpp_ID_
#define _MAKE_MSGC_filename_hpp_ID_
#if defined(__sgi) && defined(_SGI_COMPILER_VERSION)
#pragma weak _MAKE_MSGC_filename_hpp_id_w = _MAKE_MSGC_filename_hpp_id_
#endif /* __sgi */
static const char* _MAKE_MSGC_filename_hpp_id_ 
#if defined(__GNUC__)
__attribute__ ((used,unused)) 
#endif /* __GNUC__ */
= "@(#)$Id: filename.hpp 55141 2009-08-18 21:17:30Z wguan@syncsort.com $";
#endif

/******************************************************************************

                      Syncsort CLIENT-SERVER BACKUP

                            COPYRIGHT(C) 2000

        Module Name             : gen++

        Component Name          : FileName

        Program Filename        : filename.hpp

        General Description     : gen::FileNameBase and derived classes

        System Dependencies     : NONE

        History Information     : Provided by the RCS package in the following.

 * $Log: not supported by cvs2svn $
 * Revision 1.3  2007-03-16 10:06:52-04  wguan
 * T...-269: Adding String Identifier (RCS Keyword) to All Source Code - hhppccpplexyacc
 *
 * Revision 1.2  2000/12/20 23:00:54  urano
 * I...-666: Message Infrastructure in C++
 *
 * Revision 1.1  2000/10/31 19:21:53  urano
 * Initial revision
 *
 *
*****************************************************************************/
#ifndef _FILENAME_HPP_
#define _FILENAME_HPP_

// stdlib includes
#include <string>

// genlib includes
#include <syncplat.h>

namespace gen {
 
  template <char dirSep> class FileNameBase {
    std::string dirName;
    std::string baseName;
    bool hasExt;
    std::string extensionName;
  public:
    FileNameBase (const std::string & _fullName) : hasExt(false) {
      setFullName (_fullName);
    }
    void setDirectory (const std::string & _dirName) {
      dirName = _dirName;
      if ((dirName.size() > 0) && (dirName[dirName.size()-1] != dirSep))
	dirName += dirSep; 
    }
    std::string getDirectory () const {
      if (dirName.empty()) 
	return ".";
      else if ((dirName.size()==1) && (dirName[0] == dirSep))
	return std::string(1,dirSep);
      else
	return dirName.substr(0, dirName.size()-1);
    }
    void setBase (const std::string & _baseName) {baseName = _baseName;}
    const std::string & getBase () const {return baseName;}
    void addExtension (const std::string & _extensionName) {
      if (hasExt)
	setBase ( getBase() + "." + getExtension() );
      hasExt = true;
      std::string::size_type idx = _extensionName.rfind('.');
      if (idx != std::string::npos)
	extensionName = _extensionName.substr(idx+1);
      else
	extensionName = _extensionName;
    }
    void setExtension (const std::string & _extensionName) {
      if (_extensionName.empty()) 
	{
	  hasExt = false;
	  extensionName = "";
	}
      else 
	{
	  hasExt = true;
	  std::string::size_type idx = _extensionName.rfind('.');
	  if (idx != std::string::npos)
	    extensionName = _extensionName.substr(idx+1);
	  else
	    extensionName = _extensionName;
	}
    }
    void removeExtension () {setExtension("");}
    const std::string & getExtension () const {return extensionName;}
    bool hasExtension () {return hasExt;}
    void setFileName (const std::string & _fileName) {
      std::string::size_type idx = _fileName.rfind(dirSep);
      if (idx != std::string::npos) {
	setFullName (_fileName);
	return;
      }
      idx = _fileName.rfind('.');
      if (idx != std::string::npos)
	{
	  baseName = _fileName.substr(0, idx);
	  setExtension (_fileName.substr(idx));
	}
      else
	{
	  baseName = _fileName;
	  setExtension ("");
	}
    }
    std::string getFileName () const {
      if (hasExt) return baseName+"."+extensionName;
    else return baseName;
    }
    void setFullName (const std::string & _fullName) {
      std::string::size_type idx = _fullName.rfind(dirSep);
      if (idx != std::string::npos)
	{
	  setDirectory (_fullName.substr(0, idx+1));
	  setFileName (_fullName.substr(idx+1));
	}
      else
	{
	  setDirectory ("");
	  setFileName (_fullName);
	}
    }
    std::string getFullName () const {
      return dirName + getFileName();
    }
  };
  

  class UNIXFileName : public FileNameBase<'/'> {
    typedef FileNameBase<'/'> Base;
  public:
    UNIXFileName (const std::string & _fullName) : Base(_fullName) {}
  };

  class DOSFileName : public FileNameBase<'/'> {
    typedef FileNameBase<'/'> Base;
  public:
    DOSFileName (const std::string & _fullName) : Base(_fullName) {}
  };

#ifdef _SYNC_UNIX
  typedef UNIXFileName NativeFileName;
#else
  typedef DOSFileName NativeFileName;
#endif
  
}

#endif // _FILENAME_HPP_
