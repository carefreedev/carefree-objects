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

/* the object manager template
 *
 * wraps any normal c++ class
 * to get managed object ref counting and synchronized object member access
 */
#ifndef __CFO_MANAGED_HPP
#define __CFO_MANAGED_HPP

#include "common.hpp"

#include "basic_manager.hpp"
#include "methods.hpp"

namespace cfo { namespace intern
{
  template
  <typename T, bool EXC, typename INIT_T, bool INIT_NULL, typename COPY>
  class managed<T, true, EXC, INIT_T, INIT_NULL, COPY> :
    public basic_manager
  {
    friend class cfo::intern::const_accessor<T, EXC, INIT_T, COPY>;
    friend class cfo::intern::accessor<T, EXC, INIT_T, COPY>;

  private:
    T *obj;

  protected:
    inline managed(T *obj) :
      basic_manager(),
      obj(obj)
    {}

  public:
    typedef T cfo_unmanaged_type;
    typedef T* cfo_unmanaged_ptr_type;
    typedef const T* cfo_unmanaged_const_ptr_type;

    typedef INIT_T cfo_unmanaged_init_type;
    typedef INIT_T* cfo_unmanaged_init_ptr_type;
    typedef const INIT_T* cfo_unmanaged_const_init_ptr_type;

    typedef managed<T, true, false, INIT_T, false, COPY> cfo_manager_type;
    typedef forward<T, true, false, INIT_T, false, COPY>
      cfo_forward_manager_type;

    typedef T managed_type;
    typedef T* managed_ptr_type;
    typedef const T* managed_const_ptr_type;

    typedef managed<T, true, false, INIT_T, false, COPY> manager_type;
    typedef forward<T, true, false, INIT_T, false, COPY>
      forward_manager_type;

    typedef managed<T, true, true, INIT_T, false, COPY> except;

    typedef managed<T, true, false, INIT_T, true, COPY> null;

    typedef
    typename T::template cfo_managed_const_methods
    <const T, true, EXC, INIT_T, COPY>
    const_accessor;

    typedef typename T::template cfo_managed_methods
    <T, true, EXC, INIT_T, COPY>
    accessor;

    typedef managed
      <basic_manager::vector<T, true, EXC, INIT_T, COPY>, true, EXC>
      vector;

    template<typename KEY, typename... EXTRA>
      using map = managed
      <basic_manager::map
       <KEY, T, true, EXC, managed<T, true, EXC, INIT_T>, EXTRA...>,
       true>;

    inline managed() :
      basic_manager(),
      obj(INIT_NULL ? NULL : static_cast<T*>(new INIT_T))
    {}

    template<typename... A>
    inline managed(A... args) :
      basic_manager(),
      obj(static_cast<T*>(new INIT_T(args...)))
    {}

    // inline managed(const managed<T, true, true> &/*nullmanager*/) :
    //   basic_manager(),
    //   obj(NULL)
    // {}

    inline managed
    (const managed<T, true, EXC, INIT_T, false, COPY> &manager
     ) :
      basic_manager(manager),
      obj(this->cnl ? manager.obj : NULL)
    {}

    template
    <typename T_, bool EXC_, typename INIT_T_, bool INIT_NULL_,
     typename COPY_
     >
    inline managed
    (const managed<T_, true, EXC_, INIT_T_, INIT_NULL_, COPY_> &other
     ) :
      basic_manager(other),
      obj(this->cnl ? static_cast<T*>(other.unmanaged()) : NULL)
    {}

    inline ~managed()
    {
      this->basic_manager::destroy();
      if (!this->cnl)
        delete this->obj;
    }

    inline managed<T, true, EXC, INIT_T, false, COPY> copy() const;

    inline const_accessor caccess() const
    {
      return const_accessor(*this);
    }

    inline accessor access()
    {
      return accessor(*this);
    }

    inline T* unmanaged() const
    {
      return this->obj;
    }

    inline operator bool() const
    {
      return this->obj;
    }

    inline bool operator!() const
    {
      return !this->obj;
    }
  };

  template
  <typename T, bool EXC, typename INIT_T, bool INIT_NULL, typename COPY>
  inline
  managed<T, true, EXC, INIT_T, false, COPY>
  managed<T, true, EXC, INIT_T, INIT_NULL, COPY>::copy()
    const
  {
    return managed<T, true, EXC, INIT_T, false, COPY>
      (this->obj ? COPY()(*this->obj) : NULL);
  }

  template
  <typename T, bool EXC, typename INIT_T, bool INIT_NULL, typename COPY>
  class managed<T, false, EXC, INIT_T, INIT_NULL, COPY> :
    public T::template cfo_managed_methods<T, false, EXC, INIT_T, COPY>
  {
    friend T;

    template
    <typename T_, bool SYNC, bool EXC_, typename INIT_T_, bool INIT_NULL_,
     typename COPY_
     >
    friend class managed;

    typedef typename T::template cfo_managed_methods
    <T, false, EXC, INIT_T, COPY>
    basic_type;

  protected:
    inline managed(T *obj) :
      basic_type(obj)
    {}

  public:
    typedef T cfo_unmanaged_type;
    typedef T* cfo_unmanaged_ptr_type;
    typedef const T* cfo_unmanaged_const_ptr_type;

    typedef INIT_T cfo_unmanaged_init_type;
    typedef INIT_T* cfo_unmanaged_init_ptr_type;
    typedef const INIT_T* cfo_unmanaged_const_init_ptr_type;

    typedef managed<T, false, false, INIT_T, false, COPY> cfo_manager_type;
    typedef forward<T, false, false, INIT_T, false, COPY>
      cfo_forward_manager_type;

    typedef T managed_type;
    typedef T* managed_ptr_type;
    typedef const T* managed_const_ptr_type;

    typedef managed<T, false, false, INIT_T, false, COPY> manager_type;
    typedef forward<T, false, false, INIT_T, false, COPY>
      forward_manager_type;

    typedef managed<T, false, true, INIT_T, false, COPY> except;

    typedef managed<T, false, false, INIT_T, true, COPY> null;

    typedef managed
    <basic_manager::vector
     <T, false, EXC, INIT_T, COPY,
      managed<T, false, EXC, INIT_T, false, COPY>
      >,
     false, EXC
     >
    vector;

    template<typename I, typename... E>
    using map = managed
      <basic_manager::map
       <I, T, false, EXC, managed<T, false, EXC, INIT_T, false>, E...>,
       false>;

    inline managed() :
      basic_type(INIT_NULL ? NULL : static_cast<T*>(new INIT_T))
    {}

    template<typename... A>
    inline managed(A... args) :
      basic_type(static_cast<T*>(new INIT_T(args...)))
    {}

    // inline managed(const managed<T, false, true> &/*nullmanager*/) :
    //   basic_type(NULL)
    // {}

    inline managed
    (const managed<T, false, EXC, INIT_T, false, COPY> &manager
     ) :
      basic_type(manager)
    {}

    template
    <typename T_, bool EXC_, typename INIT_T_, bool INIT_NULL_,
     typename COPY_
     >
    inline managed
    (const managed<T_, false, EXC_, INIT_T_, INIT_NULL_, COPY_> &other
     ) :
      basic_type
      (std::static_pointer_cast<T>
       (static_cast<const std::shared_ptr<T_>&>(other)))
    {}

    inline std::size_t refcount() const
    {
      return this->use_count();
    }

    inline bool operator==
    (const managed<T, false, EXC, INIT_T, INIT_NULL, COPY> &other)
      const
    {
      assert
        (this->std::shared_ptr<T>::get()
         && other.std::shared_ptr<T>::get());

      return this->std::shared_ptr<T>::get()
        == other.std::shared_ptr<T>::get()

        || this->std::shared_ptr<T>::operator*()
        == other.std::shared_ptr<T>::operator*();
    }

    inline bool operator!=
    (const managed<T, false, EXC, INIT_T, INIT_NULL, COPY> &other)
      const
    {
      assert
        (this->std::shared_ptr<T>::get()
         && other.std::shared_ptr<T>::get());

      return this->std::shared_ptr<T>::get()
        != other.std::shared_ptr<T>::get()

        && this->std::shared_ptr<T>::operator*()
        != other.std::shared_ptr<T>::operator*();
    }

    template
    <typename T_, bool EXC_, typename INIT_T_, bool INIT_NULL_,
     typename COPY_
     >
    inline bool operator==
    (const managed<T_, false, EXC_, INIT_T_, INIT_NULL_, COPY_> &other)
      const
    {
      return this->operator==
        (managed<T, false, EXC, INIT_T, INIT_NULL, COPY>(other));
    }

    template
    <typename T_, bool EXC_, typename INIT_T_, bool INIT_NULL_,
     typename COPY_
     >
    inline bool operator!=
    (const managed<T_, false, EXC_, INIT_T_, INIT_NULL_, COPY_> &other)
      const
    {
      return this->operator!=
        (managed<T, false, EXC, INIT_T, INIT_NULL, COPY>(other));
    }

    inline T* operator->() const
    {
      assert(this->get());
      return this->get();
    }

    inline managed<T, false, EXC, INIT_T, false, COPY> copy() const;

    inline except except_() const
    {
      return except(*this);
    }

    inline manager_type noexcept_() const
    {
      return manager_type(*this);
    }

    inline T* unmanaged() const
    {
      assert(this->get());
      return this->get();
    }
  };

  template
  <typename T, bool EXC, typename INIT_T, bool INIT_NULL, typename COPY>
  inline
  managed<T, false, EXC, INIT_T, false, COPY>
  managed<T, false, EXC, INIT_T, INIT_NULL, COPY>::copy()
    const
  {
    return managed<T, false, EXC, INIT_T, false, COPY>
      (this->get() ? COPY()(*this->get()) : NULL);
  }
} }

#endif
