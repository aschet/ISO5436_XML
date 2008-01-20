#include <opengps/cxx/opengps.hxx>

size_t ogps_get_name_info(OGPS_Character* const text, const size_t size)
{
   OpenGPS::String src;
   OpenGPS::Info::GetName(&src);
   return src.CopyTo(text, size);
}

size_t ogps_get_software_info(OGPS_Character* const text, const size_t size)
{
   OpenGPS::String src;
   OpenGPS::Info::GetDescription(&src);
   return src.CopyTo(text, size);
}

size_t ogps_get_version_info(OGPS_Character* const text, const size_t size)
{
   OpenGPS::String src;
   OpenGPS::Info::GetVersion(&src);
   return src.CopyTo(text, size);
}

void ogps_print_version_info()
{
   OpenGPS::Info::PrintVersion();
}

size_t ogps_get_copyright_info(OGPS_Character* const text, const size_t size)
{
   OpenGPS::String src;
   OpenGPS::Info::GetCopyright(&src);
   return src.CopyTo(text, size);
}

void ogps_print_copyright_info()
{
   OpenGPS::Info::PrintCopyright();
}

size_t ogps_get_license_info(OGPS_Character* const text, const size_t size)
{
   OpenGPS::String src;
   OpenGPS::Info::GetLicense(&src);
   return src.CopyTo(text, size);
}

void ogps_print_license_info()
{
   OpenGPS::Info::PrintLicense();
}
