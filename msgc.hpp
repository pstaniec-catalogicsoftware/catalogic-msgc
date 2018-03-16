#ifndef _MAKE_MSGC_msgc_hpp_ID_
#define _MAKE_MSGC_msgc_hpp_ID_
#if defined(__sgi) && defined(_SGI_COMPILER_VERSION)
#pragma weak _MAKE_MSGC_msgc_hpp_id_w = _MAKE_MSGC_msgc_hpp_id_
#endif /* __sgi */
static const char* _MAKE_MSGC_msgc_hpp_id_ 
#if defined(__GNUC__)
__attribute__ ((used,unused)) 
#endif /* __GNUC__ */
= "@(#)$Id: msgc.hpp 55141 2009-08-18 21:17:30Z wguan@syncsort.com $";
#endif

/******************************************************************************

                      Syncsort CLIENT-SERVER BACKUP

                            COPYRIGHT(C) 2000

        Module Name             : msgc

        Component Name          : DevTools

        Program Filename        : msgc.hpp

        General Description     : Message Compiler

        System Dependencies     : NONE

        History Information     : Provided by the RCS package in the following.

 * $Log: msgc.hpp,v $
 * Revision 1.18  2006/11/06 22:23:00  urano
 * I..4-204: msgc: verstr directive
 *
 * Revision 1.17  2005/03/14 20:52:16  urano
 * I..2-820: BEXIDL: hidden qualifier for a mdata member
 *
 * Revision 1.16  2004/02/09 16:14:23  urano
 * I..1-917: BEXIDL : clear cache between 'translation units'
 *
 * Revision 1.15  2003/07/23 16:11:31  urano
 * I..1-658: msgc JAVA stub to synchronize
 *
 * Revision 1.14  2003/05/06 20:10:15  urano
 * I..1-613: BEXIDL : format specification for mdata
 *
 * Revision 1.13  2003/04/11 14:25:34  urano
 * I..1-584: bexException to keep ctlMDat; SvcInt_X in Java
 *
 * Revision 1.12  2003/01/30 20:14:26  urano
 * I...-659: Backup Express Message Library
 *
 * Revision 1.11  2003/01/15 18:49:31  urano
 * I..1-412: Improving compile time for BEXIDL C++ stubs
 *
 * Revision 1.9  2003/01/08 18:27:39  urano
 * I...-666: rstring_sy
 *
 * Revision 1.8  2002/11/15 20:10:28  urano
 * I..1-436: devui
 *
 * Revision 1.7  2002/11/06 22:25:23  urano
 * I..1-412: Improving compile time for BEXIDL C++ stubs
 *
 * Revision 1.6  2002/10/10 22:07:38  urano
 * I...-674: winnt compatibility
 *
 * Revision 1.5  2002/10/08 14:19:04  urano
 * I...-674: Device Management Module
 *
 * Revision 1.4  2002/07/23 21:14:15  urano
 * I...-674: Device Management Module
 *
 * Revision 1.3  2002/06/12 19:41:32  urano
 * I..1-069: Device Management Module
 *
 * Revision 1.2  2001/09/28 21:29:56  urano
 * I..1-069: msg_flag autogeneration in msgc
 *
 * Revision 1.1  2001/09/20 16:33:44  urano
 * Initial revision
 *
 *
*****************************************************************************/
#ifndef _MSGC_HPP_
#define _MSGC_HPP_

// stdlib includes
#include <ctype.h>	// for upper
#include <exception>
#include <fstream>
#include <iostream>
#include <map>
#include <set>
#include <stdexcept>
#include <stack>
#include <string>
#include <utility>
#include <vector>

// additional utility includes
#if !defined(yyFlexLexerOnce)
#include <FlexLexer.h>
#endif

// genlib includes
#include <cntdptr.hpp>

///////////////////////////////////////////////////////////
// Base Classes
//

class msgc_exception : public std::exception {
 protected:
   std::string message;
 public:
   virtual ~msgc_exception() throw() {}
   explicit msgc_exception (const std::string & _msg): message(_msg) {}
   virtual const char * what () const throw() {return (message.c_str()); }
};

class msgc_exception_not_in_store : public msgc_exception {
 public:
   explicit msgc_exception_not_in_store(const std::string & _msg): msgc_exception(_msg) {}
   virtual ~msgc_exception_not_in_store() throw() {}
};


// utility class stringPtrMap - adaptor to map<string,Ptr>
template<class Ptr>
class stringPtrMap : public std::map<std::string,Ptr> {
typedef std::map<std::string,Ptr> Base;
public:
  void insert(const std::string & _name, Ptr _p) {
    if (!Base::insert(std::make_pair(_name,_p)).second)
      throw msgc_exception(_name+" has already been defined.");
  }
  bool erase (const std::string & _name) { 
    return (Base::erase (_name) > 0); 
  }
  Ptr get (const std::string & _name) {
    typename Base::iterator it = Base::find (_name);
    if (it == Base::end()) 
      throw msgc_exception_not_in_store(_name+" is undefined.");
    return (*it).second;
  }
  std::vector<std::string> getKeys() const {
    std::vector<std::string> result;
    for (typename Base::const_iterator it=Base::begin(); it!=Base::end(); ++it) {
      result.push_back(it->first);
    }
    return result;
  }
};

class NamedObject : public gen::CountedObject {
  std::string name;	// name of the type
public:
  explicit NamedObject (const std::string & _name) : name(_name) {};
  const std::string & getName () const { return name; }
  void setName (const std::string & _name) { name = _name; }
};

//
// InclusionInfo
//
class InclusionInfo {
public:
  enum IncType {INC_H, INC_C, INC_HPP, INC_CPP, INC_JAVA, numIncs};
private:
  std::vector<std::string> inclusions[numIncs];
public:
  static IncType getIncType (const std::string & _incTypeStr) {
    if (_incTypeStr == "H")
      return INC_H;
    else if (_incTypeStr == "C")
      return INC_C;
    else if (_incTypeStr == "HPP")
      return INC_HPP;
    else if (_incTypeStr == "CPP")
      return INC_CPP;
    else if (_incTypeStr == "JAVA")
      return INC_JAVA;
    else
      throw msgc_exception ("Unknown inclusion type: "+_incTypeStr);
  }
  std::vector<std::string> & operator[] (IncType _incType) {
    return inclusions[_incType];
  }
  bool hasInclusion (IncType _incType) const {
    return !inclusions[_incType].empty();
  }
  void printInclusion (std::ostream & os, IncType _incType) const {
    for (std::vector<std::string>::size_type i=0; i<inclusions[_incType].size(); i++)
      os << inclusions[_incType][i] << std::endl;
  }
};

// MsgClassInfo contain info about any "object" defined in a .m file.
// They all have some C++ and Java interpretation.
// NB: the C++ and Java classes follow the Java convention that class names
//     each begins with a capital letter.

// abstract
class MsgClassInfo : public NamedObject {
  std::string cppName; /* corresponding c++ name */
  std::string jName; /* corresponding java name */
protected:
  virtual void setCPPName (const std::string & _cppName) {
    cppName = _cppName;
  }
  virtual void setJName (const std::string & _jName) {
    jName = _jName; 
  }
public:
  explicit MsgClassInfo (const std::string & _name) : NamedObject(_name) {};
  virtual const std::string & getCPPName () const { return cppName; }
  virtual const std::string & getJName() const { return jName; };
};


// DataType object will have a C interpretation as well.

// abstract
class DataTypeInfo : public MsgClassInfo {
  typedef MsgClassInfo Base;
public:
  typedef gen::CountedPtr<DataTypeInfo> Ptr;
private:
  static stringPtrMap<Ptr> store;
  std::string cName; // corresponding c type
  char   cTok;	// msg_create/parse_generic template token
  std::string jTok;	// corresponding java type token
protected:
  static bool erase (const std::string & _name) { return store.erase (_name); }
  virtual bool erase () {return erase(getName());}
  explicit DataTypeInfo (const std::string & _name) : Base (_name) 
	{ store.insert(_name, Ptr(this)); }
  virtual void setCName (const std::string & _cName) {cName = _cName;}
  virtual void setCTok (const char & _cTok) {cTok = _cTok;}
  virtual void setJTok (const std::string & _jTok) {jTok = _jTok;}
public:
  static Ptr get (const std::string & _name) { return store.get (_name); }
  virtual std::string getCName () const { return cName; }
  virtual const char & getCTok () const { return cTok; }
  virtual std::string getJTok () const { return jTok; }
  virtual bool parserMustAllocateMemory() const =0;
  virtual bool parserUsesMSList() const =0;
  virtual std::string getCTokPut (const std::string & buffPtrName, const std::string & buffEndPtrName, 
			     const std::string & dataVarName) const = 0;
  virtual std::string getCTokGet (const std::string & buffPtrName, const std::string & memHandleName,
			     const std::string & dataVarName) const = 0;
  virtual std::string getCCopy (const std::string & copyDatName, const std::string & origDatName,
			   const std::string & memHandleName) const = 0;
  virtual std::string getCOstrmPrint (const std::string & ostrmPtrName, const std::string & dataVarName,
				 const std::string & nameVarName) const = 0;
  virtual std::string getCSTreePut (const std::string & streePtrName, const std::string & dataVarName,
				 const std::string & nameVarName) const = 0;
  virtual std::string getCSTreeGet (const std::string & streePtrName, const std::string & memHandleName,
				    const std::string & dataVarName, const std::string & nameVarName) const = 0;

  virtual bool specializesArray () const { return false; } // redefine the following functions if true
  virtual bool arrayNeedsLenVar () const { return true; }
  virtual std::string getCArrayTokPut (const std::string & buffPtrName, const std::string & buffEndPtrName, 
				       const std::string & dataVarName, const std::string & lenVarName) const {}
  virtual std::string getCArrayTokGet (const std::string & buffPtrName, const std::string & memHandleName,
				       const std::string & dataVarName, const std::string & lenVarName) const {}
  virtual std::string getCArrayCopy (const std::string & copyVarName, const std::string & origVarName, 
				const std::string & memHandleName,
				const std::string & copyLenVarName, const std::string & origLenVarName) const {}
  virtual std::string getCArrayOstrmPrint (const std::string & ostrmPtrName, const std::string & dataVarName, 
					   const std::string & lenVarName, const std::string & nameVarName) const {}
  virtual std::string getCArraySTreePut (const std::string & streePtrName, const std::string & dataVarName, 
					 const std::string & lenVarName, const std::string & nameVarName) const {}
  virtual std::string getCArraySTreeGet (const std::string & streePtrName, const std::string & memHandleName,
					 const std::string & dataVarName, 
					 const std::string & lenVarName, const std::string & nameVarName) const {}

  virtual bool isInner() const =0;
  virtual bool isLink() const =0;
  virtual std::string getLinkVar() const {return "";}
  virtual std::string getCPPArrayName () const { 
    if (isLink()) 
      return "std::list<" + getCPPName() + " >";
    else
      return "std::vector<" + getCPPName() + " >";
  }
  virtual std::string getJArrayName () const { 
    return getJName() + "[]";
  }
  virtual std::string getJArrayTok () const { 
    return "[" + getJTok();
  }
};

// Primitive Types
class PrimTypeInfo : public DataTypeInfo {
  typedef DataTypeInfo Base;
public:
  typedef gen::CountedPtr<PrimTypeInfo> Ptr;
private:
  static stringPtrMap<Ptr> store;
  std::string tokPutName;
  std::string tokGetName;
protected:
  static bool erase (const std::string & _name) {return Base::erase (_name) && store.erase (_name);}
  PrimTypeInfo ( const std::string & _name, 
		 const std::string & _cName,
		 const std::string & _cppName,
		 const char        & _cTok,
		 const std::string & _tokPutName,
		 const std::string & _tokGetName,
		 const std::string & _jName,
		 const std::string & _jTok
		 ) : Base (_name), tokPutName(_tokPutName), tokGetName(_tokGetName) {
		   setCName(_cName);
		   setCPPName(_cppName);
		   setCTok(_cTok);
		   setJName(_jName);
		   setJTok(_jTok);
		   store.insert(_name,Ptr(this));
		 }
public:
  static Ptr getNew ( const std::string & _name, 
		      const std::string & _cName,
		      const std::string & _cppName,
		      const char        & _cTok,
		      const std::string & _tokPutName,
		      const std::string & _tokGetName,
		      const std::string & _jName,
		      const std::string & _jTok
		      );
  virtual bool erase () {return erase(getName());}
  static Ptr get (const std::string & _name) { return store.get (_name); }
  virtual bool parserMustAllocateMemory() const {return true;}
  virtual bool parserUsesMSList() const {return false;}
  virtual std::string getCTokPut (const std::string & buffPtrName, const std::string & buffEndPtrName, 
			     const std::string & dataVarName) const {
    return tokPutName + "( &" + buffPtrName + ", (char_sy *) " + buffEndPtrName + ", " + dataVarName + ")";
  };
  virtual std::string getCTokGet (const std::string & buffPtrName, const std::string & memHandleName,
			     const std::string & dataVarName) const {
    return tokGetName + "( (char_sy **) &" + buffPtrName + ", &" + dataVarName + ")";
  };
  virtual std::string getCCopy (const std::string & copyDatName, const std::string & origDatName,
			   const std::string & memHandleName) const {
    return copyDatName + " = " + origDatName;
  };
  virtual std::string getCOstrmPrint (const std::string & ostrmPtrName, const std::string & dataVarName,
				 const std::string & nameVarName) const {
    return "mdat_ostrm_print_" + getCName() + "_ ( " + ostrmPtrName 
      + ", " + dataVarName + ", " + nameVarName + ")";
  };
  virtual std::string getCSTreePut (const std::string & streePtrName, const std::string & dataVarName,
				 const std::string & nameVarName) const {
    return "mdat_stree_put_" + getCName() + "_ ( " + streePtrName 
      + ", " + dataVarName + ", " + nameVarName + ")";
  };
  virtual std::string getCSTreeGet (const std::string & streePtrName, const std::string & memHandleName,
				    const std::string & dataVarName,
				    const std::string & nameVarName) const {
    return "mdat_stree_get_" + getCName() + "_ ( " + streePtrName 
      + ", &" + dataVarName + ", " + nameVarName + ")";
  };
  bool isInner() const {return true;}
  bool isLink() const {return false;}
};

// special Primitive Type : "tchar_sy" (tchar_sy[] = string_sy)
class TCharSy : public PrimTypeInfo {
public:
  const static std::string NAME;
private:
  friend class PrimTypeInfo;
  TCharSy ( const std::string & _cName,
	    const char        & _cTok,
	    const std::string & _tokPutName,
	    const std::string & _tokGetName,
	    const std::string & _jName,
	    const std::string & _jTok
	    ) : PrimTypeInfo (NAME, _cName, _cName, _cTok, _tokPutName, _tokGetName, _jName, _jTok) {};
public:
  std::string getCPPArrayName () const {
    return "gen::string_sy";
  }
  std::string getJArrayName () const {
    return "String";
  }
  virtual std::string getJArrayTok () const { 
    return "Ljava/lang/String;";
  }
  bool parserMustAllocateMemory() const {return false;};
  bool parserUsesMSList() const {return true;}
  bool specializesArray () const { return true; }
  bool arrayNeedsLenVar () const { return true; }
  std::string getCArrayTokPut (const std::string & buffPtrName, const std::string & buffEndPtrName, 
			  const std::string & dataVarName, const std::string & lenVarName) const {
    return "TOK_putstring( &" + buffPtrName + ", (char_sy *) " + buffEndPtrName + ", " 
      + dataVarName + ", " + lenVarName + ")";
  }
  std::string getCArrayTokGet (const std::string & buffPtrName, const std::string & memHandleName,
			  const std::string & dataVarName, const std::string & lenVarName) const {
    return "(" + memHandleName + ")?TOK_getstring_alloc( (char_sy **) &" + buffPtrName + ", &" + dataVarName 
      + ", &" + lenVarName + ", " + memHandleName + "):"
      + "TOK_getstring( (char_sy **) &" + buffPtrName + ", &" + dataVarName 
      + ", &" + lenVarName + ")";
  }
  std::string getCArrayCopy (const std::string & copyVarName, const std::string & origVarName, 
			const std::string & memHandleName,
			const std::string & copyLenVarName, const std::string & origLenVarName) const {
    return "if(1) {if (" + memHandleName + ") {" + copyVarName + "= (char_sy *) malloc ((size_t) ("
      + origLenVarName + ")); " + "list_push(" + memHandleName + ", " + copyVarName + "); }; "
      + copyLenVarName + " = " + origLenVarName + "; " 
      + "strncpy (" + copyVarName + ", " + origVarName + ", " + origLenVarName + "); " 
      + "} else";
  }
  std::string getCArrayOstrmPrint (const std::string & ostrmPtrName, const std::string & dataVarName, 
			      const std::string & lenVarName, const std::string & nameVarName) const {
    return "mdat_ostrm_print_string_sy_ (" + ostrmPtrName + ", " + dataVarName + ", "
      + lenVarName + ", " + nameVarName + ")";
  }
  std::string getCArraySTreePut (const std::string & streePtrName, const std::string & dataVarName, 
				 const std::string & lenVarName, const std::string & nameVarName) const {
    return "mdat_stree_put_string_sy_ (" + streePtrName + ", " + dataVarName + ", "
      + lenVarName + ", " + nameVarName + ")";
  }
  std::string getCArraySTreeGet (const std::string & streePtrName, const std::string & memHandleName,
				 const std::string & dataVarName, 
				 const std::string & lenVarName, const std::string & nameVarName) const {
    return "mdat_stree_get_string_sy_ (" + streePtrName + ", &" + dataVarName + ", &"
      + lenVarName + ", " + nameVarName + ", " + memHandleName + ")";
  }
};

// special Primitive Type : "bchar_sy" (bchar_sy[] = bstring_sy)
class BCharSy : public PrimTypeInfo {
public:
  const static std::string NAME;
private:
  friend class PrimTypeInfo;
  BCharSy ( const std::string & _cName,
	    const char   & _cTok,
	    const std::string & _tokPutName,
	    const std::string & _tokGetName,
	    const std::string & _jName,
	    const std::string & _jTok
	    ) : PrimTypeInfo (NAME, _cName, _cName, _cTok, _tokPutName, _tokGetName, _jName, _jTok) {};
public:
  std::string getCPPArrayName () const {
    return "gen::string_sy";
  }
  std::string getJArrayName () const {
    return "String";
  }
  virtual std::string getJArrayTok () const { 
    return "Ljava/lang/String;";
  }
  bool parserMustAllocateMemory() const {return false;};
  bool parserUsesMSList() const {return false;}
  bool specializesArray () const { return true; }
  bool arrayNeedsLenVar () const { return true; }
  std::string getCArrayTokPut (const std::string & buffPtrName, const std::string & buffEndPtrName, 
			  const std::string & dataVarName, const std::string & lenVarName) const {
    return "TOK_putstring( &" + buffPtrName + ", (char_sy *) " + buffEndPtrName + ", " 
      + dataVarName + ", " + lenVarName + ")";
  }
  std::string getCArrayTokGet (const std::string & buffPtrName, const std::string & memHandleName,
			  const std::string & dataVarName, const std::string & lenVarName) const {
    return "TOK_getstring( (char_sy **) &" + buffPtrName + ", &" + dataVarName 
      + ", &" + lenVarName + ")";
  }
  std::string getCArrayCopy (const std::string & copyVarName, const std::string & origVarName, 
			const std::string & memHandleName,
			const std::string & copyLenVarName, const std::string & origLenVarName) const {
    return "if(1) {" + copyVarName + " = " + origVarName + "; " 
      + copyLenVarName + "=" + origLenVarName + ";} else";
  }
  std::string getCArrayOstrmPrint (const std::string & ostrmPtrName, const std::string & dataVarName, 
			      const std::string & lenVarName, const std::string & nameVarName) const {
    return "mdat_ostrm_print_string_sy_ (" + ostrmPtrName + ", " + dataVarName + ", "
      + lenVarName + ", " + nameVarName + ")";
  }
  std::string getCArraySTreePut (const std::string & streePtrName, const std::string & dataVarName, 
			      const std::string & lenVarName, const std::string & nameVarName) const {
    return "mdat_stree_put_string_sy_ (" + streePtrName + ", " + dataVarName + ", "
      + lenVarName + ", " + nameVarName + ")";
  }
  std::string getCArraySTreeGet (const std::string & streePtrName, const std::string & memHandleName,
				 const std::string & dataVarName, 
				 const std::string & lenVarName, const std::string & nameVarName) const {
    return "mdat_stree_get_bstring_sy_ (" + streePtrName + ", &" + dataVarName + ", &"
      + lenVarName + ", " + nameVarName + ")";
  }
};

// special Primitive Type : "cchar_sy" (cchar_sy[] = cstring_sy)
class CCharSy : public PrimTypeInfo {
public:
  const static std::string NAME;
private:
  friend class PrimTypeInfo;
  CCharSy ( const std::string & _cName,
	    const char   & _cTok,
	    const std::string & _tokPutName,
	    const std::string & _tokGetName,
	    const std::string & _jName,
	    const std::string & _jTok
	    ) : PrimTypeInfo (NAME, _cName, _cName, _cTok, _tokPutName, _tokGetName, _jName, _jTok) {};
public:
  std::string getCPPArrayName () const {
    return "gen::string_sy";
  }
  std::string getJArrayName () const {
    return "String";
  }
  virtual std::string getJArrayTok () const { 
    return "Ljava/lang/String;";
  }
  bool parserMustAllocateMemory() const {return false;};
  bool parserUsesMSList() const {return true;}
  bool specializesArray () const { return true; }
  bool arrayNeedsLenVar () const { return false; }
  std::string getCArrayTokPut (const std::string & buffPtrName, const std::string & buffEndPtrName, 
			  const std::string & dataVarName, const std::string & lenVarName) const {
    return "TOK_putstring( &" + buffPtrName + ", (char_sy *) " + buffEndPtrName + ", " 
      + dataVarName + ", (short_sy) (" + dataVarName + "?(strlen(" + dataVarName + ")+1):0))";
  }
  std::string getCArrayTokGet (const std::string & buffPtrName, const std::string & memHandleName,
			  const std::string & dataVarName, const std::string & lenVarName) const {
    return "TOK_getstring( (char_sy **) &" + buffPtrName + ", &" + dataVarName 
      + ", &" + lenVarName + ")";
  }
  std::string getCArrayCopy (const std::string & copyVarName, const std::string & origVarName, 
			const std::string & memHandleName,
			const std::string & copyLenVarName, const std::string & origLenVarName) const {
    return "if(1) {if (" + memHandleName + " && " + origVarName + ") {" + copyVarName 
      + "= (char_sy *) malloc ((size_t) strlen(" + origVarName + ")+1); " 
      + "list_push(" + memHandleName + ", " + copyVarName + "); }; "
      + "strcpy (" + copyVarName + ", " + origVarName + "); " 
      + "} else";
  }
  std::string getCArrayOstrmPrint (const std::string & ostrmPtrName, const std::string & dataVarName, 
			      const std::string & lenVarName, const std::string & nameVarName) const {
    return "mdat_ostrm_print_cstring_sy_ (" + ostrmPtrName + ", " + dataVarName + ", "
      + nameVarName + ")";
  }
  std::string getCArraySTreePut (const std::string & streePtrName, const std::string & dataVarName, 
			      const std::string & lenVarName, const std::string & nameVarName) const {
    return "mdat_stree_put_cstring_sy_ (" + streePtrName + ", " + dataVarName + ", "
      + nameVarName + ")";
  }
  std::string getCArraySTreeGet (const std::string & streePtrName, const std::string & memHandleName,
				 const std::string & dataVarName, 
				 const std::string & lenVarName, const std::string & nameVarName) const {
    return "mdat_stree_get_cstring_sy_ (" + streePtrName + ", &" + dataVarName + ", "
      + nameVarName + ", " + memHandleName + ")";
  }
};

// special Primitive Type : "rchar_sy" (rchar_sy[] = rstring_sy)
class RCharSy : public PrimTypeInfo {
public:
  const static std::string NAME;
private:
  friend class PrimTypeInfo;
  RCharSy ( const std::string & _cName,
	    const char   & _cTok,
	    const std::string & _tokPutName,
	    const std::string & _tokGetName,
	    const std::string & _jName,
	    const std::string & _jTok
	    ) : PrimTypeInfo (NAME, _cName, _cName, _cTok, _tokPutName, _tokGetName, _jName, _jTok) {};
public:
  std::string getCPPArrayName () const {
    return "gen::string_sy";
  }
  std::string getJArrayName () const {
    return "String";
  }
  virtual std::string getJArrayTok () const { 
    return "Ljava/lang/String;";
  }
  bool parserMustAllocateMemory() const {return false;};
  bool parserUsesMSList() const {return false;}
  bool specializesArray () const { return true; }
  bool arrayNeedsLenVar () const { return true; }
  std::string getCArrayTokPut (const std::string & buffPtrName, const std::string & buffEndPtrName, 
			  const std::string & dataVarName, const std::string & lenVarName) const {
    return "TOK_putblob( &" + buffPtrName + ", (char_sy *) " + buffEndPtrName + ", " 
      + dataVarName + ", " + lenVarName + ")";
  }
  std::string getCArrayTokGet (const std::string & buffPtrName, const std::string & memHandleName,
			  const std::string & dataVarName, const std::string & lenVarName) const {
    return "TOK_getblob( (char_sy **) &" + buffPtrName + ", &" + dataVarName 
      + ", &" + lenVarName + ")";
  }
  std::string getCArrayCopy (const std::string & copyVarName, const std::string & origVarName, 
			const std::string & memHandleName,
			const std::string & copyLenVarName, const std::string & origLenVarName) const {
    return "if(1) {" + copyVarName + " = " + origVarName + "; " 
      + copyLenVarName + "=" + origLenVarName + ";} else";
  }
  std::string getCArrayOstrmPrint (const std::string & ostrmPtrName, const std::string & dataVarName, 
			      const std::string & lenVarName, const std::string & nameVarName) const {
    return "mdat_ostrm_print_string_sy_ (" + ostrmPtrName + ", " + dataVarName + ", "
      + lenVarName + ", " + nameVarName + ")";
  }
  std::string getCArraySTreePut (const std::string & streePtrName, const std::string & dataVarName, 
			      const std::string & lenVarName, const std::string & nameVarName) const {
    return "mdat_stree_put_string_sy_ (" + streePtrName + ", " + dataVarName + ", "
      + lenVarName + ", " + nameVarName + ")";
  }
  std::string getCArraySTreeGet (const std::string & streePtrName, const std::string & memHandleName,
				 const std::string & dataVarName, 
				 const std::string & lenVarName, const std::string & nameVarName) const {
    return "mdat_stree_get_bstring_sy_ (" + streePtrName + ", &" + dataVarName + ", &"
      + lenVarName + ", " + nameVarName + ")";
  }
};

inline
PrimTypeInfo::Ptr PrimTypeInfo::getNew ( const std::string & _name, 
					 const std::string & _cName,
					 const std::string & _cppName,
					 const char        & _cTok,
					 const std::string & _tokPutName,
					 const std::string & _tokGetName,
					 const std::string & _jName,
					 const std::string & _jTok
					 ) {
  // handle special primitive types
  if (_name == TCharSy::NAME) 
    return Ptr(new TCharSy(_cName, _cTok, _tokPutName, _tokGetName, _jName, _jTok)); 
  if (_name == BCharSy::NAME) 
    return Ptr(new BCharSy(_cName, _cTok, _tokPutName, _tokGetName, _jName, _jTok)); 
  if (_name == CCharSy::NAME) 
    return Ptr(new CCharSy(_cName, _cTok, _tokPutName, _tokGetName, _jName, _jTok)); 
  if (_name == RCharSy::NAME) 
    return Ptr(new RCharSy(_cName, _cTok, _tokPutName, _tokGetName, _jName, _jTok)); 
  return Ptr(new PrimTypeInfo(_name, _cName, _cppName, _cTok, _tokPutName, _tokGetName, _jName, _jTok)); 
}

// generic "User Defined" Interface
// abstract interface
class UserDefined {
public:
  virtual void printCDeclaration (std::ostream & os) const =0;
  virtual void printCPPDeclaration (std::ostream & os) const =0;
  virtual void printCDefinition (std::ostream & os) const =0;
  virtual void printCPPDefinition (std::ostream & os) const =0;
  virtual void printJDefinition (const std::string & javaDir, const std::string & inFileName) const =0;
};

// KeywordTab is a singleton table used to hold
// information regarding all the message keywords (tokens)
class KeywordTab : public UserDefined {
  static std::map<std::string, int> tab;
  static std::map<std::string, std::vector<std::string> > classTab;
  bool done;
  int  insertSlot;
  KeywordTab () : done (false), insertSlot(0) {};
  KeywordTab (const KeywordTab &);
  KeywordTab & operator= (const KeywordTab &);
public:
  static KeywordTab & get() {
    static KeywordTab glob;
    return glob;
  }
  void add (const std::string & _name, const std::string & _idName, 
	    const std::string & _className);
  void add (const std::string & _name, int _value =0);
  void setDone ();
  const std::string getJName() const { return "keyword"; };

  void printCDeclaration (std::ostream & os) const;
  void printCPPDeclaration (std::ostream & os) const;
  void printCDefinition (std::ostream & os) const;
  void printCPPDefinition (std::ostream & os) const;
  void printJDefinition (const std::string & javaDir, const std::string & inFileName) const;
};

// User Defined Message Class 
// abstract
class UserDefinedMsgClass : public MsgClassInfo, public UserDefined {
  typedef MsgClassInfo Base;
public:
  typedef gen::CountedPtr<UserDefinedMsgClass> Ptr;
protected:
  virtual void setCPPName (const std::string & _cppName) {Base::setCPPName(_cppName);}
  virtual void setJName (const std::string & _jName) {
    std::string temp = _jName;
    if (!temp.empty()) temp[0] = toupper(temp[0]);
    Base::setJName(temp);
  }
  virtual void setCPPName () =0;
  virtual void setJName () =0;
  void setMangledNames () {
    setCPPName(); setJName();
  }
  explicit UserDefinedMsgClass (const std::string & _name) : Base (_name)  {}
public:
};

// User Defined Data Types
// abstract
class UserDefinedTypeInfo : public DataTypeInfo, public UserDefined {
  typedef DataTypeInfo Base;
public:
  typedef gen::CountedPtr<UserDefinedTypeInfo> Ptr;
private:
  static stringPtrMap<Ptr> store;
protected:
  static bool erase (const std::string & _name) {return Base::erase (_name) && store.erase (_name);}
  virtual bool erase () {return erase(getName());}
  explicit UserDefinedTypeInfo (const std::string & _name) : Base (_name)
    { store.insert(_name,Ptr(this)); }
  virtual void setCName (const std::string & _cName) {Base::setCName(_cName);}
  virtual void setCTok (const char & _cTok) {Base::setCTok(_cTok);}
  virtual void setCPPName (const std::string & _cppName) {Base::setCPPName(_cppName);}
  virtual void setJName (const std::string & _jName) {Base::setJName(_jName);}

  virtual void setCName () =0;
  virtual void setCTok () =0;
  virtual void setCPPName () =0;
  virtual void setJName () =0;
  virtual void setJTok () {Base::setJTok("Lcom/syncsort/bex/msg/" + getJName() + ";");}
  void setMangledNames () {
    setCName(); setCTok(); setCPPName(); setJName(); setJTok (); 
  }
public:
  static Ptr get (const std::string & _name) { return store.get (_name); }
  virtual bool parserMustAllocateMemory() const {return true;};
  virtual bool parserUsesMSList() const {return false;}
  virtual std::string getCTokPut (const std::string & buffPtrName, const std::string & buffEndPtrName, 
			     const std::string & dataVarName) const {
    return "mdat_create_"+getName() + "( &" + buffPtrName + ", " + buffEndPtrName + ", &" + dataVarName + ")";
  };
  virtual std::string getCTokGet (const std::string & buffPtrName, const std::string & memHandleName,
			     const std::string & dataVarName) const {
    return "mdat_parse_"+getName() + "( &" + buffPtrName + ", &" + dataVarName + ")";
  };
  virtual std::string getCCopy (const std::string & copyDatName, const std::string & origDatName,
			   const std::string & memHandleName) const {
    return "mdat_copy_"+getName() + "(" + copyDatName + ", " + origDatName + ")";
  };
  virtual std::string getCOstrmPrint (const std::string & ostrmPtrName, const std::string & dataVarName,
				 const std::string & nameVarName) const {
    return "mdat_ostrm_print_"+getName() + "( " + ostrmPtrName + ", &" + dataVarName 
      + ", " + nameVarName + ")";
  };
  virtual std::string getCSTreePut (const std::string & streePtrName, const std::string & dataVarName,
				 const std::string & nameVarName) const {
    return "mdat_stree_put_"+getName() + "( " + streePtrName + ", &" + dataVarName 
      + ", " + nameVarName + ")";
  };
  virtual std::string getCSTreeGet (const std::string & streePtrName, const std::string & memHandleName,
				    const std::string & dataVarName, const std::string & nameVarName) const {
    return "mdat_stree_get_"+getName() + "( " + streePtrName + ", &" + dataVarName 
      + ", " + nameVarName + ")";
  };
};



///////////////////////////////////////////////////////////
// Flag class related
//

class FlagEnumItem {
  std::string name;
  std::string value;
public:
  FlagEnumItem (const std::string & _name ="", const std::string & _value ="") : 
    name(_name), value(_value) {}
  std::string getName () const {return name; }
  std::string getValue () const {return value; }
};

// holds instance of " flag_sy<prim_type> name {enum_list}; "
class FlagTypeInfo : public UserDefinedTypeInfo {
  typedef UserDefinedTypeInfo Base;
public:
  typedef gen::CountedPtr<FlagTypeInfo> Ptr;
private:
  static stringPtrMap<Ptr> store;
  PrimTypeInfo::Ptr primTypePtr;
  std::vector<FlagEnumItem> flags;
  void setCName() { Base::setCName (primTypePtr->getCName()); }
  void setCTok () { Base::setCTok (primTypePtr->getCTok()); }
  void setCPPName() { Base::setCPPName (getName()); }
  void setJName() { Base::setJName (getName()); }
protected:
  static bool erase (const std::string & _name) {return Base::erase (_name) && store.erase (_name);}
  FlagTypeInfo (const std::string & _name, const std::string & _primTypeName) : Base (_name) {
    primTypePtr = PrimTypeInfo::get (_primTypeName);
    setMangledNames ();
    store.insert(_name,Ptr(this));
  }
public:
  static void eraseAll() {
    std::vector<std::string> allNames = store.getKeys();
    for (std::vector<std::string>::const_iterator it=allNames.begin(); it!=allNames.end(); ++it)
      erase (*it);
  }
  static Ptr getNew (const std::string & _name, const std::string & _primTypeName) {
    return Ptr(new FlagTypeInfo(_name, _primTypeName)); 
  }
  virtual bool erase () {return erase(getName());}
  static Ptr get (const std::string & _name) { return store.get (_name); }
  void addFlagEnumItem (const FlagEnumItem & _item) { flags.push_back(_item); }
  virtual void printCDeclaration (std::ostream & os) const;
  virtual void printCPPDeclaration (std::ostream & os) const;
  virtual void printCDefinition (std::ostream & os) const;
  virtual void printCPPDefinition (std::ostream & os) const;
  virtual void printJDefinition (const std::string & javaDir, const std::string & inFileName) const;
  bool isInner() const {return true;}
  bool isLink() const {return false;}
};

///////////////////////////////////////////////////////////
// MData related
//

class MDataTypeInfo; // early declaration
class MDataMemberInfo; // early declaration

class MDataVisualInfo {
  friend int yyparse();
  friend class MDataTypeInfo;
  std::string vType;		// name of the visual component type
  std::string vURI;		// name of the URI associated with the visual component
  std::string vFunc;		// name of the function associated with the visual component
public:
  MDataVisualInfo (const std::string & _vType ="", 
		   const std::string & _vURI ="\"\"", 
		   const std::string & _vFunc = "\"\"") :
    vType(_vType), vURI(_vURI), vFunc(_vFunc) {}
};

class TypeQualifier {
  friend class MDataMemberInfo;
  friend class MDataTypeInfo;
public:
  const static char C_NULL = '\0'; // (used to indicate that memory management is done internally in the mdata)
  const static char C_PTR = '*'; // (used to indicate that memory management is not done by the mdata)
  const static char C_MSLIST = '+'; // (used to indicate that memory management is done via ms_list subsystem)
private:
  bool b_IsPtr;
  bool b_CtorAllocatesMemory;
  bool b_ParserAllocatesMemory;
  bool b_ParserUsesMSList;
  bool b_DtorReleasesMemory;
  std::string lenName;
  std::string maxLenName;
protected:
  TypeQualifier (bool _isPtr, char _c, const std::string & _lenName, const std::string & _maxLenName) : 
    b_IsPtr(_isPtr), lenName(_lenName), maxLenName(_maxLenName) {
    switch (_c) {
    case (C_NULL) : 
      b_CtorAllocatesMemory = false;
      b_ParserAllocatesMemory = true;
      b_ParserUsesMSList = false;
      b_DtorReleasesMemory = true;
      break;
    case (C_PTR) :
      b_CtorAllocatesMemory = false;
      b_ParserAllocatesMemory = false;
      b_ParserUsesMSList = true;
      b_DtorReleasesMemory = false;
      break;
    case (C_MSLIST) :
      b_CtorAllocatesMemory = false;
      b_ParserAllocatesMemory = true;
      b_ParserUsesMSList = true;
      b_DtorReleasesMemory = false;
      break;
    default :
      throw std::logic_error (std::string("TypeQualifier::TypeQualifier() called with unknown memory management spec, \'") 
			 + _c + "\'");
      break;
    };
  }
public:
  bool isPtr() const {return b_IsPtr;}
  bool isArray() const {return isFixedArray() || isVarArray(); }
  bool isFixedArray() const {return !maxLenName.empty(); }
  bool isVarArray() const {return (maxLenName.empty() && !b_IsPtr); }
  std::string getLenName() const {return lenName;}
  void setLenName (const std::string & _lenName) {
    if (b_IsPtr) throw std::logic_error ("TypeQualifier::setLenName() called on ptr type.");
    if (isFixedArray()) throw std::logic_error ("TypeQualifier::setLenName() called on fixed array type.");
    lenName = _lenName;
  }
  std::string getMaxLenName() const {return maxLenName;}
};

class TypeQualifierPtr : public TypeQualifier {
public:
  TypeQualifierPtr(char _c =C_NULL) : TypeQualifier (true, _c, "", "") {};
};

class TypeQualifierFixedArray : public TypeQualifier {
public:
  TypeQualifierFixedArray (const std::string & _maxLenName, const std::string & _varLenName ="") :
    TypeQualifier (false, C_NULL, _varLenName, _maxLenName) {}
};

class TypeQualifierVarArray : public TypeQualifier {
public:
  TypeQualifierVarArray (const std::string & _varLenName ="", char _c =C_NULL) :
    TypeQualifier (false, _c, _varLenName, "") {}
};

class MDataMemberInfo : public NamedObject {	// name is that of the variable
  friend int yyparse();
  friend class MDataTypeInfo;
public:
  typedef gen::CountedPtr<MDataMemberInfo> Ptr;
private:
  int b_IsConst;
  int b_IsHidden;
  DataTypeInfo::Ptr typePtr;	// data type
  std::vector<TypeQualifier> typeQuals;	// data type's qualifiers
  std::string defVal;		// default value of the member
  MDataVisualInfo visInfo;	// visual info
  std::vector<std::string> comments;	// associated comment info
  static void printDecDefine (std::ostream & os, const std::string & _name, const std::string & _val);
  static void printDecOneLine (std::ostream & os, const std::string & _type, const std::string & _name);
  static std::ostream & indent (std::ostream & os, int indentCount);
public:
  MDataMemberInfo (const std::string & _name, int _isConst, int _isHidden,
		   const std::string & _typeName, const std::vector<TypeQualifier> & _typeQuals, 
		   const std::string & _defVal, const MDataVisualInfo & _visInfo, 
		   const std::vector<std::string> & _comments);
  bool isConst() const {return b_IsConst;}
  bool isHidden() const {return b_IsHidden;}
  std::string getCPPQualTypeName () const;
  std::string getCPPConstRef () const;
  std::string getCPPDefaultVal () const;
  std::string getJQualTypeName () const;
  DataTypeInfo::Ptr getTypePtr() const {return typePtr;}
  std::string getLenNameAtLevel (std::vector<TypeQualifier>::size_type level);
  std::string getMaxLenNameAtLevel (std::vector<TypeQualifier>::size_type level);
  std::string getSzName (const std::string & szNameBase, std::vector<TypeQualifier>::size_type level) const;
  void printCDeclaration (std::ostream & os, std::vector<std::string> & usedLenNames, const std::string & className) const;
  void printCTokPut (std::ostream & os, const std::string & buffPtrName, 
		    const std::string & buffEndPtrName, const std::string & structPtrName, 
		    int & loopVarCount, int & errCodeCount,
		    const std::string & varName, std::vector<TypeQualifier>::size_type level) const;
  void printCTokGet (std::ostream & os, const std::string & buffPtrName, 
		    const std::string & memHndlName, const std::string & structPtrName, 
		    int & loopVarCount, int & errCodeCount,
		    std::vector<std::string> & usedLenNames, bool & useTmp,
		    const std::string & varName, std::vector<TypeQualifier>::size_type level) const;
  void printCCopy (std::ostream & os, 
		   const std::string & copyStructPtrName, const std::string & origStructPtrName, 
		   const std::string & memHndlName, 
		   int & loopVarCount, std::vector<std::string> & usedLenNames,
		   const std::string & copyVarName, const std::string & origVarName, 
		   std::vector<TypeQualifier>::size_type level) const;
  void printCOstrmPrint (std::ostream & os, const std::string & ostrmPtrName, 
			 const std::string & structPtrName, int & loopVarCount,
			 const std::string & nameVarName, const std::string & varName, 
			 std::vector<TypeQualifier>::size_type level) const;
  void printCSTreePut (std::ostream & os, const std::string & streePtrName, 
		       const std::string & structPtrName, int & loopVarCount,
		       const std::string & nameVarName, const std::string & varName, 
		       std::vector<TypeQualifier>::size_type level) const;
  void printCSTreeGet (std::ostream & os, const std::string & streePtrName, 
		       const std::string & structPtrName, const std::string & memHndlName, int & loopVarCount,
		       std::vector<std::string> & usedLenNames, const std::string & nameVarName, 
		       const std::string & varName, std::vector<TypeQualifier>::size_type level) const;
  void printCPPDeclaration (std::ostream & os) const;
  void printCPPIntDef (std::ostream & os, 
		       const bool autoSetMsgFlag = false,
		       const std::string & msgFlagVarName = "", const std::string flagConstName = "") const;
  void printJDeclaration (std::ostream & os) const;
  void printJIntDef (std::ostream & os, const std::string & this_str ="this", 
		     const bool autoSetMsgFlag = false,
		     const std::string & msgFlagVarName = "", const std::string flagConstName = "") const;
};

// holds instance of " [control] mdata name {mdata_info} "
class MDataQualifier;  // early declaration
class MDataTypeInfo : public UserDefinedTypeInfo {
public:
  const static std::string STR_NEXT; // "next" (variable name used for link mdata's pointer to itself)
  const static std::string STR_REQID; // "reqid" (variable name used for request id)
  enum Flag {FLAG_NONE=0x00, FLAG_IS_CONTROL=0x01, FLAG_IS_LINK=0x02, 
	     FLAG_IS_INNER=0x04, FLAG_IS_VISUAL=0x08, FLAG_IS_FORMAT=0x10};
  enum Component {COMP_NONE=0x00, COMP_SERVICE=0x01};
private:
  typedef UserDefinedTypeInfo Base;
public:
  typedef gen::CountedPtr<MDataTypeInfo> Ptr;
private:
  static stringPtrMap<Ptr> store;
  void setCName() { Base::setCName("mdat_" + getName()); }
  void setCTok () { Base::setCTok('X'); };
  void setCPPName() { Base::setCPPName("MDat_" + getName()); }
  void setJName () { Base::setJName("MDat_" + getName() ); }
  bool   b_IsCtlMsg; // am control message data
  std::string keyword; // for control message data
  void setKeyword (const std::string & _keyword, const std::string & _className); // sets b_IsCtlMsg as a side effect
  std::string flagVarName; // the msg flag variable name
  std::string controlVar; // the control variable name (if control message data)
  bool   b_IsInner; // am inner message data
  bool   b_IsLink; // am linkable message data
  std::string linkVar; // the link variable name (if link message data)
  bool   b_IsVisual; // am visual message data
  std::vector<MDataMemberInfo>::size_type dispMemberIdx;
  bool   b_IsFormat; // has format info
  std::string formatString; // the format string
  std::string getFormatString() const {return formatString;}
  bool   b_CompService; // comprises service interface
  bool   b_ParserUsesMSList;
  std::string cTemplate;
  std::vector<MDataMemberInfo> members;
  std::vector<FlagTypeInfo> flags;
  InclusionInfo incInfo;
protected:
  static bool erase (const std::string & _name) {return Base::erase (_name) && store.erase (_name);}
  MDataTypeInfo (const std::string & _name, const MDataQualifier * _qual, 
		 Component _comps, const std::vector<MDataMemberInfo> & _members);
public:
  static void eraseAll() {
    std::vector<std::string> allNames = store.getKeys();
    for (std::vector<std::string>::const_iterator it=allNames.begin(); it!=allNames.end(); ++it)
      erase (*it);
  }
  static Ptr getNew (const std::string & _name, const MDataQualifier * _qual,
		     Component _comps, const std::vector<MDataMemberInfo> & _members)
    { return Ptr(new MDataTypeInfo(_name, _qual, _comps, _members)); }
  virtual bool erase () {return erase(getName());}
  static Ptr get (const std::string & _name) { return store.get (_name); }
  void addInclusions (const InclusionInfo & _incInfo) { incInfo = _incInfo; } 
  std::string getJUserName () const {
    if (!comprisesService()) return getJName(); 
    std::string tmp = getName();
    tmp[0] = toupper(tmp[0]);
    return tmp; 
  }
  virtual void printCDeclaration (std::ostream & os) const;
  virtual void printCPPDeclaration (std::ostream & os) const;
  virtual void printCDefinition (std::ostream & os) const;
  virtual void printCPPDefinition (std::ostream & os) const;
  virtual void printCPPMemberIntDef (std::ostream &os) const;
  virtual void printJDefinition (const std::string & javaDir, const std::string & inFileName) const;
  virtual void printJMemberIntDef (std::ostream &os, const std::string & this_str ="this") const;
  bool isMember(const std::string & _memberName) const;
  bool isCtlMsg() const {return b_IsCtlMsg;}
  std::string getFlagVarName() const {return flagVarName;}
  std::string getCtlVar() const {return controlVar;}
  bool isInner() const {return b_IsInner;}
  bool isLink() const {return b_IsLink;}
  bool isVisual() const {return b_IsVisual;}
  bool isEmpty() const {return (members.empty() && !isLink());}
  std::string getLinkVar() const {return linkVar;}
  bool comprisesService() const {return b_CompService;}
  bool parserUsesMSList() const {return b_ParserUsesMSList;}
  virtual std::string getCTokGet (const std::string & buffPtrName, const std::string & memHandleName,
			     const std::string & dataVarName) const {
    if (parserUsesMSList())
      return "mdat_parse_"+getName() + "( &" + buffPtrName + ", &" + dataVarName + ", " + memHandleName + ")";
    else
      return "mdat_parse_"+getName() + "( &" + buffPtrName + ", &" + dataVarName + ")";
  };
  virtual std::string getCCopy (const std::string & copyDatName, const std::string & origDatName,
			   const std::string & memHandleName) const {
    if (parserUsesMSList())
      return "mdat_copy_"+getName() + "( &" + copyDatName + ", &" + origDatName + ", " + memHandleName + ")";
    else
      return "mdat_copy_"+getName() + "( &" + copyDatName + ", &" + origDatName + ")";
  };
  virtual std::string getCSTreeGet (const std::string & streePtrName, const std::string & memHandleName,
				    const std::string & dataVarName, const std::string & nameVarName) const {
    return "mdat_stree_get_"+getName() + "( " + streePtrName + ", &" + dataVarName 
      + ", " + nameVarName + ", " + memHandleName + ")";
  };
};

class MDataQualifier {
  friend class MDataTypeInfo;
  MDataTypeInfo::Flag flag;
  std::string controlVar;
  std::string linkVar;
  std::string visualVar;
  std::string formatVar;
public:
  MDataQualifier (MDataTypeInfo::Flag _flag = MDataTypeInfo::FLAG_NONE, const std::string & _param="") :
    flag (_flag) { addQualifier (_flag, _param); }
  void addQualifier (MDataTypeInfo::Flag _flag, const std::string & _param="") {
    flag = (MDataTypeInfo::Flag)  (flag | _flag);
    if (_flag == MDataTypeInfo::FLAG_IS_CONTROL) controlVar = _param;
    if (_flag == MDataTypeInfo::FLAG_IS_LINK) {
      if (_param.empty()) linkVar = MDataTypeInfo::STR_NEXT;
      else linkVar = _param;
    }
    if (_flag == MDataTypeInfo::FLAG_IS_VISUAL) visualVar = _param;
    if (_flag == MDataTypeInfo::FLAG_IS_FORMAT) formatVar = _param;
  }
};

///////////////////////////////////////////////////////////
// Envelope related
//
class MsgSpec; // forward declaration
class SvcDef; // forward declaration
class EnvTypeInfo; // forward declaration

class EnvArg {
  friend class EnvTypeInfo;
  MDataTypeInfo::Ptr mdat_p;
  std::string varName;
  std::string cppArgTypeName;
  std::string jArgTypeName;
  bool amVector;
public:
  EnvArg (const std::string & mdatName, bool _amVector, const std::string & _varName ="") : 
    varName(_varName), amVector(_amVector) {
    mdat_p = MDataTypeInfo::get(mdatName);
    if (amVector) 
      throw msgc_exception("envelope function can not hide a vector mdata, "+mdatName);
  };
  bool isVector() const {return amVector;}
  MDataTypeInfo::Ptr getMDataPtr() const {return mdat_p;}
  std::string getVarName() const {return varName;}
};

class EnvMethodDef : public NamedObject {
  friend class EnvTypeInfo;
  std::vector<EnvArg> args;
public:
  explicit EnvMethodDef (const std::string & _name, const std::vector<EnvArg> & _args) : 
    NamedObject(_name), args(_args) { }
};

// holds instance of " envelope<svc_nam> name {exported_methods} "
class EnvTypeInfo : public UserDefinedMsgClass {
  typedef UserDefinedMsgClass Base;
public:
  typedef gen::CountedPtr<EnvTypeInfo> Ptr;
private:
  static stringPtrMap<Ptr> store;
  gen::CountedPtr<SvcDef> svc_p;
  MDataTypeInfo::Ptr rootMDat_p;
  std::vector<EnvMethodDef> methods;
  std::vector<MDataTypeInfo::Ptr> mdats;
  std::vector<EnvArg> args;
  virtual void setCPPName () {Base::setCPPName(getName());}
  virtual void setJName () {Base::setJName(getName());}
  std::vector<EnvMethodDef>::size_type findMatchMethodDef 
    (const std::vector<MsgSpec> & svcArgs, const std::string & svcName, std::vector<bool> & flagMatched,
     std::vector<std::vector<MsgSpec>::size_type> & idxMatched) const;
protected:
  static bool erase (const std::string & _name) {return store.erase (_name);}
  explicit EnvTypeInfo (const std::string & _name, const std::vector<EnvMethodDef> _methods);
public:
  static void eraseAll() {
    std::vector<std::string> allNames = store.getKeys();
    for (std::vector<std::string>::const_iterator it=allNames.begin(); it!=allNames.end(); ++it)
      erase (*it);
  }
  static Ptr getNew (const std::string & _name, const std::vector<EnvMethodDef> _methods)
    { return Ptr(new EnvTypeInfo(_name, _methods)); }
  virtual bool erase () {return erase(getName());}
  static Ptr get (const std::string & _name) { return store.get (_name); }
  void setSvc (gen::CountedPtr<SvcDef> _svc_p);
  void setMethods (const std::vector <EnvMethodDef> _methods);
  bool hasRootMData () const { return (rootMDat_p.get()!=0); }
  MDataTypeInfo::Ptr getRootMDataPtr () const {return rootMDat_p;}
  bool isFullyDefined () const { return (svc_p.get()!=0) && (methods.size()>0); }
  virtual void printCDeclaration (std::ostream & os) const;
  virtual void printCPPDeclaration (std::ostream & os) const;
  virtual void printCDefinition (std::ostream & os) const;
  virtual void printCPPDefinition (std::ostream & os) const;
  virtual void printJDefinition (const std::string & javaDir, const std::string & inFileName) const;
};

///////////////////////////////////////////////////////////
// Service related
//

class MsgSpec {
public:
  enum Type {Req, Dat, Fin, DatOrFin};
  MDataTypeInfo::Ptr	mdat_p;
  bool amVector;
  std::string idName;
  std::string keyword;
  std::string vectorSizeName;
  Type type;
  void _init (const std::string & _mDatName);
public:
  MsgSpec (const std::string & _mDatName, const std::string & _keyword, bool _amVector = false);
  MsgSpec (const std::string & _mDatName, const std::string & _keyword, const std::string & _vectorSizeName);
  void setID (Type _type, const std::string & _idName, const std::string & _svcName);
  void printCDecMsg (std::ostream &os) const;
  void printCDefMsg (std::ostream &os) const;
  void printCPPDecMsg (std::ostream &os) const;
  bool isVector() const {return amVector;}
};

class SvcFunc : public NamedObject {
public:
  std::vector<MsgSpec> msgSpecs;
  std::vector<std::string> comments; /* associated comment info */
public:
  explicit SvcFunc (const std::string & _name="") : NamedObject(_name) {}
  virtual void setMsgIDs (const std::string & _svcName) =0;
  void printCDecMsg (std::ostream &os) const;
  void printCDefMsg (std::ostream &os) const;
  void printCPPDecMsg (std::ostream &os) const;
  virtual void printCPPDeclaration (std::ostream &os) const =0;
  virtual void printCPPDecTokInfos (std::ostream & os) const =0;
  virtual void printCPPDefAddSvcFunc (std::ostream & os, const std::string & _svcName) const =0;
  virtual void printCPPDefTokInfos (std::ostream & os, const std::string & _svcName) const =0;
  virtual void printCPPDecCliSvcMemberFunc (std::ostream & os) const =0;
  virtual void printCPPDefCliSvcMemberFunc (std::ostream & os, const std::string & classNam) const =0;
  void printCPPJComments (std::ostream &os) const;
  virtual void printJDefSvc (std::ostream &os) const =0;
  virtual void printJDefSvcInt (std::ostream &os) const =0;
  virtual void printJDefSvcImp (std::ostream &os) const =0;
  virtual void printJDefEnv (std::ostream &os, const std::vector<bool> & flagMatched,
			     const std::vector<std::vector<MsgSpec>::size_type> & idxMatched,
			     const std::vector<EnvArg> & envArgs) const =0;
  virtual std::string getMangledName() const {return getName()+"_";}
};

class SvcFuncP1 : public SvcFunc {
public:
  SvcFuncP1 (const MsgSpec & _ms1) { msgSpecs.push_back (_ms1); }
  void setMsgIDs (const std::string & _svcName) {
    msgSpecs[0].setID (MsgSpec::Req, _svcName + "_" + getName(), "MDatSvc_"+_svcName);
  }
  void printCPPDeclaration (std::ostream &os) const;
  void printCPPDecTokInfos (std::ostream & os) const;
  void printCPPDefAddSvcFunc (std::ostream & os, const std::string & _svcName) const;
  void printCPPDefTokInfos (std::ostream & os, const std::string & _svcName) const;
  void printCPPDecCliSvcMemberFunc (std::ostream & os) const;
  void printCPPDefCliSvcMemberFunc (std::ostream & os, const std::string & classNam) const;
  void printJDefSvc (std::ostream &os) const;
  void printJDefSvcInt (std::ostream &os) const;
  void printJDefSvcImp (std::ostream &os) const;
  void printJDefEnv (std::ostream &os, const std::vector<bool> & flagMatched,
		     const std::vector<std::vector<MsgSpec>::size_type> & idxMatched,
		     const std::vector<EnvArg> & envArgs) const;
};

class SvcFuncP2 : public SvcFunc {
public:
  SvcFuncP2 (const MsgSpec & _ms1, const MsgSpec & _ms2) { 
    msgSpecs.push_back (_ms1); 
    msgSpecs.push_back (_ms2); 
  }
  void setMsgIDs (const std::string & _svcName) {
    msgSpecs[0].setID (MsgSpec::Req, _svcName + "_" + getName(), "MDatSvc_"+_svcName);
    msgSpecs[1].setID (MsgSpec::DatOrFin, _svcName + "_" + getName() + "_response", "MDatSvc_"+_svcName);
  }
  void printCPPDeclaration (std::ostream &os) const;
  void printCPPDecTokInfos (std::ostream & os) const;
  void printCPPDefAddSvcFunc (std::ostream & os, const std::string & _svcName) const;
  void printCPPDefTokInfos (std::ostream & os, const std::string & _svcName) const;
  void printCPPDecCliSvcMemberFunc (std::ostream & os) const;
  void printCPPDefCliSvcMemberFunc (std::ostream & os, const std::string & classNam) const;
  void printJDefSvc (std::ostream &os) const;
  void printJDefSvcInt (std::ostream &os) const;
  void printJDefSvcImp (std::ostream &os) const;
  void printJDefEnv (std::ostream &os, const std::vector<bool> & flagMatched,
		     const std::vector<std::vector<MsgSpec>::size_type> & idxMatched,
		     const std::vector<EnvArg> & envArgs) const;
};

class SvcFuncP3 : public SvcFunc {
public:
  SvcFuncP3 (const MsgSpec & _ms1, const MsgSpec & _ms2, const MsgSpec &_ms3) { 
    msgSpecs.push_back (_ms1); 
    msgSpecs.push_back (_ms2); 
    msgSpecs.push_back (_ms3); 
  }
  void setMsgIDs (const std::string & _svcName) {
    msgSpecs[0].setID (MsgSpec::Req, _svcName + "_" + getName() , "MDatSvc_"+_svcName);
    msgSpecs[1].setID (MsgSpec::Dat, _svcName + "_" + getName() + "_response", "MDatSvc_"+_svcName);
    msgSpecs[2].setID (MsgSpec::Fin, "", "MDatSvc_"+_svcName);
  }
  void printCPPDeclaration (std::ostream &os) const;
  void printCPPDecTokInfos (std::ostream & os) const;
  void printCPPDefAddSvcFunc (std::ostream & os, const std::string & _svcName) const;
  void printCPPDefTokInfos (std::ostream & os, const std::string & _svcName) const;
  void printCPPDecCliSvcMemberFunc (std::ostream & os) const;
  void printCPPDefCliSvcMemberFunc (std::ostream & os, const std::string & classNam) const;
  void printJDefSvc (std::ostream &os) const;
  void printJDefSvcInt (std::ostream &os) const;
  void printJDefSvcImp (std::ostream &os) const;
  void printJDefEnv (std::ostream &os, const std::vector<bool> & flagMatched,
		     const std::vector<std::vector<MsgSpec>::size_type> & idxMatched,
		     const std::vector<EnvArg> & envArgs) const;
};

class SvcFuncP3V : public SvcFunc {
public:
  SvcFuncP3V (const MsgSpec & _ms1, const MsgSpec & _ms2, const MsgSpec &_ms3) { 
    msgSpecs.push_back (_ms1); 
    msgSpecs.push_back (_ms2); 
    msgSpecs.push_back (_ms3); 
  }
  void setMsgIDs (const std::string & _svcName) {
    msgSpecs[0].setID (MsgSpec::Req, _svcName + "_" + getName() , "MDatSvc_"+_svcName);
    msgSpecs[1].setID (MsgSpec::Dat, _svcName + "_" + getName() + "_response", "MDatSvc_"+_svcName);
    msgSpecs[2].setID (MsgSpec::Fin, "", "MDatSvc_"+_svcName);
  }
  void printCPPDeclaration (std::ostream &os) const;
  void printCPPDecTokInfos (std::ostream & os) const;
  void printCPPDefAddSvcFunc (std::ostream & os, const std::string & _svcName) const;
  void printCPPDefTokInfos (std::ostream & os, const std::string & _svcName) const;
  void printCPPDecCliSvcMemberFunc (std::ostream & os) const;
  void printCPPDefCliSvcMemberFunc (std::ostream & os, const std::string & classNam) const;
  void printJDefSvc (std::ostream &os) const;
  void printJDefSvcInt (std::ostream &os) const;
  void printJDefSvcImp (std::ostream &os) const;
  void printJDefEnv (std::ostream &os, const std::vector<bool> & flagMatched,
		     const std::vector<std::vector<MsgSpec>::size_type> & idxMatched,
		     const std::vector<EnvArg> & envArgs) const;
};

class SvcFuncP4N : public SvcFunc {
public:
  SvcFuncP4N (const MsgSpec & _ms1, const MsgSpec & _ms2, const MsgSpec &_ms3, const MsgSpec & _ms4) { 
    if (!_ms1.mdat_p->isMember(_ms2.vectorSizeName))
      throw msgc_exception(_ms2.vectorSizeName+" is not a member of mdata "+_ms1.mdat_p->getName());
    if (!_ms4.mdat_p->isMember(_ms2.vectorSizeName))
      throw msgc_exception(_ms2.vectorSizeName+" is not a member of mdata "+_ms4.mdat_p->getName());

    msgSpecs.push_back (_ms1); 
    msgSpecs.push_back (_ms2); 
    msgSpecs.push_back (_ms3); 
    msgSpecs.push_back (_ms4); 
  }
  void setMsgIDs (const std::string & _svcName) {
    msgSpecs[0].setID (MsgSpec::Req, _svcName + "_" + getName() , "MDatSvc_"+_svcName);
    msgSpecs[1].setID (MsgSpec::Dat, _svcName + "_" + getName() + "_response", "MDatSvc_"+_svcName);
    msgSpecs[2].setID (MsgSpec::Fin, "", "MDatSvc_"+_svcName);
    msgSpecs[3].setID (MsgSpec::Fin, "", "MDatSvc_"+_svcName);
  }
  void printCPPDeclaration (std::ostream &os) const;
  void printCPPDecTokInfos (std::ostream & os) const;
  void printCPPDefAddSvcFunc (std::ostream & os, const std::string & _svcName) const;
  void printCPPDefTokInfos (std::ostream & os, const std::string & _svcName) const;
  void printCPPDecCliSvcMemberFunc (std::ostream & os) const;
  void printCPPDefCliSvcMemberFunc (std::ostream & os, const std::string & classNam) const;
  void printJDefSvc (std::ostream &os) const;
  void printJDefSvcInt (std::ostream &os) const;
  void printJDefSvcImp (std::ostream &os) const;
  void printJDefEnv (std::ostream &os, const std::vector<bool> & flagMatched,
		     const std::vector<std::vector<MsgSpec>::size_type> & idxMatched,
		     const std::vector<EnvArg> & envArgs) const;
};

class SvcMembers {
public:
  std::vector<SvcFunc *> methods;
  std::vector<EnvTypeInfo::Ptr> envs;
};

// holds instance of " server_mod service name {svc_info} "
class SvcDef : public UserDefinedMsgClass {
  friend class EnvTypeInfo;
  typedef UserDefinedMsgClass Base;
public:
  typedef gen::CountedPtr<SvcDef> Ptr;
private:
  static stringPtrMap<Ptr> store;
  std::string serverName; // module name
  std::vector<SvcFunc *> methods;
  std::vector<EnvTypeInfo::Ptr> envs;
  InclusionInfo incInfo;
  // C++ related
  void setCPPName ();	// sets cppName and the following
  std::string cppCliSvcNam;  // CPP Service Client Class name
  // Java related
  void setJName ();	// sets jName and the following
  std::string jSvcName;	// Java Service Interface Class name
  std::string jSvcImpName;	// Java Service Implementation Class name
protected:
  static bool erase (const std::string & _name) {return store.erase (_name);}
  explicit SvcDef(const std::string & _name, const std::string & _serverName, const SvcMembers & _members);
public:
  static void eraseAll() {
    std::vector<std::string> allNames = store.getKeys();
    for (std::vector<std::string>::const_iterator it=allNames.begin(); it!=allNames.end(); ++it)
      erase (*it);
  }
  static Ptr getNew (const std::string & _name, const std::string & _serverName, const SvcMembers & _members)
    { return Ptr(new SvcDef(_name, _serverName, _members)); }
  virtual bool erase () {return erase(getName());}
  static Ptr get (const std::string & _name) { return store.get (_name); }
  std::string getJSvcName () const {return jSvcName; }
  std::string getJSvcIntName () const {return getJName(); }
  std::string getJSvcImpName () const {return jSvcImpName; }

  void addInclusions (const InclusionInfo & _incInfo) { incInfo = _incInfo; } 

  void printCDeclaration (std::ostream & os) const; // calls the C Dec... methods
  void printCDecMsg (std::ostream & os) const;	// print associated msg_create/parse declarations

  void printCPPDeclaration (std::ostream & os) const; // calls the CPP Dec... methods
  void printCPPDecSvc (std::ostream & os) const;	// print C++ service interface class declaration
  void printCPPDecCliSvc (std::ostream & os) const;	// print C++ service client class declaration
  void printCPPDecEnvs (std::ostream & os) const;	// print C++ declarations for the associated envelope classes

  void printCDefinition (std::ostream & os) const; // calls the C Def... methods
  void printCDefMsg (std::ostream & os) const;	// print associated msg_create/parse definitions

  void printCPPDefinition (std::ostream & os) const; // calls the CPP Def... methods
  void printCPPDefSvc (std::ostream & os) const;	// print C++ service interface class definition
  void printCPPDefCliSvc (std::ostream & os) const;	// print C++ service client class definition
  void printCPPDefEnvs (std::ostream & os) const;	// print C++ definitions for the associated envelope classes

  void printJDefinition (const std::string & javaDir, 
			 const std::string & inFileName) const; // calls the J Def... methods
  void printJDefSvc (const std::string & javaDir, 
		     const std::string & inFileName) const; // print Java service interface class declaration
  void printJDefSvcInt (const std::string & javaDir, 
			const std::string & inFileName) const; // print Java service client class declaration
  void printJDefSvcImp (const std::string & javaDir, 
			const std::string & inFileName) const; // print Java service implementation class declaration
  void printJDefEnvs (const std::string & javaDir, 
			const std::string & inFileName) const; // print Java implementation for assoc. env. classes
};

///////////////////////////////////////////////////////////
// Server related
//

class SvrSvcSpec {
public:
  SvcDef::Ptr svc_p;
  std::string svcDefName;
  SvrSvcSpec (SvcDef::Ptr _svc_p, const std::string & _svcDefName) 
    : svc_p(_svc_p), svcDefName(_svcDefName) {}
};

// holds instance of " server name (id) {svr_info} "
class SvrDef : public UserDefinedMsgClass {
  typedef UserDefinedMsgClass Base;
public:
  typedef gen::CountedPtr<SvrDef> Ptr;
private:
  static stringPtrMap<Ptr> store;
  std::string modID; // module ID
  std::vector<SvrSvcSpec> svcs;
  InclusionInfo incInfo;
  // C++ related
  void setCPPName ();	// sets cppName and the following
  // Java related
  void setJName ();	// sets jName and the following
  std::string jSvrImpName;	// Java Service Implementation Class name
protected:
  static bool erase (const std::string & _name) {return store.erase (_name);}
  explicit SvrDef(const std::string & _name, const std::string & _modID, const std::vector<SvrSvcSpec> _svcs);
public:
  static void eraseAll() {
    std::vector<std::string> allNames = store.getKeys();
    for (std::vector<std::string>::const_iterator it=allNames.begin(); it!=allNames.end(); ++it)
      erase (*it);
  }
  static Ptr getNew (const std::string & _name, const std::string & _modID, const std::vector<SvrSvcSpec> _svcs)
    { return Ptr(new SvrDef(_name, _modID, _svcs)); }
  virtual bool erase () {return erase(getName());}
  static Ptr get (const std::string & _name) { return store.get (_name); }
  std::string getJSvrIntName () const {return getJName(); }
  std::string getJSvrImpName () const {return jSvrImpName; }

  void addInclusions (const InclusionInfo & _incInfo) { incInfo = _incInfo; } 

  void printCDeclaration (std::ostream & os) const; // calls the C Dec... methods
  void printCDecMsg (std::ostream & os) const;	// print associated msg_create/parse declarations
  void printCPPDeclaration (std::ostream & os) const; // calls the CPP Dec... methods

  void printCDefinition (std::ostream & os) const; // calls the C Def... methods
  void printCDefMsg (std::ostream & os) const;	// print associated msg_create/parse definitions
  void printCPPDefinition (std::ostream & os) const; // calls the CPP Def... methods

  void printJDefinition (const std::string & javaDir, 
			 const std::string & inFileName) const; // calls the J Def... methods
  void printJDefSvrInt (const std::string & javaDir, 
			const std::string & inFileName) const; // print Java service interface class declaration
  void printJDefSvrImp (const std::string & javaDir, 
			const std::string & inFileName) const; // print Java service implementation class declaration
  void printJDefEnvs (const std::string & javaDir, 
			const std::string & inFileName) const; // print Java implementation for assoc. env. classes
};


///////////////////////////////////////////////////////////
// msgc specific
//

class LineNos {
  int lineno;
  int tok_lineno;
  int last_tok_lineno;
public:
  explicit LineNos () {reset ();}
  void reset () {lineno=1; tok_lineno=1; last_tok_lineno=1;}
  void countEOL () {lineno++;}
  void countTok () {last_tok_lineno = tok_lineno; tok_lineno=lineno;}
  int getLastTokLineNo () {return last_tok_lineno;}
};


class MsgCLexer : public yyFlexLexer {
  friend int yyparse();
  InclusionInfo incInfo;
  bool allow_inclusion_f[InclusionInfo::numIncs];
  std::string incTypeStr;
  InclusionInfo::IncType incType;
  std::stack<yy_buffer_state *> yyin_stack;
  std::stack<LineNos> lineNos_stack;
  LineNos lineNos;
  std::stack<std::string> inDesc_stack;
  std::string inDesc;
  void setIncTypeStr (const std::string & _incTypeStr) {
    if (!incTypeStr.empty())
      throw std::logic_error ("MsgCLexer::setIncTypeStr called while another Inclusion is in effect.");
    incType = InclusionInfo::getIncType(_incTypeStr);
    incTypeStr = _incTypeStr;
  }
  void unsetIncTypeStr () { incTypeStr = "";}
  std::string getIncTypeStr () const {return incTypeStr;}
  const InclusionInfo::IncType & getIncType () const {
    if (incTypeStr.empty()) 
      throw std::logic_error ("MsgCLexer::getIncTypeID called while no Inclusion is in effect.");
    return incType;
  }
public:
  MsgCLexer() {}
  ~MsgCLexer() {}
  int yywrap();
  int yylex();
  void push_yyin (std::istream * _is_p, const std::string & _inDesc ="unknown");
  int pop_yyin ();
  bool isNested ();
  const LineNos & getLineNos() {return lineNos;}
  void countEOL () {lineNos.countEOL();}
  void countTok () {lineNos.countTok();}
  void reset ();
  void resetLineNos () {lineNos.reset();}
  void resetInputDescription (const std::string & _inDesc) {inDesc = _inDesc;}
  void printErrorLineNo (std::ostream & os) {
    os << "in " << inDesc << ", on line " << lineNos.getLastTokLineNo() << std::endl;
  }
  void LexerError( const char* msg ) {throw msgc_exception(msg);}
  void beginInclusion (const std::string & _yystr);
  void saveInclusion (const std::string & _yystr);
  void endInclusion (const std::string & _yystr, std::ostream * osFile_p =0);

  void allowInclusion (InclusionInfo::IncType _incType) {
    if (!incInfo[_incType].empty())
      throw std::logic_error ("MsgCLexer::allowInclusion() called for non empty inclusion type.");
    allow_inclusion_f[_incType] = true;
  }
  void disallowInclusion (InclusionInfo::IncType _incType) {allow_inclusion_f[_incType] = false;}
  void clearInclusion (InclusionInfo::IncType _incType) {incInfo[_incType].clear();}
  void printInclusion (std::ostream & os, InclusionInfo::IncType _incType) const {incInfo.printInclusion(os, _incType);}
};

struct MsgCGlobInfo {

  MsgCLexer lexer;
  int debug_level;

  MDataTypeInfo::Ptr	mdat_p;
  MDataMemberInfo::Ptr	mdat_member_p;
  FlagTypeInfo::Ptr	flag_p;
  EnvTypeInfo::Ptr	env_p;
  SvcDef::Ptr		svc_p;
  SvrDef::Ptr		svr_p;

  // parser context info
  std::string svc_nam;	// current svc name
  std::string svc_func_nam;	// current svc name
  int num_msg_specs;

  std::string inFileName;	// input file name
  std::string inFileFullName;	// input file full path name
  std::string inFileGuardNameBase;	// guard macro name based on input file name
  std::string inFileVerStr;	// version string of the inFile
  std::string cOutDir;	// c output directory
  std::string cxxOutDir;// c++ output directory
  std::string jOutDir;	// java output directory 
  std::vector<std::string> incDirs;	// include directories
  std::ostream * osHFile_p;
  std::ostream * osCFile_p;
  std::ostream * osHPPFile_p;
  std::ostream * osCPPFile_p;
  std::ostream * osJLISTFile_p;

  std::set<std::string> mdatafile_set;	// list of all .m files imported
  std::set<std::string> svcfile_set;	// list of all .s files imported

  std::vector<std::string> javafiles;	// list of all .java files generated
  
  // to handle attaching comments to appropriate declarations 
  int pass_comment_f;

};

extern MsgCGlobInfo msgc_glob;

template<class X>
inline void msgc_delete(X * ptr) {
  delete ptr;
  ptr = 0;
}
//#define msgc_delete(X) if (1) {delete X; X=0;} else

int yywrap ();
int yyparse ();
void yyerror (char const * s);
int yylex();


#endif // _MSGC_HPP_
