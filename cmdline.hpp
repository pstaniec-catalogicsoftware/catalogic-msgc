#ifndef _MAKE_MSGC_cmdline_hpp_ID_
#define _MAKE_MSGC_cmdline_hpp_ID_
#if defined(__sgi) && defined(_SGI_COMPILER_VERSION)
#pragma weak _MAKE_MSGC_cmdline_hpp_id_w = _MAKE_MSGC_cmdline_hpp_id_
#endif /* __sgi */
static const char* _MAKE_MSGC_cmdline_hpp_id_ 
#if defined(__GNUC__)
__attribute__ ((used,unused)) 
#endif /* __GNUC__ */
= "@(#)$Id: cmdline.hpp 55141 2009-08-18 21:17:30Z wguan@syncsort.com $";
#endif

/******************************************************************************
 
                      Syncsort CLIENT-SERVER BACKUP
 
                            COPYRIGHT(C) 2000
 
        Module Name             : gen++
 
        Component Name          : gen::CmdLine
 
        Program Filename        : cmdline.hpp
 
        General Description     : gen::CmdLine and related classes to parse 
				  the command line
 
        System Dependencies     : NONE
 
        History Information     : Provided by the RCS package in the following.
 
 * $Log: not supported by cvs2svn $
 * Revision 1.6  2007-03-16 10:06:35-04  wguan
 * T...-269: Adding String Identifier (RCS Keyword) to All Source Code - hhppccpplexyacc
 *
 * Revision 1.5  2004/10/19 21:42:55  urano
 * I..2-486: DMM: sorting device servers
 *
 * Revision 1.4  2003/12/10 04:13:46  urano
 * I...-674: Device Management Module
 *
 * Revision 1.3  2002/09/25 21:54:41  urano
 * I...-674: compile under gcc3.0
 *
 * Revision 1.2  2000/12/20 23:00:32  urano
 * I...-666: Message Infrastructure in C++
 *
 * Revision 1.1  2000/10/30 19:55:09  urano
 * Initial revision
 *
 *
*****************************************************************************/

#ifndef _CMDLINE_HPP_
#define _CMDLINE_HPP_

// libstdc++ includes
#include <string>
#include <vector>

//
// command line argument processing utility class
//

namespace gen {

  class CmdLine {
  public:
    // thrown if an argument is required, but not found
    struct NoArgument : public std::exception {
      virtual ~NoArgument() throw() {}
      virtual const char * what() throw() {return "CmdLine::NoArgument";}
    };

    CmdLine (int argc, char ** argv);

    // Returns the zero'th argv element.
    std::string getCmd() {return cmd;}

    // Sets/unsets the "option terminating" argv element (default "--".)  None of the argv 
    // element(s) following this element is considered as a candidate for an option
    // or an option argument.
    void setOptTerminator (const std::string & _optTerm ="--") {optTerm = _optTerm;}
    void unsetOptTerminator () {optTerm = "";}

    // Sets/gets the "standard" option prefixes.  If an option has an optional argument
    // and the argv element in the position to be that argument has a standard prefix,
    // then the option is considered to not have an argument.
    void setStdPrefixes (const std::string & _prefixes ="-+") {stdPrefixes = _prefixes;}
    std::string getStdPrefixes () {return stdPrefixes; }

    // Treats any argv element starting with one of the prefixes to be single letter
    // options grouped together.   E.g., -abc = -a -b -c.   Returns 0 if not found.
    // Otherwise, returns the position+1 in _prefixes of the type of prefix found.
    // The option character is removed from the internal data.
    int extractOpt (char _optChar, const std::string & _prefixes = "-"); 

    // Same as above, except the characters following the _optChar (if any) are treated
    // as an argument, possibly separated by a separator char.  E.g.,if _optchar =b, then
    // -abc will return c as the option argument.  The option char and arg are removed.
    int extractOpt (char _optChar, std::string & _optArg, const std::string & _prefixes = "-",
		    bool _reqArg =false, const std::string & _separators ="=");

    // Here, the prefix must be part of the argument.  E.g., extractOpt ("+s"), extractOpt ("-server"),
    // extractOpt ("--server"), etc.  Returns true, if found.  The option is removed from the
    // internal data.
    bool extractOpt (const std::string & _optStr);

    // just as above, except the function returns -1 for none found, index into the
    // _optStrs vector if a match is found.
    int extractOpt (const std::vector<std::string> & _optStrs);

    // Same as above, where an argumen may be specified in the argv separated by a separator
    // char, or specified in the following argv element.  If allowNoSep is not false (default),
    // a separator is necessary; so, extractOpt ("-a", arg) will not match -ablah.   The option
    // is removed from the internal data.  Note that _optArg value is not touched unless
    // the option and its argument are found.
    bool extractOpt (const std::string & _optStr, std::string & _optArg, 
		     bool _reqArg =false, const std::string & _separators ="=",
		     bool allowNoSep =false);

    // true if first option on cmdline is an option
    bool hasOpt () const;

    // Remove and retrieve all the remaining argv elements (preceding the optTerm element) which
    // has a standard prefix.
    std::vector<std::string> extractOpts ();

    // Returns all the arguments left over.  (Right after construction, this is
    // everything after the zero'th argument, minus the optTerm element.)  This does _not_
    // remove any elements from the internal data.
    std::vector<std::string> getArgs() const;

    // Sets the mode so that extractOpt()'s examine only the front-most argv entries.
    // This mode can be used to emulate the traditional UNIX getopt/getopts processing.
    void setfSequential () {setFlag (SEQUENTIAL);}
    void unsetfSequential () {unsetFlag (SEQUENTIAL);}
    bool getfSequential () const {return isSet (SEQUENTIAL);}

  private:
    std::vector<std::string>::iterator getLastOpt ();
    std::vector<std::string>::const_iterator getLastOpt () const;

    enum Flag {SEQUENTIAL};
    void setFlag (Flag _flag);
    void unsetFlag (Flag _flag);
    bool isSet (Flag _flag) const;

    std::string cmd;
    std::vector<std::string> args;
    std::string optTerm;
    std::string stdPrefixes;
    unsigned int flags;
  };

}
#endif /* _CMDLINE_HPP_ */
