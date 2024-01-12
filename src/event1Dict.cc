// Do NOT change. Changes will be lost next time file is generated

#define R__DICTIONARY_FILENAME event1Dict
#define R__NO_DEPRECATION

/*******************************************************************/
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#define G__DICTIONARY
#include "ROOT/RConfig.hxx"
#include "TClass.h"
#include "TDictAttributeMap.h"
#include "TInterpreter.h"
#include "TROOT.h"
#include "TBuffer.h"
#include "TMemberInspector.h"
#include "TInterpreter.h"
#include "TVirtualMutex.h"
#include "TError.h"

#ifndef G__ROOT
#define G__ROOT
#endif

#include "RtypesImp.h"
#include "TIsAProxy.h"
#include "TFileMergeInfo.h"
#include <algorithm>
#include "TCollectionProxyInfo.h"
/*******************************************************************/

#include "TDataMember.h"

// Header files passed as explicit arguments
#include "event1.h"

// Header files passed via #pragma extra_include

// The generated code does not explicitly qualify STL entities
namespace std {} using namespace std;

namespace ROOT {
   static TClass *vec_Dictionary();
   static void vec_TClassManip(TClass*);
   static void *new_vec(void *p = nullptr);
   static void *newArray_vec(Long_t size, void *p);
   static void delete_vec(void *p);
   static void deleteArray_vec(void *p);
   static void destruct_vec(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const ::vec*)
   {
      ::vec *ptr = nullptr;
      static ::TVirtualIsAProxy* isa_proxy = new ::TIsAProxy(typeid(::vec));
      static ::ROOT::TGenericClassInfo 
         instance("vec", "vec.h", 12,
                  typeid(::vec), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &vec_Dictionary, isa_proxy, 4,
                  sizeof(::vec) );
      instance.SetNew(&new_vec);
      instance.SetNewArray(&newArray_vec);
      instance.SetDelete(&delete_vec);
      instance.SetDeleteArray(&deleteArray_vec);
      instance.SetDestructor(&destruct_vec);
      return &instance;
   }
   TGenericClassInfo *GenerateInitInstance(const ::vec*)
   {
      return GenerateInitInstanceLocal(static_cast<::vec*>(nullptr));
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal(static_cast<const ::vec*>(nullptr)); R__UseDummy(_R__UNIQUE_DICT_(Init));

   // Dictionary for non-ClassDef classes
   static TClass *vec_Dictionary() {
      TClass* theClass =::ROOT::GenerateInitInstanceLocal(static_cast<const ::vec*>(nullptr))->GetClass();
      vec_TClassManip(theClass);
   return theClass;
   }

   static void vec_TClassManip(TClass* ){
   }

} // end of namespace ROOT

namespace ROOT {
   static TClass *vect_Dictionary();
   static void vect_TClassManip(TClass*);
   static void *new_vect(void *p = nullptr);
   static void *newArray_vect(Long_t size, void *p);
   static void delete_vect(void *p);
   static void deleteArray_vect(void *p);
   static void destruct_vect(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const ::vect*)
   {
      ::vect *ptr = nullptr;
      static ::TVirtualIsAProxy* isa_proxy = new ::TIsAProxy(typeid(::vect));
      static ::ROOT::TGenericClassInfo 
         instance("vect", "vect.h", 14,
                  typeid(::vect), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &vect_Dictionary, isa_proxy, 4,
                  sizeof(::vect) );
      instance.SetNew(&new_vect);
      instance.SetNewArray(&newArray_vect);
      instance.SetDelete(&delete_vect);
      instance.SetDeleteArray(&deleteArray_vect);
      instance.SetDestructor(&destruct_vect);
      return &instance;
   }
   TGenericClassInfo *GenerateInitInstance(const ::vect*)
   {
      return GenerateInitInstanceLocal(static_cast<::vect*>(nullptr));
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal(static_cast<const ::vect*>(nullptr)); R__UseDummy(_R__UNIQUE_DICT_(Init));

   // Dictionary for non-ClassDef classes
   static TClass *vect_Dictionary() {
      TClass* theClass =::ROOT::GenerateInitInstanceLocal(static_cast<const ::vect*>(nullptr))->GetClass();
      vect_TClassManip(theClass);
   return theClass;
   }

   static void vect_TClassManip(TClass* ){
   }

} // end of namespace ROOT

namespace ROOT {
   static TClass *particle_Dictionary();
   static void particle_TClassManip(TClass*);
   static void *new_particle(void *p = nullptr);
   static void *newArray_particle(Long_t size, void *p);
   static void delete_particle(void *p);
   static void deleteArray_particle(void *p);
   static void destruct_particle(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const ::particle*)
   {
      ::particle *ptr = nullptr;
      static ::TVirtualIsAProxy* isa_proxy = new ::TIsAProxy(typeid(::particle));
      static ::ROOT::TGenericClassInfo 
         instance("particle", "particle.h", 17,
                  typeid(::particle), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &particle_Dictionary, isa_proxy, 4,
                  sizeof(::particle) );
      instance.SetNew(&new_particle);
      instance.SetNewArray(&newArray_particle);
      instance.SetDelete(&delete_particle);
      instance.SetDeleteArray(&deleteArray_particle);
      instance.SetDestructor(&destruct_particle);
      return &instance;
   }
   TGenericClassInfo *GenerateInitInstance(const ::particle*)
   {
      return GenerateInitInstanceLocal(static_cast<::particle*>(nullptr));
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal(static_cast<const ::particle*>(nullptr)); R__UseDummy(_R__UNIQUE_DICT_(Init));

   // Dictionary for non-ClassDef classes
   static TClass *particle_Dictionary() {
      TClass* theClass =::ROOT::GenerateInitInstanceLocal(static_cast<const ::particle*>(nullptr))->GetClass();
      particle_TClassManip(theClass);
   return theClass;
   }

   static void particle_TClassManip(TClass* ){
   }

} // end of namespace ROOT

namespace ROOT {
   static TClass *line_Dictionary();
   static void line_TClassManip(TClass*);
   static void *new_line(void *p = nullptr);
   static void *newArray_line(Long_t size, void *p);
   static void delete_line(void *p);
   static void deleteArray_line(void *p);
   static void destruct_line(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const ::line*)
   {
      ::line *ptr = nullptr;
      static ::TVirtualIsAProxy* isa_proxy = new ::TIsAProxy(typeid(::line));
      static ::ROOT::TGenericClassInfo 
         instance("line", "params.h", 20,
                  typeid(::line), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &line_Dictionary, isa_proxy, 4,
                  sizeof(::line) );
      instance.SetNew(&new_line);
      instance.SetNewArray(&newArray_line);
      instance.SetDelete(&delete_line);
      instance.SetDeleteArray(&deleteArray_line);
      instance.SetDestructor(&destruct_line);
      return &instance;
   }
   TGenericClassInfo *GenerateInitInstance(const ::line*)
   {
      return GenerateInitInstanceLocal(static_cast<::line*>(nullptr));
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal(static_cast<const ::line*>(nullptr)); R__UseDummy(_R__UNIQUE_DICT_(Init));

   // Dictionary for non-ClassDef classes
   static TClass *line_Dictionary() {
      TClass* theClass =::ROOT::GenerateInitInstanceLocal(static_cast<const ::line*>(nullptr))->GetClass();
      line_TClassManip(theClass);
   return theClass;
   }

   static void line_TClassManip(TClass* ){
   }

} // end of namespace ROOT

namespace ROOT {
   static TClass *params_Dictionary();
   static void params_TClassManip(TClass*);
   static void *new_params(void *p = nullptr);
   static void *newArray_params(Long_t size, void *p);
   static void delete_params(void *p);
   static void deleteArray_params(void *p);
   static void destruct_params(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const ::params*)
   {
      ::params *ptr = nullptr;
      static ::TVirtualIsAProxy* isa_proxy = new ::TIsAProxy(typeid(::params));
      static ::ROOT::TGenericClassInfo 
         instance("params", "params.h", 103,
                  typeid(::params), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &params_Dictionary, isa_proxy, 4,
                  sizeof(::params) );
      instance.SetNew(&new_params);
      instance.SetNewArray(&newArray_params);
      instance.SetDelete(&delete_params);
      instance.SetDeleteArray(&deleteArray_params);
      instance.SetDestructor(&destruct_params);
      return &instance;
   }
   TGenericClassInfo *GenerateInitInstance(const ::params*)
   {
      return GenerateInitInstanceLocal(static_cast<::params*>(nullptr));
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal(static_cast<const ::params*>(nullptr)); R__UseDummy(_R__UNIQUE_DICT_(Init));

   // Dictionary for non-ClassDef classes
   static TClass *params_Dictionary() {
      TClass* theClass =::ROOT::GenerateInitInstanceLocal(static_cast<const ::params*>(nullptr))->GetClass();
      params_TClassManip(theClass);
   return theClass;
   }

   static void params_TClassManip(TClass* ){
   }

} // end of namespace ROOT

namespace ROOT {
   static TClass *flags_Dictionary();
   static void flags_TClassManip(TClass*);
   static void *new_flags(void *p = nullptr);
   static void *newArray_flags(Long_t size, void *p);
   static void delete_flags(void *p);
   static void deleteArray_flags(void *p);
   static void destruct_flags(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const ::flags*)
   {
      ::flags *ptr = nullptr;
      static ::TVirtualIsAProxy* isa_proxy = new ::TIsAProxy(typeid(::flags));
      static ::ROOT::TGenericClassInfo 
         instance("flags", "event1.h", 10,
                  typeid(::flags), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &flags_Dictionary, isa_proxy, 4,
                  sizeof(::flags) );
      instance.SetNew(&new_flags);
      instance.SetNewArray(&newArray_flags);
      instance.SetDelete(&delete_flags);
      instance.SetDeleteArray(&deleteArray_flags);
      instance.SetDestructor(&destruct_flags);
      return &instance;
   }
   TGenericClassInfo *GenerateInitInstance(const ::flags*)
   {
      return GenerateInitInstanceLocal(static_cast<::flags*>(nullptr));
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal(static_cast<const ::flags*>(nullptr)); R__UseDummy(_R__UNIQUE_DICT_(Init));

   // Dictionary for non-ClassDef classes
   static TClass *flags_Dictionary() {
      TClass* theClass =::ROOT::GenerateInitInstanceLocal(static_cast<const ::flags*>(nullptr))->GetClass();
      flags_TClassManip(theClass);
   return theClass;
   }

   static void flags_TClassManip(TClass* ){
   }

} // end of namespace ROOT

namespace ROOT {
   static void *new_event(void *p = nullptr);
   static void *newArray_event(Long_t size, void *p);
   static void delete_event(void *p);
   static void deleteArray_event(void *p);
   static void destruct_event(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const ::event*)
   {
      ::event *ptr = nullptr;
      static ::TVirtualIsAProxy* isa_proxy = new ::TInstrumentedIsAProxy< ::event >(nullptr);
      static ::ROOT::TGenericClassInfo 
         instance("event", ::event::Class_Version(), "event1.h", 32,
                  typeid(::event), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &::event::Dictionary, isa_proxy, 4,
                  sizeof(::event) );
      instance.SetNew(&new_event);
      instance.SetNewArray(&newArray_event);
      instance.SetDelete(&delete_event);
      instance.SetDeleteArray(&deleteArray_event);
      instance.SetDestructor(&destruct_event);
      return &instance;
   }
   TGenericClassInfo *GenerateInitInstance(const ::event*)
   {
      return GenerateInitInstanceLocal(static_cast<::event*>(nullptr));
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal(static_cast<const ::event*>(nullptr)); R__UseDummy(_R__UNIQUE_DICT_(Init));
} // end of namespace ROOT

//______________________________________________________________________________
atomic_TClass_ptr event::fgIsA(nullptr);  // static to hold class pointer

//______________________________________________________________________________
const char *event::Class_Name()
{
   return "event";
}

//______________________________________________________________________________
const char *event::ImplFileName()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::event*)nullptr)->GetImplFileName();
}

//______________________________________________________________________________
int event::ImplFileLine()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::event*)nullptr)->GetImplFileLine();
}

//______________________________________________________________________________
TClass *event::Dictionary()
{
   fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::event*)nullptr)->GetClass();
   return fgIsA;
}

//______________________________________________________________________________
TClass *event::Class()
{
   if (!fgIsA.load()) { R__LOCKGUARD(gInterpreterMutex); fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::event*)nullptr)->GetClass(); }
   return fgIsA;
}

namespace ROOT {
   // Wrappers around operator new
   static void *new_vec(void *p) {
      return  p ? new(p) ::vec : new ::vec;
   }
   static void *newArray_vec(Long_t nElements, void *p) {
      return p ? new(p) ::vec[nElements] : new ::vec[nElements];
   }
   // Wrapper around operator delete
   static void delete_vec(void *p) {
      delete (static_cast<::vec*>(p));
   }
   static void deleteArray_vec(void *p) {
      delete [] (static_cast<::vec*>(p));
   }
   static void destruct_vec(void *p) {
      typedef ::vec current_t;
      (static_cast<current_t*>(p))->~current_t();
   }
} // end of namespace ROOT for class ::vec

namespace ROOT {
   // Wrappers around operator new
   static void *new_vect(void *p) {
      return  p ? new(p) ::vect : new ::vect;
   }
   static void *newArray_vect(Long_t nElements, void *p) {
      return p ? new(p) ::vect[nElements] : new ::vect[nElements];
   }
   // Wrapper around operator delete
   static void delete_vect(void *p) {
      delete (static_cast<::vect*>(p));
   }
   static void deleteArray_vect(void *p) {
      delete [] (static_cast<::vect*>(p));
   }
   static void destruct_vect(void *p) {
      typedef ::vect current_t;
      (static_cast<current_t*>(p))->~current_t();
   }
} // end of namespace ROOT for class ::vect

namespace ROOT {
   // Wrappers around operator new
   static void *new_particle(void *p) {
      return  p ? new(p) ::particle : new ::particle;
   }
   static void *newArray_particle(Long_t nElements, void *p) {
      return p ? new(p) ::particle[nElements] : new ::particle[nElements];
   }
   // Wrapper around operator delete
   static void delete_particle(void *p) {
      delete (static_cast<::particle*>(p));
   }
   static void deleteArray_particle(void *p) {
      delete [] (static_cast<::particle*>(p));
   }
   static void destruct_particle(void *p) {
      typedef ::particle current_t;
      (static_cast<current_t*>(p))->~current_t();
   }
} // end of namespace ROOT for class ::particle

namespace ROOT {
   // Wrappers around operator new
   static void *new_line(void *p) {
      return  p ? new(p) ::line : new ::line;
   }
   static void *newArray_line(Long_t nElements, void *p) {
      return p ? new(p) ::line[nElements] : new ::line[nElements];
   }
   // Wrapper around operator delete
   static void delete_line(void *p) {
      delete (static_cast<::line*>(p));
   }
   static void deleteArray_line(void *p) {
      delete [] (static_cast<::line*>(p));
   }
   static void destruct_line(void *p) {
      typedef ::line current_t;
      (static_cast<current_t*>(p))->~current_t();
   }
} // end of namespace ROOT for class ::line

namespace ROOT {
   // Wrappers around operator new
   static void *new_params(void *p) {
      return  p ? new(p) ::params : new ::params;
   }
   static void *newArray_params(Long_t nElements, void *p) {
      return p ? new(p) ::params[nElements] : new ::params[nElements];
   }
   // Wrapper around operator delete
   static void delete_params(void *p) {
      delete (static_cast<::params*>(p));
   }
   static void deleteArray_params(void *p) {
      delete [] (static_cast<::params*>(p));
   }
   static void destruct_params(void *p) {
      typedef ::params current_t;
      (static_cast<current_t*>(p))->~current_t();
   }
} // end of namespace ROOT for class ::params

namespace ROOT {
   // Wrappers around operator new
   static void *new_flags(void *p) {
      return  p ? new(p) ::flags : new ::flags;
   }
   static void *newArray_flags(Long_t nElements, void *p) {
      return p ? new(p) ::flags[nElements] : new ::flags[nElements];
   }
   // Wrapper around operator delete
   static void delete_flags(void *p) {
      delete (static_cast<::flags*>(p));
   }
   static void deleteArray_flags(void *p) {
      delete [] (static_cast<::flags*>(p));
   }
   static void destruct_flags(void *p) {
      typedef ::flags current_t;
      (static_cast<current_t*>(p))->~current_t();
   }
} // end of namespace ROOT for class ::flags

//______________________________________________________________________________
void event::Streamer(TBuffer &R__b)
{
   // Stream an object of class event.

   if (R__b.IsReading()) {
      R__b.ReadClassBuffer(event::Class(),this);
   } else {
      R__b.WriteClassBuffer(event::Class(),this);
   }
}

namespace ROOT {
   // Wrappers around operator new
   static void *new_event(void *p) {
      return  p ? new(p) ::event : new ::event;
   }
   static void *newArray_event(Long_t nElements, void *p) {
      return p ? new(p) ::event[nElements] : new ::event[nElements];
   }
   // Wrapper around operator delete
   static void delete_event(void *p) {
      delete (static_cast<::event*>(p));
   }
   static void deleteArray_event(void *p) {
      delete [] (static_cast<::event*>(p));
   }
   static void destruct_event(void *p) {
      typedef ::event current_t;
      (static_cast<current_t*>(p))->~current_t();
   }
} // end of namespace ROOT for class ::event

namespace ROOT {
   static TClass *vectorlEparticlegR_Dictionary();
   static void vectorlEparticlegR_TClassManip(TClass*);
   static void *new_vectorlEparticlegR(void *p = nullptr);
   static void *newArray_vectorlEparticlegR(Long_t size, void *p);
   static void delete_vectorlEparticlegR(void *p);
   static void deleteArray_vectorlEparticlegR(void *p);
   static void destruct_vectorlEparticlegR(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const vector<particle>*)
   {
      vector<particle> *ptr = nullptr;
      static ::TVirtualIsAProxy* isa_proxy = new ::TIsAProxy(typeid(vector<particle>));
      static ::ROOT::TGenericClassInfo 
         instance("vector<particle>", -2, "c++/v1/vector", 340,
                  typeid(vector<particle>), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &vectorlEparticlegR_Dictionary, isa_proxy, 4,
                  sizeof(vector<particle>) );
      instance.SetNew(&new_vectorlEparticlegR);
      instance.SetNewArray(&newArray_vectorlEparticlegR);
      instance.SetDelete(&delete_vectorlEparticlegR);
      instance.SetDeleteArray(&deleteArray_vectorlEparticlegR);
      instance.SetDestructor(&destruct_vectorlEparticlegR);
      instance.AdoptCollectionProxyInfo(TCollectionProxyInfo::Generate(TCollectionProxyInfo::Pushback< vector<particle> >()));

      ::ROOT::AddClassAlternate("vector<particle>","std::__1::vector<particle, std::__1::allocator<particle>>");
      return &instance;
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal(static_cast<const vector<particle>*>(nullptr)); R__UseDummy(_R__UNIQUE_DICT_(Init));

   // Dictionary for non-ClassDef classes
   static TClass *vectorlEparticlegR_Dictionary() {
      TClass* theClass =::ROOT::GenerateInitInstanceLocal(static_cast<const vector<particle>*>(nullptr))->GetClass();
      vectorlEparticlegR_TClassManip(theClass);
   return theClass;
   }

   static void vectorlEparticlegR_TClassManip(TClass* ){
   }

} // end of namespace ROOT

namespace ROOT {
   // Wrappers around operator new
   static void *new_vectorlEparticlegR(void *p) {
      return  p ? ::new((::ROOT::Internal::TOperatorNewHelper*)p) vector<particle> : new vector<particle>;
   }
   static void *newArray_vectorlEparticlegR(Long_t nElements, void *p) {
      return p ? ::new((::ROOT::Internal::TOperatorNewHelper*)p) vector<particle>[nElements] : new vector<particle>[nElements];
   }
   // Wrapper around operator delete
   static void delete_vectorlEparticlegR(void *p) {
      delete (static_cast<vector<particle>*>(p));
   }
   static void deleteArray_vectorlEparticlegR(void *p) {
      delete [] (static_cast<vector<particle>*>(p));
   }
   static void destruct_vectorlEparticlegR(void *p) {
      typedef vector<particle> current_t;
      (static_cast<current_t*>(p))->~current_t();
   }
} // end of namespace ROOT for class vector<particle>

namespace {
  void TriggerDictionaryInitialization_event1Dict_Impl() {
    static const char* headers[] = {
"event1.h",
nullptr
    };
    static const char* includePaths[] = {
"/Users/hprasad/root/root_install/include/",
"/Users/hprasad/nuwro-2/nuwro-6/src/",
nullptr
    };
    static const char* fwdDeclCode = R"DICTFWDDCLS(
#line 1 "event1Dict dictionary forward declarations' payload"
#pragma clang diagnostic ignored "-Wkeyword-compat"
#pragma clang diagnostic ignored "-Wignored-attributes"
#pragma clang diagnostic ignored "-Wreturn-type-c-linkage"
extern int __Cling_AutoLoading_Map;
class __attribute__((annotate("$clingAutoload$particle.h")))  __attribute__((annotate("$clingAutoload$event1.h")))  particle;
namespace std{inline namespace __1{template <class _Tp> class __attribute__((annotate("$clingAutoload$iosfwd")))  __attribute__((annotate("$clingAutoload$string")))  allocator;
}}
class __attribute__((annotate("$clingAutoload$vec.h")))  __attribute__((annotate("$clingAutoload$event1.h")))  vec;
class __attribute__((annotate("$clingAutoload$vect.h")))  __attribute__((annotate("$clingAutoload$event1.h")))  vect;
struct __attribute__((annotate("$clingAutoload$params.h")))  __attribute__((annotate("$clingAutoload$event1.h")))  line;
class __attribute__((annotate("$clingAutoload$params.h")))  __attribute__((annotate("$clingAutoload$event1.h")))  params;
class __attribute__((annotate("$clingAutoload$event1.h")))  flags;
class __attribute__((annotate("$clingAutoload$event1.h")))  event;
)DICTFWDDCLS";
    static const char* payloadCode = R"DICTPAYLOAD(
#line 1 "event1Dict dictionary payload"


#define _BACKWARD_BACKWARD_WARNING_H
// Inline headers
#include "event1.h"

#undef  _BACKWARD_BACKWARD_WARNING_H
)DICTPAYLOAD";
    static const char* classesHeaders[] = {
"GeV", payloadCode, "@",
"GeV2", payloadCode, "@",
"MeV", payloadCode, "@",
"cm", payloadCode, "@",
"cm2", payloadCode, "@",
"event", payloadCode, "@",
"flags", payloadCode, "@",
"line", payloadCode, "@",
"params", payloadCode, "@",
"particle", payloadCode, "@",
"vec", payloadCode, "@",
"vect", payloadCode, "@",
nullptr
};
    static bool isInitialized = false;
    if (!isInitialized) {
      TROOT::RegisterModule("event1Dict",
        headers, includePaths, payloadCode, fwdDeclCode,
        TriggerDictionaryInitialization_event1Dict_Impl, {}, classesHeaders, /*hasCxxModule*/false);
      isInitialized = true;
    }
  }
  static struct DictInit {
    DictInit() {
      TriggerDictionaryInitialization_event1Dict_Impl();
    }
  } __TheDictionaryInitializer;
}
void TriggerDictionaryInitialization_event1Dict() {
  TriggerDictionaryInitialization_event1Dict_Impl();
}
