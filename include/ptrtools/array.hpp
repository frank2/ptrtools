#ifndef __PTRTOOLS_ARRAY_HPP
#define __PTRTOOLS_ARRAY_HPP

#include <ptrtools/basic.hpp>

namespace ptrtools
{
   template <typename T, typename Allocator=std::allocator<std::uint8_t>>
   class array_ptr : public basic_ptr<T,sizeof(T),alignof(T),Allocator>
   {
   public:
      using basic_ptr_decl = basic_ptr<T,sizeof(T),alignof(T),Allocator>;
      using value_type = typename basic_ptr_decl::value_type;
      using pointer = typename basic_ptr_decl::pointer;
      using const_pointer = typename basic_ptr_decl::const_pointer;
      using reference = typename basic_ptr_decl::reference;
      using const_reference = typename basic_ptr_decl::const_reference;
      using allocator = Allocator;

      array_ptr() : basic_ptr_decl(false) {}
      array_ptr(std::size_t elements) {
         if (elements > 0)
            this->allocate(elements);
      }
      array_ptr(pointer ptr, std::size_t elements, bool clone=false) {
         if (clone)
            this->clone(ptr, elements);
         else
            this->set(ptr, elements);
      }
      array_ptr(const_pointer ptr, std::size_t elements, bool clone=false) {
         if (clone)
            this->clone(ptr, elements);
         else
            this->set(ptr, elements);
      }
      array_ptr(basic_ptr_decl &ptr) : basic_ptr_decl(ptr) {}
      array_ptr(const basic_ptr_decl &ptr) : basic_ptr_decl(ptr) {}
      array_ptr(array_ptr<T,Allocator> &other) : basic_ptr_decl(static_cast<basic_ptr_decl &>(other)) {}
      array_ptr(const array_ptr<T,Allocator> &other) : basic_ptr_decl(static_cast<const basic_ptr_decl &>(other)) {}
      array_ptr(array_ptr<T,Allocator> &&other) : basic_ptr_decl(static_cast<basic_ptr_decl &&>(other)) {}
      ~array_ptr() {}

      array_ptr<T,Allocator> &operator=(array_ptr<T,Allocator> &other) {
         basic_ptr_decl::operator=(static_cast<basic_ptr_decl &>(other));

         return *this;
      }
      array_ptr<T,Allocator> &operator=(const array_ptr<T,Allocator> &other) {
         basic_ptr_decl::operator=(static_cast<const basic_ptr_decl &>(other));

         return *this;
      }
      array_ptr<T,Allocator> &operator=(array_ptr<T,Allocator> &&other) {
         basic_ptr_decl::operator=(static_cast<basic_ptr_decl &&>(other));

         return *this;
      }

   private:
      using basic_ptr_decl::operator->;
      using basic_ptr_decl::allocate;
      using basic_ptr_decl::reallocate;
      using basic_ptr_decl::resize;
      using basic_ptr_decl::set;
      using basic_ptr_decl::clone;
      using basic_ptr_decl::copy;

   public:
      void allocate(std::size_t elements) {
         basic_ptr_decl::allocate(elements * this->aligned_type_size());
      }
      void reallocate(std::size_t elements) {
         basic_ptr_decl::reallocate(elements * this->aligned_type_size());
      }
      void resize(std::size_t elements) {
         basic_ptr_decl::resize(elements * this->aligned_type_size());
      }

      void set(pointer ptr, std::size_t elements) {
         basic_ptr_decl::set(ptr, elements * this->aligned_type_size());
      }
      void set(const_pointer ptr, std::size_t elements) {
         basic_ptr_decl::set(ptr, elements * this->aligned_type_size());
      }

      void clone(const_pointer ptr, std::size_t elements) {
         basic_ptr_decl::clone(ptr, elements * this->aligned_type_size());
      }
      void clone(const basic_ptr_decl &other) {
         array_ptr<T,Allocator>::clone(other.get(), other.elements());
      }
      void clone(const array_ptr<T,Allocator> &other) {
         array_ptr<T,Allocator>::clone(other.get(), other.elements());
      }
      void copy(const_pointer ptr, std::size_t elements, std::size_t index=0) {
         basic_ptr_decl::copy(ptr, elements * this->aligned_type_size(), index * this->aligned_type_size(), true);
      }
      void copy(const basic_ptr_decl &other, std::size_t index=0) {
         array_ptr<T,Allocator>::copy(other.get(), other.elements(), index);
      }
      void copy(const array_ptr<T,Allocator> &other, std::size_t index=0) {
         array_ptr<T,Allocator>::copy(other.get(), other.elements(), index);
      }
   };
}

#endif
