#ifndef _MAKE_MSGC_cntdptr_hpp_ID_
#define _MAKE_MSGC_cntdptr_hpp_ID_
#if defined(__sgi) && defined(_SGI_COMPILER_VERSION)
#pragma weak _MAKE_MSGC_cntdptr_hpp_id_w = _MAKE_MSGC_cntdptr_hpp_id_
#endif /* __sgi */
static const char* _MAKE_MSGC_cntdptr_hpp_id_ 
#if defined(__GNUC__)
__attribute__ ((used,unused)) 
#endif /* __GNUC__ */
= "@(#)$Id: cntdptr.hpp 55141 2009-08-18 21:17:30Z wguan@syncsort.com $";
#endif

/******************************************************************************

                      Syncsort CLIENT-SERVER BACKUP

                            COPYRIGHT(C) 2000

        Module Name             : gen++

        Component Name          : CountedPtr

        Program Filename        : cntdptr.hpp

        General Description     : CountedPtr and related classes

        System Dependencies     : NONE

        History Information     : Provided by the RCS package in the following.

 * $Log: not supported by cvs2svn $
 * Revision 1.3  2007-03-16 10:06:48-04  wguan
 * T...-269: Adding String Identifier (RCS Keyword) to All Source Code - hhppccpplexyacc
 *
 * Revision 1.2  2000/12/20 23:00:47  urano
 * I...-666: Message Infrastructure in C++
 *
 * Revision 1.1  2000/10/30 19:55:26  urano
 * Initial revision
 *
 *
*****************************************************************************/

#ifndef _CNTDPTR_HPP_
#define _CNTDPTR_HPP_

#include <memory>             // for std::auto_ptr
#include <algorithm>          // for std::swap

// gen::CountedPtr and related classes
//
// Usage:
// derive your object, T from gen::CountedObject like this:
//	class T : public gen::CountedObject {
//	  T(args...);	// prevent construction on stack
//	  T(const & T);
//	public:
//	  struct Ptr : public gen::CountedPtr<T> {
//	    Ptr(T* t=0) : gen::CountedPtr<T> (t) {}
//	    Ptr(args...) : gen::CountedPtr<T> (new T(args...)) {}
//	  };
// 	};

namespace gen {

  class CountedObject {
    friend class CountedPtrBase;
  protected:
    long useCount;	// keeps track of how many CountedPtr objects point to this object
  public:
    CountedObject () : useCount(0) {}
  };
  
  class CountedPtrBase {
  protected:
    long & count (CountedObject * p) {return p->useCount;}
  };

  // T should be a child of CountedObject  
  template<typename T> class CountedPtr : public CountedPtrBase {
  public:
    explicit CountedPtr (T* p =0) : px(p) { if (px) ++count(px); }
    CountedPtr (const CountedPtr & r) throw() : px(r.px) { if (px) ++count(px); }
    ~CountedPtr () { dispose(); }
    
    CountedPtr & operator= (const CountedPtr & r) {
      share(r.px);
      return *this;
    }
    
    // templated member functions
#ifndef SYNC_NO_MEMBER_TEMPLATES
    template<typename Y>
      CountedPtr (const CountedPtr<Y> & r) throw() : px(r.px) { if (px) ++count(px); }
    
    template<typename Y>
      CountedPtr (std::auto_ptr<Y>& r) { 
      px = r.release();
      if (px) ++count(px);
    } 
    
    template<typename Y>
      CountedPtr & operator= (const CountedPtr<Y>& r) {
      share(r.px);
      return *this;
    }
    
    template<typename Y>
      CountedPtr & operator=(std::auto_ptr<Y>& r) {
      // code choice driven by guarantee of "no effect if new throws"
      if (px && (--count(px) == 0)) cp_delete (px);
      px = r.release();
      if (px) ++count(px);
      return *this;
    }
    // end templated member functions
#endif // SYNC_NO_MEMBER_TEMPLATES
    
    void reset(T* p=0) {
      if ( px == p ) return;  // fix: self-assignment safe
      dispose();
      px = p;
      if (px) ++count(px);
    } // reset
    
    T& operator*() throw()  { return *px; }
    T* operator->() throw() { return px; }
    T* get() throw()        { return px; }
    const T& operator*() const throw()  { return *px; }
    const T* operator->() const throw() { return px; }
    const T* get() const throw()        { return px; }
    
#ifdef SYNC_ALLOW_COUNTED_PTR_CONVERSION
    // get() is safer than this automatic conversion
    operator T*() throw()   { return px; } 
    operator const T*() const throw()   { return px; } 
#endif
    
    long getUseCount () const throw() { return px?count(px):0; }
    bool isUnique() const throw()   { return getUseCount() == 1; }
    
    void swap(CountedPtr<T>& other) throw()
      { std::swap(px,other.px); }
    
    // Tasteless as this may seem, making all members public allows member templates
    // to work in the absence of member templates friends. (Matthew Langston)
#ifndef SYNC_NO_MEMBER_TEMPLATE_FRIENDS
  private:
    template<typename Y> friend class CountedPtr;
#endif
    
    T*     px;     // contained pointer
    
    void dispose() { if (px && (--count(px) == 0)) cp_delete (px); }
    void share(T* rpx) {
      if (px != rpx) {
	dispose();
	px = rpx;
	if (px) ++count(px);
      }
    } 
    void cp_delete (T* p) {
      delete p;
    }
    
  }; // CountedPtr
  
} // namespace gen

#endif // _CNTDPTR_HPP_

