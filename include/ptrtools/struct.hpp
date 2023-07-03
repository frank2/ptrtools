#ifndef __PTRTOOLS_STRUCT_HPP
#define __PTRTOOLS_STRUCT_HPP

#include <ptrtools/basic.hpp>

namespace ptrtools
{
   template <typename T, typename Allocator=std::allocator<std::uint8_t>>
   class struct_ptr : public basic_ptr<T,sizeof(T),alignof(T),Allocator>
   {
   public:
      using basic_ptr_decl = basic_ptr<T,sizeof(T),alignof(T),Allocator>;
      using value_type = typename basic_ptr_decl::value_type;
      using pointer = typename basic_ptr_decl::pointer;
      using const_pointer = typename basic_ptr_decl::const_pointer;
      using reference = typename basic_ptr_decl::reference;
      using const_reference = typename basic_ptr_decl::const_reference;
      using allocator = Allocator;

   private:
      using basic_ptr_decl::iterator;
      using basic_ptr_decl::const_iterator;
      using basic_ptr_decl::reverse_iterator;
      using basic_ptr_decl::const_reverse_iterator;

   public:
      struct_ptr(bool allocate=false) : basic_ptr_decl(allocate) {}
      struct_ptr(std::size_t size) : basic_ptr_decl(size) {}
      struct_ptr(pointer ptr, std::size_t size=basic_ptr_decl::type_size, bool clone=false)
         : basic_ptr_decl(ptr, size, clone)
      {}
      struct_ptr(const_pointer ptr, std::size_t size=basic_ptr_decl::type_size, bool clone=false)
         : basic_ptr_decl(ptr, size, clone)
      {}
      struct_ptr(basic_ptr_decl &ptr) : basic_ptr_decl(ptr) {}
      struct_ptr(const basic_ptr_decl &ptr) : basic_ptr_decl(ptr) {}
      struct_ptr(struct_ptr<T,Allocator> &other) : basic_ptr_decl(static_cast<basic_ptr_decl &>(other)) {}
      struct_ptr(const struct_ptr<T,Allocator> &other) : basic_ptr_decl(static_cast<const basic_ptr_decl &>(other)) {}
      struct_ptr(struct_ptr<T,Allocator> &&other) : basic_ptr_decl(static_cast<basic_ptr_decl &&>(other)) {}
      ~struct_ptr() {}

      struct_ptr<T,Allocator> &operator=(struct_ptr<T,Allocator> &other) {
         basic_ptr_decl::operator=(static_cast<basic_ptr_decl &>(other));

         return *this;
      }
      struct_ptr<T,Allocator> &operator=(const struct_ptr<T,Allocator> &other) {
         basic_ptr_decl::operator=(static_cast<const basic_ptr_decl &>(other));

         return *this;
      }
      struct_ptr<T,Allocator> &operator=(struct_ptr<T,Allocator> &&other) {
         basic_ptr_decl::operator=(static_cast<basic_ptr_decl &&>(other));

         return *this;
      }

   private:
      using basic_ptr_decl::operator[];
      using basic_ptr_decl::reob;
      using basic_ptr_decl::elements;
      using basic_ptr_decl::front;
      using basic_ptr_decl::back;
      using basic_ptr_decl::begin;
      using basic_ptr_decl::cbegin;
      using basic_ptr_decl::rbegin;
      using basic_ptr_decl::crbegin;
      using basic_ptr_decl::end;
      using basic_ptr_decl::cend;
      using basic_ptr_decl::rend;
      using basic_ptr_decl::crend;
      using basic_ptr_decl::offset;
      using basic_ptr_decl::index;
      using basic_ptr_decl::at;
      using basic_ptr_decl::assign;
      using basic_ptr_decl::clone;
      using basic_ptr_decl::copy;

   public:
      void clone(const_pointer ptr, std::size_t size=basic_ptr_decl::type_size) {
         basic_ptr_decl::clone(ptr, size);
      }
      void clone(const basic_ptr_decl &other) {
         basic_ptr_decl::clone(other.get(), other.size());
      }
      void clone(const struct_ptr<T,Allocator> &other) {
         struct_ptr<T,Allocator>::clone(other.get(), other.size());
      }
      void copy(const_pointer ptr, std::size_t size=basic_ptr_decl::type_size) {
         basic_ptr_decl::copy(ptr, size, 0, true);
      }
      void copy(const basic_ptr_decl &other) {
         basic_ptr_decl::copy(other.get(), other.size());
      }
      void copy(const struct_ptr<T,Allocator> &other) {
         struct_ptr<T,Allocator>::copy(other.get(), other.size());
      }
   };
}

#endif
