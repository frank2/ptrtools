#ifndef __PTRTOOLS_FLEXIBLE_HPP
#define __PTRTOOLS_FLEXIBLE_HPP

#include <ptrtools/array.hpp>
#include <ptrtools/struct.hpp>

namespace ptrtools
{
   template <
      typename T,
      typename FlexibleType,
      std::size_t StructElements=1,
      typename Allocator=std::allocator<std::uint8_t>>
   class flexible_ptr : public struct_ptr<T,Allocator>
   {
   public:
      using struct_ptr_decl = struct_ptr<T,Allocator>;
      using value_type = typename struct_ptr_decl::value_type;
      using pointer = typename struct_ptr_decl::pointer;
      using const_pointer = typename struct_ptr_decl::const_pointer;
      using reference = typename struct_ptr_decl::reference;
      using const_reference = typename struct_ptr_decl::const_reference;
      using allocator = Allocator;
      using flexible_type = FlexibleType;

      const static std::size_t struct_elements = StructElements;

      flexible_ptr() : struct_ptr_decl(false) {}
      flexible_ptr(std::size_t elements) {
         this->allocate(elements);
      }
      flexible_ptr(pointer ptr, std::size_t elements, bool clone=false) {
         if (clone)
            this->clone(ptr, elements);
         else
            this->set(ptr, elements);
      }
      flexible_ptr(const_pointer ptr, std::size_t elements, bool clone=false) {
         if (clone)
            this->clone(ptr, elements);
         else
            this->set(ptr, elements);
      }
      flexible_ptr(typename struct_ptr_decl::basic_ptr_decl &ptr) : struct_ptr_decl(ptr) {}
      flexible_ptr(const typename struct_ptr_decl::basic_ptr_decl &ptr) : struct_ptr_decl(ptr) {}
      flexible_ptr(struct_ptr_decl &ptr) : struct_ptr_decl(ptr) {}
      flexible_ptr(const struct_ptr_decl &ptr) : struct_ptr_decl(ptr) {}
      flexible_ptr(flexible_ptr<T,FlexibleType,StructElements,Allocator> &other) : struct_ptr_decl(static_cast<struct_ptr_decl &>(other)) {}
      flexible_ptr(const flexible_ptr<T,FlexibleType,StructElements,Allocator> &other) : struct_ptr_decl(static_cast<const struct_ptr_decl &>(other)) {}
      flexible_ptr(flexible_ptr<T,FlexibleType,StructElements,Allocator> &&other) : struct_ptr_decl(static_cast<struct_ptr_decl &&>(other)) {}
      ~flexible_ptr() {}

      flexible_ptr<T,FlexibleType,StructElements,Allocator> &operator=(flexible_ptr<T,FlexibleType,StructElements,Allocator> &other) {
         struct_ptr_decl::operator=(static_cast<struct_ptr_decl &>(other));

         return *this;
      }
      flexible_ptr<T,FlexibleType,StructElements,Allocator> &operator=(const flexible_ptr<T,FlexibleType,StructElements,Allocator> &other) {
         struct_ptr_decl::operator=(static_cast<const struct_ptr_decl &>(other));

         return *this;
      }
      flexible_ptr<T,FlexibleType,StructElements,Allocator> &operator=(flexible_ptr<T,FlexibleType,StructElements,Allocator> &&other) {
         struct_ptr_decl::operator=(static_cast<struct_ptr_decl &&>(other));

         return *this;
      }

   private:
      using struct_ptr_decl::allocate;
      using struct_ptr_decl::reallocate;
      using struct_ptr_decl::resize;
      using struct_ptr_decl::set;
      using struct_ptr_decl::clone;

   public:
      FlexibleType &operator[](std::size_t index) { return this->flexible_array()[index]; }
      const FlexibleType &operator[](std::size_t index) const { return this->flexible_array()[index]; }
      
      std::size_t adjusted_type_size() const { return this->type_size - (sizeof(FlexibleType) * this->struct_elements); }
      std::size_t elements() const { return (this->size() - this->adjusted_type_size()) / sizeof(FlexibleType); }
            
      void allocate(std::size_t elements) {
         if (elements < this->struct_elements)
            throw std::runtime_error("insufficient size: not enough elements given to allocate flexible array structure");
         
         struct_ptr_decl::allocate(this->adjusted_type_size() + elements * sizeof(FlexibleType));
      }
      void reallocate(std::size_t elements) {
         if (elements < this->struct_elements)
            throw std::runtime_error("insufficient size: not enough elements given to allocate flexible array structure");
         
         struct_ptr_decl::reallocate(this->adjusted_type_size() + elements * sizeof(FlexibleType));
      }
      void resize(std::size_t elements) {
         if (elements < this->struct_elements)
            throw std::runtime_error("insufficient size: not enough elements given to allocate flexible array structure");

         struct_ptr_decl::resize(this->adjusted_type_size() + elements * sizeof(FlexibleType));
      }

      void set(pointer ptr, std::size_t elements) {
         if (elements < this->struct_elements)
            throw std::runtime_error("insufficient size: not enough elements given to allocate flexible array structure");

         struct_ptr_decl::set(ptr, this->adjusted_type_size() + elements * sizeof(FlexibleType));
      }
      void set(const_pointer ptr, std::size_t elements) {
         if (elements < this->struct_elements)
            throw std::runtime_error("insufficient size: not enough elements given to allocate flexible array structure");

         struct_ptr_decl::set(ptr, this->adjusted_type_size() + elements * sizeof(FlexibleType));
      }

      void clone(const_pointer ptr, std::size_t elements) {
         if (elements < this->struct_elements)
            throw std::runtime_error("insufficient size: not enough elements given to allocate flexible array structure");

         struct_ptr_decl::clone(ptr, this->adjusted_type_size() + elements * sizeof(FlexibleType));
      }
      void clone(const flexible_ptr<T,FlexibleType,StructElements,Allocator> &other) {
         flexible_ptr<T,FlexibleType,StructElements,Allocator>::clone(other.get(), other.elements());
      }
      void copy(const_pointer ptr, std::size_t elements) {
         if (elements < this->struct_elements)
            throw std::runtime_error("insufficient size: not enough elements given to allocate flexible array structure");

         struct_ptr_decl::copy(ptr, this->adjusted_type_size() + elements * sizeof(FlexibleType));
      }
      void copy(const flexible_ptr<T,FlexibleType,StructElements,Allocator> &other) {
         flexible_ptr<T,FlexibleType,StructElements,Allocator>::copy(other.get(), other.elements());
      }

      array_ptr<FlexibleType,Allocator> flexible_array() {
         // keep the ptr object on the stack so it doesn't get passed as const to the constructor
         auto ptr = this->template ptr_at<FlexibleType,sizeof(FlexibleType),alignof(FlexibleType)>(this->adjusted_type_size(), false);
         auto result = array_ptr<FlexibleType,Allocator>(ptr);
         result.resize(this->elements());

         return result;
      }
      const array_ptr<FlexibleType,Allocator> flexible_array() const {
         auto ptr = this->template ptr_at<FlexibleType,sizeof(FlexibleType),alignof(FlexibleType)>(this->adjusted_type_size(), false);
         auto result = array_ptr<FlexibleType,Allocator>(ptr);
         result.resize(this->elements());

         return result;
      }
   };
}

#endif
