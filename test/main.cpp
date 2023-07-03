#include <framework.hpp>
#include <ptrtools.hpp>

using namespace ptrtools;

struct test_struct_basic
{
   std::uint8_t u8;
   std::uint16_t u16;
   std::uint32_t u32;
};

struct test_struct_flexible
{
   std::uint8_t u8;
   std::uint16_t u16;
   std::uint32_t u32;
   std::uint64_t flex[1];
};

int test_basic()
{
   INIT();
   
   auto data = "\xde\xad\xbe\xef\xab\xad\x1d\xea\xde\xad\xbe\xa7\xde\xfa\xce\xd1";
   basic_ptr<std::uint8_t> cloned_u8(reinterpret_cast<const std::uint8_t *>(data), std::strlen(data), true);
   basic_ptr<std::uint8_t> slice_u8;

   ASSERT(cloned_u8.is_allocated());
   ASSERT(!cloned_u8.is_const());
   ASSERT(!cloned_u8.is_null());
   ASSERT(!slice_u8.is_allocated());
   ASSERT(!slice_u8.is_const());
   ASSERT(slice_u8.is_null());

   ASSERT_SUCCESS(slice_u8.set(cloned_u8.get(), cloned_u8.size()));
   ASSERT(slice_u8 != cloned_u8);
   ASSERT(&slice_u8[0] == &cloned_u8[0]);
   ASSERT(*slice_u8 == *cloned_u8);
   ASSERT(slice_u8.get() == cloned_u8.get());
   ASSERT(slice_u8[0] == 0xDE);
   ASSERT(slice_u8.size() == 16);
   ASSERT(slice_u8.eob() == reinterpret_cast<std::uint8_t *>(reinterpret_cast<std::uintptr_t>(slice_u8.get())+16));
   ASSERT(slice_u8.front() == 0xDE);
   ASSERT(slice_u8.back() == 0xD1);
   ASSERT(slice_u8.offset(4) == 4);
   ASSERT(slice_u8.index(4) == 4);
   ASSERT(slice_u8.ptr_at(4) == cloned_u8.ptr_at(4));
   ASSERT(&slice_u8[4] == &cloned_u8[4]);
   ASSERT(slice_u8[4] == 0xAB);
   ASSERT(&slice_u8[0xC] == &cloned_u8[0xC])
   ASSERT(slice_u8[0xC] == 0xDE);
   ASSERT_THROWS(slice_u8.ptr_at(16), std::runtime_error);

   const basic_ptr<std::uint32_t> slice_u32(reinterpret_cast<const std::uint32_t *>(cloned_u8.get()), cloned_u8.size());
   ASSERT(slice_u32[0] == 0xEFBEADDE);
   ASSERT(slice_u32[1] == *slice_u8.ptr_at<std::uint32_t>(4));
   ASSERT(slice_u32.size() == 16);
   ASSERT(slice_u32.elements() == 4);

   auto ptr_u32 = reinterpret_cast<const std::uint32_t *>(data);

   for (auto iter=slice_u32.cbegin(); iter!=slice_u32.cend(); ++iter)
   {
      ASSERT(*iter == *ptr_u32);
      ptr_u32++;
   }
   
   ASSERT_SUCCESS(slice_u8.assign(0, 0xD1));
   ASSERT(cloned_u8[0] == 0xD1);

   basic_ptr<test_struct_basic> struct_ptr(true);
   ASSERT_SUCCESS(struct_ptr->u8 = 0x69);
   ASSERT_SUCCESS(struct_ptr->u16 = 0xBEEF);
   ASSERT_SUCCESS(struct_ptr->u32 = 0xABAD1DEA);

   auto assumed_struct_data = "\x69\x00\xef\xbe\xea\x1d\xad\xab";
   auto assumed_struct_ptr = reinterpret_cast<const std::uint8_t *>(assumed_struct_data);
   auto struct_u8 = basic_ptr<std::uint8_t>(reinterpret_cast<std::uint8_t *>(struct_ptr.get()), struct_ptr.size());

   ASSERT(struct_u8.size() == struct_ptr.size());
   ASSERT(struct_u8.size() == 8);

   if (struct_u8.size() == 8)
   {
      for (auto u8 : struct_u8)
      {
         ASSERT(u8 == *assumed_struct_ptr);
         assumed_struct_ptr++;
      }
   }

   COMPLETE();
}

int test_struct()
{
   INIT();
   
   basic_ptr<test_struct_basic> struct_ptr(true);
   ASSERT_SUCCESS(struct_ptr->u8 = 0x69);
   ASSERT_SUCCESS(struct_ptr->u16 = 0xBEEF);
   ASSERT_SUCCESS(struct_ptr->u32 = 0xABAD1DEA);

   auto assumed_struct_data = "\x69\x00\xef\xbe\xea\x1d\xad\xab";
   auto assumed_struct_ptr = reinterpret_cast<const std::uint8_t *>(assumed_struct_data);
   auto struct_u8 = basic_ptr<std::uint8_t>(reinterpret_cast<std::uint8_t *>(struct_ptr.get()), struct_ptr.size());

   ASSERT(struct_u8.size() == struct_ptr.size());
   ASSERT(struct_u8.size() == 8);

   if (struct_u8.size() == 8)
   {
      for (auto u8 : struct_u8)
      {
         ASSERT(u8 == *assumed_struct_ptr);
         assumed_struct_ptr++;
      }
   }

   ASSERT_SUCCESS(struct_ptr.clone(reinterpret_cast<const test_struct_basic *>(assumed_struct_data)));
   ASSERT(struct_ptr->u8 == 0x69);
   ASSERT(struct_ptr->u16 == 0xBEEF);
   ASSERT(struct_ptr->u32 == 0xABAD1DEA);

   COMPLETE();
}

int test_array()
{
   INIT();
   
   auto data = "\xde\xad\xbe\xef\xab\xad\x1d\xea\xde\xad\xbe\xa7\xde\xfa\xce\xd1";
   array_ptr<std::uint8_t> cloned_u8(reinterpret_cast<const std::uint8_t *>(data), std::strlen(data), true);
   array_ptr<std::uint8_t> slice_u8;

   ASSERT(cloned_u8.is_allocated());
   ASSERT(!cloned_u8.is_const());
   ASSERT(!cloned_u8.is_null());
   ASSERT(!slice_u8.is_allocated());
   ASSERT(!slice_u8.is_const());
   ASSERT(slice_u8.is_null());

   ASSERT_SUCCESS(slice_u8.set(cloned_u8.get(), cloned_u8.size()));
   ASSERT(slice_u8 != cloned_u8);
   ASSERT(&slice_u8[0] == &cloned_u8[0]);
   ASSERT(*slice_u8 == *cloned_u8);
   ASSERT(slice_u8.get() == cloned_u8.get());
   ASSERT(slice_u8[0] == 0xDE);
   ASSERT(slice_u8.size() == 16);
   ASSERT(slice_u8.eob() == reinterpret_cast<std::uint8_t *>(reinterpret_cast<std::uintptr_t>(slice_u8.get())+16));
   ASSERT(slice_u8.front() == 0xDE);
   ASSERT(slice_u8.back() == 0xD1);
   ASSERT(slice_u8.offset(4) == 4);
   ASSERT(slice_u8.index(4) == 4);
   ASSERT(slice_u8.ptr_at(4) == cloned_u8.ptr_at(4));
   ASSERT(&slice_u8[4] == &cloned_u8[4]);
   ASSERT(slice_u8[4] == 0xAB);
   ASSERT(&slice_u8[0xC] == &cloned_u8[0xC])
   ASSERT(slice_u8[0xC] == 0xDE);
   ASSERT_THROWS(slice_u8.ptr_at(16), std::runtime_error);

   const array_ptr<std::uint32_t> slice_u32(reinterpret_cast<const std::uint32_t *>(cloned_u8.get()), cloned_u8.size()/sizeof(std::uint32_t));
   ASSERT(slice_u32[0] == 0xEFBEADDE);
   ASSERT(slice_u32[1] == *slice_u8.ptr_at<std::uint32_t>(4));
   ASSERT(slice_u32.size() == 16);
   ASSERT(slice_u32.elements() == 4);

   auto ptr_u32 = reinterpret_cast<const std::uint32_t *>(data);

   for (auto iter=slice_u32.cbegin(); iter!=slice_u32.cend(); ++iter)
   {
      ASSERT(*iter == *ptr_u32);
      ptr_u32++;
   }
   
   ASSERT_SUCCESS(slice_u8.assign(0, 0xD1));
   ASSERT(cloned_u8[0] == 0xD1);

   COMPLETE();
}

int test_flexible()
{
   INIT();
   
   flexible_ptr<test_struct_flexible,std::uint64_t> flex_ptr(4);

   ASSERT(flex_ptr.size() == sizeof(test_struct_flexible)+3*sizeof(std::uint64_t));
   ASSERT(flex_ptr.elements() == 4);

   ASSERT_SUCCESS(flex_ptr->u8 = 0x69);
   ASSERT_SUCCESS(flex_ptr->u16 = 0xBEEF);
   ASSERT_SUCCESS(flex_ptr->u32 = 0xABAD1DEA);
   
   ASSERT_SUCCESS(flex_ptr[0] = 0xC01DC0FFEE);
   ASSERT_SUCCESS(flex_ptr[1] = 0xFACEBABEABAD1DEA);
   ASSERT_SUCCESS(flex_ptr[2] = 0xDEFACED1B00B7355);
   ASSERT_SUCCESS(flex_ptr[3] = 0x8675309);

   auto assumed_struct_data = "\x69\x00\xef\xbe\xea\x1d\xad\xab"
      "\xee\xff\xc0\x1d\xc0\x00\x00\x00"
      "\xea\x1d\xad\xab\xbe\xba\xce\xfa"
      "\x55\x73\x0b\xb0\xd1\xce\xfa\xde"
      "\x09\x53\x67\x08\x00\x00\x00\x00";
   auto assumed_struct_ptr = reinterpret_cast<const std::uint64_t *>(assumed_struct_data);
   auto struct_u8 = basic_ptr<std::uint64_t>(reinterpret_cast<std::uint64_t *>(flex_ptr.get()), flex_ptr.size());

   ASSERT(struct_u8.size() == flex_ptr.size());
   ASSERT(struct_u8.size() == 8+4*8);

   if (struct_u8.size() == 8)
   {
      for (auto u8 : struct_u8)
      {
         ASSERT(u8 == *assumed_struct_ptr);
         assumed_struct_ptr++;
      }
   }

   COMPLETE();
}

int
main
(int argc, char *argv[])
{
   INIT();

   LOG_INFO("Testing basic_ptr objects.");
   PROCESS_RESULT(test_basic);

   LOG_INFO("Testing struct_ptr objects.");
   PROCESS_RESULT(test_struct);

   LOG_INFO("Testing array_ptr objects.");
   PROCESS_RESULT(test_array);

   LOG_INFO("Testing flexible_ptr objects.");
   PROCESS_RESULT(test_flexible);

   COMPLETE();
}
