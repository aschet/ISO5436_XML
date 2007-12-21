#ifndef _OPENGPS_H
#define	_OPENGPS_H

#ifndef NULL
# define NULL 0
#endif

#ifndef FALSE
# ifdef __cplusplus
#   define FALSE false
# else
#   define FALSE 0
# endif /* __cplusplus */
#endif /* FALSE */

#ifndef TRUE
# ifdef __cplusplus
#   define TRUE true
# else
#   define TRUE 1
# endif /* __cplusplus */
#endif /* TRUE */

#ifndef OGPS_Boolean
# ifdef __cplusplus
typedef bool OGPS_Boolean;
# else
typedef int OGPS_Boolean;
# endif /* __cplusplus */
#endif /* OGPS_Boolean */

#ifdef _UNICODE
typedef wchar_t OGPS_Character;
#else
typedef char OGPS_Character;
#endif /* _UNICODE */

#ifdef _DEBUG

#include <assert.h>

#define _ASSERT(x) assert(x)
#define _VERIFY(x) _ASSERT(x)

#else /* _DEBUG */

#define _ASSERT(x) ((void)0)
#defihne _VERIFY(x) x

#endif /* _DEBUG */

#endif	/* _OPENGPS_H */

