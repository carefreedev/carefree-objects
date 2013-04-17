/* carefree-objects
 *
 * a thread-safe object manager extension for c++
 *
 * Copyright (C) 2011-2013 Stefan Zimmermann <zimmermann.code@gmail.com>
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

#ifndef __CFO_CONST_METHODS_HPP
#define __CFO_CONST_METHODS_HPP

#include "common.hpp"

#include "const_accessor.hpp"

namespace cfo { namespace intern
{
  template<typename T, bool EXC>
  class const_methods<T, true, EXC> : public const_accessor<T, EXC>
  {
  private:
    const_methods(const const_methods<T, true, EXC>&);

  protected:
    template<bool SYNC_switch>
    using cfo_sync = const_methods<T, SYNC_switch, EXC>;

    inline const_methods
    (const managed<T, true, EXC, false> &manager, bool shared) :

      const_accessor<T, EXC>(manager, shared)
    {}

  public:
    inline const_methods(const managed<T, true, EXC, false> &manager) :
      const_accessor<T, EXC>(manager)
    {}

    inline const_methods(const_methods<T, true, EXC> &&methods) :
      const_accessor<T, EXC>
      (static_cast<const_accessor<T, EXC>&&>(methods))
    {}
  };

  template<typename T, bool EXC>
  class const_methods<T, false, EXC> : protected std::shared_ptr<T>
  {
  protected:
    template<bool SYNC_switch>
    using cfo_sync = const_methods<T, SYNC_switch, EXC>;

    inline const_methods(T *obj) :
      std::shared_ptr<T>(obj)
    {}

    inline const_methods(const std::shared_ptr<T> &manager) :
      std::shared_ptr<T>(manager)
    {}

    inline const_methods
    (const const_methods<T, false, EXC> &methods) :

      std::shared_ptr<T>(methods)
    {}

  public:
    inline managed<T, false, EXC, false>& manager()
    {
      return static_cast<managed<T, false, EXC, false>&>(*this);
    }

    // inline const managed<T, false, false>& manager() const
    // {
    //   return static_cast<const managed<T, false, false>&>(*this);
    // }

    inline const managed<T, false, EXC, false>& const_manager() const
    {
      return static_cast<const managed<T, false, EXC, false>&>(*this);
    }

    inline const T* operator->() const
    {
      assert(this->get());
      return this->get();
    }

    inline operator bool() const
    {
      return this->get();
    }

    inline bool operator!() const
    {
      return !this->get();
    }

    // template<bool INIT_NULL>
    // inline bool operator==
    // (const managed<T, false, INIT_NULL> &manager)
    //   const
    // {
    //   assert(this->get());
    //   return this->get() == manager.unmanaged();
    // }
  };
} }

#define cfo_MANAGED_BASIC_CONST_METHODS(TYPE, METHODS)                  \
  template                                                              \
  <typename cfo_T, bool cfo_SYNC, bool cfo_EXC, typename cfo_BASE>      \
  class _cfo_managed_const_methods : public cfo_BASE                    \
  {                                                                     \
    template                                                            \
      <typename cfo_T_other, bool cfo_SYNC_other, bool cfo_EXC_other,   \
       typename cfo_BASE_other                                          \
       >                                                                \
      friend class _cfo_managed_const_methods;                          \
                                                                        \
  private:                                                              \
    _cfo_managed_const_methods                                          \
      (const _cfo_managed_const_methods                                 \
       <cfo_T, true, cfo_EXC, cfo_BASE>&);                              \
                                                                        \
    template<bool cfo_SYNC_basic = cfo_SYNC>                            \
      using cfo_basic_type                                              \
      = typename cfo_BASE::template cfo_sync<cfo_SYNC_basic>;           \
                                                                        \
  protected:                                                            \
    template<bool cfo_SYNC_switch>                                      \
      using cfo_sync                                                    \
      = _cfo_managed_const_methods                                      \
      <cfo_T, cfo_SYNC_switch, cfo_EXC, cfo_BASE>;                      \
                                                                        \
    inline _cfo_managed_const_methods(cfo_T *obj) :                     \
      cfo_basic_type<false>(obj)                                        \
    {}                                                                  \
                                                                        \
    inline _cfo_managed_const_methods                                   \
      (const std::shared_ptr<cfo_T> &manager) :                         \
                                                                        \
      cfo_basic_type<false>(manager)                                    \
    {}                                                                  \
                                                                        \
    inline _cfo_managed_const_methods                                   \
      (const _cfo_managed_const_methods                                 \
       <cfo_T, false, cfo_EXC, cfo_BASE>                                \
       &methods) :                                                      \
                                                                        \
      cfo_basic_type<false>(methods)                                    \
    {}                                                                  \
                                                                        \
    inline _cfo_managed_const_methods                                   \
      (const cfo::intern::managed<cfo_T, true, cfo_EXC, false>          \
       &manager,                                                        \
                                                                        \
       bool shared) :                                                   \
                                                                        \
      cfo_basic_type<true>(manager, shared)                             \
    {}                                                                  \
                                                                        \
  public:                                                               \
    inline _cfo_managed_const_methods                                   \
      (const cfo::intern::managed<cfo_T, true, cfo_EXC, false>          \
       &manager) :                                                      \
                                                                        \
      cfo_basic_type<true>(manager)                                     \
    {}                                                                  \
                                                                        \
    inline _cfo_managed_const_methods                                   \
      (_cfo_managed_const_methods<cfo_T, true, cfo_EXC, cfo_BASE>       \
       &&methods) :                                                     \
                                                                        \
      cfo_basic_type<true>                                              \
      (static_cast<cfo_basic_type<true>&&>(methods))                    \
    {}                                                                  \
                                                                        \
    template<typename cfo_M>                                            \
    inline                                                              \
    typename cfo_M::managed_type                                        \
    ::template cfo_managed_const_methods<cfo_T, cfo_SYNC, cfo_EXC>      \
    &                                                                   \
    cast()                                                              \
    {                                                                   \
      return *this;                                                     \
    }                                                                   \
                                                                        \
    template<typename cfo_M>                                            \
    inline                                                              \
    const                                                               \
    typename cfo_M::managed_type                                        \
    ::template cfo_managed_const_methods<cfo_T, cfo_SYNC, cfo_EXC>      \
    &                                                                   \
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
  template                                                              \
  <typename cfo_T, bool cfo_SYNC, bool cfo_EXC,                         \
   typename cfo_BASE                                                    \
   = cfo::intern::const_methods<cfo_T, cfo_SYNC, cfo_EXC>               \
   >                                                                    \
  using cfo_managed_const_methods                                       \
  = _cfo_managed_const_methods<cfo_T, cfo_SYNC, cfo_EXC, cfo_BASE>;     \
                                                                        \
  template                                                              \
  <typename cfo_T, bool cfo_SYNC, bool cfo_EXC, typename cfo_BASE>      \
  friend class TYPE::_cfo_managed_const_methods;                        \

#define cfo_MANAGED_CONST_METHODS(TYPE, BASE, METHODS)                  \
  template                                                              \
  <typename cfo_T, bool cfo_SYNC, bool cfo_EXC, typename cfo_BASE>      \
  class _cfo_managed_const_methods : public cfo_BASE                    \
  {                                                                     \
    template                                                            \
      <typename cfo_T_other, bool cfo_SYNC_other, bool cfo_EXC_other,   \
       typename cfo_BASE_other                                          \
       >                                                                \
      friend class _cfo_managed_const_methods;                          \
                                                                        \
  private:                                                              \
    template<bool cfo_SYNC_basic = cfo_SYNC>                            \
      using cfo_basic_type                                              \
      = typename cfo_BASE::template cfo_sync<cfo_SYNC_basic>;           \
                                                                        \
    _cfo_managed_const_methods                                          \
      (const _cfo_managed_const_methods                                 \
       <cfo_T, true, cfo_EXC, cfo_BASE>&);                              \
                                                                        \
  protected:                                                            \
    template<bool cfo_SYNC_switch>                                      \
      using cfo_sync                                                    \
      = _cfo_managed_const_methods                                      \
      <cfo_T, cfo_SYNC_switch, cfo_EXC, cfo_BASE>;                      \
                                                                        \
    inline _cfo_managed_const_methods(cfo_T *obj) :                     \
      cfo_basic_type<false>(obj)                                        \
    {}                                                                  \
                                                                        \
    inline _cfo_managed_const_methods                                   \
      (const std::shared_ptr<cfo_T> &manager) :                         \
                                                                        \
      cfo_basic_type<false>(manager)                                    \
    {}                                                                  \
                                                                        \
    inline _cfo_managed_const_methods                                   \
      (const _cfo_managed_const_methods                                 \
       <cfo_T, false, cfo_EXC, cfo_BASE>                                \
       &methods) :                                                      \
                                                                        \
      cfo_basic_type<false>(methods)                                    \
    {}                                                                  \
                                                                        \
    inline _cfo_managed_const_methods                                   \
      (const cfo::intern::managed<cfo_T, true, cfo_EXC, false>          \
       &manager,                                                        \
                                                                        \
       bool shared) :                                                   \
                                                                        \
      cfo_basic_type<true>(manager, shared)                             \
    {}                                                                  \
                                                                        \
  public:                                                               \
    inline _cfo_managed_const_methods                                   \
      (const cfo::intern::managed<cfo_T, true, cfo_EXC, false>          \
       &manager) :                                                      \
                                                                        \
      cfo_basic_type<true>(manager)                                     \
    {}                                                                  \
                                                                        \
    inline _cfo_managed_const_methods                                   \
      (_cfo_managed_const_methods<cfo_T, true, cfo_EXC, cfo_BASE>       \
       &&methods) :                                                     \
                                                                        \
      cfo_basic_type<true>                                              \
      (static_cast<cfo_basic_type<true>&&>(methods))                    \
    {}                                                                  \
                                                                        \
    template<typename cfo_M>                                            \
    inline                                                              \
    typename cfo_M::managed_type                                        \
    ::template cfo_managed_const_methods<cfo_T, cfo_SYNC, cfo_EXC>      \
    &                                                                   \
    cast()                                                              \
    {                                                                   \
      return *this;                                                     \
    }                                                                   \
                                                                        \
    template<typename cfo_M>                                            \
    inline                                                              \
    const                                                               \
    typename cfo_M::managed_type                                        \
    ::template cfo_managed_const_methods<cfo_T, cfo_SYNC, cfo_EXC>      \
    &                                                                   \
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
  template                                                              \
  <typename cfo_T, bool cfo_SYNC, bool cfo_EXC,                         \
   typename cfo_BASE                                                    \
   = typename BASE::template cfo_managed_const_methods                  \
   <cfo_T, cfo_SYNC, cfo_EXC>                                           \
   >                                                                    \
  using cfo_managed_const_methods                                       \
  = _cfo_managed_const_methods<cfo_T, cfo_SYNC, cfo_EXC, cfo_BASE>;     \
                                                                        \
  template                                                              \
  <typename cfo_T, bool cfo_SYNC, bool cfo_EXC, typename cfo_BASE>      \
  friend class TYPE::_cfo_managed_const_methods;                        \

#endif