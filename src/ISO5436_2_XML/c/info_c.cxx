#include <opengps/info.h>
#include <opengps/cxx/info.hxx>
#include <opengps/cxx/string.hxx>
#include "messages_c.hxx"

size_t ogps_GetNameInfo(OGPS_Character* const text, const size_t size) throw()
{
   _OPENGPS_GENERIC_EXCEPTION_HANDLER( \
      OpenGPS::String src; \
      OpenGPS::Info::GetName(&src); \
      return src.CopyTo(text, size); \
      );
   return 0;
}

size_t ogps_GetAboutInfo(OGPS_Character* const text, const size_t size) throw()
{
   _OPENGPS_GENERIC_EXCEPTION_HANDLER( \
      OpenGPS::String src; \
      OpenGPS::Info::GetAbout(&src); \
      return src.CopyTo(text, size); \
      );
   return 0;
}

size_t ogps_GetVersionInfo(OGPS_Character* const text, const size_t size) throw()
{
   _OPENGPS_GENERIC_EXCEPTION_HANDLER( \
      OpenGPS::String src; \
      OpenGPS::Info::GetVersion(&src); \
      return src.CopyTo(text, size); \
      );
   return 0;
}

void ogps_PrintVersionInfo() throw()
{
   _OPENGPS_GENERIC_EXCEPTION_HANDLER(OpenGPS::Info::PrintVersion());
}

size_t ogps_GetCopyrightInfo(OGPS_Character* const text, const size_t size) throw()
{
   _OPENGPS_GENERIC_EXCEPTION_HANDLER( \
      OpenGPS::String src; \
      OpenGPS::Info::GetCopyright(&src); \
      return src.CopyTo(text, size); \
      );
   return 0;
}

void ogps_PrintCopyrightInfo() throw()
{
   _OPENGPS_GENERIC_EXCEPTION_HANDLER(OpenGPS::Info::PrintCopyright());
}

size_t ogps_GetLicenseInfo(OGPS_Character* const text, const size_t size) throw()
{
   _OPENGPS_GENERIC_EXCEPTION_HANDLER( \
      OpenGPS::String src; \
      OpenGPS::Info::GetLicense(&src); \
      return src.CopyTo(text, size); \
      );
   return 0;
}

void ogps_PrintLicenseInfo() throw()
{
   _OPENGPS_GENERIC_EXCEPTION_HANDLER(OpenGPS::Info::PrintLicense());
}
