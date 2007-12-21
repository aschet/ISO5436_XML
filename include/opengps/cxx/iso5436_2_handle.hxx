#ifndef _OPENGPS_ISO5436_2_HANDLE_HXX
#define _OPENGPS_ISO5436_2_HANDLE_HXX

#ifndef _OPENGPS_HXX
#	include <opengps/opengps.hxx>
#endif

#ifndef _OPENGPS_ISO5436_2_HANDLE_H
#	include <opengps/iso5436_2_handle.h>
#endif

namespace xsd
{
class ISO5436_2Type;
class Record1Type;
class Record2Type;
class MatrixDimensionType;
}

/* Create a new ISO5436-2 file. Optionally specify temporary path, where
 unpacked data ogps_Gets stored. Must use ogps_CloseISO5436_2 on this handle. */
OGPS_ISO5436_2Handle ogps_CreateMatrixISO5436_2(
        const OGPS_Character* file,
        const OGPS_Character* temp,
        const xsd::Record1Type& record1,
        const xsd::Record2Type& record2,
        const xsd::MatrixDimensionType& matrix,
        const OGPS_Boolean useBinaryData = TRUE);

OGPS_ISO5436_2Handle ogps_CreateListISO5436_2(
        const OGPS_Character* file,
        const OGPS_Character* temp,
        const xsd::Record1Type& record1,
        const xsd::Record2Type& record2,
        const OGPS_Boolean useBinaryData = TRUE);

/* Gets pointer to XML-document with read/write access. Returns NULL on error. */
xsd::ISO5436_2Type * ogps_GetDocument(const OGPS_ISO5436_2Handle handle);

#endif /* _OPENGPS_ISO5436_2_HANDLE_HXX */
