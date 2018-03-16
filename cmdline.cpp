#ifndef _MAKE_MSGC_cmdline_cpp_ID_
#define _MAKE_MSGC_cmdline_cpp_ID_
#if defined(__sgi) && defined(_SGI_COMPILER_VERSION)
#pragma weak _MAKE_MSGC_cmdline_cpp_id_w = _MAKE_MSGC_cmdline_cpp_id_
#endif /* __sgi */
static const char* _MAKE_MSGC_cmdline_cpp_id_ 
#if defined(__GNUC__)
__attribute__ ((used,unused)) 
#endif /* __GNUC__ */
= "@(#)$Id: cmdline.cpp 55141 2009-08-18 21:17:30Z wguan@syncsort.com $";
#endif

/******************************************************************************
 
                      Syncsort CLIENT-SERVER BACKUP
 
                            COPYRIGHT(C) 2000
 
        Module Name             : gen++
 
        Component Name          : gen::CmdLine
 
        Program Filename        : cmdline.cpp
 
        General Description     : gen::CmdLine and related classes to parse 
				  the command line
 
        System Dependencies     : NONE
 
        History Information     : Provided by the RCS package in the following.
 
 * $Log: not supported by cvs2svn $
 * Revision 1.8  2007-03-16 10:06:31-04  wguan
 * T...-269: Adding String Identifier (RCS Keyword) to All Source Code - hhppccpplexyacc
 *
 * Revision 1.7  2005/01/12 21:25:23  urano
 * I..2-713: DMM: --KEY incompatibility
 *
 * Revision 1.6  2004/10/19 21:42:58  urano
 * I..2-486: DMM: sorting device servers
 *
 * Revision 1.5  2003/12/10 04:13:43  urano
 * I...-674: Device Management Module
 *
 * Revision 1.4  2001/02/15 16:54:14  urano
 * I...-674: Device Management Module
 *
 * Revision 1.3  2000/12/20 23:00:13  urano
 * I...-666: Message Infrastructure in C++
 *
 * Revision 1.2  2000/11/07 19:17:48  urano
 * I...-666: Message Infrastructure in C++
 *
 * Revision 1.1  2000/10/30 19:55:06  urano
 * Initial revision
 *
 *
*****************************************************************************/
#ifdef _WIN32
#pragma warning(disable:4786)
#endif

#define CLDBG(X)
/* - uncomment the following to turn on debug
#define CLDBG(X) X
#include <iostream> 
*/

#include <cmdline.hpp>

namespace gen {

CmdLine::CmdLine (int argc, char ** argv) 
  : optTerm("--"), stdPrefixes ("-+"), flags(0) {
  cmd = argv[0];
  for (int i=1; i < argc; i++) 
    args.push_back (std::string(argv[i]));
}

void CmdLine::setFlag (Flag _flag) { flags |= 2^_flag; }
void CmdLine::unsetFlag (Flag _flag) { flags &= ~(2^_flag); }
bool CmdLine::isSet (Flag _flag) const { return (0 !=(flags & (2^_flag))); }

// private
std::vector<std::string>::iterator CmdLine::getLastOpt () {
  std::vector<std::string>::iterator last;
  if (getfSequential()) {
    last = args.begin();
    if ( (*last != optTerm) && (last!=args.end()) ) ++last;
  } else {
    for (last=args.begin(); last!=args.end(); ++last)
      if (*last == optTerm) break;
  }
  return last;
}

std::vector<std::string>::const_iterator CmdLine::getLastOpt () const {
  std::vector<std::string>::const_iterator last;
  if (getfSequential()) {
    last = args.begin();
    if (last!=args.end()) ++last;
  } else {
    for (last=args.begin(); last!=args.end(); ++last)
      if (*last == optTerm) break;
  }
  return last;
}

int CmdLine::extractOpt (char _optChar, const std::string & _prefixes) {
  std::vector<std::string>::const_iterator last = getLastOpt();
  for (std::vector<std::string>::iterator it=args.begin(); it!=last; ++it) {
    std::string::size_type prefix_type = _prefixes.find((*it)[0]);
    if (prefix_type != std::string::npos) {
      std::string::size_type pos = it->find (_optChar, 1);
      if (pos == std::string::npos) continue;
      it->erase(pos,1);
      if (it->size()==1) args.erase(it);
      return (int) prefix_type+1;
    }
  }
  return 0;
}

int CmdLine::extractOpt (char _optChar, std::string & _optArg, const std::string & _prefixes,
			   bool _reqArg, const std::string & _separators) {
  std::vector<std::string>::const_iterator last = getLastOpt();
  for (std::vector<std::string>::iterator it=args.begin(); it!=last; ++it) {
    std::string::size_type prefix_type = _prefixes.find((*it)[0]);
    if (prefix_type != std::string::npos) {
      std::string::size_type pos = it->find (_optChar, 1);
      if (pos == std::string::npos) continue;
      std::string optStr = it->substr(pos);
      it->erase(pos);
      if (it->size()==1) args.erase(it);
      optStr.erase(0,1);
      if ((!optStr.empty())) { // inline argument
        if (_separators.find(optStr[0]) != std::string::npos) optStr.erase(0,1);
	_optArg = optStr;
	return prefix_type+1;
      }
      std::vector<std::string>::iterator next = it+1;
      if (_reqArg) {
        if (next == last) throw NoArgument();
	_optArg = *next;
	args.erase(next);
	return prefix_type+1;
      }
      if (next == last) return prefix_type+1;
      if (stdPrefixes.find((*next)[0])!=std::string::npos) 
	return prefix_type+1;
      _optArg = *next;
      args.erase(next);
      return prefix_type+1;
    }
  }
  return 0;
}

bool CmdLine::extractOpt (const std::string & _optStr) {
  std::vector<std::string>::const_iterator last = getLastOpt();
  for (std::vector<std::string>::iterator it=args.begin(); it!=last; ++it)
    if (*it == _optStr) {
      args.erase(it);
      CLDBG (std::cout << "CmdLine::extractOpt: parsed " << _optStr << std::endl);
      return true;
    }
  return false;
}

int CmdLine::extractOpt (const std::vector<std::string> & _optStrs) {
  std::vector<std::string>::const_iterator last = getLastOpt();
  for (std::vector<std::string>::iterator it=args.begin(); it!=last; ++it) {
    for (size_t idx = 0; idx < _optStrs.size(); ++idx) {
      if (*it == _optStrs[idx]) {
	args.erase(it);
	return (int) idx;
      }
    }
  }
  return -1;
}

bool CmdLine::extractOpt (const std::string & _optStr, std::string & _optArg, bool _reqArg,
			    const std::string & _separators, bool allowNoSep) {
  std::string separators = _separators;

  std::vector<std::string>::const_iterator last = getLastOpt();
  for (std::vector<std::string>::iterator it=args.begin(); it!=last; ++it)
    if (it->find(_optStr) == 0) {
      std::string optArg = it->substr(_optStr.size());
      if (!optArg.empty()) { // inline argument
	std::string::size_type sepPos = separators.find(optArg[0]);
	if (sepPos==std::string::npos) {
	  if (!allowNoSep) return false;
	} else
	  optArg.erase(sepPos,1);
	_optArg = optArg;
	args.erase(it);
	CLDBG (std::cout << "CmdLine::extractOpt: parsed " << _optStr << " " << _optArg << std::endl);
	return true;
      }
      std::vector<std::string>::iterator next = args.erase(it);
      if (_reqArg) {
        if (next == last) throw NoArgument();
	_optArg = *next;
	args.erase(next);
	CLDBG (std::cout << "CmdLine::extractOpt: parsed " << _optStr << " " << _optArg << std::endl);
	return true;
      }
      if (next == last) {
	CLDBG (std::cout << "CmdLine::extractOpt: parsed " << _optStr << " " << _optArg << std::endl);
	return true;
      }
      if (stdPrefixes.find((*next)[0])!=std::string::npos) return true;
      _optArg = *next;
      args.erase(next);
      CLDBG (std::cout << "CmdLine::extractOpt: parsed " << _optStr << " " << _optArg << std::endl);
      return true;
    }
  return false;
}

std::vector<std::string> CmdLine::getArgs() const {
  std::vector<std::string> _args = args;
  std::vector<std::string>::iterator it;
  for (it=_args.begin(); it<_args.end(); ++it)
    if (*it == optTerm) {
      _args.erase (it);
      break;
    }
  return _args;
}

bool CmdLine::hasOpt () const {
  std::vector<std::string>::const_iterator it;
  for (it=args.begin(); it!= args.end(); ++it) {
    if (stdPrefixes.find((*it)[0]) != std::string::npos)
      return true;
  }
  return false;
}

std::vector<std::string> CmdLine::extractOpts () {
  std::vector<std::string>::iterator last = getLastOpt();
  if (last != args.end()) {
    std::vector<std::string>::iterator begin = args.begin();
    std::vector<std::string> opts(begin,last); 
    args.erase(begin,last);
    return opts;
  }
  std::vector<std::string> opts;
  std::vector<std::string>::iterator it;
  for (it=args.begin(); it!= args.end();) {
    if (stdPrefixes.find((*it)[0]) != std::string::npos) {
      opts.push_back (*it);
      it = args.erase(it);
    } else ++it;
  }
  return opts;
}

} // namespace gen

