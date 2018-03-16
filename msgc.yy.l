%{

#ifndef _MAKE_MSGC_msgc_yy_l_ID_
#define _MAKE_MSGC_msgc_yy_l_ID_
#if defined(__sgi) && defined(_SGI_COMPILER_VERSION)
#pragma weak _MAKE_MSGC_msgc_yy_l_id_w = _MAKE_MSGC_msgc_yy_l_id_
#endif /* __sgi */
static const char* _MAKE_MSGC_msgc_yy_l_id_ 
#if defined(__GNUC__)
__attribute__ ((used,unused)) 
#endif /* __GNUC__ */
= "@(#)$Id: msgc.yy.l 55141 2009-08-18 21:17:30Z wguan@syncsort.com $";
#endif

/******************************************************************************

                      Syncsort CLIENT-SERVER BACKUP

                            COPYRIGHT(C) 2000

        Module Name             : msgc

        Component Name          : DevTools

        Program Filename        : msgc.l

        General Description     : Message Compiler

        System Dependencies     : NONE

        History Information     : Provided by the RCS package in the following.

 * $Log: msgc.yy.l,v $
 * Revision 1.8  2006/11/06 22:23:09  urano
 * I..4-204: msgc: verstr directive
 *
 * Revision 1.7  2005/03/14 20:52:23  urano
 * I..2-820: BEXIDL: hidden qualifier for a mdata member
 *
 * Revision 1.6  2004/02/09 16:14:31  urano
 * I..1-917: BEXIDL : clear cache between 'translation units'
 *
 * Revision 1.5  2003/05/06 20:10:21  urano
 * I..1-613: BEXIDL : format specification for mdata
 *
 * Revision 1.4  2002/10/08 14:19:10  urano
 * I...-674: Device Management Module
 *
 * Revision 1.3  2002/06/12 19:41:41  urano
 * I...-674: Device Management Module
 *
 * Revision 1.2  2001/09/28 21:30:25  urano
 * I..1-069: msg_flag autogeneration in msgc
 *
 * Revision 1.1  2001/09/20 16:34:06  urano
 * Initial revision
 *
 *
*****************************************************************************/

#define YYDEBUG 1

#include <string.h>
#include <msgc.hpp>
#include <msgc.tab.h>

#include <string>
using std::string;

#include <vector>
using std::vector;

%}

%s INCL
%s S_COMMENT

%option c++
%option debug
%option yyclass="MsgCLexer"

%%

<S_COMMENT>[^*\n]*        /* eat anything that's not a '*' */
<S_COMMENT>"*"+[^*/\n]*   /* eat up '*'s not followed by '/'s */
<S_COMMENT>\n             countEOL();
<S_COMMENT>"*"+"/"        BEGIN(0);

<INCL>^[\t ]*\%\}[\t ]*\n { BEGIN 0; 
        InclusionInfo::IncType incType;
        incType = msgc_glob.lexer.getIncType();
        switch (incType) {
          case (InclusionInfo::INC_H): {
            msgc_glob.lexer.endInclusion (YYText(), msgc_glob.osHFile_p);
            break;
          }
          case (InclusionInfo::INC_C): {
            msgc_glob.lexer.endInclusion (YYText(), msgc_glob.osCFile_p);
            break;
          }
          case (InclusionInfo::INC_HPP): {
            msgc_glob.lexer.endInclusion (YYText(), msgc_glob.osHPPFile_p);
            break;
          }
          case (InclusionInfo::INC_CPP): {
            msgc_glob.lexer.endInclusion (YYText(), msgc_glob.osCPPFile_p);
            break;
          }
          default: {
            msgc_glob.lexer.endInclusion (YYText());
            break;
          }
        }
        msgc_glob.lexer.clearInclusion (incType);
    	countEOL();
      };

<INCL>^\n { countEOL(); msgc_glob.lexer.saveInclusion (""); };

<INCL>^.*$ { msgc_glob.lexer.saveInclusion (YYText()); };


^[\t ]*\%INC[\t ]*\([\t a-zA-Z0-9_\.\,]+\)[\t ]*\{[\t ]*\n { BEGIN INCL; 
        msgc_glob.lexer.beginInclusion (YYText());
    	countEOL();
      };

\/\/.*$ { 
	if (msgc_glob.pass_comment_f) { 
		yylval.m_p_string = new std::string(&YYText()[2]); 
		return (COMMENT); 
	} 
      };


[\t ]+	/* ignore whitespace */ ;

"/*" { BEGIN(S_COMMENT); };


keyword_tab { countTok(); return (KEYWORD); }
control { countTok(); return (CONTROL); }
inner { countTok(); return (INNER); }
link { countTok(); return (LINK); }
visual { countTok(); return (VISUAL); }
format { countTok(); return (FORMAT); }
mdata { countTok(); return (MDATA); }
comprises { countTok(); return (COMPRISES); }
const { countTok(); return (CONST); }
hidden { countTok(); return (HIDDEN); }
mdatafile { countTok(); return (IMPORT); }
verstr { countTok(); return (VERSTR); }
\%import { countTok(); return (IMPORT); }
service { countTok(); return (SVC); }
server { countTok(); return (SVR); }
envelope { countTok(); return (ENVELOPE); }
throws { countTok(); return (THROWS); }
catches { countTok(); return (CATCHES); }

\'.*[^\\]\' {
	/* character const */
	yylval.m_p_string = new std::string(YYText()); countTok(); return (QCHAR);
}

[-+]?([0-9]+\.[0-9]*|[0-9]*\.[0-9]+)([eE][-+]?[0-9]+)? {
	/* float_dec */
	yylval.m_p_string = new std::string(YYText()); countTok(); return (FLOAT);
}

[-+]?[0-9]+[eE][-+]?[0-9]+ {
	/* float_exp */
	yylval.m_p_string = new std::string(YYText()); countTok(); return (FLOAT);
}

([-+])?0[xX]([0-9a-fA-F]*) {
	/* hex integer */
	yylval.m_p_string = new std::string(YYText()); countTok(); return (INTEGER);
}

[-+]?0[0-7]* {
	/* oct integer */
	yylval.m_p_string = new std::string(YYText()); countTok(); return (INTEGER);
}

[-+]?[1-9][0-9]* {
	/* dec integer */
	yylval.m_p_string = new std::string(YYText()); countTok(); return (INTEGER);
}

\".*[^\\]\" {
	/* quoted string */
	yylval.m_p_string = new std::string(YYText()); countTok(); return (QSTRING);
}

[a-zA-Z0-9_\.]+ { 
	/* identifier */
	yylval.m_p_string = new std::string(YYText()); countTok(); return (LITERAL);
}


\n { countEOL(); }

. { return YYText()[0] ; }

%%

void MsgCLexer::push_yyin (std::istream * _is_p, const std::string & _inDesc) {
  yyin_stack.push (YY_CURRENT_BUFFER);
  lineNos_stack.push (lineNos);
  inDesc_stack.push (inDesc);
  yy_switch_to_buffer (yy_new_buffer (_is_p, YY_BUF_SIZE));
  lineNos.reset();
  inDesc = _inDesc;
}

int MsgCLexer::pop_yyin () {
  if (yyin_stack.empty())
    return 1;
  yy_delete_buffer (YY_CURRENT_BUFFER);
  yy_switch_to_buffer (yyin_stack.top ());
  yyin_stack.pop ();
  lineNos = lineNos_stack.top();
  lineNos_stack.pop();
  inDesc = inDesc_stack.top();
  inDesc_stack.pop();
  return 0;
}

bool MsgCLexer::isNested () {
  return !(yyin_stack.empty());
}

void MsgCLexer::reset () {
  while (isNested())
    pop_yyin();
  resetLineNos();
  for (int i=0; i<InclusionInfo::numIncs; i++)
    {
      incInfo[(InclusionInfo::IncType) i].clear();
      allow_inclusion_f[i] = false;
    }
}

void MsgCLexer::beginInclusion (const std::string & _yystr) {
  std::string::size_type idx1 = _yystr.find('(');
  std::string::size_type idx2 = _yystr.find(')');
  if (idx1 == std::string::npos || idx2 == std::string::npos)
    throw std::logic_error ("MsgCLexer::beginInclusion called with badly formatted yytext: " + _yystr);
  if (!allow_inclusion_f[InclusionInfo::getIncType(_yystr.substr (idx1+1, idx2-idx1-1))])
    throw msgc_exception ("Unexpected inclusion specification.");
  setIncTypeStr (_yystr.substr (idx1+1, idx2-idx1-1));
  if (!isNested()) {
    incInfo[getIncType()].push_back ("/* Begin INC(" + getIncTypeStr() + ") */");
  }

#if 0
  std::string::size_type idx1 = _yystr.find('(');
  std::string::size_type idx2 = _yystr.find(')');
  if (idx1 == std::string::npos || idx2 == std::string::npos)
    throw std::logic_error ("MsgCLexer::beginInclusion called with badly formatted yytext: " + _yystr);
  std::string incTypeStr = _yystr.substr (idx1+1, idx2-idx1-1);
  {
    std::string::size_type i1 = 0;
    std::string::size_type i2 = incTypeStr.find(',');
    while (i2 != std::string::npos) {
      if (!allow_inclusion_f[InclusionInfo::getIncType(incTypeStr.substr (i1, i2-i1))])
        throw msgc_exception ("Unexpected inclusion specification.");
      i1 = i2+1;
      i2 = incTypeStr.find(',',i1);
    }
    if (!allow_inclusion_f[InclusionInfo::getIncType(incTypeStr.substr (i1))])
      throw msgc_exception ("Unexpected inclusion specification.");
  }
  setIncTypeStr (incTypeStr);
  incInfo[getIncType()].push_back ("/* Begin INC(" + getIncTypeStr() + ") */");
#endif

}

void MsgCLexer::saveInclusion (const std::string & _yystr) {
  if (!allow_inclusion_f)
    throw std::logic_error ("MsgCLexer::saveInclusion called and allow_inclusion flag is off.");
  if (!isNested()) {
    incInfo[getIncType()].push_back (_yystr);
  }
}

void MsgCLexer::endInclusion (const std::string & _yystr, std::ostream * osFile_p) {
  if (!allow_inclusion_f)
    throw std::logic_error ("MsgCLexer::endInclusion called and allow_inclusion flag is off.");
  if (!isNested()) {
    incInfo[getIncType()].push_back ("/* End INC(" + getIncTypeStr() + ") */");
    if (osFile_p) {
      printInclusion (*osFile_p, getIncType());
      *osFile_p << std::endl;
    }
  }
  unsetIncTypeStr ();
}

int MsgCLexer::yywrap() {
  return yyFlexLexer::yywrap();
}

