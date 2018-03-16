%{

#ifndef _MAKE_MSGC_msgc_tab_l_ID_
#define _MAKE_MSGC_msgc_tab_l_ID_
#if defined(__sgi) && defined(_SGI_COMPILER_VERSION)
#pragma weak _MAKE_MSGC_msgc_tab_l_id_w = _MAKE_MSGC_msgc_tab_l_id_
#endif /* __sgi */
static const char* _MAKE_MSGC_msgc_tab_l_id_ 
#if defined(__GNUC__)
__attribute__ ((used,unused)) 
#endif /* __GNUC__ */
= "@(#)$Id: msgc.tab.y 55141 2009-08-18 21:17:30Z wguan@syncsort.com $";
#endif

/******************************************************************************

                      Syncsort CLIENT-SERVER BACKUP

                            COPYRIGHT(C) 2000

        Module Name             : msgc

        Component Name          : DevTools

        Program Filename        : msgc.y

        General Description     : Message Compiler

        System Dependencies     : NONE

        History Information     : Provided by the RCS package in the following.

 * $Log: msgc.tab.y,v $
 * Revision 1.12  2007/03/29 21:02:35  wguan
 * I..4-524: BEXIDL: verstr is sometimes optimized out in C/C++
 *
 * Revision 1.11  2007/01/30 22:19:57  urano
 * I..4-415: BEXIDL: verstr is sometimes optimized out in C/C++
 *
 * Revision 1.10  2006/11/08 14:57:06  urano
 * I..4-204: remove 'const' from the identifiers so compiler won't optimize it out
 *
 * Revision 1.9  2006/11/06 22:23:05  urano
 * I..4-204: msgc: verstr directive
 *
 * Revision 1.8  2005/03/14 20:52:26  urano
 * I..2-820: BEXIDL: hidden qualifier for a mdata member
 *
 * Revision 1.7  2003/05/06 20:10:23  urano
 * I..1-613: BEXIDL : format specification for mdata
 *
 * Revision 1.6  2003/01/08 18:27:42  urano
 * I...-666: rstring_sy
 *
 * Revision 1.5  2002/11/06 22:25:29  urano
 * I..1-412: Improving compile time for BEXIDL C++ stubs
 *
 * Revision 1.4  2002/10/08 14:19:07  urano
 * I...-674: Device Management Module
 *
 * Revision 1.3  2002/06/12 19:41:37  urano
 * I...-674: Device Management Module
 *
 * Revision 1.2  2001/09/28 21:30:11  urano
 * I..1-069: msg_flag autogeneration in msgc
 *
 * Revision 1.1  2001/09/20 16:34:01  urano
 * Initial revision
 *
 *
*****************************************************************************/

#include <msgc.hpp>
#include <filename.hpp>

#include <fstream>
using std::ifstream;
using std::endl;
#include <string>
using std::string;
#include <strstream>
using std::istrstream;
using std::strstream;

#ifdef YYDEBUG
#include <stdio.h>
#endif

MsgCGlobInfo msgc_glob; 

%}

%union {
  int  m_int;
  char m_char;
  std::string * m_p_string;
  EnvArg * m_p_EnvArg;
  EnvMethodDef * m_p_EnvMethodDef;
  MDataTypeInfo::Component m_Component;
  MDataMemberInfo * m_p_MDataMemberInfo;
  MDataQualifier * m_p_MDataQualifier;
  MDataVisualInfo * m_p_MDataVisualInfo;
  MsgSpec * m_p_MsgSpec;
  SvcFunc * m_p_SvcFunc;
  SvcMembers * m_p_SvcMembers;
  SvrSvcSpec * m_p_SvrSvcSpec;
  TypeQualifier * m_p_TypeQualifier;
  std::vector<std::string> * m_p_v_string;
  std::vector<EnvArg> * m_p_v_EnvArg;
  std::vector<EnvMethodDef> * m_p_v_EnvMethodDef;
  std::vector<MDataMemberInfo> * m_p_v_MDataMemberInfo;
  std::vector<SvrSvcSpec> * m_p_v_SvrSvcSpec;
  std::vector<TypeQualifier> * m_p_v_TypeQualifier;
}

%token KEYWORD
%token CONTROL
%token INNER
%token LINK
%token VISUAL
%token FORMAT
%token MDATA 
%token CONST
%token HIDDEN
%token COMPRISES
%token VERSTR
%token IMPORT
%token ENVELOPE
%token <m_p_string> QCHAR
%token <m_p_string> FLOAT
%token <m_p_string> INTEGER
%token <m_p_string> QSTRING
%token <m_p_string> LITERAL
%token <m_p_string> COMMENT 
%token SVC
%token SVR
%token THROWS
%token CATCHES
%token END 0 "end of file"

%type <m_p_string> keyword_name
%type <m_int> opt_keyword_value
%type <m_p_MDataQualifier> opt_mdata_qualifier_list
%type <m_p_string> qual_control
%type <m_p_string> qual_link
%type <m_p_string> qual_visual
%type <m_p_string> qual_format
%type <m_p_string> qstring_list 
%type <m_p_string> mdata_name
%type <m_Component> opt_mdata_comprisals
%type <m_Component> mdata_comprisal_list
%type <m_Component> mdata_comprisal
%type <m_int> opt_const
%type <m_int> opt_hidden
%type <m_p_string> mdata_member_type_info
%type <m_p_string> opt_type_param
%type <m_p_v_TypeQualifier> opt_type_qual_list
%type <m_p_TypeQualifier> type_qualifier
%type <m_p_TypeQualifier> type_qual_ptr
%type <m_p_TypeQualifier> type_qual_fixed_array
%type <m_p_TypeQualifier> type_qual_var_array
%type <m_p_string> opt_var_array_size_name
%type <m_p_string> mdata_member_name
%type <m_p_string> opt_mdata_member_def_val
%type <m_p_string> comment
%type <m_p_string> svc_mod_name
%type <m_p_string> svc_name
%type <m_p_string> svc_func_name
%type <m_p_string> opt_keyword
%type <m_p_string> method_qual_fixed_array
%type <m_p_v_string> comment_list
%type <m_p_MDataMemberInfo> mdata_member
%type <m_p_v_MDataMemberInfo> mdata_member_list
%type <m_p_SvcMembers> svc_members
%type <m_p_SvcFunc> svc_func
%type <m_p_SvcFunc> svc_func_sig
%type <m_p_SvcFunc> p1_spec
%type <m_p_SvcFunc> p2_spec
%type <m_p_SvcFunc> p3_spec
%type <m_p_SvcFunc> p4_spec
%type <m_p_MsgSpec> msg_spec
%type <m_p_string> svc_func_arg_mdata
%type <m_p_string> svc_func_arg_mdata_name
%type <m_p_MsgSpec> ctl_msg_spec
%type <m_p_MsgSpec> vector_msg_spec
%type <m_p_MsgSpec> fixed_vector_msg_spec
%type <m_p_v_MDataMemberInfo> opt_mdata_def
%type <m_p_MDataVisualInfo> opt_mdata_member_visual_info
%type <m_p_MDataVisualInfo> opt_mdata_member_vuri_vfunc
%type <m_p_MDataVisualInfo> opt_mdata_member_vfunc
%type <m_p_string> env_desc
%type <m_p_v_EnvMethodDef> opt_env_method_list
%type <m_p_v_EnvMethodDef> env_method_list
%type <m_p_EnvMethodDef> env_method_desc
%type <m_p_string> env_method_name
%type <m_p_v_EnvArg> env_method_arg_list
%type <m_p_EnvArg> env_method_arg_spec
%type <m_p_EnvArg> env_method_arg_root_mdat
%type <m_p_EnvArg> env_method_arg_mdat
%type <m_int> opt_method_qual_array
%type <m_p_string> svr_name
%type <m_p_string> svr_mod_id
%type <m_p_v_SvrSvcSpec> svr_svc_spec_list
%type <m_p_SvrSvcSpec> svr_svc_spec
%type <m_p_string> svr_svc_id
%type <m_p_string> svr_svc_name

%%

/* msg_file_cmd_list : | msg_file_cmd_list msg_file_cmd_block ; */
msg_file_cmd_list : msg_file_cmd_block msg_file_cmd_list | msg_file_cmd_block ;

msg_file_cmd_block : close_inclusion msg_file_cmd open_inclusion;

msg_file_cmd : verstr_dec | mfile_dec | mdata_def | svc_def | svr_def | keyword_def ;

open_inclusion : 
	{
	  msgc_glob.lexer.allowInclusion (InclusionInfo::INC_H);
	  msgc_glob.lexer.allowInclusion (InclusionInfo::INC_CPP);
	} ;

close_inclusion : 
	{
	  msgc_glob.lexer.disallowInclusion (InclusionInfo::INC_CPP);
	  msgc_glob.lexer.disallowInclusion (InclusionInfo::INC_H);
/*
	  if (msgc_glob.osHFile_p) {
            msgc_glob.lexer.printInclusion (*msgc_glob.osHFile_p, InclusionInfo::INC_H);
	    *msgc_glob.osHFile_p << endl;
          }
	  msgc_glob.lexer.clearInclusion (InclusionInfo::INC_H);
*/
	} ;

verstr_dec :  VERSTR QSTRING 
        {
		if (!msgc_glob.lexer.isNested()) 
		{
			msgc_glob.inFileVerStr = *$2;
			if (msgc_glob.osHFile_p) {
				*msgc_glob.osHFile_p << "#if defined(__sgi) && defined(_SGI_COMPILER_VERSION)\n"
				                     << "#pragma weak _" << msgc_glob.inFileGuardNameBase << "_h_id_w = " 
				                     << msgc_glob.inFileGuardNameBase << "_h_id_\n" 
				                     << "#endif /* __sgi */\n"
				                     << "static const char* _" << msgc_glob.inFileGuardNameBase << "_h_id_\n"
				                     << "#ifdef __GNUC__\n"
				                     << "__attribute__ ((used,unused))\n"
				                     << "#endif /* __GNUC__ */" << endl;
				*msgc_glob.osHFile_p << "= " << *$2 << ";" << endl;
				*msgc_glob.osHFile_p << endl;
			}
			if (msgc_glob.osCFile_p) {
				*msgc_glob.osCFile_p << "#if defined(__sgi) && defined(_SGI_COMPILER_VERSION)\n"
				                     << "#pragma weak _" << msgc_glob.inFileGuardNameBase << "_c_id_w = " 
				                     << msgc_glob.inFileGuardNameBase << "_c_id_\n" 
				                     << "#endif /* __sgi */\n"
				                     << "static const char* _" << msgc_glob.inFileGuardNameBase << "_c_id_\n"
				                     << "#ifdef __GNUC__\n"
				                     << "__attribute__ ((used,unused))\n"
				                     << "#endif /* __GNUC__ */" << endl;
				*msgc_glob.osCFile_p << "= " << *$2 << ";" << endl;
				*msgc_glob.osCFile_p << endl;
			}
			if (msgc_glob.osHPPFile_p) {
				*msgc_glob.osHPPFile_p << "#if defined(__sgi) && defined(_SGI_COMPILER_VERSION)\n"
				                     << "#pragma weak _" << msgc_glob.inFileGuardNameBase << "_hpp_id_w = " 
				                     << msgc_glob.inFileGuardNameBase << "_hpp_id_\n" 
				                     << "#endif /* __sgi */\n"
				                     << "static const char* _" << msgc_glob.inFileGuardNameBase << "_hpp_id_\n"
				                     << "#ifdef __GNUC__\n"
				                     << "__attribute__ ((used,unused))\n"
				                     << "#endif /* __GNUC__ */" << endl;
				*msgc_glob.osHPPFile_p << "= " << *$2 << ";" << endl;
				*msgc_glob.osHPPFile_p << endl;
			}
			if (msgc_glob.osCPPFile_p) {
				*msgc_glob.osCPPFile_p << "#if defined(__sgi) && defined(_SGI_COMPILER_VERSION)\n"
				                     << "#pragma weak _" << msgc_glob.inFileGuardNameBase << "_cpp_id_w = " 
				                     << msgc_glob.inFileGuardNameBase << "_cpp_id_\n" 
				                     << "#endif /* __sgi */\n"
				                     << "static const char* _" << msgc_glob.inFileGuardNameBase << "_cpp_id_\n"
				                     << "#ifdef __GNUC__\n"
				                     << "__attribute__ ((used,unused))\n"
				                     << "#endif /* __GNUC__ */" << endl;
				*msgc_glob.osCPPFile_p << "= " << *$2 << ";" << endl;
				*msgc_glob.osCPPFile_p << endl;
			}
		}
		msgc_delete ($2);
        } ;

mfile_dec :  IMPORT QSTRING 
	{
  	  string fileName;
	  fileName = $2->substr(1, $2->size()-2);  // strip off the quotes
	  msgc_delete ($2);
          if ( (msgc_glob.mdatafile_set.find(fileName) == msgc_glob.mdatafile_set.end()) 
	       && (msgc_glob.svcfile_set.find(fileName) == msgc_glob.svcfile_set.end()) )
	    {
              gen::NativeFileName inputFile(msgc_glob.inFileFullName);
              gen::NativeFileName incFile(fileName);
	      if (incFile.getExtension()=="m") 
		msgc_glob.mdatafile_set.insert (fileName);
	      if (incFile.getExtension()=="s") 
		msgc_glob.svcfile_set.insert (fileName);
              if (incFile.getDirectory()!=".")
		throw msgc_exception("The import directive can not take a directory specification: "+fileName+".");
              incFile.setDirectory (inputFile.getDirectory());
	      ifstream * isInc_p = new ifstream(incFile.getFullName().c_str());
              for (std::vector<std::string>::size_type i=0; i<msgc_glob.incDirs.size(); i++) {
	        if (*isInc_p) break;
                incFile.setDirectory (msgc_glob.incDirs[i]);
                delete isInc_p;
                isInc_p = new ifstream(incFile.getFullName().c_str());
              }
	      if (!*isInc_p) {
                delete isInc_p;
		throw msgc_exception("Could not open import file "+incFile.getFullName()+" for read.");
              }
	      msgc_glob.lexer.push_yyin (isInc_p, "included file "+fileName);
	    }
	  if (msgc_glob.osHFile_p) {
	    *msgc_glob.osHFile_p << "#include <" << fileName+".h" << ">" << endl;
	    *msgc_glob.osHFile_p << endl;
          }
	  if (msgc_glob.osHPPFile_p) {
	    *msgc_glob.osHPPFile_p << "#include <" << fileName+".hpp" << ">" << endl;
	    *msgc_glob.osHPPFile_p << endl;
          }
	} ;

keyword_def :
	KEYWORD '{' keyword_entry_list '}'
		{
		  KeywordTab::get().setDone();

		  if (msgc_glob.osHFile_p) {
		    KeywordTab::get().printCDeclaration (*msgc_glob.osHFile_p);
		    *msgc_glob.osHFile_p << endl;
		  }
		  /*
		  if (msgc_glob.osCFile_p) {
		    KeywordTab::get().printCDefinition (*msgc_glob.osCFile_p);
		    *msgc_glob.osCFile_p << endl;
		  }
		  */
		  if (msgc_glob.osHPPFile_p) {
		    KeywordTab::get().printCPPDeclaration (*msgc_glob.osHPPFile_p);
		    *msgc_glob.osHPPFile_p << endl;
		  }
 		  if (msgc_glob.osCPPFile_p) {
 		    KeywordTab::get().printCPPDefinition (*msgc_glob.osCPPFile_p);
 		    *msgc_glob.osCPPFile_p << endl;
 		  }
 		  KeywordTab::get().printJDefinition (msgc_glob.jOutDir, msgc_glob.inFileName);
		};

keyword_entry_list :
	keyword_entry
|	keyword_entry_list ',' keyword_entry 
	;

keyword_entry : 
	keyword_name opt_keyword_value
		{
		  KeywordTab::get().add(*$1, $2);
		  msgc_delete ($1);
		};

keyword_name : LITERAL ;

opt_keyword_value :  
	NULL
	{
	  $$ = -1;
	}
|	'=' INTEGER
	{
	  std::istrstream is($2->c_str());
          is.exceptions (std::ios::badbit | std::ios::failbit);
          int val;
          is >> val;
          $$ = val;
          msgc_delete ($2);
	};


mdata_def : 
	opt_mdata_qualifier_list MDATA mdata_name opt_mdata_comprisals
	'{' mdata_allow_incl mdata_member_list mdata_disallow_incl '}'
		{ 
		  msgc_glob.mdat_p = MDataTypeInfo::getNew (*$3, $1, $4, *$7);
		  msgc_delete ($1);
		  msgc_delete ($3);
		  msgc_delete ($7);
/*
		  msgc_glob.mdat_p->addInclusions (msgc_glob.lexer.incInfo);
		  msgc_glob.lexer.clearInclusion (InclusionInfo::INC_H);
*/
		  if (!msgc_glob.lexer.isNested())
		    {
	              if (msgc_glob.osHFile_p) {
		        msgc_glob.mdat_p->printCDeclaration (*msgc_glob.osHFile_p);
		        *msgc_glob.osHFile_p << endl;
                      }
	              if (msgc_glob.osCFile_p) {
		        msgc_glob.mdat_p->printCDefinition (*msgc_glob.osCFile_p);
		        *msgc_glob.osCFile_p << endl;
                      }
	              if (msgc_glob.osHPPFile_p) {
		        msgc_glob.mdat_p->printCPPDeclaration (*msgc_glob.osHPPFile_p);
		        *msgc_glob.osHPPFile_p << endl;
                      }
	              if (msgc_glob.osCPPFile_p) {
		        msgc_glob.mdat_p->printCPPDefinition (*msgc_glob.osCPPFile_p);
		        *msgc_glob.osCPPFile_p << endl;
                      }
		      msgc_glob.mdat_p->printJDefinition (msgc_glob.jOutDir, msgc_glob.inFileName);
		    }
		};

opt_mdata_qualifier_list :
	NULL
	{ $$ = new MDataQualifier(); }
|	opt_mdata_qualifier_list qual_control
	{ 
	  $$ = $1;
	  $$->addQualifier (MDataTypeInfo::FLAG_IS_CONTROL, *$2); 
	  msgc_delete ($2); 
	}
|	opt_mdata_qualifier_list INNER
	{ 
	  $$ = $1;
	  $$->addQualifier (MDataTypeInfo::FLAG_IS_INNER); 
	}
|	opt_mdata_qualifier_list qual_link
	{ 
	  $$ = $1;
	  $$->addQualifier (MDataTypeInfo::FLAG_IS_LINK, *$2); 
	  msgc_delete ($2); 
	} 
|	opt_mdata_qualifier_list qual_visual
	{ 
	  $$ = $1;
	  $$->addQualifier (MDataTypeInfo::FLAG_IS_VISUAL, *$2); 
	  msgc_delete ($2); 
	} 
|	opt_mdata_qualifier_list qual_format
	{
	  $$ = $1;
	  $$->addQualifier (MDataTypeInfo::FLAG_IS_FORMAT, *$2); 
	  msgc_delete ($2); 
	}
;

qual_control : CONTROL '(' LITERAL ')' { $$ = $3; } ;
qual_link : LINK { $$ = new string(""); } | LINK '(' LITERAL ')' { $$ = $3; } ;
qual_visual : VISUAL '(' LITERAL ')' { $$ = $3; } ;
qual_format : FORMAT '(' qstring_list ')' { $$ = $3; } ;

qstring_list : 
	QSTRING { $$ = $1; }
|	qstring_list QSTRING 
	{
	  $$ = new std::string (*$1 + *$2);
	  msgc_delete ($1);
	  msgc_delete ($2);
	}
;

mdata_name : LITERAL ;

opt_mdata_comprisals : 
	NULL {$$ = MDataTypeInfo::COMP_NONE;} 
|	COMPRISES mdata_comprisal_list {$$=$2;}
;

mdata_comprisal_list :  
	mdata_comprisal { $$ = $1; }
|	mdata_comprisal_list mdata_comprisal { $$ = (MDataTypeInfo::Component) ($1|$2); }
;

mdata_comprisal :
	SVC { $$ = MDataTypeInfo::COMP_SERVICE; }
;

mdata_allow_incl : NULL {msgc_glob.lexer.allowInclusion (InclusionInfo::INC_H); };
mdata_disallow_incl : NULL {msgc_glob.lexer.disallowInclusion (InclusionInfo::INC_H); };

mdata_member_list :
	NULL
		{ 
		  $$ = new std::vector<MDataMemberInfo>; 
		}
|	mdata_member_list mdata_member 
		{ 
		  $$ = $1; 
		  $$->push_back(*$2); 
		  msgc_delete ($2);
		}
;

mdata_member : 
	opt_const opt_hidden mdata_member_type_info opt_type_param opt_type_qual_list opt_mdata_member_visual_info 
	mdata_member_name opt_mdata_member_def_val ';' 
	PASS_COMMENT_BEGIN comment_list PASS_COMMENT_END
		{
	          if (*$3=="string_sy") 
		    {
		      *$3 = "tchar_sy";
		      $5->insert ($5->begin(), TypeQualifierVarArray(*$4, TypeQualifier::C_MSLIST));
		    }
	          else if (*$3=="bstring_sy") 
		    {
		      *$3 = "bchar_sy";
		      $5->insert ($5->begin(), TypeQualifierVarArray(*$4, TypeQualifier::C_NULL));
		    }
	          else if (*$3=="cstring_sy") 
		    {
		      *$3 = "cchar_sy";
		      if (!$4->empty())
		        throw msgc_exception ("Type "+*$3+" does not take a parameter.");
		      $5->insert ($5->begin(), TypeQualifierVarArray(*$4, TypeQualifier::C_MSLIST));
		    }
	          else if (*$3=="rstring_sy") 
		    {
		      *$3 = "rchar_sy";
		      $5->insert ($5->begin(), TypeQualifierVarArray(*$4, TypeQualifier::C_NULL));
		    }
		  else if (!$4->empty())
		    throw msgc_exception ("Type "+*$3+" does not take a parameter.");
		  msgc_delete ($4);
		  $$ = new MDataMemberInfo (*$7, $1, $2, *$3, *$5, *$8, *$6, *$11);
		  msgc_delete ($3);
		  msgc_delete ($5);
		  msgc_delete ($6);
		  msgc_delete ($7);
		  msgc_delete ($8);
		  msgc_delete ($11);
		}
;

opt_const : 
	NULL { $$ = 0; }
|	CONST { $$ = 1; }

opt_hidden : 
	NULL { $$ = 0; }
|	HIDDEN { $$ = 1; }

mdata_member_type_info : LITERAL ;

opt_type_param :
	NULL
	{
	  $$ = new string("");
 	}
|	'<' LITERAL '>'
	{
	  $$ = $2;
	}
;

opt_type_qual_list : 
	NULL 
	{
	  $$ = new std::vector <TypeQualifier>;
	}
|	opt_type_qual_list type_qualifier
	{
	  $$ = $1;
 	  $$->push_back (*$2);
	  msgc_delete ($2);
	}
;

type_qualifier : type_qual_ptr | type_qual_fixed_array | type_qual_var_array ;

type_qual_ptr : '*' {$$ = new TypeQualifierPtr(TypeQualifier::C_MSLIST);} ;

type_qual_fixed_array : 
	'[' LITERAL '/' LITERAL ']' 
	{
	  $$ = new TypeQualifierFixedArray (*$4, *$2);
	  msgc_delete ($4);
	  msgc_delete ($2);
	} ;

type_qual_var_array : '[' opt_var_array_size_name ']' 
	{
	  $$ = new TypeQualifierVarArray (*$2, TypeQualifier::C_MSLIST);
	  msgc_delete ($2);
	}
;

opt_var_array_size_name : 
	NULL	{$$=new string("");}
|	LITERAL {$$=$1;}
;

opt_mdata_member_visual_info : 
	NULL
		{
		  $$ = new MDataVisualInfo ();
		}	
|	'(' LITERAL opt_mdata_member_vuri_vfunc ')' 
		{
		  $$ = $3;
		  $$->vType = *$2;
		  msgc_delete ($2);
		}
;

opt_mdata_member_vuri_vfunc : 	
	NULL
		{
		  $$ = new MDataVisualInfo ();
		}
|	',' QSTRING opt_mdata_member_vfunc 	
		{
		  $$ = $3;
		  $$->vURI = *$2;
		  msgc_delete ($2);
		}		
|	',' opt_mdata_member_vfunc 		
		{
		  $$ = $2;
		}
;

opt_mdata_member_vfunc : 		
	NULL
		{
		  $$ = new MDataVisualInfo ();
		}	
|	',' QSTRING	
		{
		  $$ = new MDataVisualInfo ();
		  $$->vFunc = *$2;
		  msgc_delete ($2);
		}
;

mdata_member_name : LITERAL ;

opt_mdata_member_def_val : 
	'=' QCHAR {$$=$2;} 
|	'=' FLOAT {$$=$2;} 
|	'=' INTEGER {$$=$2;} 
|	'=' QSTRING {$$=$2;} 
|	'=' LITERAL {$$=$2;} 
|	NULL {$$=new string("");} ;

comment_list : 
	NULL
		{ 
		  $$ = new std::vector<string>; 
		}
|	comment_list comment
		{ 
		  $$ = $1; 
		  $$->push_back(*$2); 
		  msgc_delete ($2);
		}
;

comment : COMMENT ;

svc_def : svc_mod_name SVC svc_name '{' svc_allow_incl svc_members svc_disallow_incl '}' 
		{ 
		  // see if an mdata comprises this service
		  try {
		    MDataTypeInfo::Ptr mdat_p;
		    mdat_p = MDataTypeInfo::get (*$3);
		    if (mdat_p->comprisesService()) {
		      // see if user already specified an envelope
		      try {
			EnvTypeInfo::get (*$3);
		      } catch (msgc_exception_not_in_store) {
			std::vector<EnvMethodDef> v_envMD;
			{
			  std::vector<EnvArg> v_envArg;
			  EnvArg envArg(*$3,false);
			  v_envArg.push_back (envArg);
			  EnvMethodDef envMD1 ("*", v_envArg);
			  v_envArg.push_back (envArg);
			  EnvMethodDef envMD2 ("*", v_envArg);
			  v_envMD.push_back (envMD2);
			  v_envMD.push_back (envMD1);
			}
			EnvTypeInfo::Ptr env_p;
			env_p = EnvTypeInfo::getNew (*$3, v_envMD);
			$6->envs.push_back (env_p);
		      }
		    }
		  } catch (msgc_exception_not_in_store) {
		  };
		  msgc_glob.svc_p = SvcDef::getNew (*$3, *$1, *$6);
		  if (!msgc_glob.lexer.isNested())
		    {
/*
		      msgc_glob.svc_p->addInclusions (msgc_glob.lexer.incInfo);
*/
	              if (msgc_glob.osHFile_p) {
		        msgc_glob.svc_p->printCDeclaration (*msgc_glob.osHFile_p);
		        *msgc_glob.osHFile_p << endl;
                      }
	              if (msgc_glob.osCFile_p) {
		        msgc_glob.svc_p->printCDefinition (*msgc_glob.osCFile_p);
		        *msgc_glob.osCFile_p << endl;
                      }
	              if (msgc_glob.osHPPFile_p) {
		        msgc_glob.svc_p->printCPPDeclaration (*msgc_glob.osHPPFile_p);
		        *msgc_glob.osHPPFile_p << endl;
                      }
	              if (msgc_glob.osCPPFile_p) {
		        msgc_glob.svc_p->printCPPDefinition (*msgc_glob.osCPPFile_p);
		        *msgc_glob.osCPPFile_p << endl;
                      }
		      msgc_glob.svc_p->printJDefinition (msgc_glob.jOutDir, msgc_glob.inFileName);
		    }
/*
		  msgc_glob.lexer.clearInclusion (InclusionInfo::INC_H);
*/
		  msgc_delete ($1);
		  msgc_delete ($3);
		  msgc_delete ($6);
		};

svc_mod_name : LITERAL ;

svc_name : LITERAL {msgc_glob.svc_nam = *$1; $$ = $1;} ;

svc_allow_incl : NULL {msgc_glob.lexer.allowInclusion (InclusionInfo::INC_H); };
svc_disallow_incl : NULL {msgc_glob.lexer.disallowInclusion (InclusionInfo::INC_H); };

svc_members :
	svc_func
		{
		  $$ = new SvcMembers;
		  $$->methods.push_back($1);
		}
|	env_desc
		{
		  $$ = new SvcMembers;
		  $$->envs.push_back(EnvTypeInfo::get(*$1));
		  msgc_delete ($1);
		}
|	svc_members svc_func
		{
		  $$ = $1;
		  $$->methods.push_back($2);
		};
|	svc_members env_desc
		{
		  $$ = $1;
		  $$->envs.push_back(EnvTypeInfo::get(*$2));
		  msgc_delete ($2);
		}
;
		
svc_func : 
	svc_func_name svc_func_sig ';' 
	PASS_COMMENT_BEGIN comment_list PASS_COMMENT_END
		{
		  $$ = $2;
		  $$->setName (*$1);
		  $$->comments = *$5;
		  msgc_delete ($1);
		  msgc_delete ($5);
		};

svc_func_name : LITERAL 
	{
	  msgc_glob.svc_func_nam = *$1; 
	  msgc_glob.num_msg_specs = 0; 
	  $$ = $1;
	};

svc_func_sig : p1_spec | p2_spec | p3_spec | p4_spec ;

p1_spec : '(' msg_spec ')' { $$ = new SvcFuncP1 (*$2); msgc_delete ($2);};

p2_spec : '(' msg_spec ',' msg_spec ')'
		{ $$ = new SvcFuncP2 (*$2, *$4); msgc_delete ($2); msgc_delete ($4);}
|	  '(' msg_spec ')' THROWS ctl_msg_spec
		{ $$ = new SvcFuncP2 (*$2, *$5); msgc_delete ($2); msgc_delete ($5);}
;

p3_spec : '(' msg_spec ',' msg_spec ')' THROWS ctl_msg_spec 
		{ $$ = new SvcFuncP3 (*$2, *$4, *$7); msgc_delete ($2); msgc_delete ($4); msgc_delete ($7);}
|	  '(' msg_spec ',' vector_msg_spec ')' THROWS ctl_msg_spec
		{ $$ = new SvcFuncP3V (*$2, *$4, *$7); msgc_delete ($2); msgc_delete ($4); msgc_delete ($7);}
;

p4_spec : '(' msg_spec '/' msg_spec ',' fixed_vector_msg_spec ')' THROWS ctl_msg_spec 
		{ $$ = new SvcFuncP4N (*$2, *$6, *$9, *$4); 
		msgc_delete ($2); msgc_delete ($4); msgc_delete ($6); msgc_delete ($9); }
;

msg_spec : svc_func_arg_mdata opt_keyword
	{ 
	  $$ = new MsgSpec(*$1, *$2); 
	  msgc_delete ($1); 
	  msgc_delete ($2);
	};

svc_func_arg_mdata : svc_func_arg_mdata_name opt_mdata_def 
	{ 
	  if ($2)
	    {
	      msgc_glob.mdat_p = MDataTypeInfo::getNew (*$1, NULL, MDataTypeInfo::COMP_NONE, *$2);
	      msgc_delete ($2);
	      if (!msgc_glob.lexer.isNested())
		{
	          if (msgc_glob.osHFile_p) {
		    msgc_glob.mdat_p->printCDeclaration (*msgc_glob.osHFile_p);
		    *msgc_glob.osHFile_p << endl;
                  }
	          if (msgc_glob.osCFile_p) {
		    msgc_glob.mdat_p->printCDefinition (*msgc_glob.osCFile_p);
		    *msgc_glob.osCFile_p << endl;
                  }
	          if (msgc_glob.osHPPFile_p) {
		    msgc_glob.mdat_p->printCPPDeclaration (*msgc_glob.osHPPFile_p);
		    *msgc_glob.osHPPFile_p << endl;
                  }
	          if (msgc_glob.osCPPFile_p) {
		    msgc_glob.mdat_p->printCPPDefinition (*msgc_glob.osCPPFile_p);
		    *msgc_glob.osCPPFile_p << endl;
                  }
		  msgc_glob.mdat_p->printJDefinition (msgc_glob.jOutDir, msgc_glob.inFileName);
		}
	    }
	  $$ = $1;
	};

svc_func_arg_mdata_name : 
	NULL	
	{
	  msgc_glob.num_msg_specs++;
	  $$ = new string(msgc_glob.svc_nam + "_" + msgc_glob.svc_func_nam);
	  if (msgc_glob.num_msg_specs == 2) *$$ += "_response";
	}
|	mdata_name 
	{
	  msgc_glob.num_msg_specs++;
	  $$ = $1;
	}
;

opt_mdata_def : 
	NULL	{$$=NULL;}
|	'{' mdata_member_list '}' {$$=$2;};
;

ctl_msg_spec : LITERAL
	{ 
	  string ctl_msg_nam (*$1);
	  $$ = new MsgSpec(ctl_msg_nam, ""); 
	  if (!$$->mdat_p->isCtlMsg()) 
	    throw msgc_exception("In service function spec, " + ctl_msg_nam + " is not a control mdata.");
	}

vector_msg_spec : svc_func_arg_mdata opt_keyword method_qual_array
	{ $$ = new MsgSpec(*$1, *$2, true); msgc_delete ($1); msgc_delete($2);};

fixed_vector_msg_spec : svc_func_arg_mdata opt_keyword method_qual_fixed_array
	{ $$ = new MsgSpec(*$1, *$2, *$3); msgc_delete ($1); msgc_delete($2); msgc_delete($3); };

opt_keyword : {$$ = new string("");} | '(' LITERAL ')' {$$ = $2;} ;

method_qual_array : '[' ']' ;

method_qual_fixed_array : '[' LITERAL ']' {$$ = $2;} ;

env_desc : ENVELOPE LITERAL opt_env_method_list ';'
	{
	  EnvTypeInfo::getNew (*$2, *$3);
	  msgc_delete ($3);
	  $$ = $2;
	};

opt_env_method_list : NULL {$$ = 0;} | '(' env_method_list ')' {$$ = $2;} ;

env_method_list : 
	env_method_desc 
	{
	  $$ = new std::vector<EnvMethodDef>;
	  $$->push_back (*$1);
	  msgc_delete ($1);
	}
|	env_method_list ',' env_method_desc
	{
	  $$ = $1;
	  $$->push_back (*$3);
	  msgc_delete ($3);
	}
;

env_method_desc : env_method_name '(' env_method_arg_list ')'
        {
	  $$ = new EnvMethodDef (*$1, *$3);
	  msgc_delete ($1);
	  msgc_delete ($3);
	};

env_method_name : LITERAL { $$ = $1; } | '*' { $$ = new string("*"); } ;

env_method_arg_list : 
	env_method_arg_spec 
	{
	  $$ = new std::vector<EnvArg>;
	  $$->push_back (*$1);
	  msgc_delete ($1);
	}
| 	env_method_arg_list ',' env_method_arg_spec
	{
	  $$ = $1;
	  $$->push_back (*$3);
	  msgc_delete ($3);
	}
;

env_method_arg_spec : env_method_arg_root_mdat | env_method_arg_mdat ;

env_method_arg_root_mdat : LITERAL opt_method_qual_array { $$ = new EnvArg (*$1, $2?true:false); msgc_delete ($1);};

env_method_arg_mdat : LITERAL opt_method_qual_array '(' LITERAL ')' 
	{ 
	  $$ = new EnvArg (*$1, $2?true:false, *$4); 
	  msgc_delete ($1);
	  msgc_delete ($4);
	};

opt_method_qual_array : 
	NULL 			{$$ = 0;}
|	method_qual_array	{$$ = 1;}
;

svr_def : SVR svr_name '(' svr_mod_id ')' '{' svr_svc_spec_list '}' 
	{
	  msgc_glob.svr_p = SvrDef::getNew (*$2, *$4, *$7);
/*
	  msgc_glob.svr_p->addInclusions (msgc_glob.lexer.incInfo);
*/
	  if (!msgc_glob.lexer.isNested())
	    {
	      if (msgc_glob.osHFile_p) {
		msgc_glob.svr_p->printCDeclaration (*msgc_glob.osHFile_p);
		*msgc_glob.osHFile_p << endl;
	      }
	      if (msgc_glob.osCFile_p) {
		msgc_glob.svr_p->printCDefinition (*msgc_glob.osCFile_p);
		*msgc_glob.osCFile_p << endl;
	      }
	      if (msgc_glob.osHPPFile_p) {
		msgc_glob.svr_p->printCPPDeclaration (*msgc_glob.osHPPFile_p);
		*msgc_glob.osHPPFile_p << endl;
	      }
	      if (msgc_glob.osCPPFile_p) {
		msgc_glob.svr_p->printCPPDefinition (*msgc_glob.osCPPFile_p);
		*msgc_glob.osCPPFile_p << endl;
	      }
	      msgc_glob.svr_p->printJDefinition (msgc_glob.jOutDir, msgc_glob.inFileName);
	    }
/*
	  msgc_glob.lexer.clearInclusion (InclusionInfo::INC_H);
*/
	};

svr_name : LITERAL ;
svr_mod_id : LITERAL ;

svr_svc_spec_list : 
	svr_svc_spec 
	{
	  $$ = new std::vector<SvrSvcSpec>;
	  $$->push_back (*$1);
	  msgc_delete ($1);
	}
|
	svr_svc_spec_list svr_svc_spec
	{
	  $$ = $1;
	  $$->push_back (*$2);
	  msgc_delete ($2);
	}
;

svr_svc_spec :
	svr_svc_id svr_svc_name ';'
	{
	  SvcDef::Ptr svc_p;
	  svc_p = SvcDef::get (*$1);
	  $$ = new SvrSvcSpec (svc_p, *$2);
          msgc_delete ($1);
          msgc_delete ($2);
	}
;

svr_svc_id : LITERAL ;
svr_svc_name : LITERAL ;

NULL : ;
PASS_COMMENT_BEGIN : {msgc_glob.pass_comment_f = 1; };
PASS_COMMENT_END : {msgc_glob.pass_comment_f = 0; };

%%

void
yyerror (char const * s)
{
  throw msgc_exception(s);
}

// extern "C" int yywrap();
int yyFlexLexer::yywrap ()
{
  return msgc_glob.lexer.pop_yyin();
}

int
yylex() 
{
  return msgc_glob.lexer.yylex();
}
