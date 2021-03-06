/* carefree-objects
 *
 * a thread-safe object manager extension for c++
 *
 * Copyright (C) 2011-2016 Stefan Zimmermann <zimmermann.code@gmail.com>
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

/* managed vector for managed objects - const iterator class
 */

#ifndef __CFO_VECTOR_CONST_METHODS_HPP
#define __CFO_VECTOR_CONST_METHODS_HPP

#include "../vector.hpp"

#include "./const_iterator.hpp"

namespace cfo { namespace intern
{
  template
  <typename T, bool SYNC, bool EXC, typename INIT_T, typename COPY,
   typename MGR
   >
  cfo_DEFINE_MANAGED_BASIC_CONST_METHODS
  (basic_manager::vector<_cfo_MANAGED_VECTOR_TEMPLATE_ARGS>,
   vector<_cfo_MANAGED_VECTOR_TEMPLATE_ARGS>)

   cfo_DEFINE_MANAGED_PROPERTIES

     struct : property<std::size_t>
     {
       inline std::size_t operator()() const
       {
         return (*this)->size();
       }
     }
       size;

   cfo_END_MANAGED_PROPERTIES

   public:

     typedef MGR value_type;

     typedef const MGR& const_reference;
     typedef const MGR& reference;

     typedef
       typename vector<_cfo_MANAGED_VECTOR_TEMPLATE_ARGS>::const_iterator
       const_iterator;

     // inline std::size_t size() const
     // {
     //   return (*this)->size();
     // }

     inline const MGR& operator[](std::size_t index) const
     noexcept(!cfo_EXC)
     {
       if (cfo_EXC && index >= this->size())
         {
           std::ostringstream what;
           what << "Index out of range: " << index;
           throw std::out_of_range(what.str());
         }

       return (*this)->operator[](index);
     }

     inline const MGR& last() const
     noexcept(!cfo_EXC)
     {
       if (cfo_EXC && !this->size())
         throw std::out_of_range("No last element");

       return (*this)[this->size() - 1];
     }

     inline const_iterator cbegin() const
     {
       return const_iterator(this->const_manager());
     }

     inline const_iterator cend() const
     {
       return const_iterator(this->const_manager(), this->size());
     }

     inline const_iterator begin() const
     {
       return const_iterator(this->const_manager());
     }

     inline const_iterator end() const
     {
       return const_iterator(this->const_manager(), this->size());
     }

  cfo_END_MANAGED_BASIC_CONST_METHODS /* basic_manager::vector<> */

} } /* namespace cfo::intern */

#endif /* __CFO_VECTOR_CONST_METHODS_HPP */
