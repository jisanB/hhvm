/*
   +----------------------------------------------------------------------+
   | HipHop for PHP                                                       |
   +----------------------------------------------------------------------+
   | Copyright (c) 2010-2014 Facebook, Inc. (http://www.facebook.com)     |
   +----------------------------------------------------------------------+
   | This source file is subject to version 3.01 of the PHP license,      |
   | that is bundled with this package in the file LICENSE, and is        |
   | available through the world-wide-web at the following url:           |
   | http://www.php.net/license/3_01.txt                                  |
   | If you did not receive a copy of the PHP license and are unable to   |
   | obtain it through the world-wide-web, please send a note to          |
   | license@php.net so we can mail you a copy immediately.               |
   +----------------------------------------------------------------------+
*/

#ifndef incl_HPHP_ARRAY_DEFS_H_
#define incl_HPHP_ARRAY_DEFS_H_

#include "hphp/runtime/base/array-data.h"
#include <algorithm>
#include "hphp/runtime/base/complex-types.h"

namespace HPHP {
///////////////////////////////////////////////////////////////////////////////

namespace {
inline bool isIntKey(const Cell* cell) {
  return IS_INT_KEY_TYPE(cell->m_type);
}

inline int64_t getIntKey(const Cell* cell) {
  assert(cell->m_type == KindOfInt64);
  return cell->m_data.num;
}

inline StringData* getStringKey(const Cell* cell) {
  assert(IS_STRING_TYPE(cell->m_type));
  return cell->m_data.pstr;
}
}

inline bool ArrayData::exists(const String& k) const {
  assert(IsValidKey(k));
  return exists(k.get());
}

inline bool ArrayData::exists(const Variant& k) const {
  assert(IsValidKey(k));
  auto const cell = k.asCell();
  return isIntKey(cell) ? exists(getIntKey(cell))
                        : exists(getStringKey(cell));
}

inline const Variant& ArrayData::get(const String& k, bool error) const {
  assert(IsValidKey(k));
  return get(k.get(), error);
}

inline const Variant& ArrayData::get(int64_t k, bool error) const {
  auto tv = nvGet(k);
  return tv ? tvAsCVarRef(tv) : getNotFound(k, error);
}

inline const Variant& ArrayData::get(const StringData* k, bool error) const {
  auto tv = nvGet(k);
  return tv ? tvAsCVarRef(tv) : getNotFound(k, error);
}

inline ArrayData* ArrayData::lval(const String& k, Variant *&ret, bool copy) {
  assert(IsValidKey(k));
  return lval(k.get(), ret, copy);
}

inline ArrayData* ArrayData::lval(const Variant& k, Variant *&ret, bool copy) {
  assert(IsValidKey(k));
  auto const cell = k.asCell();
  return isIntKey(cell) ? lval(getIntKey(cell), ret, copy)
                        : lval(getStringKey(cell), ret, copy);
}

inline ArrayData* ArrayData::set(const String& k, const Variant& v,
                                 bool copy) {
  assert(IsValidKey(k));
  return set(k.get(), v, copy);
}

inline ArrayData* ArrayData::set(const Variant& k, const Variant& v, bool copy) {
  assert(IsValidKey(k));
  auto const cell = k.asCell();
  return isIntKey(cell) ? set(getIntKey(cell), v, copy)
                        : set(getStringKey(cell), v, copy);
}

inline ArrayData* ArrayData::setRef(const String& k, Variant& v, bool copy) {
  assert(IsValidKey(k));
  return setRef(k.get(), v, copy);
}

inline ArrayData* ArrayData::setRef(const Variant& k, Variant& v, bool copy) {
  assert(IsValidKey(k));
  auto const cell = k.asCell();
  return isIntKey(cell) ? setRef(getIntKey(cell), v, copy)
                        : setRef(getStringKey(cell), v, copy);
}

inline ArrayData* ArrayData::add(const String& k, const Variant& v, bool copy) {
  assert(IsValidKey(k));
  return add(k.get(), v, copy);
}

inline ArrayData* ArrayData::add(const Variant& k, const Variant& v, bool copy) {
  assert(IsValidKey(k));
  auto const cell = k.asCell();
  return isIntKey(cell) ? add(getIntKey(cell), v, copy)
                        : add(getStringKey(cell), v, copy);
}

inline ArrayData* ArrayData::remove(const String& k, bool copy) {
  assert(IsValidKey(k));
  return remove(k.get(), copy);
}

inline ArrayData* ArrayData::remove(const Variant& k, bool copy) {
  assert(IsValidKey(k));
  auto const cell = k.asCell();
  return isIntKey(cell) ? remove(getIntKey(cell), copy)
                        : remove(getStringKey(cell), copy);
}

inline Variant ArrayData::getValue(ssize_t pos) const {
  return getValueRef(pos);
}

inline Variant ArrayData::getKey(ssize_t pos) const {
  TypedValue tv;
  nvGetKey(&tv, pos);
  return std::move(tvAsVariant(&tv));
}

///////////////////////////////////////////////////////////////////////////////

inline void ArrayData::release() {
  return g_array_funcs.release[m_kind](this);
}

inline ArrayData* ArrayData::append(const Variant& v, bool copy) {
  return g_array_funcs.append[m_kind](this, v, copy);
}

inline ArrayData* ArrayData::appendRef(Variant& v, bool copy) {
  return g_array_funcs.appendRef[m_kind](this, v, copy);
}

inline ArrayData* ArrayData::appendWithRef(const Variant& v, bool copy) {
  return g_array_funcs.appendWithRef[m_kind](this, v, copy);
}

inline const TypedValue* ArrayData::nvGet(int64_t ikey) const {
  return g_array_funcs.nvGetInt[m_kind](this, ikey);
}

inline const TypedValue* ArrayData::nvGetConverted(int64_t ikey) const {
  return g_array_funcs.nvGetIntConverted[m_kind](this, ikey);
}

inline const TypedValue* ArrayData::nvGet(const StringData* skey) const {
  return g_array_funcs.nvGetStr[m_kind](this, skey);
}

inline void ArrayData::nvGetKey(TypedValue* out, ssize_t pos) const {
  g_array_funcs.nvGetKey[m_kind](this, out, pos);
}

inline ArrayData* ArrayData::set(int64_t k, const Variant& v, bool copy) {
  return g_array_funcs.setInt[m_kind](this, k, *v.asCell(), copy);
}

inline ArrayData* ArrayData::setConverted(int64_t k, const Variant& v,
                                          bool copy) {
  return g_array_funcs.setIntConverted[m_kind](this, k, *v.asCell(), copy);
}

inline ArrayData* ArrayData::set(StringData* k, const Variant& v, bool copy) {
  return g_array_funcs.setStr[m_kind](this, k, *v.asCell(), copy);
}

inline ArrayData* ArrayData::zSet(int64_t k, RefData* v) {
  return g_array_funcs.zSetInt[m_kind](this, k, v);
}

inline ArrayData* ArrayData::zSet(StringData* k, RefData* v) {
  return g_array_funcs.zSetStr[m_kind](this, k, v);
}

inline ArrayData* ArrayData::zAppend(RefData* v, int64_t* key_ptr) {
  return g_array_funcs.zAppend[m_kind](this, v, key_ptr);
}

inline size_t ArrayData::vsize() const {
  return g_array_funcs.vsize[m_kind](this);
}

inline const Variant& ArrayData::getValueRef(ssize_t pos) const {
  return g_array_funcs.getValueRef[m_kind](this, pos);
}

inline bool ArrayData::noCopyOnWrite() const {
  // GlobalsArray doesn't support COW.
  return m_kind == kGlobalsKind;
}

inline bool ArrayData::isVectorData() const {
  return g_array_funcs.isVectorData[m_kind](this);
}

inline bool ArrayData::exists(int64_t k) const {
  return g_array_funcs.existsInt[m_kind](this, k);
}

inline bool ArrayData::exists(const StringData* k) const {
  return g_array_funcs.existsStr[m_kind](this, k);
}

inline ArrayData* ArrayData::lval(int64_t k, Variant*& ret, bool copy) {
  return g_array_funcs.lvalInt[m_kind](this, k, ret, copy);
}

inline ArrayData* ArrayData::lval(StringData* k, Variant*& ret, bool copy) {
  return g_array_funcs.lvalStr[m_kind](this, k, ret, copy);
}

inline ArrayData* ArrayData::lvalNew(Variant*& ret, bool copy) {
  return g_array_funcs.lvalNew[m_kind](this, ret, copy);
}

inline ArrayData* ArrayData::lvalNewRef(Variant*& ret, bool copy) {
  return g_array_funcs.lvalNewRef[m_kind](this, ret, copy);
}

inline ArrayData* ArrayData::setRef(int64_t k, Variant& v, bool copy) {
  return g_array_funcs.setRefInt[m_kind](this, k, v, copy);
}

inline ArrayData* ArrayData::setRef(StringData* k, Variant& v, bool copy) {
  return g_array_funcs.setRefStr[m_kind](this, k, v, copy);
}

inline ArrayData* ArrayData::add(int64_t k, const Variant& v, bool copy) {
  return g_array_funcs.addInt[m_kind](this, k, *v.asCell(), copy);
}

inline ArrayData* ArrayData::add(StringData* k, const Variant& v, bool copy) {
  return g_array_funcs.addStr[m_kind](this, k, *v.asCell(), copy);
}

inline ArrayData* ArrayData::remove(int64_t k, bool copy) {
  return g_array_funcs.removeInt[m_kind](this, k, copy);
}

inline ArrayData* ArrayData::remove(const StringData* k, bool copy) {
  return g_array_funcs.removeStr[m_kind](this, k, copy);
}

inline ssize_t ArrayData::iter_begin() const {
  return g_array_funcs.iterBegin[m_kind](this);
}

inline ssize_t ArrayData::iter_last() const {
  return g_array_funcs.iterLast[m_kind](this);
}

inline ssize_t ArrayData::iter_end() const {
  return g_array_funcs.iterEnd[m_kind](this);
}

inline ssize_t ArrayData::iter_advance(ssize_t pos) const {
  return g_array_funcs.iterAdvance[m_kind](this, pos);
}

inline ssize_t ArrayData::iter_rewind(ssize_t pos) const {
  return g_array_funcs.iterRewind[m_kind](this, pos);
}

inline bool ArrayData::validMArrayIter(const MArrayIter& fp) const {
  return g_array_funcs.validMArrayIter[m_kind](this, fp);
}

inline bool ArrayData::advanceMArrayIter(MArrayIter& fp) {
  return g_array_funcs.advanceMArrayIter[m_kind](this, fp);
}

inline ArrayData* ArrayData::escalateForSort() {
  return g_array_funcs.escalateForSort[m_kind](this);
}

inline void ArrayData::ksort(int sort_flags, bool ascending) {
  return g_array_funcs.ksort[m_kind](this, sort_flags, ascending);
}

inline void ArrayData::sort(int sort_flags, bool ascending) {
  return g_array_funcs.sort[m_kind](this, sort_flags, ascending);
}

inline void ArrayData::asort(int sort_flags, bool ascending) {
  return g_array_funcs.asort[m_kind](this, sort_flags, ascending);
}

inline bool ArrayData::uksort(const Variant& compare) {
  return g_array_funcs.uksort[m_kind](this, compare);
}

inline bool ArrayData::usort(const Variant& compare) {
  return g_array_funcs.usort[m_kind](this, compare);
}

inline bool ArrayData::uasort(const Variant& compare) {
  return g_array_funcs.uasort[m_kind](this, compare);
}

inline ArrayData* ArrayData::copy() const {
  return g_array_funcs.copy[m_kind](this);
}

inline ArrayData* ArrayData::copyWithStrongIterators() const {
  return g_array_funcs.copyWithStrongIterators[m_kind](this);
}

inline ArrayData* ArrayData::nonSmartCopy() const {
  return g_array_funcs.nonSmartCopy[m_kind](this);
}

inline ArrayData* ArrayData::pop(Variant& value) {
  return g_array_funcs.pop[m_kind](this, value);
}

inline ArrayData* ArrayData::dequeue(Variant& value) {
  return g_array_funcs.dequeue[m_kind](this, value);
}

inline ArrayData* ArrayData::prepend(const Variant& value, bool copy) {
  return g_array_funcs.prepend[m_kind](this, value, copy);
}

inline void ArrayData::renumber() {
  return g_array_funcs.renumber[m_kind](this);
}

inline void ArrayData::onSetEvalScalar() {
  return g_array_funcs.onSetEvalScalar[m_kind](this);
}

inline ArrayData* ArrayData::escalate() const {
  return g_array_funcs.escalate[m_kind](this);
}

inline ArrayData* ArrayData::plusEq(const ArrayData* elms) {
  return g_array_funcs.plusEq[m_kind](this, elms);
}

inline ArrayData* ArrayData::merge(const ArrayData* elms) {
  return g_array_funcs.merge[m_kind](this, elms);
}

///////////////////////////////////////////////////////////////////////////////
}

#endif // incl_HPHP_ARRAY_DEFS_H_
