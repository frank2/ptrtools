#ifndef __PTRTOOLS_BASIC_HPP
#define __PTRTOOLS_BASIC_HPP

#include <algorithm>
#include <cstddef>
#include <cstdint>
#include <cstring>
#include <exception>
#include <memory>
#include <optional>
#include <type_traits>
#include <utility>
#include <variant>

#include <ptrtools/utility.hpp>

namespace ptrtools
{
   template <
      typename T,
      std::size_t TypeSize=sizeof(T),
      std::size_t TypeAlign=alignof(T),
      typename Allocator=std::allocator<std::uint8_t>>
   class basic_ptr
   {
      static_assert(!std::is_same<T,void>::value, "Type cannot be void");
      static_assert(std::is_same<typename Allocator::value_type,std::uint8_t>::value, "Allocator must be a byte allocator (unsigned char/uint8_t)");

   public:
      using value_type = T;
      using pointer = value_type *;
      using const_pointer = const value_type *;
      using reference = value_type &;
      using const_reference = const value_type &;
      using allocator = Allocator;

      const static std::size_t type_size = TypeSize;
      const static std::size_t type_align = TypeAlign;

   protected:
      std::variant<pointer, const_pointer> _ptr;
      std::size_t _size;
      std::optional<Allocator> _allocator;

   public:
      class iterator
      {
      public:
         using iterator_category = std::forward_iterator_tag;
         using difference_type = std::ptrdiff_t;
         using value_type = T;
         using pointer = iterator::value_type *;
         using reference = iterator::value_type &;

      private:
         basic_ptr<T,TypeSize,TypeAlign,Allocator> *_base;
         iterator::pointer _iter;

      public:
         iterator(basic_ptr<T,TypeSize,TypeAlign,Allocator> &base, bool end=false) : _base(&base) {
            if (end)
               this->_iter = base.eob();
            else
               this->_iter = base.get();
         }
         iterator(const iterator &other) : _base(other._base), _iter(other._iter) {}
         ~iterator() {}

         iterator &operator=(const iterator &other) {
            this->_base = other._base;
            this->_iter = other._iter;

            return *this;
         }
         iterator &operator++() {
            if (this->_iter == nullptr)
               throw std::runtime_error("null pointer: attempting to advance an iterator on a null pointer");

            auto uintptr_cast = reinterpret_cast<std::uintptr_t>(this->_iter);
            auto size = this->_base->aligned_type_size();

            this->_iter = reinterpret_cast<iterator::pointer>(uintptr_cast+size);

            return *this;
         }
         iterator operator++(int) { auto &tmp = *this; ++(*this); return tmp; }
         bool operator==(const iterator &other) const { return other._base == this->_base && other._iter == this->_iter; }
         bool operator!=(const iterator &other) const { return !(*this == other); }
         iterator::reference operator*() {
            if (this->_iter == nullptr)
               throw std::runtime_error("null pointer: attempting to dereference an iterator on a null pointer");

            return *this->_iter;
         }
         iterator::pointer operator->() {
            if (this->_iter == nullptr)
               throw std::runtime_error("null pointer: attempting to dereference an iterator on a null pointer");

            return this->_iter;
         }
      };
      class const_iterator
      {
      public:
         using iterator_category = std::forward_iterator_tag;
         using difference_type = std::ptrdiff_t;
         using value_type = const T;
         using pointer = const_iterator::value_type *;
         using reference = const_iterator::value_type &;

      private:
         const basic_ptr<T,TypeSize,TypeAlign,Allocator> *_base;
         const_iterator::pointer _iter;

      public:
         const_iterator(const basic_ptr<T,TypeSize,TypeAlign,Allocator> &base, bool end=false) : _base(&base) {
            if (end)
               this->_iter = base.eob();
            else
               this->_iter = base.get();
         }
         const_iterator(const const_iterator &other) : _base(other._base), _iter(other._iter) {}
         ~const_iterator() {}

         const_iterator &operator=(const const_iterator &other) {
            this->_base = other._base;
            this->_iter = other._iter;

            return *this;
         }
         const_iterator &operator++() {
            if (this->_iter == nullptr)
               throw std::runtime_error("null pointer: attempting to advance an const_iterator on a null pointer");

            auto uintptr_cast = reinterpret_cast<std::uintptr_t>(this->_iter);
            auto size = this->_base->aligned_type_size();

            this->_iter = reinterpret_cast<const_iterator::pointer>(uintptr_cast+size);

            return *this;
         }
         const_iterator operator++(int) { auto tmp = *this; ++(*this); return tmp; }
         bool operator==(const const_iterator &other) const { return other._base == this->_base && other._iter == this->_iter; }
         bool operator!=(const const_iterator &other) const { return !(*this == other); }
         const_iterator::reference operator*() {
            if (this->_iter == nullptr)
               throw std::runtime_error("null pointer: attempting to dereference an iterator on a null pointer");

            return *this->_iter;
         }
         const_iterator::pointer operator->() {
            if (this->_iter == nullptr)
               throw std::runtime_error("null pointer: attempting to dereference an iterator on a null pointer");

            return this->_iter;
         }
      };
      class reverse_iterator
      {
      public:
         using iterator_category = std::forward_iterator_tag;
         using difference_type = std::ptrdiff_t;
         using value_type = T;
         using pointer = reverse_iterator::value_type *;
         using reference = reverse_iterator::value_type &;

      private:
         basic_ptr<T,TypeSize,TypeAlign,Allocator> *_base;
         reverse_iterator::pointer _iter;

      public:
         reverse_iterator(basic_ptr<T,TypeSize,TypeAlign,Allocator> &base, bool end=false) : _base(&base) {
            if (end)
               this->_iter = base.reob();
            else if (base.elements() > 0)
               this->_iter = &base.back();
            else
               this->_iter = base.get();
         }
         reverse_iterator(const reverse_iterator &other) : _base(other._base), _iter(other._iter) {}
         ~reverse_iterator() {}

         reverse_iterator &operator=(const reverse_iterator &other) {
            this->_base = other._base;
            this->_iter = other._iter;

            return *this;
         }
         reverse_iterator &operator++() {
            if (this->_iter == nullptr)
               throw std::runtime_error("null pointer: attempting to advance an iterator on a null pointer");

            auto uintptr_cast = reinterpret_cast<std::uintptr_t>(this->_iter);
            auto size = this->_base->aligned_type_size();

            this->_iter = reinterpret_cast<reverse_iterator::pointer>(uintptr_cast-size);

            return *this;
         }
         reverse_iterator operator++(int) { auto tmp = *this; ++(*this); return tmp; }
         bool operator==(const reverse_iterator &other) const { return other._base == this->_base && other._iter == this->_iter; }
         bool operator!=(const reverse_iterator &other) const { return !(*this == other); }
         reverse_iterator::reference operator*() {
            if (this->_iter == nullptr)
               throw std::runtime_error("null pointer: attempting to dereference an iterator on a null pointer");

            return *this->_iter;
         }
         reverse_iterator::pointer operator->() {
            if (this->_iter == nullptr)
               throw std::runtime_error("null pointer: attempting to dereference an iterator on a null pointer");

            return this->_iter;
         }
      };
      class const_reverse_iterator
      {
      public:
         using iterator_category = std::forward_iterator_tag;
         using difference_type = std::ptrdiff_t;
         using value_type = const T;
         using pointer = const_reverse_iterator::value_type *;
         using reference = const_reverse_iterator::value_type &;

      private:
         const basic_ptr<T,TypeSize,TypeAlign,Allocator> *_base;
         const_reverse_iterator::pointer _iter;

      public:
         const_reverse_iterator(const basic_ptr<T,TypeSize,TypeAlign,Allocator> &base, bool end=false) : _base(&base) {
            if (end)
               this->_iter = base.reob();
            else if (base.elements() > 0)
               this->_iter = &base.back();
            else
               this->_iter = base.get();
         }

         const_reverse_iterator(const const_reverse_iterator &other) : _base(other._base), _iter(other._iter) {}
         ~const_reverse_iterator() {}

         const_reverse_iterator &operator=(const const_reverse_iterator &other) {
            this->_base = other._base;
            this->_iter = other._iter;

            return *this;
         }
         const_reverse_iterator &operator++() {
            if (this->_iter == nullptr)
               throw std::runtime_error("null pointer: attempting to advance an iterator on a null pointer");

            auto uintptr_cast = reinterpret_cast<std::uintptr_t>(this->_iter);
            auto size = this->_base->aligned_type_size();

            this->_iter = reinterpret_cast<const_reverse_iterator::pointer>(uintptr_cast-size);

            return *this;
         }
         const_reverse_iterator operator++(int) { auto tmp = *this; ++(*this); return tmp; }
         bool operator==(const const_reverse_iterator &other) const { return other._base == this->_base && other._iter == this->_iter; }
         bool operator!=(const const_reverse_iterator &other) const { return !(*this == other); }
         const_reverse_iterator::reference operator*() {
            if (this->_iter == nullptr)
               throw std::runtime_error("null pointer: attempting to dereference an iterator on a null pointer");

            return *this->_iter;
         }
         const_reverse_iterator::pointer operator->() {
            if (this->_iter == nullptr)
               throw std::runtime_error("null pointer: attempting to dereference an iterator on a null pointer");

            return this->_iter;
         }
      };

      basic_ptr(bool allocate=false) : _ptr(static_cast<pointer>(nullptr)), _size(0), _allocator(std::nullopt) {
         if (allocate)
            this->allocate(this->type_size);
      }
      basic_ptr(std::size_t size) : _ptr(static_cast<pointer>(nullptr)), _size(0), _allocator(std::nullopt) {
         if (size > 0)
            this->allocate(size);
      }
      basic_ptr(pointer ptr, std::size_t size=TypeSize, bool clone=false) {
         if (clone)
            this->clone(ptr, size);
         else
            this->set(ptr, size);
      }
      basic_ptr(const_pointer ptr, std::size_t size=TypeSize, bool clone=false) {
         if (clone)
            this->clone(ptr, size);
         else
            this->set(ptr, size);
      }
      basic_ptr(basic_ptr<T,TypeSize,TypeAlign,Allocator> &other) {
         if (other.is_allocated())
         {
            if (this->is_allocated())
               this->deallocate();
         
            this->_allocator = other._allocator;
            this->clone(other);
         }
         else
            this->set(other.get(), other.size());
      }
      basic_ptr(const basic_ptr<T,TypeSize,TypeAlign,Allocator> &other) {
         if (other.is_allocated())
         {
            if (this->is_allocated())
               this->deallocate();
         
            this->_allocator = other._allocator;
            this->clone(other);
         }
         else
            this->set(other.get(), other.size());
      }
      basic_ptr(basic_ptr<T,TypeSize,TypeAlign,Allocator> &&other) {
         this->_ptr = std::move(other._ptr);
         this->_size = std::move(other._size);
         this->_allocator = std::move(other._allocator);
      }
      virtual ~basic_ptr() {
         if (this->is_allocated())
            this->deallocate();
      }

      basic_ptr<T,TypeSize,TypeAlign,Allocator> &operator=(basic_ptr<T,TypeSize,TypeAlign,Allocator> &other) {
         if (other.is_allocated())
         {
            if (this->is_allocated())
               this->deallocate();
         
            this->_allocator = other._allocator;
            this->clone(other);
         }
         else
            this->set(other.get(), other.size());

         return *this;
      }
      basic_ptr<T,TypeSize,TypeAlign,Allocator> &operator=(const basic_ptr<T,TypeSize,TypeAlign,Allocator> &other) {
         if (other.is_allocated())
         {
            if (this->is_allocated())
               this->deallocate();
         
            this->_allocator = other._allocator;
            this->clone(other);
         }
         else
            this->set(other.get(), other.size());

         return *this;
      }
      basic_ptr<T,TypeSize,TypeAlign,Allocator> &operator=(basic_ptr<T,TypeSize,TypeAlign,Allocator> &&other) {
         this->_ptr = std::move(other._ptr);
         this->_size = std::move(other._size);
         this->_allocator = std::move(other._allocator);

         return *this;
      }
      bool operator<(const basic_ptr<T,TypeSize,TypeAlign,Allocator> &other) const {
         if (this->get() == other.get() && this->size() == other.size() && this->is_allocated() == other.is_allocated())
            return false;

         if (this->get() != other.get())
            return reinterpret_cast<std::uintptr_t>(this->get()) < reinterpret_cast<std::uintptr_t>(other.get());

         if (this->size() != other.size())
            return this->size() < other.size();

         return static_cast<std::uint8_t>(this->is_allocated()) < static_cast<std::uint8_t>(other.is_allocated());
      }
      bool operator==(const basic_ptr<T,TypeSize,TypeAlign,Allocator> &other) const {
         return !(*this < other) && !(other < *this);
      }
      bool operator!=(const basic_ptr<T,TypeSize,TypeAlign,Allocator> &other) const {
         return !(*this == other);
      }
      reference operator[](std::size_t pos) { return this->at(pos); }
      const_reference operator[](std::size_t pos) const { return this->at(pos); }
      reference operator*() {
         auto ptr = this->get();

         if (ptr == nullptr)
            throw std::runtime_error("null pointer: attempting to dereference a null pointer");
         
         return *ptr;
      }
      const_reference operator*() const {
         auto ptr = this->get();

         if (ptr == nullptr)
            throw std::runtime_error("null pointer: attempting to dereference a null pointer");
         
         return *ptr;
      }
      pointer operator->() { return &**this; }
      const_pointer operator->() const { return &**this; }

      bool is_const() const { return std::get_if<const_pointer>(&this->_ptr) != nullptr; }
      bool is_allocated() const { return this->_allocator.has_value(); }
      bool is_null() const {
         if (auto mut = std::get_if<pointer>(&this->_ptr))
            return *mut == nullptr;
         else
            return std::get<const_pointer>(this->_ptr) == nullptr;
      }

      void allocate(std::size_t size) {
         if (size == 0)
            throw std::runtime_error("null allocation: cannot allocate a zero-sized buffer");

         if (size < this->type_size)
            throw std::runtime_error("insufficient size: the allocation size is not large enough to hold a single element");
         
         if (this->is_allocated())
            this->deallocate();

         this->_allocator = Allocator();
         this->_ptr = reinterpret_cast<pointer>(this->_allocator->allocate(size));
         std::memset(std::get<pointer>(this->_ptr), 0, size);
         
         this->_size = size;
      }
      void deallocate() {
         if (!this->is_allocated())
            throw std::runtime_error("invalid deallocation: attempting to release a pointer that is not allocated");

         this->_allocator->deallocate(reinterpret_cast<std::uint8_t *>(this->get()), this->size());
         this->_allocator = std::nullopt;
         this->_ptr = static_cast<pointer>(nullptr);
         this->_size = 0;
      }
      void reallocate(std::size_t size) {
         if (!this->is_allocated())
         {
            this->allocate(size);
            return;
         }
         
         if (size == 0)
            throw std::runtime_error("null allocation: cannot allocate a zero-sized buffer");

         if (size < this->type_size)
            throw std::runtime_error("insufficient size: the allocation size is not large enough to hold a single element");

         if (size == this->size())
            return;
                  
         auto new_ptr = this->_allocator->allocate(size);
         std::memset(new_ptr, 0, size);
         
         auto old_ptr = this->get();
         auto copy_size = std::min(size, this->size());
         std::memcpy(new_ptr, old_ptr, copy_size);

         this->_allocator->deallocate(reinterpret_cast<std::uint8_t *>(old_ptr), this->size());
         this->_size = size;
         this->_ptr = reinterpret_cast<pointer>(new_ptr);
      }
      void resize(std::size_t size) {
         if (this->is_allocated())
         {
            this->reallocate(size);
            return;
         }

         if (size < this->type_size)
            throw std::runtime_error("insufficient size: the new size cannot contain a single element");

         this->_size = size;
      }

      void set(pointer ptr, std::size_t size) {
         if (this->is_allocated())
            this->deallocate();

         this->_ptr = ptr;
         this->_size = size;
         this->_allocator = std::nullopt;
      }
      void set(const_pointer ptr, std::size_t size) {
         if (this->is_allocated())
            this->deallocate();

         this->_ptr = ptr;
         this->_size = size;
         this->_allocator = std::nullopt;
      }
      pointer get() {
         if (auto mut = std::get_if<pointer>(&this->_ptr))
            return *mut;
         else
            throw std::runtime_error("const conflict: attempting to get a mutable pointer from a const pointer");
      }
      const_pointer get() const {
         if (auto mut = std::get_if<pointer>(&this->_ptr))
            return const_cast<const_pointer>(*mut);
         else
            return std::get<const_pointer>(this->_ptr);
      }
      pointer eob() {
         auto ptr = this->get();

         if (ptr == nullptr)
            return ptr;

         auto uintptr_cast = reinterpret_cast<std::uintptr_t>(ptr);
         auto size = this->size();

         return reinterpret_cast<pointer>(uintptr_cast+size);
      }
      const_pointer eob() const {
         auto ptr = this->get();

         if (ptr == nullptr)
            return ptr;

         auto uintptr_cast = reinterpret_cast<std::uintptr_t>(ptr);
         auto size = this->size();

         return reinterpret_cast<const_pointer>(uintptr_cast+size);
      }
      pointer reob() {
         auto ptr = this->get();

         if (ptr == nullptr)
            return ptr;

         auto uintptr_cast = reinterpret_cast<std::uintptr_t>(ptr);
         auto type_size = this->aligned_type_size();

         return reinterpret_cast<pointer>(uintptr_cast-type_size);
      }
      const_pointer reob() const {
         auto ptr = this->get();

         if (ptr == nullptr)
            return ptr;

         auto uintptr_cast = reinterpret_cast<std::uintptr_t>(ptr);
         auto type_size = this->aligned_type_size();

         return reinterpret_cast<const_pointer>(uintptr_cast-type_size);
      }

      std::size_t size() const { return this->_size; }
      std::size_t aligned_type_size() const { return align(this->type_size, this->type_align); }
      std::size_t elements() const { return this->size() / this->aligned_type_size(); }
      reference front() { return (*this)[0]; }
      const_reference front() const { return (*this)[0]; }
      reference back() { return (*this)[this->elements()-1]; }
      const_reference back() const { return (*this)[this->elements()-1]; }

      iterator begin() { return iterator(*this); }
      const_iterator cbegin() const { return const_iterator(*this); }
      reverse_iterator rbegin() { return reverse_iterator(*this); }
      const_reverse_iterator crbegin() const { return const_reverse_iterator(*this); }
      iterator end() { return iterator(*this, true); }
      const_iterator cend() const { return const_iterator(*this, true); }
      reverse_iterator rend() { return reverse_iterator(*this, true); }
      const_reverse_iterator crend() const { return const_reverse_iterator(*this, true); }
      
      std::size_t offset(std::size_t index) const {
         auto aligned_offset = this->aligned_type_size() * index;

         if (aligned_offset >= this->size())
            std::runtime_error("out of bounds: the given index goes out of bounds of the aligned pointer allocation");

         return aligned_offset;
      }
      std::size_t index(std::size_t offset) const {
         if (offset >= this->size())
            std::runtime_error("out of bounds: the given offset goes out of bounds of the allocation");

         return offset / this->aligned_type_size();
      }

      template <typename U=T, std::size_t LocalTypeSize=sizeof(U), std::size_t LocalTypeAlign=alignof(U)>
      basic_ptr<U,LocalTypeSize,LocalTypeAlign,Allocator> ptr_at(std::size_t offset, bool aligned=true) {
         auto ptr = this->get();

         if (ptr == nullptr)
            throw std::runtime_error("null pointer: attempting to access a null pointer");

         if (aligned && offset % this->type_align != 0)
            throw std::runtime_error("alignment error: the given offset is not aligned to the type boundary");
         
         auto uintptr_cast = reinterpret_cast<std::uintptr_t>(ptr);
         auto aligned_size = align(LocalTypeSize, LocalTypeAlign);
         auto end = offset + aligned_size;

         if (end > this->size())
            throw std::runtime_error("out of bounds: the given offset exceeds the allocation boundary");

         return basic_ptr<U,LocalTypeSize,LocalTypeAlign,Allocator>(reinterpret_cast<U*>(uintptr_cast+offset), aligned_size);
      }
      template <typename U=T, std::size_t LocalTypeSize=sizeof(U), std::size_t LocalTypeAlign=alignof(U)>
      const basic_ptr<U,LocalTypeSize,LocalTypeAlign,Allocator> ptr_at(std::size_t offset, bool aligned=true) const {
         auto ptr = this->get();

         if (ptr == nullptr)
            throw std::runtime_error("null pointer: attempting to access a null pointer");

         if (aligned && offset % this->type_align != 0)
            throw std::runtime_error("alignment error: the given offset is not aligned to the type boundary");
         
         auto uintptr_cast = reinterpret_cast<std::uintptr_t>(ptr);
         auto aligned_size = align(LocalTypeSize, LocalTypeAlign);
         auto end = offset + aligned_size;

         if (end > this->size())
            throw std::runtime_error("out of bounds: the given offset exceeds the allocation boundary");

         return basic_ptr<U,LocalTypeSize,LocalTypeAlign,Allocator>(reinterpret_cast<const U*>(uintptr_cast+offset), aligned_size);
      }
      reference at(std::size_t index) {
         return *this->ptr_at<T,TypeSize,TypeAlign>(this->offset(index));
      }
      const_reference at(std::size_t index) const {
         return *this->ptr_at<T,TypeSize,TypeAlign>(this->offset(index));
      }
      void assign(std::size_t index, const_reference value) {
         this->at(index) = value;
      }

      void consume() {
         if (this->is_allocated())
            throw std::runtime_error("invalid argument: an allocated pointer cannot be consumed");

         this->clone(this->get(), this->size());
      }
      void clone(const_pointer ptr, std::size_t size) {
         this->allocate(size);
         this->copy(ptr, size);
      }
      void clone(const basic_ptr<T,TypeSize,TypeAlign,Allocator> &other) {
         this->clone(other.get(), other.size());
      }
      void copy(const_pointer ptr, std::size_t size, std::size_t offset=0, bool aligned=true) {
         auto local_ptr = this->get();

         if (local_ptr == nullptr)
            throw std::runtime_error("null pointer: attempting to copy to a null pointer");

         if (aligned && offset % this->type_align != 0)
            throw std::runtime_error("invalid alignment: alignment enforcement is set but the offset is not aligned");
         
         auto end = offset + size;

         if (end > this->size())
            throw std::runtime_error("out of bounds: copy of other pointer exceeds current allocation, try reallocating or cloning");

         auto uintptr_cast = reinterpret_cast<std::uintptr_t>(local_ptr);
         std::memcpy(reinterpret_cast<pointer>(uintptr_cast+offset), ptr, size);
      }
      void copy(const basic_ptr<T,TypeSize,TypeAlign,Allocator> &other, std::size_t offset=0, bool aligned=true) {
         this->copy(other.get(), other.size(), offset, aligned);
      }
   };
}

#endif
