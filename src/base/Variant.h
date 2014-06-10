/*
 * type.h
 *
 *  Created on: 2013-11-03
 *      Author: kin
 */
#pragma once


typedef uint64 nuiAttributeType;

uint64 nuiGetNewAttributeUniqueId();

template <class Type>
class nuiAttributeTypeTrait
{
public:
  static uint64 mTypeId;
};


#define NGL_ASSERT assert


#define GCC_VERSION (__GNUC__ * 10000 \
                               + __GNUC_MINOR__ * 100 \
                               + __GNUC_PATCHLEVEL__)
          /* Test for GCC > 3.2.0 */
          //#if GCC_VERSION > 30200

#if defined __clang__ || GCC_VERSION > 40600
template <> uint64 nuiAttributeTypeTrait<bool>::mTypeId;

template <> uint64 nuiAttributeTypeTrait<int8>::mTypeId;
template <> uint64 nuiAttributeTypeTrait<int16>::mTypeId;
template <> uint64 nuiAttributeTypeTrait<int32>::mTypeId;
template <> uint64 nuiAttributeTypeTrait<int64>::mTypeId;

template <> uint64 nuiAttributeTypeTrait<uint8>::mTypeId;
template <> uint64 nuiAttributeTypeTrait<uint16>::mTypeId;
template <> uint64 nuiAttributeTypeTrait<uint32>::mTypeId;
template <> uint64 nuiAttributeTypeTrait<uint64>::mTypeId;

template <> uint64 nuiAttributeTypeTrait<float>::mTypeId;
template <> uint64 nuiAttributeTypeTrait<double>::mTypeId;



class nuiVariant
{
public:
  nuiVariant()
  {
    mIsPointer = false;
    mIsObject = false;
    mIsPOD = false;
    mIsArray = false;
    mType = nuiAttributeTypeTrait<void>::mTypeId;
  }

  // POD CTOR
  template <typename Type>
  nuiVariant(Type data)
  {
    mIsPointer = is_pointer<Type>::value;
    mIsObject = is_base_of<nuiObject, Type>::value;
    mType = nuiAttributeTypeTrait<Type>::mTypeId;
    mIsPOD = false;
    mIsArray = false;
    NGL_ASSERT(0);
  }

  template <typename Type>
  nuiVariant(const std::vector<Type>& data)
  {
    mIsPointer = false;
    mIsObject = false;
    mType = 0;
    mIsPOD = false;
    mIsArray = true;
    mData.mpArray = new std::vector<nuiVariant>();
    mData.mpArray->reserve(data.size());
    for (int32 i = 0; i < data.size(); i++)
      mData.mpArray->push_back(data[i]);
  }

#define CTOR(TYPE)\
  nuiVariant(TYPE data)\
  {\
    mIsPointer = is_pointer<TYPE>::value;\
    mIsObject = is_base_of<nuiObject, TYPE>::value;\
    mType = nuiAttributeTypeTrait<TYPE>::mTypeId;\
    mIsPOD = true;\
    mIsArray = false;\
    if (is_same_type<TYPE, int8>::value || is_same_type<TYPE, int16>::value
|| is_same_type<TYPE, int32>::value || is_same_type<TYPE, int64>::value)\
    {\
      mData.mInt = (int64)data;\
    }\
    else if (is_same_type<TYPE, uint8>::value || is_same_type<TYPE, uint16>::
value || is_same_type<TYPE, uint32>::value || is_same_type<TYPE, uint64>::
value)\
    {\
      mData.mUInt = (uint64)data;\
    }\
    else if (is_same_type<TYPE, float>::value || is_same_type<TYPE, double>::
value)\
    {\
      mData.mFloat = (double)data;\
    }\
  }

  CTOR(int8);
  CTOR(int32);
  CTOR(int64);

  CTOR(uint8);
  CTOR(uint32);
  CTOR(uint64);

  CTOR(float);
  CTOR(double);

#undef CTOR


  nuiVariant(bool set)
  {
    mIsPointer = false;
    mIsObject = false;
    mType = nuiAttributeTypeTrait<bool>::mTypeId;
    mIsPOD = true;
    mIsArray = false;

    mData.mBool = set;
  }


  // Copy CTOR
  nuiVariant(const nuiVariant& rObject)
  {
    mData = rObject.mData;

    mString = rObject.mString;
    mColor = rObject.mColor;
    mRect = rObject.mRect;

    mType = rObject.mType;

    mIsPointer = rObject.mIsPointer;
    mIsObject = rObject.mIsObject;
    mIsPOD = rObject.mIsPOD;
    mIsArray = rObject.mIsArray;

    if (mIsObject)
      mData.mpObject->Acquire();

    if (mIsArray)
      mData.mpArray = new std::vector<nuiVariant>(*rObject.mData.mpArray);
  }

  // Pointer CTOR
  template <typename Type>
  nuiVariant(Type* pData)
  {
    mIsPointer = true;
    mIsObject = is_base_of<nuiObject, Type>::value;
    mType = nuiAttributeTypeTrait<Type*>::mTypeId;
    mData.mpPointer = (void*)pData;
    mIsPOD = false;
    mIsArray = false;

    if (mIsObject)
      mData.mpObject->Acquire();
  }


  // DTOR:
  ~nuiVariant()
  {

  }

  nuiVariant& operator=(const nuiVariant& rObject)
  {
    if (mIsObject)
      mData.mpObject->Release();
    if (mIsArray)
      delete mData.mpArray;

    mData = rObject.mData;

    mString = rObject.mString;
    mColor = rObject.mColor;
    mRect = rObject.mRect;

    mType = rObject.mType;

    mIsPointer = rObject.mIsPointer;
    mIsObject = rObject.mIsObject;
    mIsPOD = rObject.mIsPOD;
    mIsArray = rObject.mIsArray;

    if (mIsObject)
      mData.mpObject->Acquire();

    if (mIsArray)
      mData.mpArray = new std::vector<nuiVariant>(*rObject.mData.mpArray);
    return *this;
  }

  nuiAttributeType GetType() const
  {
    return mType;
  }

  void Clear()
  {
    if (mIsObject)
      mData.mpObject->Release();
    if (mIsArray)
      delete mData.mpArray;

    mIsPointer = false;
    mIsObject = false;
    mIsPOD = false;
    mIsArray = false;
    mType = nuiAttributeTypeTrait<void>::mTypeId;
  }

  bool IsVoid() const
  {
    return mType == nuiAttributeTypeTrait<void>::mTypeId;
  }

  bool IsPointer() const
  {
    return mIsPointer;
  }

  bool IsPOD() const
  {
    return mIsPOD;
  }


  // POD Cast:
#define CAST(TYPE)\
operator TYPE() const\
  {\
    if (nuiAttributeTypeTrait<TYPE>::mTypeId == nuiAttributeTypeTrait<float>::
mTypeId\
        || nuiAttributeTypeTrait<TYPE>::mTypeId == nuiAttributeTypeTrait<
double>::mTypeId)\
    {\
      if (mType == nuiAttributeTypeTrait<float>::mTypeId || mType ==
nuiAttributeTypeTrait<double>::mTypeId)\
        return static_cast<TYPE>(mData.mFloat);\
      else if (mType == nuiAttributeTypeTrait<uint8>::mTypeId || mType ==
nuiAttributeTypeTrait<uint16>::mTypeId ||mType == nuiAttributeTypeTrait<uint32
>::mTypeId || mType == nuiAttributeTypeTrait<uint64>::mTypeId) \
        return static_cast<TYPE>(mData.mUInt); \
      else if (mType == nuiAttributeTypeTrait<int8>::mTypeId || mType ==
nuiAttributeTypeTrait<int16>::mTypeId ||mType == nuiAttributeTypeTrait<int32>
::mTypeId || mType == nuiAttributeTypeTrait<int64>::mTypeId) \
        return static_cast<TYPE>(mData.mInt); \
      else if (mType == nuiAttributeTypeTrait<nglString>::mTypeId)\
        return static_cast<TYPE>(mString.GetCDouble());\
    }\
    else if (nuiAttributeTypeTrait<TYPE>::mTypeId == nuiAttributeTypeTrait<
int8>::mTypeId \
             || nuiAttributeTypeTrait<TYPE>::mTypeId == nuiAttributeTypeTrait<
int16>::mTypeId\
             || nuiAttributeTypeTrait<TYPE>::mTypeId == nuiAttributeTypeTrait<
int32>::mTypeId\
             || nuiAttributeTypeTrait<TYPE>::mTypeId == nuiAttributeTypeTrait<
int64>::mTypeId)\
    {\
      if (mType == nuiAttributeTypeTrait<uint8>::mTypeId || mType ==
nuiAttributeTypeTrait<uint16>::mTypeId ||mType == nuiAttributeTypeTrait<uint32
>::mTypeId || mType == nuiAttributeTypeTrait<uint64>::mTypeId\
          || mType == nuiAttributeTypeTrait<int8>::mTypeId || mType ==
nuiAttributeTypeTrait<int16>::mTypeId ||mType == nuiAttributeTypeTrait<int32>
::mTypeId || mType == nuiAttributeTypeTrait<int64>::mTypeId)\
        return static_cast<TYPE>(mData.mInt);\
      if (mType == nuiAttributeTypeTrait<float>::mTypeId || mType ==
nuiAttributeTypeTrait<double>::mTypeId)\
        return static_cast<TYPE>(ToBelow(mData.mFloat));\
      else if (mType == nuiAttributeTypeTrait<nglString>::mTypeId)\
          return static_cast<TYPE>(mString.GetCInt64());\
    }\
    else if (nuiAttributeTypeTrait<TYPE>::mTypeId == nuiAttributeTypeTrait<
uint8>::mTypeId\
             || nuiAttributeTypeTrait<TYPE>::mTypeId == nuiAttributeTypeTrait<
uint16>::mTypeId\
             || nuiAttributeTypeTrait<TYPE>::mTypeId == nuiAttributeTypeTrait<
uint32>::mTypeId\
             || nuiAttributeTypeTrait<TYPE>::mTypeId == nuiAttributeTypeTrait<
uint64>::mTypeId)\
    {\
      if (mType == nuiAttributeTypeTrait<uint8>::mTypeId || mType ==
nuiAttributeTypeTrait<uint16>::mTypeId ||mType == nuiAttributeTypeTrait<uint32
>::mTypeId || mType == nuiAttributeTypeTrait<uint64>::mTypeId\
          || mType == nuiAttributeTypeTrait<int8>::mTypeId || mType ==
nuiAttributeTypeTrait<int16>::mTypeId ||mType == nuiAttributeTypeTrait<int32>
::mTypeId || mType == nuiAttributeTypeTrait<int64>::mTypeId)\
        return static_cast<TYPE>(mData.mUInt);\
      if (mType == nuiAttributeTypeTrait<float>::mTypeId || mType ==
nuiAttributeTypeTrait<double>::mTypeId)\
        return static_cast<TYPE>(ToBelow(mData.mFloat));\
      else if (mType == nuiAttributeTypeTrait<nglString>::mTypeId)\
        return static_cast<TYPE>(mString.GetCUInt64());\
    }\
    NGL_ASSERT(0);\
    return static_cast<TYPE>(0);\
  }

  CAST(int8);
  CAST(int16);
  CAST(int32);
  CAST(int64);

  CAST(uint8);
  CAST(uint16);
  CAST(uint32);
  CAST(uint64);

  CAST(float);
  CAST(double);

#undef CAST



  operator bool() const
  {
    if (mType == nuiAttributeTypeTrait<bool>::mTypeId)
      return mData.mBool;

    if (mType == nuiAttributeTypeTrait<float>::mTypeId
        || mType == nuiAttributeTypeTrait<double>::mTypeId)
    {
      return (bool)(mData.mFloat != 0);
    }
    else if (mType == nuiAttributeTypeTrait<int8>::mTypeId
             || mType == nuiAttributeTypeTrait<int16>::mTypeId
             || mType == nuiAttributeTypeTrait<int32>::mTypeId
             || mType == nuiAttributeTypeTrait<int64>::mTypeId)
    {
      return (bool)(mData.mInt != 0);
    }
    else if (mType == nuiAttributeTypeTrait<uint8>::mTypeId
             || mType == nuiAttributeTypeTrait<uint16>::mTypeId
             || mType == nuiAttributeTypeTrait<uint32>::mTypeId
             || mType == nuiAttributeTypeTrait<uint64>::mTypeId)
    {
      return (bool)(mData.mUInt != 0);
    }

    return false;
  }

private:
  nuiAttributeType mType;
  union
  {
    int64 mInt;
    uint64 mUInt;
    double mFloat;
    bool mBool;

    void* mpPointer;
    std::vector<nuiVariant>* mpArray;
  } mData;

  bool mIsPointer : 1;
  bool mIsObject : 1;
  bool mIsPOD : 1;
  bool mIsArray : 1;
};


