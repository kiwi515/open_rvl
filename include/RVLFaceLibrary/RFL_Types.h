#ifndef RVL_FACE_LIBRARY_TYPES_H
#define RVL_FACE_LIBRARY_TYPES_H
#include <revolution/types.h>
#ifdef __cplusplus
extern "C" {
#endif

/**
 * Common types used throughout many RFL files.
 *
 * To avoid circular depencies, common structures and all enums have been moved
 * here.
 *
 * Everything is organized in alphabetical order (when possible, otherwise
 * dependency order).
 */

/**
 * Common defines
 */

#define RFL_NAME_LEN 10
#define RFL_CREATOR_LEN 10
#define RFL_CREATEID_LEN 8

#define RFL_CTRL_MAX 4
#define RFL_CTRL_CHAR_MAX 10

#define RFL_DB_CHAR_MAX 100

/**
 * Common enums
 */

typedef enum {
    RFLiArcID_Beard,
    RFLiArcID_Eye,
    RFLiArcID_Eyebrow,
    RFLiArcID_Faceline,
    RFLiArcID_FaceTex,
    RFLiArcID_ForeHead,
    RFLiArcID_Glass,
    RFLiArcID_GlassTex,
    RFLiArcID_Hair,
    RFLiArcID_Mask,
    RFLiArcID_Mole,
    RFLiArcID_Mouth,
    RFLiArcID_Mustache,
    RFLiArcID_Nose,
    RFLiArcID_Nline,
    RFLiArcID_NlineTex,
    RFLiArcID_Cap,
    RFLiArcID_CapTex,
    RFLiArcID_Max
} RFLiArcID;

typedef enum {
    RFLiAsyncTag_WriteFile,
    RFLiAsyncTag_WriteCloseFile,
    RFLiAsyncTag_ReadFile,
    RFLiAsyncTag_ReadCloseFile,
    RFLiAsyncTag_ReadCache,
    RFLiAsyncTag_ReadCacheClose,
    RFLiAsyncTag_WriteHDBFile,
    RFLiAsyncTag_WriteHDBFileClose,
    RFLiAsyncTag_ReadHDBFile,
    RFLiAsyncTag_ReadHDBFileClose,
    RFLiAsyncTag_CreateAsync,
    RFLiAsyncTag_OpenAsync,
    RFLiAsyncTag_SeekAsync,
    RFLiAsyncTag_ReadAsync,
    RFLiAsyncTag_WriteAsync,
    RFLiAsyncTag_CloseAsync,
    RFLiAsyncTag_GetLengthAsync,
    RFLiAsyncTag_DeleteAsync,
    RFLiAsyncTag_CreateDirAsync,

    RFLiAsyncTag_Max
} RFLiAsyncTag;

typedef enum {
    RFLDataSource_Official,
    RFLDataSource_Hidden,
    RFLDataSource_Controller1,
    RFLDataSource_Controller2,
    RFLDataSource_Controller3,
    RFLDataSource_Controller4,
    RFLDataSource_Default,
    RFLDataSource_Middle,

    RFLDataSource_Max
} RFLDataSource;

typedef enum {
    RFLErrcode_Success,
    RFLErrcode_NotAvailable,
    RFLErrcode_NANDCommandfail,
    RFLErrcode_Loadfail,
    RFLErrcode_Savefail,
    RFLErrcode_Fatal,
    RFLErrcode_Busy,
    RFLErrcode_Broken,
    RFLErrcode_Exist,
    RFLErrcode_DBFull,
    RFLErrcode_DBNodata,
    RFLErrcode_Controllerfail,
    RFLErrcode_NWC24Fail,
    RFLErrcode_MaxFiles,
    RFLErrcode_MaxBlocks,
    RFLErrcode_WrongParam,
    RFLErrcode_NoFriends,
    RFLErrcode_Isolation,

    RFLErrcode_Unknown = 0xFF
} RFLErrcode;

typedef enum {
    RFLExp_Normal,
    RFLExp_Smile,
    RFLExp_Anger,
    RFLExp_Sorrow,
    RFLExp_Surprise,
    RFLExp_Blink,
    RFLExp_OpenMouth,

    RFLExp_Max
} RFLExpression;

typedef enum {
    RFLFavoriteColor_Red,
    RFLFavoriteColor_Orange,
    RFLFavoriteColor_Yellow,
    RFLFavoriteColor_YellowGreen,
    RFLFavoriteColor_Green,
    RFLFavoriteColor_Blue,
    RFLFavoriteColor_SkyBlue,
    RFLFavoriteColor_Pink,
    RFLFavoriteColor_Purple,
    RFLFavoriteColor_Brown,
    RFLFavoriteColor_White,
    RFLFavoriteColor_Black,

    RFLFavoriteColor_Max
} RFLFavoriteColor;

typedef enum {
    RFLiFileBrokenType_DBNotFound,
    RFLiFileBrokenType_DBBroken,
    RFLiFileBrokenType_ResBroken,
    RFLiFileBrokenType_Corrupt,

    RFLiFileBrokenType_Max
} RFLiFileBrokenType;

typedef enum {
    RFLiFileType_Database,
    RFLiFileType_Resource,

    RFLiFileType_Max
} RFLiFileType;

typedef enum {
    RFLiOpenType_Multi,
    RFLiOpenType_Single,
    RFLiOpenType_Opened
} RFLiOpenType;

typedef enum {
    RFLiPartsShp_Nose,
    RFLiPartsShp_Forehead,
    RFLiPartsShp_Faceline,
    RFLiPartsShp_Hair,
    RFLiPartsShp_Cap,
    RFLiPartsShp_Beard,
    RFLiPartsShp_Noseline,
    RFLiPartsShp_Mask,
    RFLiPartsShp_Glass,

    RFLiPartsShp_Max
} RFLiPartsShp;

typedef enum {
    RFLiPartsShpTex_Face,
    RFLiPartsShpTex_Cap,
    RFLiPartsShpTex_Noseline,
    RFLiPartsShpTex_Glass,

    RFLiPartsShpTex_Max
} RFLiPartsShpTex;

typedef enum {
    RFLiPartsTex_Eye,
    RFLiPartsTex_Eyebrow,
    RFLiPartsTex_Mouth,
    RFLiPartsTex_Mustache,
    RFLiPartsTex_Mole,

    RFLiPartsTex_Max
} RFLiPartsTex;

typedef enum {
    RFLResolution_64 = 64,
    RFLResolution_128 = 128,
    RFLResolution_256 = 256,

    RFLResolution_64M = 96,
    RFLResolution_128M = 224,
    RFLResolution_256M = 480
} RFLResolution;

/**
 * Common typedefs
 */

typedef void (*RFLSystemCallback)(void);
typedef void (*RFLIconDrawDoneCallback)(void);
typedef void (*RFLModelDrawDoneCallback)(void);

typedef void (*RFLAccessCallback)(void);
typedef void (*RFLAlarmCallback)(RFLiFileType);

typedef void (*RFLDatabaseCallback)(void);

#ifdef __cplusplus
}
#endif
#endif
