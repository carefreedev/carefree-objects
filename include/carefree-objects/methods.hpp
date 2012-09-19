/* carefree-objects
 *
 * a thread-safe object manager extension for c++
 *
 * Copyright (C) 2011-2012 Stefan Zimmermann <zimmermann.code@googlemail.com>
 *
 * carefree-objects is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * carefree-objects is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public License
 * along with carefree-objects.  If not, see <http://www.gnu.org/licenses/>.
 */

#ifndef __CFO_METHODS_HPP
#define __CFO_METHODS_HPP

#include "common.hpp"

#include "accessor.hpp"

namespace cfo { namespace intern
{
  template<typename T>
  class methods<T, true> : public accessor<T>
  {
  private:
    methods(const methods<T, true>&);

  protected:
    template<bool SYNC_switch>
    using sync = methods<T, SYNC_switch>;

  public:
    inline methods(const managed<T, true, false> &manager) :
      accessor<T>(manager)
    {}

    inline methods(methods<T, true> &&methods) :
      accessor<T>(static_cast<accessor<T>&&>(methods))
    {}
  };

  template<typename T>
  class methods<T, false> : public const_methods<T, false>
  {
  private:
    typedef const_methods<T, false> basic_type;

  protected:
    template<bool SYNC_switch>
    using sync = methods<T, SYNC_switch>;

    inline methods(T *obj) :
      basic_type(obj)
    {}

    inline methods(const std::shared_ptr<T> &manager) :
      basic_type(manager)
    {}

    inline methods(const methods<T, false> &methods) :
      basic_type(methods)
    {}

  public:
    inline const T* operator->() const
    {
      assert(this->get());
      return this->get();
    }

    inline T* operator->()
    {
      assert(this->get());
      return this->get();
    }
  };
} }

#define cfo_MANAGED_BASIC_METHODS(TYPE, METHODS)                        \
  template<typename cfo_T, bool cfo_SYNC>                               \
  class cfo_managed_methods :                                           \
    public TYPE::template cfo_managed_const_methods                     \
      <cfo_T, cfo_SYNC, cfo::intern::methods<cfo_T, cfo_SYNC> >         \
  {                                                                     \
    template<typename cfo_T_other, bool cfo_SYNC_other>                 \
      friend class cfo_managed_methods;                                 \
                                                                        \
  private:                                                              \
    template<bool cfo_SYNC_basic = cfo_SYNC>                            \
      using cfo_basic_type                                              \
      = typename TYPE::template cfo_managed_const_methods               \
      <cfo_T, cfo_SYNC_basic,                                           \
       cfo::intern::methods<cfo_T, cfo_SYNC_basic> >;                   \
                                                                        \
    typedef                                                             \
      typename TYPE::template cfo_managed_const_methods                 \
      <cfo_T, cfo_SYNC, cfo::intern::methods<cfo_T, cfo_SYNC> >         \
      const_methods;                                                    \
                                                                        \
    cfo_managed_methods(const cfo_managed_methods<cfo_T, true>&);       \
                                                                        \
  protected:                                                            \
    template<bool cfo_SYNC_switch>                                      \
      using sync = cfo_managed_methods<cfo_T, cfo_SYNC_switch>;         \
                                                                        \
    inline cfo_managed_methods(cfo_T *obj) :                            \
      cfo_basic_type<false>(obj)                                        \
    {}                                                                  \
                                                                        \
    inline cfo_managed_methods(const std::shared_ptr<cfo_T> &manager) : \
      cfo_basic_type<false>(manager)                                    \
    {}                                                                  \
                                                                        \
    inline cfo_managed_methods                                          \
      (const cfo_managed_methods<cfo_T, false> &methods) :              \
                                                                        \
      cfo_basic_type<false>(methods)                                    \
    {}                                                                  \
                                                                        \
  public:                                                               \
    inline cfo_managed_methods                                          \
      (const cfo::intern::managed<cfo_T, true, false> &manager) :       \
                                                                        \
      cfo_basic_type<true>(manager)                                     \
    {}                                                                  \
                                                                        \
    inline cfo_managed_methods                                          \
      (cfo_managed_methods<cfo_T, true> &&methods) :                    \
                                                                        \
      cfo_basic_type<true>                                              \
      (static_cast<cfo_basic_type<true>&&>(methods))                    \
    {}                                                                  \
                                                                        \
  protected:                                                            \
    inline const TYPE* operator->() const                               \
    {                                                                   \
      return static_cast<const TYPE*>                                   \
        (this->cfo_basic_type<>::operator->());                         \
    }                                                                   \
                                                                        \
    inline TYPE* operator->()                                           \
    {                                                                   \
      return static_cast<TYPE*>(this->cfo_basic_type<>::operator->());  \
    }                                                                   \
                                                                        \
    METHODS                                                             \
  };                                                                    \
                                                                        \
  template<typename cfo_T, bool cfo_SYNC>                               \
  friend class TYPE::cfo_managed_methods;                               \

#define cfo_MANAGED_METHODS(TYPE, BASE, METHODS)                        \
  template<typename cfo_T, bool cfo_SYNC>                               \
  class cfo_managed_methods :                                           \
    public TYPE::template cfo_managed_const_methods                     \
      <cfo_T, cfo_SYNC,                                                 \
       typename BASE::template cfo_managed_methods<cfo_T, cfo_SYNC> >   \
  {                                                                     \
    template<typename cfo_T_other, bool cfo_SYNC_other>                 \
      friend class cfo_managed_methods;                                 \
                                                                        \
  private:                                                              \
    template<bool cfo_SYNC_basic = cfo_SYNC>                            \
      using cfo_basic_type                                              \
      = typename TYPE::template cfo_managed_const_methods               \
      <cfo_T, cfo_SYNC_basic,                                           \
       typename BASE::template cfo_managed_methods                      \
       <cfo_T, cfo_SYNC_basic> >;                                       \
                                                                        \
    typedef                                                             \
      typename TYPE::template cfo_managed_const_methods                 \
      <cfo_T, cfo_SYNC,                                                 \
       typename BASE::template cfo_managed_methods<cfo_T, cfo_SYNC>     \
       >                                                                \
      const_methods;                                                    \
                                                                        \
    cfo_managed_methods(const cfo_managed_methods<cfo_T, true>&);       \
                                                                        \
  protected:                                                            \
    template<bool cfo_SYNC_switch>                                      \
      using sync = cfo_managed_methods<cfo_T, cfo_SYNC_switch>;         \
                                                                        \
    inline cfo_managed_methods(cfo_T *obj) :                            \
      cfo_basic_type<false>(obj)                                        \
    {}                                                                  \
                                                                        \
    inline cfo_managed_methods(const std::shared_ptr<cfo_T> &manager) : \
      cfo_basic_type<false>(manager)                                    \
    {}                                                                  \
                                                                        \
    inline cfo_managed_methods                                          \
      (const cfo_managed_methods<cfo_T, false> &methods) :              \
                                                                        \
      cfo_basic_type<false>(methods)                                    \
    {}                                                                  \
                                                                        \
  public:                                                               \
    inline cfo_managed_methods                                          \
      (const cfo::intern::managed<cfo_T, true, false> &manager) :       \
                                                                        \
      cfo_basic_type<true>(manager)                                     \
    {}                                                                  \
                                                                        \
    inline cfo_managed_methods                                          \
      (cfo_managed_methods<cfo_T, true> &&methods) :                    \
                                                                        \
      cfo_basic_type<true>                                              \
      (static_cast<cfo_basic_type<true>&&>(methods))                    \
    {}                                                                  \
                                                                        \
    template<typename cfo_M>                                            \
    inline                                                              \
    typename cfo_M::managed_type                                        \
    ::template cfo_managed_methods<cfo_T, cfo_SYNC>&                    \
    cast()                                                              \
    {                                                                   \
      return *this;                                                     \
    }                                                                   \
                                                                        \
    template<typename cfo_M>                                            \
    inline                                                              \
    const                                                               \
    typename cfo_M::managed_type                                        \
    ::template cfo_managed_methods<cfo_T, cfo_SYNC>&                    \
    cast()                                                              \
      const                                                             \
    {                                                                   \
      return *this;                                                     \
    }                                                                   \
                                                                        \
  protected:                                                            \
    inline const TYPE* operator->() const                               \
    {                                                                   \
      return static_cast<const TYPE*>                                   \
        (this->cfo_basic_type<>::operator->());                         \
    }                                                                   \
                                                                        \
    inline TYPE* operator->()                                           \
    {                                                                   \
      return static_cast<TYPE*>(this->cfo_basic_type<>::operator->());  \
    }                                                                   \
                                                                        \
    METHODS                                                             \
  };                                                                    \
                                                                        \
  template<typename cfo_T, bool cfo_SYNC>                               \
  friend class TYPE::cfo_managed_methods;                               \

#endif
