// Do NOT change. Changes will be lost next time file is generated

#define R__DICTIONARY_FILENAME srcdIMcEventCollection_p5_dict

/*******************************************************************/
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>
#include <assert.h>
#define G__DICTIONARY
#include "RConfig.h"
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

// Since CINT ignores the std namespace, we need to do so in this file.
namespace std {} using namespace std;

// Header files passed as explicit arguments
#include "generator/GenParticle_p5.h"
#include "generator/GenEvent_p5.h"
#include "generator/GenVertex_p5.h"
#include "generator/McEventCollection_p5.h"

// Header files passed via #pragma extra_include

namespace ROOT {
   static void *new_GenEvent_p5(void *p = 0);
   static void *newArray_GenEvent_p5(Long_t size, void *p);
   static void delete_GenEvent_p5(void *p);
   static void deleteArray_GenEvent_p5(void *p);
   static void destruct_GenEvent_p5(void *p);
   static void streamer_GenEvent_p5(TBuffer &buf, void *obj);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const ::GenEvent_p5*)
   {
      ::GenEvent_p5 *ptr = 0;
      static ::TVirtualIsAProxy* isa_proxy = new ::TInstrumentedIsAProxy< ::GenEvent_p5 >(0);
      static ::ROOT::TGenericClassInfo 
         instance("GenEvent_p5", ::GenEvent_p5::Class_Version(), "generator/GenEvent_p5.h", 10,
                  typeid(::GenEvent_p5), DefineBehavior(ptr, ptr),
                  &::GenEvent_p5::Dictionary, isa_proxy, 16,
                  sizeof(::GenEvent_p5) );
      instance.SetNew(&new_GenEvent_p5);
      instance.SetNewArray(&newArray_GenEvent_p5);
      instance.SetDelete(&delete_GenEvent_p5);
      instance.SetDeleteArray(&deleteArray_GenEvent_p5);
      instance.SetDestructor(&destruct_GenEvent_p5);
      instance.SetStreamerFunc(&streamer_GenEvent_p5);
      return &instance;
   }
   TGenericClassInfo *GenerateInitInstance(const ::GenEvent_p5*)
   {
      return GenerateInitInstanceLocal((::GenEvent_p5*)0);
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_(Init) = GenerateInitInstanceLocal((const ::GenEvent_p5*)0x0); R__UseDummy(_R__UNIQUE_(Init));
} // end of namespace ROOT

namespace ROOT {
   static void *new_GenVertex_p5(void *p = 0);
   static void *newArray_GenVertex_p5(Long_t size, void *p);
   static void delete_GenVertex_p5(void *p);
   static void deleteArray_GenVertex_p5(void *p);
   static void destruct_GenVertex_p5(void *p);
   static void streamer_GenVertex_p5(TBuffer &buf, void *obj);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const ::GenVertex_p5*)
   {
      ::GenVertex_p5 *ptr = 0;
      static ::TVirtualIsAProxy* isa_proxy = new ::TInstrumentedIsAProxy< ::GenVertex_p5 >(0);
      static ::ROOT::TGenericClassInfo 
         instance("GenVertex_p5", ::GenVertex_p5::Class_Version(), "generator/GenVertex_p5.h", 8,
                  typeid(::GenVertex_p5), DefineBehavior(ptr, ptr),
                  &::GenVertex_p5::Dictionary, isa_proxy, 16,
                  sizeof(::GenVertex_p5) );
      instance.SetNew(&new_GenVertex_p5);
      instance.SetNewArray(&newArray_GenVertex_p5);
      instance.SetDelete(&delete_GenVertex_p5);
      instance.SetDeleteArray(&deleteArray_GenVertex_p5);
      instance.SetDestructor(&destruct_GenVertex_p5);
      instance.SetStreamerFunc(&streamer_GenVertex_p5);
      return &instance;
   }
   TGenericClassInfo *GenerateInitInstance(const ::GenVertex_p5*)
   {
      return GenerateInitInstanceLocal((::GenVertex_p5*)0);
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_(Init) = GenerateInitInstanceLocal((const ::GenVertex_p5*)0x0); R__UseDummy(_R__UNIQUE_(Init));
} // end of namespace ROOT

namespace ROOT {
   static void *new_GenParticle_p5(void *p = 0);
   static void *newArray_GenParticle_p5(Long_t size, void *p);
   static void delete_GenParticle_p5(void *p);
   static void deleteArray_GenParticle_p5(void *p);
   static void destruct_GenParticle_p5(void *p);
   static void streamer_GenParticle_p5(TBuffer &buf, void *obj);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const ::GenParticle_p5*)
   {
      ::GenParticle_p5 *ptr = 0;
      static ::TVirtualIsAProxy* isa_proxy = new ::TInstrumentedIsAProxy< ::GenParticle_p5 >(0);
      static ::ROOT::TGenericClassInfo 
         instance("GenParticle_p5", ::GenParticle_p5::Class_Version(), "generator/GenParticle_p5.h", 9,
                  typeid(::GenParticle_p5), DefineBehavior(ptr, ptr),
                  &::GenParticle_p5::Dictionary, isa_proxy, 16,
                  sizeof(::GenParticle_p5) );
      instance.SetNew(&new_GenParticle_p5);
      instance.SetNewArray(&newArray_GenParticle_p5);
      instance.SetDelete(&delete_GenParticle_p5);
      instance.SetDeleteArray(&deleteArray_GenParticle_p5);
      instance.SetDestructor(&destruct_GenParticle_p5);
      instance.SetStreamerFunc(&streamer_GenParticle_p5);
      return &instance;
   }
   TGenericClassInfo *GenerateInitInstance(const ::GenParticle_p5*)
   {
      return GenerateInitInstanceLocal((::GenParticle_p5*)0);
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_(Init) = GenerateInitInstanceLocal((const ::GenParticle_p5*)0x0); R__UseDummy(_R__UNIQUE_(Init));
} // end of namespace ROOT

namespace ROOT {
   static void *new_McEventCollection_p5(void *p = 0);
   static void *newArray_McEventCollection_p5(Long_t size, void *p);
   static void delete_McEventCollection_p5(void *p);
   static void deleteArray_McEventCollection_p5(void *p);
   static void destruct_McEventCollection_p5(void *p);
   static void streamer_McEventCollection_p5(TBuffer &buf, void *obj);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const ::McEventCollection_p5*)
   {
      ::McEventCollection_p5 *ptr = 0;
      static ::TVirtualIsAProxy* isa_proxy = new ::TInstrumentedIsAProxy< ::McEventCollection_p5 >(0);
      static ::ROOT::TGenericClassInfo 
         instance("McEventCollection_p5", ::McEventCollection_p5::Class_Version(), "generator/McEventCollection_p5.h", 12,
                  typeid(::McEventCollection_p5), DefineBehavior(ptr, ptr),
                  &::McEventCollection_p5::Dictionary, isa_proxy, 16,
                  sizeof(::McEventCollection_p5) );
      instance.SetNew(&new_McEventCollection_p5);
      instance.SetNewArray(&newArray_McEventCollection_p5);
      instance.SetDelete(&delete_McEventCollection_p5);
      instance.SetDeleteArray(&deleteArray_McEventCollection_p5);
      instance.SetDestructor(&destruct_McEventCollection_p5);
      instance.SetStreamerFunc(&streamer_McEventCollection_p5);
      return &instance;
   }
   TGenericClassInfo *GenerateInitInstance(const ::McEventCollection_p5*)
   {
      return GenerateInitInstanceLocal((::McEventCollection_p5*)0);
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_(Init) = GenerateInitInstanceLocal((const ::McEventCollection_p5*)0x0); R__UseDummy(_R__UNIQUE_(Init));
} // end of namespace ROOT

//______________________________________________________________________________
atomic_TClass_ptr GenEvent_p5::fgIsA(0);  // static to hold class pointer

//______________________________________________________________________________
const char *GenEvent_p5::Class_Name()
{
   return "GenEvent_p5";
}

//______________________________________________________________________________
const char *GenEvent_p5::ImplFileName()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::GenEvent_p5*)0x0)->GetImplFileName();
}

//______________________________________________________________________________
int GenEvent_p5::ImplFileLine()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::GenEvent_p5*)0x0)->GetImplFileLine();
}

//______________________________________________________________________________
TClass *GenEvent_p5::Dictionary()
{
   fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::GenEvent_p5*)0x0)->GetClass();
   return fgIsA;
}

//______________________________________________________________________________
TClass *GenEvent_p5::Class()
{
   if (!fgIsA.load()) { R__LOCKGUARD2(gInterpreterMutex); fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::GenEvent_p5*)0x0)->GetClass(); }
   return fgIsA;
}

//______________________________________________________________________________
atomic_TClass_ptr GenVertex_p5::fgIsA(0);  // static to hold class pointer

//______________________________________________________________________________
const char *GenVertex_p5::Class_Name()
{
   return "GenVertex_p5";
}

//______________________________________________________________________________
const char *GenVertex_p5::ImplFileName()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::GenVertex_p5*)0x0)->GetImplFileName();
}

//______________________________________________________________________________
int GenVertex_p5::ImplFileLine()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::GenVertex_p5*)0x0)->GetImplFileLine();
}

//______________________________________________________________________________
TClass *GenVertex_p5::Dictionary()
{
   fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::GenVertex_p5*)0x0)->GetClass();
   return fgIsA;
}

//______________________________________________________________________________
TClass *GenVertex_p5::Class()
{
   if (!fgIsA.load()) { R__LOCKGUARD2(gInterpreterMutex); fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::GenVertex_p5*)0x0)->GetClass(); }
   return fgIsA;
}

//______________________________________________________________________________
atomic_TClass_ptr GenParticle_p5::fgIsA(0);  // static to hold class pointer

//______________________________________________________________________________
const char *GenParticle_p5::Class_Name()
{
   return "GenParticle_p5";
}

//______________________________________________________________________________
const char *GenParticle_p5::ImplFileName()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::GenParticle_p5*)0x0)->GetImplFileName();
}

//______________________________________________________________________________
int GenParticle_p5::ImplFileLine()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::GenParticle_p5*)0x0)->GetImplFileLine();
}

//______________________________________________________________________________
TClass *GenParticle_p5::Dictionary()
{
   fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::GenParticle_p5*)0x0)->GetClass();
   return fgIsA;
}

//______________________________________________________________________________
TClass *GenParticle_p5::Class()
{
   if (!fgIsA.load()) { R__LOCKGUARD2(gInterpreterMutex); fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::GenParticle_p5*)0x0)->GetClass(); }
   return fgIsA;
}

//______________________________________________________________________________
atomic_TClass_ptr McEventCollection_p5::fgIsA(0);  // static to hold class pointer

//______________________________________________________________________________
const char *McEventCollection_p5::Class_Name()
{
   return "McEventCollection_p5";
}

//______________________________________________________________________________
const char *McEventCollection_p5::ImplFileName()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::McEventCollection_p5*)0x0)->GetImplFileName();
}

//______________________________________________________________________________
int McEventCollection_p5::ImplFileLine()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::McEventCollection_p5*)0x0)->GetImplFileLine();
}

//______________________________________________________________________________
TClass *McEventCollection_p5::Dictionary()
{
   fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::McEventCollection_p5*)0x0)->GetClass();
   return fgIsA;
}

//______________________________________________________________________________
TClass *McEventCollection_p5::Class()
{
   if (!fgIsA.load()) { R__LOCKGUARD2(gInterpreterMutex); fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::McEventCollection_p5*)0x0)->GetClass(); }
   return fgIsA;
}

//______________________________________________________________________________
void GenEvent_p5::Streamer(TBuffer &R__b)
{
   // Stream an object of class GenEvent_p5.

   UInt_t R__s, R__c;
   if (R__b.IsReading()) {
      Version_t R__v = R__b.ReadVersion(&R__s, &R__c); if (R__v) { }
      TObject::Streamer(R__b);
      R__b >> m_signalProcessId;
      R__b >> m_eventNbr;
      R__b >> m_mpi;
      R__b >> m_eventScale;
      R__b >> m_alphaQCD;
      R__b >> m_alphaQED;
      R__b >> m_signalProcessVtx;
      R__b >> m_beamParticle1;
      R__b >> m_beamParticle2;
      {
         vector<double> &R__stl =  m_weights;
         R__stl.clear();
         int R__i, R__n;
         R__b >> R__n;
         R__stl.reserve(R__n);
         for (R__i = 0; R__i < R__n; R__i++) {
            double R__t;
            R__b >> R__t;
            R__stl.push_back(R__t);
         }
      }
      {
         vector<long> &R__stl =  m_randomStates;
         R__stl.clear();
         int R__i, R__n;
         R__b >> R__n;
         R__stl.reserve(R__n);
         for (R__i = 0; R__i < R__n; R__i++) {
            long R__t;
            R__b >> R__t;
            R__stl.push_back(R__t);
         }
      }
      {
         vector<double> &R__stl =  m_crossSection;
         R__stl.clear();
         int R__i, R__n;
         R__b >> R__n;
         R__stl.reserve(R__n);
         for (R__i = 0; R__i < R__n; R__i++) {
            double R__t;
            R__b >> R__t;
            R__stl.push_back(R__t);
         }
      }
      {
         vector<float> &R__stl =  m_heavyIon;
         R__stl.clear();
         int R__i, R__n;
         R__b >> R__n;
         R__stl.reserve(R__n);
         for (R__i = 0; R__i < R__n; R__i++) {
            float R__t;
            R__b >> R__t;
            R__stl.push_back(R__t);
         }
      }
      {
         vector<double> &R__stl =  m_pdfinfo;
         R__stl.clear();
         int R__i, R__n;
         R__b >> R__n;
         R__stl.reserve(R__n);
         for (R__i = 0; R__i < R__n; R__i++) {
            double R__t;
            R__b >> R__t;
            R__stl.push_back(R__t);
         }
      }
      R__b >> m_momentumUnit;
      R__b >> m_lengthUnit;
      R__b >> m_verticesBegin;
      R__b >> m_verticesEnd;
      R__b >> m_particlesBegin;
      R__b >> m_particlesEnd;
      R__b.CheckByteCount(R__s, R__c, GenEvent_p5::IsA());
   } else {
      R__c = R__b.WriteVersion(GenEvent_p5::IsA(), kTRUE);
      TObject::Streamer(R__b);
      R__b << m_signalProcessId;
      R__b << m_eventNbr;
      R__b << m_mpi;
      R__b << m_eventScale;
      R__b << m_alphaQCD;
      R__b << m_alphaQED;
      R__b << m_signalProcessVtx;
      R__b << m_beamParticle1;
      R__b << m_beamParticle2;
      {
         vector<double> &R__stl =  m_weights;
         int R__n=int(R__stl.size());
         R__b << R__n;
         if(R__n) {
            vector<double>::iterator R__k;
            for (R__k = R__stl.begin(); R__k != R__stl.end(); ++R__k) {
            R__b << (*R__k);
            }
         }
      }
      {
         vector<long> &R__stl =  m_randomStates;
         int R__n=int(R__stl.size());
         R__b << R__n;
         if(R__n) {
            vector<long>::iterator R__k;
            for (R__k = R__stl.begin(); R__k != R__stl.end(); ++R__k) {
            R__b << (*R__k);
            }
         }
      }
      {
         vector<double> &R__stl =  m_crossSection;
         int R__n=int(R__stl.size());
         R__b << R__n;
         if(R__n) {
            vector<double>::iterator R__k;
            for (R__k = R__stl.begin(); R__k != R__stl.end(); ++R__k) {
            R__b << (*R__k);
            }
         }
      }
      {
         vector<float> &R__stl =  m_heavyIon;
         int R__n=int(R__stl.size());
         R__b << R__n;
         if(R__n) {
            vector<float>::iterator R__k;
            for (R__k = R__stl.begin(); R__k != R__stl.end(); ++R__k) {
            R__b << (*R__k);
            }
         }
      }
      {
         vector<double> &R__stl =  m_pdfinfo;
         int R__n=int(R__stl.size());
         R__b << R__n;
         if(R__n) {
            vector<double>::iterator R__k;
            for (R__k = R__stl.begin(); R__k != R__stl.end(); ++R__k) {
            R__b << (*R__k);
            }
         }
      }
      R__b << m_momentumUnit;
      R__b << m_lengthUnit;
      R__b << m_verticesBegin;
      R__b << m_verticesEnd;
      R__b << m_particlesBegin;
      R__b << m_particlesEnd;
      R__b.SetByteCount(R__c, kTRUE);
   }
}

namespace ROOT {
   // Wrappers around operator new
   static void *new_GenEvent_p5(void *p) {
      return  p ? new(p) ::GenEvent_p5 : new ::GenEvent_p5;
   }
   static void *newArray_GenEvent_p5(Long_t nElements, void *p) {
      return p ? new(p) ::GenEvent_p5[nElements] : new ::GenEvent_p5[nElements];
   }
   // Wrapper around operator delete
   static void delete_GenEvent_p5(void *p) {
      delete ((::GenEvent_p5*)p);
   }
   static void deleteArray_GenEvent_p5(void *p) {
      delete [] ((::GenEvent_p5*)p);
   }
   static void destruct_GenEvent_p5(void *p) {
      typedef ::GenEvent_p5 current_t;
      ((current_t*)p)->~current_t();
   }
   // Wrapper around a custom streamer member function.
   static void streamer_GenEvent_p5(TBuffer &buf, void *obj) {
      ((::GenEvent_p5*)obj)->::GenEvent_p5::Streamer(buf);
   }
} // end of namespace ROOT for class ::GenEvent_p5

//______________________________________________________________________________
void GenVertex_p5::Streamer(TBuffer &R__b)
{
   // Stream an object of class GenVertex_p5.

   UInt_t R__s, R__c;
   if (R__b.IsReading()) {
      Version_t R__v = R__b.ReadVersion(&R__s, &R__c); if (R__v) { }
      TObject::Streamer(R__b);
      R__b >> m_x;
      R__b >> m_y;
      R__b >> m_z;
      R__b >> m_t;
      {
         vector<int> &R__stl =  m_particlesIn;
         R__stl.clear();
         int R__i, R__n;
         R__b >> R__n;
         R__stl.reserve(R__n);
         for (R__i = 0; R__i < R__n; R__i++) {
            int R__t;
            R__b >> R__t;
            R__stl.push_back(R__t);
         }
      }
      {
         vector<int> &R__stl =  m_particlesOut;
         R__stl.clear();
         int R__i, R__n;
         R__b >> R__n;
         R__stl.reserve(R__n);
         for (R__i = 0; R__i < R__n; R__i++) {
            int R__t;
            R__b >> R__t;
            R__stl.push_back(R__t);
         }
      }
      R__b >> m_id;
      {
         vector<float> &R__stl =  m_weights;
         R__stl.clear();
         int R__i, R__n;
         R__b >> R__n;
         R__stl.reserve(R__n);
         for (R__i = 0; R__i < R__n; R__i++) {
            float R__t;
            R__b >> R__t;
            R__stl.push_back(R__t);
         }
      }
      R__b >> m_barcode;
      R__b.CheckByteCount(R__s, R__c, GenVertex_p5::IsA());
   } else {
      R__c = R__b.WriteVersion(GenVertex_p5::IsA(), kTRUE);
      TObject::Streamer(R__b);
      R__b << m_x;
      R__b << m_y;
      R__b << m_z;
      R__b << m_t;
      {
         vector<int> &R__stl =  m_particlesIn;
         int R__n=int(R__stl.size());
         R__b << R__n;
         if(R__n) {
            vector<int>::iterator R__k;
            for (R__k = R__stl.begin(); R__k != R__stl.end(); ++R__k) {
            R__b << (*R__k);
            }
         }
      }
      {
         vector<int> &R__stl =  m_particlesOut;
         int R__n=int(R__stl.size());
         R__b << R__n;
         if(R__n) {
            vector<int>::iterator R__k;
            for (R__k = R__stl.begin(); R__k != R__stl.end(); ++R__k) {
            R__b << (*R__k);
            }
         }
      }
      R__b << m_id;
      {
         vector<float> &R__stl =  m_weights;
         int R__n=int(R__stl.size());
         R__b << R__n;
         if(R__n) {
            vector<float>::iterator R__k;
            for (R__k = R__stl.begin(); R__k != R__stl.end(); ++R__k) {
            R__b << (*R__k);
            }
         }
      }
      R__b << m_barcode;
      R__b.SetByteCount(R__c, kTRUE);
   }
}

namespace ROOT {
   // Wrappers around operator new
   static void *new_GenVertex_p5(void *p) {
      return  p ? new(p) ::GenVertex_p5 : new ::GenVertex_p5;
   }
   static void *newArray_GenVertex_p5(Long_t nElements, void *p) {
      return p ? new(p) ::GenVertex_p5[nElements] : new ::GenVertex_p5[nElements];
   }
   // Wrapper around operator delete
   static void delete_GenVertex_p5(void *p) {
      delete ((::GenVertex_p5*)p);
   }
   static void deleteArray_GenVertex_p5(void *p) {
      delete [] ((::GenVertex_p5*)p);
   }
   static void destruct_GenVertex_p5(void *p) {
      typedef ::GenVertex_p5 current_t;
      ((current_t*)p)->~current_t();
   }
   // Wrapper around a custom streamer member function.
   static void streamer_GenVertex_p5(TBuffer &buf, void *obj) {
      ((::GenVertex_p5*)obj)->::GenVertex_p5::Streamer(buf);
   }
} // end of namespace ROOT for class ::GenVertex_p5

//______________________________________________________________________________
void GenParticle_p5::Streamer(TBuffer &R__b)
{
   // Stream an object of class GenParticle_p5.

   UInt_t R__s, R__c;
   if (R__b.IsReading()) {
      Version_t R__v = R__b.ReadVersion(&R__s, &R__c); if (R__v) { }
      TObject::Streamer(R__b);
      R__b >> m_px;
      R__b >> m_py;
      R__b >> m_pz;
      R__b >> m_m;
      R__b >> m_pdgId;
      R__b >> m_status;
      {
         vector<pair<int,int> > &R__stl =  m_flow;
         R__stl.clear();
         TClass *R__tcl1 = TBuffer::GetClass(typeid(struct std::__1::pair<int, int>));
         if (R__tcl1==0) {
            Error("m_flow streamer","Missing the TClass object for struct std::__1::pair<int, int>!");
            return;
         }
         int R__i, R__n;
         R__b >> R__n;
         R__stl.reserve(R__n);
         for (R__i = 0; R__i < R__n; R__i++) {
            pair<int,int> R__t;
            R__b.StreamObject(&R__t,R__tcl1);
            R__stl.push_back(R__t);
         }
      }
      R__b >> m_thetaPolarization;
      R__b >> m_phiPolarization;
      R__b >> m_prodVtx;
      R__b >> m_endVtx;
      R__b >> m_barcode;
      R__b >> m_generated_mass;
      R__b >> m_recoMethod;
      R__b.CheckByteCount(R__s, R__c, GenParticle_p5::IsA());
   } else {
      R__c = R__b.WriteVersion(GenParticle_p5::IsA(), kTRUE);
      TObject::Streamer(R__b);
      R__b << m_px;
      R__b << m_py;
      R__b << m_pz;
      R__b << m_m;
      R__b << m_pdgId;
      R__b << m_status;
      {
         vector<pair<int,int> > &R__stl =  m_flow;
         int R__n=int(R__stl.size());
         R__b << R__n;
         if(R__n) {
         TClass *R__tcl1 = TBuffer::GetClass(typeid(struct std::__1::pair<int, int>));
         if (R__tcl1==0) {
            Error("m_flow streamer","Missing the TClass object for struct std::__1::pair<int, int>!");
            return;
         }
            vector<pair<int,int> >::iterator R__k;
            for (R__k = R__stl.begin(); R__k != R__stl.end(); ++R__k) {
            R__b.StreamObject((pair<int,int>*)&(*R__k),R__tcl1);
            }
         }
      }
      R__b << m_thetaPolarization;
      R__b << m_phiPolarization;
      R__b << m_prodVtx;
      R__b << m_endVtx;
      R__b << m_barcode;
      R__b << m_generated_mass;
      R__b << m_recoMethod;
      R__b.SetByteCount(R__c, kTRUE);
   }
}

namespace ROOT {
   // Wrappers around operator new
   static void *new_GenParticle_p5(void *p) {
      return  p ? new(p) ::GenParticle_p5 : new ::GenParticle_p5;
   }
   static void *newArray_GenParticle_p5(Long_t nElements, void *p) {
      return p ? new(p) ::GenParticle_p5[nElements] : new ::GenParticle_p5[nElements];
   }
   // Wrapper around operator delete
   static void delete_GenParticle_p5(void *p) {
      delete ((::GenParticle_p5*)p);
   }
   static void deleteArray_GenParticle_p5(void *p) {
      delete [] ((::GenParticle_p5*)p);
   }
   static void destruct_GenParticle_p5(void *p) {
      typedef ::GenParticle_p5 current_t;
      ((current_t*)p)->~current_t();
   }
   // Wrapper around a custom streamer member function.
   static void streamer_GenParticle_p5(TBuffer &buf, void *obj) {
      ((::GenParticle_p5*)obj)->::GenParticle_p5::Streamer(buf);
   }
} // end of namespace ROOT for class ::GenParticle_p5

//______________________________________________________________________________
void McEventCollection_p5::Streamer(TBuffer &R__b)
{
   // Stream an object of class McEventCollection_p5.

   UInt_t R__s, R__c;
   if (R__b.IsReading()) {
      Version_t R__v = R__b.ReadVersion(&R__s, &R__c); if (R__v) { }
      TObject::Streamer(R__b);
      {
         vector<GenEvent_p5> &R__stl =  m_genEvents;
         R__stl.clear();
         int R__i, R__n;
         R__b >> R__n;
         R__stl.reserve(R__n);
         for (R__i = 0; R__i < R__n; R__i++) {
            GenEvent_p5 R__t;
            R__t.Streamer(R__b);
            R__stl.push_back(R__t);
         }
      }
      {
         vector<GenVertex_p5> &R__stl =  m_genVertices;
         R__stl.clear();
         int R__i, R__n;
         R__b >> R__n;
         R__stl.reserve(R__n);
         for (R__i = 0; R__i < R__n; R__i++) {
            GenVertex_p5 R__t;
            R__t.Streamer(R__b);
            R__stl.push_back(R__t);
         }
      }
      {
         vector<GenParticle_p5> &R__stl =  m_genParticles;
         R__stl.clear();
         int R__i, R__n;
         R__b >> R__n;
         R__stl.reserve(R__n);
         for (R__i = 0; R__i < R__n; R__i++) {
            GenParticle_p5 R__t;
            R__t.Streamer(R__b);
            R__stl.push_back(R__t);
         }
      }
      R__b.CheckByteCount(R__s, R__c, McEventCollection_p5::IsA());
   } else {
      R__c = R__b.WriteVersion(McEventCollection_p5::IsA(), kTRUE);
      TObject::Streamer(R__b);
      {
         vector<GenEvent_p5> &R__stl =  m_genEvents;
         int R__n=int(R__stl.size());
         R__b << R__n;
         if(R__n) {
            vector<GenEvent_p5>::iterator R__k;
            for (R__k = R__stl.begin(); R__k != R__stl.end(); ++R__k) {
            ((GenEvent_p5&)(*R__k)).Streamer(R__b);
            }
         }
      }
      {
         vector<GenVertex_p5> &R__stl =  m_genVertices;
         int R__n=int(R__stl.size());
         R__b << R__n;
         if(R__n) {
            vector<GenVertex_p5>::iterator R__k;
            for (R__k = R__stl.begin(); R__k != R__stl.end(); ++R__k) {
            ((GenVertex_p5&)(*R__k)).Streamer(R__b);
            }
         }
      }
      {
         vector<GenParticle_p5> &R__stl =  m_genParticles;
         int R__n=int(R__stl.size());
         R__b << R__n;
         if(R__n) {
            vector<GenParticle_p5>::iterator R__k;
            for (R__k = R__stl.begin(); R__k != R__stl.end(); ++R__k) {
            ((GenParticle_p5&)(*R__k)).Streamer(R__b);
            }
         }
      }
      R__b.SetByteCount(R__c, kTRUE);
   }
}

namespace ROOT {
   // Wrappers around operator new
   static void *new_McEventCollection_p5(void *p) {
      return  p ? new(p) ::McEventCollection_p5 : new ::McEventCollection_p5;
   }
   static void *newArray_McEventCollection_p5(Long_t nElements, void *p) {
      return p ? new(p) ::McEventCollection_p5[nElements] : new ::McEventCollection_p5[nElements];
   }
   // Wrapper around operator delete
   static void delete_McEventCollection_p5(void *p) {
      delete ((::McEventCollection_p5*)p);
   }
   static void deleteArray_McEventCollection_p5(void *p) {
      delete [] ((::McEventCollection_p5*)p);
   }
   static void destruct_McEventCollection_p5(void *p) {
      typedef ::McEventCollection_p5 current_t;
      ((current_t*)p)->~current_t();
   }
   // Wrapper around a custom streamer member function.
   static void streamer_McEventCollection_p5(TBuffer &buf, void *obj) {
      ((::McEventCollection_p5*)obj)->::McEventCollection_p5::Streamer(buf);
   }
} // end of namespace ROOT for class ::McEventCollection_p5

namespace ROOT {
   static TClass *vectorlEpairlEintcOintgRsPgR_Dictionary();
   static void vectorlEpairlEintcOintgRsPgR_TClassManip(TClass*);
   static void *new_vectorlEpairlEintcOintgRsPgR(void *p = 0);
   static void *newArray_vectorlEpairlEintcOintgRsPgR(Long_t size, void *p);
   static void delete_vectorlEpairlEintcOintgRsPgR(void *p);
   static void deleteArray_vectorlEpairlEintcOintgRsPgR(void *p);
   static void destruct_vectorlEpairlEintcOintgRsPgR(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const vector<pair<int,int> >*)
   {
      vector<pair<int,int> > *ptr = 0;
      static ::TVirtualIsAProxy* isa_proxy = new ::TIsAProxy(typeid(vector<pair<int,int> >));
      static ::ROOT::TGenericClassInfo 
         instance("vector<pair<int,int> >", -2, "vector", 477,
                  typeid(vector<pair<int,int> >), DefineBehavior(ptr, ptr),
                  &vectorlEpairlEintcOintgRsPgR_Dictionary, isa_proxy, 4,
                  sizeof(vector<pair<int,int> >) );
      instance.SetNew(&new_vectorlEpairlEintcOintgRsPgR);
      instance.SetNewArray(&newArray_vectorlEpairlEintcOintgRsPgR);
      instance.SetDelete(&delete_vectorlEpairlEintcOintgRsPgR);
      instance.SetDeleteArray(&deleteArray_vectorlEpairlEintcOintgRsPgR);
      instance.SetDestructor(&destruct_vectorlEpairlEintcOintgRsPgR);
      instance.AdoptCollectionProxyInfo(TCollectionProxyInfo::Generate(TCollectionProxyInfo::Pushback< vector<pair<int,int> > >()));
      return &instance;
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_(Init) = GenerateInitInstanceLocal((const vector<pair<int,int> >*)0x0); R__UseDummy(_R__UNIQUE_(Init));

   // Dictionary for non-ClassDef classes
   static TClass *vectorlEpairlEintcOintgRsPgR_Dictionary() {
      TClass* theClass =::ROOT::GenerateInitInstanceLocal((const vector<pair<int,int> >*)0x0)->GetClass();
      vectorlEpairlEintcOintgRsPgR_TClassManip(theClass);
   return theClass;
   }

   static void vectorlEpairlEintcOintgRsPgR_TClassManip(TClass* ){
   }

} // end of namespace ROOT

namespace ROOT {
   // Wrappers around operator new
   static void *new_vectorlEpairlEintcOintgRsPgR(void *p) {
      return  p ? ::new((::ROOT::TOperatorNewHelper*)p) vector<pair<int,int> > : new vector<pair<int,int> >;
   }
   static void *newArray_vectorlEpairlEintcOintgRsPgR(Long_t nElements, void *p) {
      return p ? ::new((::ROOT::TOperatorNewHelper*)p) vector<pair<int,int> >[nElements] : new vector<pair<int,int> >[nElements];
   }
   // Wrapper around operator delete
   static void delete_vectorlEpairlEintcOintgRsPgR(void *p) {
      delete ((vector<pair<int,int> >*)p);
   }
   static void deleteArray_vectorlEpairlEintcOintgRsPgR(void *p) {
      delete [] ((vector<pair<int,int> >*)p);
   }
   static void destruct_vectorlEpairlEintcOintgRsPgR(void *p) {
      typedef vector<pair<int,int> > current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class vector<pair<int,int> >

namespace ROOT {
   static TClass *vectorlElonggR_Dictionary();
   static void vectorlElonggR_TClassManip(TClass*);
   static void *new_vectorlElonggR(void *p = 0);
   static void *newArray_vectorlElonggR(Long_t size, void *p);
   static void delete_vectorlElonggR(void *p);
   static void deleteArray_vectorlElonggR(void *p);
   static void destruct_vectorlElonggR(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const vector<long>*)
   {
      vector<long> *ptr = 0;
      static ::TVirtualIsAProxy* isa_proxy = new ::TIsAProxy(typeid(vector<long>));
      static ::ROOT::TGenericClassInfo 
         instance("vector<long>", -2, "vector", 477,
                  typeid(vector<long>), DefineBehavior(ptr, ptr),
                  &vectorlElonggR_Dictionary, isa_proxy, 4,
                  sizeof(vector<long>) );
      instance.SetNew(&new_vectorlElonggR);
      instance.SetNewArray(&newArray_vectorlElonggR);
      instance.SetDelete(&delete_vectorlElonggR);
      instance.SetDeleteArray(&deleteArray_vectorlElonggR);
      instance.SetDestructor(&destruct_vectorlElonggR);
      instance.AdoptCollectionProxyInfo(TCollectionProxyInfo::Generate(TCollectionProxyInfo::Pushback< vector<long> >()));
      return &instance;
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_(Init) = GenerateInitInstanceLocal((const vector<long>*)0x0); R__UseDummy(_R__UNIQUE_(Init));

   // Dictionary for non-ClassDef classes
   static TClass *vectorlElonggR_Dictionary() {
      TClass* theClass =::ROOT::GenerateInitInstanceLocal((const vector<long>*)0x0)->GetClass();
      vectorlElonggR_TClassManip(theClass);
   return theClass;
   }

   static void vectorlElonggR_TClassManip(TClass* ){
   }

} // end of namespace ROOT

namespace ROOT {
   // Wrappers around operator new
   static void *new_vectorlElonggR(void *p) {
      return  p ? ::new((::ROOT::TOperatorNewHelper*)p) vector<long> : new vector<long>;
   }
   static void *newArray_vectorlElonggR(Long_t nElements, void *p) {
      return p ? ::new((::ROOT::TOperatorNewHelper*)p) vector<long>[nElements] : new vector<long>[nElements];
   }
   // Wrapper around operator delete
   static void delete_vectorlElonggR(void *p) {
      delete ((vector<long>*)p);
   }
   static void deleteArray_vectorlElonggR(void *p) {
      delete [] ((vector<long>*)p);
   }
   static void destruct_vectorlElonggR(void *p) {
      typedef vector<long> current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class vector<long>

namespace ROOT {
   static TClass *vectorlEintgR_Dictionary();
   static void vectorlEintgR_TClassManip(TClass*);
   static void *new_vectorlEintgR(void *p = 0);
   static void *newArray_vectorlEintgR(Long_t size, void *p);
   static void delete_vectorlEintgR(void *p);
   static void deleteArray_vectorlEintgR(void *p);
   static void destruct_vectorlEintgR(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const vector<int>*)
   {
      vector<int> *ptr = 0;
      static ::TVirtualIsAProxy* isa_proxy = new ::TIsAProxy(typeid(vector<int>));
      static ::ROOT::TGenericClassInfo 
         instance("vector<int>", -2, "vector", 477,
                  typeid(vector<int>), DefineBehavior(ptr, ptr),
                  &vectorlEintgR_Dictionary, isa_proxy, 0,
                  sizeof(vector<int>) );
      instance.SetNew(&new_vectorlEintgR);
      instance.SetNewArray(&newArray_vectorlEintgR);
      instance.SetDelete(&delete_vectorlEintgR);
      instance.SetDeleteArray(&deleteArray_vectorlEintgR);
      instance.SetDestructor(&destruct_vectorlEintgR);
      instance.AdoptCollectionProxyInfo(TCollectionProxyInfo::Generate(TCollectionProxyInfo::Pushback< vector<int> >()));
      return &instance;
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_(Init) = GenerateInitInstanceLocal((const vector<int>*)0x0); R__UseDummy(_R__UNIQUE_(Init));

   // Dictionary for non-ClassDef classes
   static TClass *vectorlEintgR_Dictionary() {
      TClass* theClass =::ROOT::GenerateInitInstanceLocal((const vector<int>*)0x0)->GetClass();
      vectorlEintgR_TClassManip(theClass);
   return theClass;
   }

   static void vectorlEintgR_TClassManip(TClass* ){
   }

} // end of namespace ROOT

namespace ROOT {
   // Wrappers around operator new
   static void *new_vectorlEintgR(void *p) {
      return  p ? ::new((::ROOT::TOperatorNewHelper*)p) vector<int> : new vector<int>;
   }
   static void *newArray_vectorlEintgR(Long_t nElements, void *p) {
      return p ? ::new((::ROOT::TOperatorNewHelper*)p) vector<int>[nElements] : new vector<int>[nElements];
   }
   // Wrapper around operator delete
   static void delete_vectorlEintgR(void *p) {
      delete ((vector<int>*)p);
   }
   static void deleteArray_vectorlEintgR(void *p) {
      delete [] ((vector<int>*)p);
   }
   static void destruct_vectorlEintgR(void *p) {
      typedef vector<int> current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class vector<int>

namespace ROOT {
   static TClass *vectorlEfloatgR_Dictionary();
   static void vectorlEfloatgR_TClassManip(TClass*);
   static void *new_vectorlEfloatgR(void *p = 0);
   static void *newArray_vectorlEfloatgR(Long_t size, void *p);
   static void delete_vectorlEfloatgR(void *p);
   static void deleteArray_vectorlEfloatgR(void *p);
   static void destruct_vectorlEfloatgR(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const vector<float>*)
   {
      vector<float> *ptr = 0;
      static ::TVirtualIsAProxy* isa_proxy = new ::TIsAProxy(typeid(vector<float>));
      static ::ROOT::TGenericClassInfo 
         instance("vector<float>", -2, "vector", 477,
                  typeid(vector<float>), DefineBehavior(ptr, ptr),
                  &vectorlEfloatgR_Dictionary, isa_proxy, 4,
                  sizeof(vector<float>) );
      instance.SetNew(&new_vectorlEfloatgR);
      instance.SetNewArray(&newArray_vectorlEfloatgR);
      instance.SetDelete(&delete_vectorlEfloatgR);
      instance.SetDeleteArray(&deleteArray_vectorlEfloatgR);
      instance.SetDestructor(&destruct_vectorlEfloatgR);
      instance.AdoptCollectionProxyInfo(TCollectionProxyInfo::Generate(TCollectionProxyInfo::Pushback< vector<float> >()));
      return &instance;
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_(Init) = GenerateInitInstanceLocal((const vector<float>*)0x0); R__UseDummy(_R__UNIQUE_(Init));

   // Dictionary for non-ClassDef classes
   static TClass *vectorlEfloatgR_Dictionary() {
      TClass* theClass =::ROOT::GenerateInitInstanceLocal((const vector<float>*)0x0)->GetClass();
      vectorlEfloatgR_TClassManip(theClass);
   return theClass;
   }

   static void vectorlEfloatgR_TClassManip(TClass* ){
   }

} // end of namespace ROOT

namespace ROOT {
   // Wrappers around operator new
   static void *new_vectorlEfloatgR(void *p) {
      return  p ? ::new((::ROOT::TOperatorNewHelper*)p) vector<float> : new vector<float>;
   }
   static void *newArray_vectorlEfloatgR(Long_t nElements, void *p) {
      return p ? ::new((::ROOT::TOperatorNewHelper*)p) vector<float>[nElements] : new vector<float>[nElements];
   }
   // Wrapper around operator delete
   static void delete_vectorlEfloatgR(void *p) {
      delete ((vector<float>*)p);
   }
   static void deleteArray_vectorlEfloatgR(void *p) {
      delete [] ((vector<float>*)p);
   }
   static void destruct_vectorlEfloatgR(void *p) {
      typedef vector<float> current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class vector<float>

namespace ROOT {
   static TClass *vectorlEdoublegR_Dictionary();
   static void vectorlEdoublegR_TClassManip(TClass*);
   static void *new_vectorlEdoublegR(void *p = 0);
   static void *newArray_vectorlEdoublegR(Long_t size, void *p);
   static void delete_vectorlEdoublegR(void *p);
   static void deleteArray_vectorlEdoublegR(void *p);
   static void destruct_vectorlEdoublegR(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const vector<double>*)
   {
      vector<double> *ptr = 0;
      static ::TVirtualIsAProxy* isa_proxy = new ::TIsAProxy(typeid(vector<double>));
      static ::ROOT::TGenericClassInfo 
         instance("vector<double>", -2, "vector", 477,
                  typeid(vector<double>), DefineBehavior(ptr, ptr),
                  &vectorlEdoublegR_Dictionary, isa_proxy, 4,
                  sizeof(vector<double>) );
      instance.SetNew(&new_vectorlEdoublegR);
      instance.SetNewArray(&newArray_vectorlEdoublegR);
      instance.SetDelete(&delete_vectorlEdoublegR);
      instance.SetDeleteArray(&deleteArray_vectorlEdoublegR);
      instance.SetDestructor(&destruct_vectorlEdoublegR);
      instance.AdoptCollectionProxyInfo(TCollectionProxyInfo::Generate(TCollectionProxyInfo::Pushback< vector<double> >()));
      return &instance;
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_(Init) = GenerateInitInstanceLocal((const vector<double>*)0x0); R__UseDummy(_R__UNIQUE_(Init));

   // Dictionary for non-ClassDef classes
   static TClass *vectorlEdoublegR_Dictionary() {
      TClass* theClass =::ROOT::GenerateInitInstanceLocal((const vector<double>*)0x0)->GetClass();
      vectorlEdoublegR_TClassManip(theClass);
   return theClass;
   }

   static void vectorlEdoublegR_TClassManip(TClass* ){
   }

} // end of namespace ROOT

namespace ROOT {
   // Wrappers around operator new
   static void *new_vectorlEdoublegR(void *p) {
      return  p ? ::new((::ROOT::TOperatorNewHelper*)p) vector<double> : new vector<double>;
   }
   static void *newArray_vectorlEdoublegR(Long_t nElements, void *p) {
      return p ? ::new((::ROOT::TOperatorNewHelper*)p) vector<double>[nElements] : new vector<double>[nElements];
   }
   // Wrapper around operator delete
   static void delete_vectorlEdoublegR(void *p) {
      delete ((vector<double>*)p);
   }
   static void deleteArray_vectorlEdoublegR(void *p) {
      delete [] ((vector<double>*)p);
   }
   static void destruct_vectorlEdoublegR(void *p) {
      typedef vector<double> current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class vector<double>

namespace ROOT {
   static TClass *vectorlEGenVertex_p5gR_Dictionary();
   static void vectorlEGenVertex_p5gR_TClassManip(TClass*);
   static void *new_vectorlEGenVertex_p5gR(void *p = 0);
   static void *newArray_vectorlEGenVertex_p5gR(Long_t size, void *p);
   static void delete_vectorlEGenVertex_p5gR(void *p);
   static void deleteArray_vectorlEGenVertex_p5gR(void *p);
   static void destruct_vectorlEGenVertex_p5gR(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const vector<GenVertex_p5>*)
   {
      vector<GenVertex_p5> *ptr = 0;
      static ::TVirtualIsAProxy* isa_proxy = new ::TIsAProxy(typeid(vector<GenVertex_p5>));
      static ::ROOT::TGenericClassInfo 
         instance("vector<GenVertex_p5>", -2, "vector", 477,
                  typeid(vector<GenVertex_p5>), DefineBehavior(ptr, ptr),
                  &vectorlEGenVertex_p5gR_Dictionary, isa_proxy, 4,
                  sizeof(vector<GenVertex_p5>) );
      instance.SetNew(&new_vectorlEGenVertex_p5gR);
      instance.SetNewArray(&newArray_vectorlEGenVertex_p5gR);
      instance.SetDelete(&delete_vectorlEGenVertex_p5gR);
      instance.SetDeleteArray(&deleteArray_vectorlEGenVertex_p5gR);
      instance.SetDestructor(&destruct_vectorlEGenVertex_p5gR);
      instance.AdoptCollectionProxyInfo(TCollectionProxyInfo::Generate(TCollectionProxyInfo::Pushback< vector<GenVertex_p5> >()));
      return &instance;
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_(Init) = GenerateInitInstanceLocal((const vector<GenVertex_p5>*)0x0); R__UseDummy(_R__UNIQUE_(Init));

   // Dictionary for non-ClassDef classes
   static TClass *vectorlEGenVertex_p5gR_Dictionary() {
      TClass* theClass =::ROOT::GenerateInitInstanceLocal((const vector<GenVertex_p5>*)0x0)->GetClass();
      vectorlEGenVertex_p5gR_TClassManip(theClass);
   return theClass;
   }

   static void vectorlEGenVertex_p5gR_TClassManip(TClass* ){
   }

} // end of namespace ROOT

namespace ROOT {
   // Wrappers around operator new
   static void *new_vectorlEGenVertex_p5gR(void *p) {
      return  p ? ::new((::ROOT::TOperatorNewHelper*)p) vector<GenVertex_p5> : new vector<GenVertex_p5>;
   }
   static void *newArray_vectorlEGenVertex_p5gR(Long_t nElements, void *p) {
      return p ? ::new((::ROOT::TOperatorNewHelper*)p) vector<GenVertex_p5>[nElements] : new vector<GenVertex_p5>[nElements];
   }
   // Wrapper around operator delete
   static void delete_vectorlEGenVertex_p5gR(void *p) {
      delete ((vector<GenVertex_p5>*)p);
   }
   static void deleteArray_vectorlEGenVertex_p5gR(void *p) {
      delete [] ((vector<GenVertex_p5>*)p);
   }
   static void destruct_vectorlEGenVertex_p5gR(void *p) {
      typedef vector<GenVertex_p5> current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class vector<GenVertex_p5>

namespace ROOT {
   static TClass *vectorlEGenParticle_p5gR_Dictionary();
   static void vectorlEGenParticle_p5gR_TClassManip(TClass*);
   static void *new_vectorlEGenParticle_p5gR(void *p = 0);
   static void *newArray_vectorlEGenParticle_p5gR(Long_t size, void *p);
   static void delete_vectorlEGenParticle_p5gR(void *p);
   static void deleteArray_vectorlEGenParticle_p5gR(void *p);
   static void destruct_vectorlEGenParticle_p5gR(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const vector<GenParticle_p5>*)
   {
      vector<GenParticle_p5> *ptr = 0;
      static ::TVirtualIsAProxy* isa_proxy = new ::TIsAProxy(typeid(vector<GenParticle_p5>));
      static ::ROOT::TGenericClassInfo 
         instance("vector<GenParticle_p5>", -2, "vector", 477,
                  typeid(vector<GenParticle_p5>), DefineBehavior(ptr, ptr),
                  &vectorlEGenParticle_p5gR_Dictionary, isa_proxy, 4,
                  sizeof(vector<GenParticle_p5>) );
      instance.SetNew(&new_vectorlEGenParticle_p5gR);
      instance.SetNewArray(&newArray_vectorlEGenParticle_p5gR);
      instance.SetDelete(&delete_vectorlEGenParticle_p5gR);
      instance.SetDeleteArray(&deleteArray_vectorlEGenParticle_p5gR);
      instance.SetDestructor(&destruct_vectorlEGenParticle_p5gR);
      instance.AdoptCollectionProxyInfo(TCollectionProxyInfo::Generate(TCollectionProxyInfo::Pushback< vector<GenParticle_p5> >()));
      return &instance;
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_(Init) = GenerateInitInstanceLocal((const vector<GenParticle_p5>*)0x0); R__UseDummy(_R__UNIQUE_(Init));

   // Dictionary for non-ClassDef classes
   static TClass *vectorlEGenParticle_p5gR_Dictionary() {
      TClass* theClass =::ROOT::GenerateInitInstanceLocal((const vector<GenParticle_p5>*)0x0)->GetClass();
      vectorlEGenParticle_p5gR_TClassManip(theClass);
   return theClass;
   }

   static void vectorlEGenParticle_p5gR_TClassManip(TClass* ){
   }

} // end of namespace ROOT

namespace ROOT {
   // Wrappers around operator new
   static void *new_vectorlEGenParticle_p5gR(void *p) {
      return  p ? ::new((::ROOT::TOperatorNewHelper*)p) vector<GenParticle_p5> : new vector<GenParticle_p5>;
   }
   static void *newArray_vectorlEGenParticle_p5gR(Long_t nElements, void *p) {
      return p ? ::new((::ROOT::TOperatorNewHelper*)p) vector<GenParticle_p5>[nElements] : new vector<GenParticle_p5>[nElements];
   }
   // Wrapper around operator delete
   static void delete_vectorlEGenParticle_p5gR(void *p) {
      delete ((vector<GenParticle_p5>*)p);
   }
   static void deleteArray_vectorlEGenParticle_p5gR(void *p) {
      delete [] ((vector<GenParticle_p5>*)p);
   }
   static void destruct_vectorlEGenParticle_p5gR(void *p) {
      typedef vector<GenParticle_p5> current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class vector<GenParticle_p5>

namespace ROOT {
   static TClass *vectorlEGenEvent_p5gR_Dictionary();
   static void vectorlEGenEvent_p5gR_TClassManip(TClass*);
   static void *new_vectorlEGenEvent_p5gR(void *p = 0);
   static void *newArray_vectorlEGenEvent_p5gR(Long_t size, void *p);
   static void delete_vectorlEGenEvent_p5gR(void *p);
   static void deleteArray_vectorlEGenEvent_p5gR(void *p);
   static void destruct_vectorlEGenEvent_p5gR(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const vector<GenEvent_p5>*)
   {
      vector<GenEvent_p5> *ptr = 0;
      static ::TVirtualIsAProxy* isa_proxy = new ::TIsAProxy(typeid(vector<GenEvent_p5>));
      static ::ROOT::TGenericClassInfo 
         instance("vector<GenEvent_p5>", -2, "vector", 477,
                  typeid(vector<GenEvent_p5>), DefineBehavior(ptr, ptr),
                  &vectorlEGenEvent_p5gR_Dictionary, isa_proxy, 4,
                  sizeof(vector<GenEvent_p5>) );
      instance.SetNew(&new_vectorlEGenEvent_p5gR);
      instance.SetNewArray(&newArray_vectorlEGenEvent_p5gR);
      instance.SetDelete(&delete_vectorlEGenEvent_p5gR);
      instance.SetDeleteArray(&deleteArray_vectorlEGenEvent_p5gR);
      instance.SetDestructor(&destruct_vectorlEGenEvent_p5gR);
      instance.AdoptCollectionProxyInfo(TCollectionProxyInfo::Generate(TCollectionProxyInfo::Pushback< vector<GenEvent_p5> >()));
      return &instance;
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_(Init) = GenerateInitInstanceLocal((const vector<GenEvent_p5>*)0x0); R__UseDummy(_R__UNIQUE_(Init));

   // Dictionary for non-ClassDef classes
   static TClass *vectorlEGenEvent_p5gR_Dictionary() {
      TClass* theClass =::ROOT::GenerateInitInstanceLocal((const vector<GenEvent_p5>*)0x0)->GetClass();
      vectorlEGenEvent_p5gR_TClassManip(theClass);
   return theClass;
   }

   static void vectorlEGenEvent_p5gR_TClassManip(TClass* ){
   }

} // end of namespace ROOT

namespace ROOT {
   // Wrappers around operator new
   static void *new_vectorlEGenEvent_p5gR(void *p) {
      return  p ? ::new((::ROOT::TOperatorNewHelper*)p) vector<GenEvent_p5> : new vector<GenEvent_p5>;
   }
   static void *newArray_vectorlEGenEvent_p5gR(Long_t nElements, void *p) {
      return p ? ::new((::ROOT::TOperatorNewHelper*)p) vector<GenEvent_p5>[nElements] : new vector<GenEvent_p5>[nElements];
   }
   // Wrapper around operator delete
   static void delete_vectorlEGenEvent_p5gR(void *p) {
      delete ((vector<GenEvent_p5>*)p);
   }
   static void deleteArray_vectorlEGenEvent_p5gR(void *p) {
      delete [] ((vector<GenEvent_p5>*)p);
   }
   static void destruct_vectorlEGenEvent_p5gR(void *p) {
      typedef vector<GenEvent_p5> current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class vector<GenEvent_p5>

namespace {
  void TriggerDictionaryInitialization_McEventCollection_p5_dict_Impl() {
    static const char* headers[] = {
"generator/GenParticle_p5.h",
"generator/GenEvent_p5.h",
"generator/GenVertex_p5.h",
"generator/McEventCollection_p5.h",
0
    };
    static const char* includePaths[] = {
"/Users/caminal/.ROOTCERN/root-v6-02/include",
"/Users/caminal/WorkArea/H4bAnalysis/",
0
    };
    static const char* fwdDeclCode = 
R"DICTFWDDCLS(
#pragma clang diagnostic ignored "-Wkeyword-compat"
#pragma clang diagnostic ignored "-Wignored-attributes"
#pragma clang diagnostic ignored "-Wreturn-type-c-linkage"
extern int __Cling_Autoloading_Map;
namespace std{inline namespace __1{template <class _T1, class _T2> struct __attribute__((annotate("$clingAutoload$string")))  pair;
}}
namespace std{inline namespace __1{template <class _Tp> class __attribute__((annotate("$clingAutoload$string")))  allocator;
}}
class __attribute__((annotate("$clingAutoload$links/McEventCollection_p5_linkdef.h")))  GenEvent_p5;
class __attribute__((annotate("$clingAutoload$links/McEventCollection_p5_linkdef.h")))  GenVertex_p5;
class __attribute__((annotate("$clingAutoload$links/McEventCollection_p5_linkdef.h")))  GenParticle_p5;
class __attribute__((annotate("$clingAutoload$links/McEventCollection_p5_linkdef.h")))  McEventCollection_p5;
)DICTFWDDCLS";
    static const char* payloadCode = R"DICTPAYLOAD(

#ifndef G__VECTOR_HAS_CLASS_ITERATOR
  #define G__VECTOR_HAS_CLASS_ITERATOR 1
#endif

#define _BACKWARD_BACKWARD_WARNING_H
#include "generator/GenParticle_p5.h"
#include "generator/GenEvent_p5.h"
#include "generator/GenVertex_p5.h"
#include "generator/McEventCollection_p5.h"

#undef  _BACKWARD_BACKWARD_WARNING_H
)DICTPAYLOAD";
    static const char* classesHeaders[]={
"GenEvent_p5", payloadCode, "@",
"GenParticle_p5", payloadCode, "@",
"GenVertex_p5", payloadCode, "@",
"McEventCollection_p5", payloadCode, "@",
"vector<GenEvent_p5>", payloadCode, "@",
"vector<GenParticle_p5>", payloadCode, "@",
"vector<GenVertex_p5>", payloadCode, "@",
"vector<double>", payloadCode, "@",
"vector<float>", payloadCode, "@",
"vector<long int>", payloadCode, "@",
"vector<long>", payloadCode, "@",
"vector<pair<int,int> >", payloadCode, "@",
"vector<std::pair<int,int> >", payloadCode, "@",
nullptr};

    static bool isInitialized = false;
    if (!isInitialized) {
      TROOT::RegisterModule("McEventCollection_p5_dict",
        headers, includePaths, payloadCode, fwdDeclCode,
        TriggerDictionaryInitialization_McEventCollection_p5_dict_Impl, {}, classesHeaders);
      isInitialized = true;
    }
  }
  static struct DictInit {
    DictInit() {
      TriggerDictionaryInitialization_McEventCollection_p5_dict_Impl();
    }
  } __TheDictionaryInitializer;
}
void TriggerDictionaryInitialization_McEventCollection_p5_dict() {
  TriggerDictionaryInitialization_McEventCollection_p5_dict_Impl();
}
